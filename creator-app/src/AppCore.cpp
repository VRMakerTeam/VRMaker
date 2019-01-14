#include <QApplication>
#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QResource>
#include <QJsonDocument>
#include <QLabel>
#include <QProcess>
#include <UiLoader.h>
#include <QDebug>
#include <QDir>
#include <QMovie>
#include <QBuffer>
#include <QCryptographicHash>
#include <QMessageBox>
#include "Settings.h"
#include "PluginBean/PluginBean.h"
#include "PluginUnity/PluginUnity.h"
#include "PluginScreenplay/pluginScreenplay.h"
#include "PluginDiagram/PluginDiagram.h"
#include "PluginStore/PluginStore.h"
#include "PluginBucket/PluginBucket.h"
#include "PluginDiagram/PluginDiagram.h"
#include "PluginFactory.h"
#include "Dispatcher.h"
#include "AppActions.h"
#include "ConfigUtil.h"
#include "PathUtil.h"
#include "SVM.h"
#include "Utils.h"
#include "AppCore.h"

DECLARE_PRODUCT_CLASS(IPlugin, PluginBean);
DECLARE_PRODUCT_CLASS(IPlugin, PluginStore);
DECLARE_PRODUCT_CLASS(IPlugin, PluginBucket);
DECLARE_PRODUCT_CLASS(IPlugin, PluginUnity);
DECLARE_PRODUCT_CLASS(IPlugin, PluginScreenplay);
DECLARE_PRODUCT_CLASS(IPlugin, PluginDiagram);

REGISTER_PRODUCT_CLASS(IPlugin, PluginBean);
REGISTER_PRODUCT_CLASS(IPlugin, PluginStore);
REGISTER_PRODUCT_CLASS(IPlugin, PluginBucket);
REGISTER_PRODUCT_CLASS(IPlugin, PluginUnity);
REGISTER_PRODUCT_CLASS(IPlugin, PluginScreenplay);
REGISTER_PRODUCT_CLASS(IPlugin, PluginDiagram);

class AppCoreInternal
{
public:
	struct UIElement
	{
		QWidget* rootUI;
		QLayout* ptrTitlebarContainer;
		QLayout* ptrPanelContainer;
		QLayout* ptrToolbarContainer;
		QStackedWidget* ptrPages;
		QLabel* ptrLoadingMovie;
		QLabel* ptrLoadingTip;

		QStackedWidget* ptrTitlebarLeft;
		QStackedWidget* ptrTitlebarMiddle;
		QWidget* ptrTitlebarRight;

		QSpacerItem* ptrToolbarLeftSpace;
		QSpacerItem* ptrToolbarRightSpace;
	};

	UIElement uiElements;
	QWidget* ptrPanel;
	QWidget* ptrToolbar;

	QApplication* ptrApplication_;
	bool running;
	
	QHash<QString, IPlugin*> preloadPlugins;
	QHash<QString, IPlugin*> cachePlugins;
	static QHash< QString, QHash<QString, QString> > translator;

	IPlugin* ptrActivePlugin;

	void DetachPanel();
	void DetachToolbar();

	SVM* svm;
};

QHash< QString, QHash<QString, QString> > AppCoreInternal::translator;

//-----------------------------------------------------------------------
void AppCoreInternal::DetachPanel()
{
	if (NULL != ptrPanel)
	{
		uiElements.ptrPanelContainer->removeWidget(ptrPanel);
		//this will remove the UI
		ptrPanel->setParent(NULL);
		ptrPanel = NULL;
	}
}

//-----------------------------------------------------------------------
void AppCoreInternal::DetachToolbar()
{
	if (NULL != ptrToolbar)
	{
		uiElements.ptrToolbarContainer->removeWidget(ptrToolbar);
		//this will remove the UI
		ptrToolbar->setParent(NULL);
		ptrToolbar = NULL;
	}
}


