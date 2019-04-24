#include <assert.h>
#include <QDebug>
#include <QDir>
#include <QtGlobal>
#include <QMessageBox>
#include <QWebEngineView>
#include <QProcess>
#include <QFile>
#include "Utils.h"
#include "ui_toolbar.h"
#include "ui_panel.h"
#include "ui_player.h"
#include "PathUtil.h"
#include "AppCore.h"
#include "Dispatcher.h"
#include "HandleBus.hxx"
#include "WebSocketServer.h"
#include "PluginUnityLogger.h"
#include "PluginUnity/UnityActions.h"
#include "PluginUnity/PluginUnity.h"

class PluginUnityInternal
{
public:
	void HandleMessageToUnity(IAction* _action)
	{
		CAST_ACTION(UnityActions::WSMessageToUnityAction, _action, action);
		wsServer->SendMessageToUnity(QString("%1|%2|%3").arg(action->method).arg(action->activity).arg(action->param));
	}

	void HandleRunPlayer(IAction* _action)
	{
		rootPlayer->show();
		if (NULL == player)
		{
#ifndef _DEBUG
			player = new QWebEngineView();
			player->setAcceptDrops(false);
			rootPlayer->layout()->addWidget(player);

			QString playerIndex = QDir::current().absoluteFilePath("player/reviewer_web/index.html");
			QUrl playerURI = QUrl::fromLocalFile(playerIndex);
			player->setUrl(playerURI);
#endif
		}
		
	}

	void LinkAssets()
	{
		QDir srcDirPKG(PathUtil::PackageLocation() + "/pkgs");
		QDir destDirPKG(QDir::current().path() + "/player/editor_web/StreamingAssets/pkgs");
		QDir srcDirUGC(PathUtil::PackageLocation() + "/ugc");
		QDir destDirUGC(QDir::current().path() + "/player/editor_web/StreamingAssets/ugc");

		QDir dir(PathUtil::PackageLocation());
		if (!dir.exists("ugc"))
			dir.mkdir("ugc");
		if (!dir.exists("pkgs"))
			dir.mkdir("pkgs");
#if defined(Q_OS_UNIX)
		//QFile::link(sourceDir.absolutePath(), destDir.absolutePath());
#elif defined(Q_OS_WIN)
		{
			QProcess process;
			QStringList argv = QStringList() << "/C" << "mklink" << "/J" << QDir::toNativeSeparators(destDirPKG.path()) << QDir::toNativeSeparators(srcDirPKG.path());
			process.execute("cmd.exe", argv);
		}
		{
			QProcess process;
			QStringList argv = QStringList() << "/C" << "mklink" << "/J" << QDir::toNativeSeparators(destDirUGC.path()) << QDir::toNativeSeparators(srcDirUGC.path());
			int result = process.execute("cmd.exe", argv);
		}
#endif
	}

	AppCore* ptrAppCore;
	QWidget* rootPanel;
	Ui::PluginUnityPanel* uiPanel;
	Ui::PlayerUi* uiPlayer;
	QWebEngineView* webView;
	QWebEngineView* player;
	UnityWebSocketServer* wsServer;
	QWidget* rootPlayer;
};
	
//-----------------------------------------------------------------------
void PluginUnity::Initialize(AppCore* _appCore)
{
	internal_ = new PluginUnityInternal();
	internal_->ptrAppCore = _appCore;
	internal_->rootPanel = new QWidget();
	internal_->uiPanel = new Ui::PluginUnityPanel;
	internal_->uiPanel->setupUi(internal_->rootPanel);

	internal_->rootPlayer = new QWidget();
	//internal_->uiPlayer = new Ui::PlayerUi;
	//internal_->uiPlayer->setupUi(internal_->rootPlayer);

	internal_->LinkAssets();

	internal_->ptrAppCore->NotifyLoadingTip(AppCore::Translate("ui.loadingtip.init_socket"));
	internal_->wsServer = new UnityWebSocketServer();
	if (0 == internal_->wsServer->Initialize())
	{
		internal_->ptrAppCore->NotifyLoadingTip(AppCore::Translate("ui.loadingtip.init_websocket_success"));
		LOGX_INFO(XC_PluginUnity, XL_HIGH_PluginUnity, "Initialize unity websocket success");
	}
	else
	{
		internal_->ptrAppCore->NotifyLoadingTip(AppCore::Translate("ui.loadingtip.init_websocket_fail"));
		LOGX_ERROR(XC_PluginUnity, XL_HIGH_PluginUnity, "Initialize unity websocket fail");
		return;
	}

	Dispatcher::Subscribe<UnityActions::WSMessageToUnityReceiver, UnityActions::WSMessageToUnityAction>();
	//Dispatcher::Subscribe<UnityActions::RunPlayerReceiver, UnityActions::RunPlayerAction>();

	UnityActions::API::DispatchAll();
	
	HandleBus<PluginUnityInternal>::Route<UnityActions::WSMessageToUnityReceiver>(internal_, &PluginUnityInternal::HandleMessageToUnity);
	HandleBus<PluginUnityInternal>::Route<UnityActions::RunPlayerReceiver>(internal_, &PluginUnityInternal::HandleRunPlayer);


#ifndef _DEBUG
	internal_->webView = new QWebEngineView();
	internal_->webView->setAcceptDrops(false);
#endif
}

//-----------------------------------------------------------------------
void PluginUnity::Attach()
{
}

//-----------------------------------------------------------------------
void PluginUnity::Show()
{
}

//-----------------------------------------------------------------------
void PluginUnity::Hide()
{
}

//-----------------------------------------------------------------------
void PluginUnity::Detach()
{
}

//-----------------------------------------------------------------------
void PluginUnity::Release()
{
#ifndef _DEBUG
	internal_->webView->stop();
#endif
	SAFE_DELETE(internal_->webView);
	SAFE_DELETE(internal_->player);
	SAFE_DELETE(internal_->rootPanel);
	SAFE_DELETE(internal_->rootPlayer);
	SAFE_DELETE(internal_->uiPanel);
	SAFE_DELETE(internal_->uiPlayer);
	SAFE_DELETE(internal_);
}

void PluginUnity::LoadH5()
{
#ifndef _DEBUG
	QDir dir = QDir::current();
	QString webviewIndex = dir.absoluteFilePath("player/editor_web/index.html");
	QUrl embeddedURI = QUrl::fromLocalFile(webviewIndex);
	internal_->webView->setUrl(embeddedURI);
	internal_->ptrAppCore->NotifyLoadingTip(AppCore::Translate(embeddedURI.toString()));

#else
	//选择OK需要用用caddy加上浏览器。选择cacle的话直接进首页，不加载Unity
	if (QMessageBox::StandardButton::Ok != QMessageBox::information(0, "", "1.enter runpath\\player\\editor_web \n2. open cmd.exe and run caddy.exe \n3. use web browser access localhost:33168\n\nSelect cancel, will not load unity", QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel))
	{
		internal_->ptrAppCore->EnterHomePage();
		internal_->ptrAppCore->FinishModelTask();
		internal_->ptrAppCore->Maximum();
	}
#endif // !DEBUG
}

//-----------------------------------------------------------------------
QWidget* PluginUnity::GetGameView()
{
#ifndef _DEBUG
	internal_->ptrAppCore->NotifyLoadingTip("GetGameView");
	return internal_->webView;
#else
	return new QWidget();
#endif
}