AppCore::AppCore(QApplication* _application, QWidget* _rootUi)
{
	internal_ = new AppCoreInternal();
	internal_->ptrApplication_ = _application;
	internal_->uiElements.rootUI = _rootUi;
	internal_->running = false;
	
	internal_->uiElements.ptrTitlebarContainer = _rootUi->findChild<QWidget*>("__titlebar_container")->layout();
	internal_->uiElements.ptrPanelContainer = _rootUi->findChild<QWidget*>("__panel_container")->layout();
	internal_->uiElements.ptrToolbarContainer = _rootUi->findChild<QWidget*>("__toolbar_container")->layout();
	internal_->uiElements.ptrPages = _rootUi->findChild<QStackedWidget*>("__pages");
	internal_->uiElements.ptrLoadingMovie = _rootUi->findChild<QLabel*>("__loading_movie");
	internal_->uiElements.ptrLoadingTip = _rootUi->findChild<QLabel*>("__loading_tip");
	internal_->uiElements.ptrTitlebarLeft = _rootUi->findChild<QStackedWidget*>("__titlebar_left");
	internal_->uiElements.ptrTitlebarMiddle = _rootUi->findChild<QStackedWidget*>("__titlebar_middle");
	internal_->uiElements.ptrTitlebarRight = _rootUi->findChild<QWidget*>("__titlebar_right");
	//internal_->uiElements.ptrToolbarLeftSpace = _rootUi->findChild<QSpacerItem*>("__toolbar_root");
	//internal_->uiElements.ptrToolbarRightSpace = _rootUi->findChild<QSpacerItem*>("__toolbar_right_space");

	internal_->uiElements.ptrLoadingMovie->setMovie(new QMovie(":/app/__gif/loading"));
	internal_->ptrPanel = NULL;
	internal_->ptrToolbar = NULL;

	internal_->ptrActivePlugin = NULL;
}

void AppCore::Alert(const QString& _title, const QString& _message)
{
	QMessageBox::warning(NULL, _title, _message, QMessageBox::StandardButton::Ok);
}

void AppCore::Initialize()
{
	SVM::InitConsole();
	internal_->svm = new SVM();
	internal_->svm->Init();
}

//-----------------------------------------------------------------------
void AppCore::Run()
{
	if (internal_->running)
		return;
	internal_->running = true;

	internal_->svm->Run();

	NotifyLoadingTip(AppCore::Translate("ui.loadingtip.preload_plugins"));
	PreloadPlugins();
	NotifyLoadingTip("preload plugins finish");
}

void AppCore::Release()
{
	internal_->svm->Release();
	SAFE_DELETE(internal_->svm);

	foreach(IPlugin* plugin, internal_->cachePlugins.values())
	{
		plugin->Release();
	}
}

void AppCore::Maximum()
{
	QMainWindow* window = dynamic_cast<QMainWindow*>(internal_->uiElements.rootUI);
	window->showMaximized();
}

void AppCore::StartModelTask()
{
	internal_->uiElements.ptrPages->setCurrentIndex(0);
	internal_->uiElements.ptrLoadingTip->setText("");
	internal_->uiElements.ptrLoadingMovie->movie()->start();
}

void AppCore::FinishModelTask()
{
	internal_->uiElements.ptrPages->setCurrentIndex(1);
	internal_->uiElements.ptrLoadingMovie->movie()->stop();

}

//-----------------------------------------------------------------------
void AppCore::PreloadPlugins()
{
	NotifyLoadingTip(AppCore::Translate("ui.loadingtip.init_bucket"));
	IPlugin* bucketPlugin = PluginFactory::create("PluginBucket");
	bucketPlugin->Initialize(this);
	//IPlugin* storePlugin = PluginFactory::create("PluginStore");
	//storePlugin->Initialize(this);
	NotifyLoadingTip(AppCore::Translate("ui.loadingtip.init_bean"));
	IPlugin* beanPlugin = PluginFactory::create("PluginBean");
	beanPlugin->Initialize(this);

	NotifyLoadingTip(AppCore::Translate("ui.loadingtip.register_bean_source"));
	PluginScreenplay::RegisterToBeanSource();

	internal_->preloadPlugins.insert("PluginBucket", bucketPlugin);
	//internal_->preloadPlugins.insert("PluginStore", storePlugin);
	internal_->preloadPlugins.insert("PluginBean", beanPlugin);

	internal_->ptrActivePlugin = beanPlugin;

	StartModelTask();
	NotifyLoadingTip(AppCore::Translate("ui.loadingtip.init_diagram"));
	IPlugin* diagramPlugin = PluginFactory::create("PluginDiagram");
	diagramPlugin->Initialize(this);
	RegisterCachePlugin("PluginDiagram", diagramPlugin);

	IPlugin* unityPlugin = PluginFactory::create("PluginUnity");
	NotifyLoadingTip(AppCore::Translate("ui.loadingtip.init_canvas"));
	unityPlugin->Initialize(this);
	RegisterCachePlugin("PluginUnity", unityPlugin);

	IPlugin* screenPlayPlugin = PluginFactory::create("PluginScreenplay");
	NotifyLoadingTip(AppCore::Translate("ui.loadingtip.init_screenplay"));
	screenPlayPlugin->Initialize(this);
	RegisterCachePlugin("PluginScreenplay", screenPlayPlugin);

	NotifyLoadingTip(AppCore::Translate("ui.loadingtip.preload_plugins_success"));

	PluginUnity* pluginUnity = dynamic_cast<PluginUnity*>(unityPlugin);
	pluginUnity->LoadH5();
	NotifyLoadingTip(AppCore::Translate("ui.loadingtip.wait_canvas_ready"));
}

//-----------------------------------------------------------------------
void AppCore::AppendTitlebarLauncher(QWidget* _launcher)
{
	assert(_launcher);
	QHBoxLayout* layout = dynamic_cast<QHBoxLayout*>(internal_->uiElements.ptrTitlebarContainer);
	layout->insertWidget(layout->count()-1, _launcher);
}

//-----------------------------------------------------------------------
void AppCore::AttachPanel(QWidget* _root)
{
	DetachToolbar();

	assert(_root);
	internal_->ptrPanel = _root;
	internal_->uiElements.ptrPanelContainer->addWidget(internal_->ptrPanel);
}

//-----------------------------------------------------------------------
void AppCore::AttachToolbar(QWidget* _root, int _align /*=0*/)
{
	assert(_root);
	internal_->ptrToolbar = _root;
	internal_->uiElements.ptrToolbarContainer->addWidget(internal_->ptrToolbar);
	return;

	//left
	if (1 == _align)
	{
		internal_->uiElements.ptrToolbarLeftSpace->changeSize(0, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);
		internal_->uiElements.ptrToolbarRightSpace->changeSize(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	}
	//center
	else if (2 == _align)
	{
		internal_->uiElements.ptrToolbarLeftSpace->changeSize(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		internal_->uiElements.ptrToolbarRightSpace->changeSize(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	}
	//right
	else if (3 == _align)
	{
		internal_->uiElements.ptrToolbarLeftSpace->changeSize(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		internal_->uiElements.ptrToolbarRightSpace->changeSize(0, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);
	}
	else
	{
		internal_->uiElements.ptrToolbarLeftSpace->changeSize(0, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);
		internal_->uiElements.ptrToolbarRightSpace->changeSize(0, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);
	}
	
}

void AppCore::DetachToolbar()
{
	if (0 == internal_->ptrToolbar)
		return;

	internal_->uiElements.ptrToolbarContainer->removeWidget(internal_->ptrToolbar);
}

//-----------------------------------------------------------------------
void AppCore::EnterPluginPage()
{
	internal_->DetachPanel();
	internal_->DetachToolbar();

	internal_->uiElements.ptrTitlebarMiddle->setCurrentIndex(1);
	internal_->uiElements.ptrTitlebarLeft->setCurrentIndex(1);
}

//-----------------------------------------------------------------------
void AppCore::EnterHomePage()
{
	internal_->DetachPanel();
	internal_->DetachToolbar();
	internal_->uiElements.ptrTitlebarMiddle->setCurrentIndex(0);
	internal_->uiElements.ptrTitlebarLeft->setCurrentIndex(0);
	internal_->ptrActivePlugin->Attach();
}

//-----------------------------------------------------------------------
IPlugin* AppCore::FindPreloadPlugin(const char* _plugin)
{
	if (internal_->preloadPlugins.contains(_plugin))
		return internal_->preloadPlugins.value(_plugin);
	return NULL;
}

//-----------------------------------------------------------------------
IPlugin* AppCore::CreatePlugin(const char* _plugin)
{
	return PluginFactory::create(_plugin);
}

//-----------------------------------------------------------------------
void AppCore::LaunchPlugin(IPlugin* _plugin)
{
	internal_->DetachPanel();
	internal_->DetachToolbar();
	if (NULL != internal_->ptrActivePlugin)
		internal_->ptrActivePlugin->Detach();
	_plugin->Attach();
	internal_->ptrActivePlugin = _plugin;
}

int AppCore::RegisterCachePlugin(const char* _name, IPlugin* _plugin)
{
	if (internal_->cachePlugins.contains(_name))
		return 1;
	internal_->cachePlugins.insert(_name, _plugin);
	return 0;
}

IPlugin* AppCore::FindCachePlugin(const char* _name)
{
	if (internal_->cachePlugins.contains(_name))
		return internal_->cachePlugins.value(_name);
	return NULL;
}

void AppCore::RemoveCachePlugin(const char* _name)
{
	if (!internal_->cachePlugins.contains(_name))
		return;
	internal_->cachePlugins.remove(_name);
}

QWidget* AppCore::CreateUI(const QString& _uiFile)
{
	QFile file(QString("res/%1.ui").arg(_uiFile));
	if (!file.open(QIODevice::ReadOnly))
		return false;

	UiLoader loader;
	QWidget* w = loader.load(&file);
	file.close();
	return w;
}

void AppCore::LoadRCC(const QString& _resourceFile)
{
	QResource::registerResource(QString("res/%1.rcc").arg(_resourceFile));
}

void AppCore::LoadTS(const QString& _translatorFile)
{
	QFile file(QString("res/%1.ts").arg(_translatorFile));
	if (!file.open(QIODevice::ReadOnly))
		return;

	QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
	QJsonObject objRoot = jsonDoc.object();

	foreach(QString lang, objRoot.keys())
	{
		//if (!AppCoreInternal::translator.contains(lang))
		//	AppCoreInternal::translator.insert(lang, QHash<QString, QString>());

		QHash<QString, QString>& transMap = AppCoreInternal::translator[lang];
		QJsonObject objLang = objRoot.value(lang).toObject();
		foreach(QString sourceText, objLang.keys())
		{
			QString targetText = objLang.value(sourceText).toString();
			transMap[sourceText] = targetText;
		}
	}
}

QString AppCore::Translate(const QString& _txt)
{
	QString language = "";
	if (!ConfigUtil::FindString("language", language))
	{
		language = QLocale::system().name();
	}

	if (!AppCoreInternal::translator.contains(language))
		return _txt;

	const QHash<QString, QString>& values = AppCoreInternal::translator.value(language);
	if (!values.contains(_txt))
		return _txt;

	return values.value(_txt);
}


QString AppCore::Language()
{
	QString language = "";
	if (!ConfigUtil::FindString("language", language))
	{
		language = QLocale::system().name();
	}
	return language;
}

void AppCore::NotifyLoadingTip(const QString& _text)
{
	internal_->uiElements.ptrLoadingTip->setText(_text);
	QApplication::processEvents();

	static bool clear = false;
	if (!clear)
	{
		QFile::remove("./log.txt");
		clear = true;
	}

	QFile file("./log.txt");
	if (file.open(QIODevice::WriteOnly  | QIODevice::Append))
	{
		file.write(_text.toStdString().data());
		file.write("\r\n");
		file.flush();
		file.close();
	}
}

SVM* AppCore::GetSVM()
{
	return internal_->svm;
}

void AppCore::ChangeLanguage(int _languageIndex )
{
	QString language;
	switch (_languageIndex) {
		/// us
	case 0:
	default:
		language = "en_US";
		break;
		/// cn
	case 1:
		language = "zh_CN";
		break;

	case 2:
		language = "fr";
		break;

	case 3:
		language = "sp";
		break;

	case 4:
		language = "it";
		break;

	case 5:
		language = "jp";
		break;

	case 6:
		language = "kr";
		break;

	case 7:
		language = "pt";
		break;

	case 8:
		language = "de";
		break;

	case 9:
		language = "ru";
		break;
	}

	ConfigUtil::ChangeLanguage(language);	
	ConfigUtil::WriteConfig();
}


