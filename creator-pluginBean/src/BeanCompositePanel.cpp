#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QStandardPaths>
#include <QDir>
#include <QStackedWidget>
#include <QWidget>
#include <QLayout>
#include "AppCore.h"
#include "Settings.h"
#include "Utils.h"
#include "NotifyBox.h"
#include "PathUtil.h"
#include "MemDBUtil.h"
#include "ClickWidget.h"
#include "Dispatcher.h"
#include "HandleBus.hxx"
#include "HttpUtil.h"
#include "ConfigUtil.h"
#include "DeviceUtil.h"
#include "AppUtil.h"
#include "Settings.h"
#include "BeanNotifyContent.h"
#include "PluginBean/BeanActions.h"
#include "PluginBeanLogger.h"
#include "PluginBean/BeanModel.h"
#include "BeanCompositeCore.h"
#include "BeanCompositeCard.h"
#include "BeanCompositeWall.h"
#include "BeanCompositePicker.h"
#include "BeanCompositeToolbar.h"
#include "BeanCompositePanel.h"

namespace Bean
{
	class PanelUI
	{
	public:
		~PanelUI();
		QWidget* root;
		QStackedWidget* pages;
		QWidget* wallRoot;
		QWidget* pickerRoot;
	};
}


Bean::PanelUI::~PanelUI()
{
	SAFE_DELETE(root);
}
//-----------------------------------------------------------------------

Bean::PanelView::~PanelView()
{
	SAFE_DELETE(ui_);
}

//-----------------------------------------------------------------------
void Bean::PanelView::setupUI()
{
	ui_ = new PanelUI();
	ui_->root = AppCore::CreateUI("beanPanel");

	ui_->pages = ui_->root->findChild<QStackedWidget*>("__pages");
	ui_->wallRoot = ui_->root->findChild<QWidget*>("__rootWall");
	ui_->pickerRoot = ui_->root->findChild<QWidget*>("__rootPicker");

	ui_->pages->setCurrentIndex(0);
}

//-----------------------------------------------------------------------
void Bean::PanelView::onUpdate(const QString& _tag)
{

}

void Bean::PanelView::onAttach(View* _view)
{
	{
		DYN_CAST(WallView, _view, view);
		if (NULL != view)
			ui_->wallRoot->layout()->addWidget(view->GetRoot());
	}

	{
		DYN_CAST(ToolbarView, _view, view);
		if (NULL != view)
			ui_->wallRoot->layout()->addWidget(view->GetRoot());
	}

	{
		DYN_CAST(PickerView, _view, view);
		if (NULL != view)
			ui_->pickerRoot->layout()->addWidget(view->GetRoot());

	}
}

//-----------------------------------------------------------------------
QWidget* Bean::PanelView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Bean::PanelView::OpenDashboardPage()
{
	ui_->pages->setCurrentIndex(0);
}

//-----------------------------------------------------------------------
void Bean::PanelView::OpenPickPage()
{
	ui_->pages->setCurrentIndex(1);
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
void Bean::PanelComponent::initialize(Composite::Core* _core)
{
	DYN_CAST(PanelView, view_, view);
	wallComponent_ = new Bean::WallComponent(new WallView());
	this->AddMVCChild(wallComponent_);

	toolbarComponent_ = new Bean::ToolbarComponent(new ToolbarView());
	this->AddMVCChild(toolbarComponent_);

	pickerComponent_ = new Bean::PickerComponent(new PickerView());
	this->AddMVCChild(pickerComponent_);

	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "Route Handlers");

	HandleBus<Bean::PanelComponent>::Route<BeanActions::OpenPageReceiver>(this, &Bean::PanelComponent::HandleOpenPage);
	HandleBus<Bean::PanelComponent>::Route<BeanActions::PublishBeanReceiver>(this, &Bean::PanelComponent::HandlePublishBean);
	HandleBus<Bean::PanelComponent>::Route<BeanActions::InstallBeanReceiver>(this, &Bean::PanelComponent::HandleInstallBean);

	DYN_CAST(Bean::CompositeCore, _core, core);
}

Bean::PanelComponent::PanelComponent(PanelView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
void Bean::PanelComponent::HandlePublishBean(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_MEDIMU_PluginBean, "HandlePublishBean");

	MemDB* memDB = BeanModel::Runtime::GetMemDB();
	QString beanGUID = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
	Bean::CardComponent* card = dynamic_cast<Bean::CardComponent*>(wallComponent_->FindChild(beanGUID));
	assert(card);
	card->StartPublish();
	QApplication::processEvents();

	NEW_ACTION(BeanActions::CompileBeanAction, action);
	action->guid = beanGUID;
	action->onFinish = std::bind(&Bean::PanelComponent::readyPublishBean, this, std::placeholders::_1);
	INVOKE_ACTION(BeanActions::CompileBeanAction, action);
}

//-----------------------------------------------------------------------
void Bean::PanelComponent::HandleInstallBean(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_MEDIMU_PluginBean, "HandleInstallBean");

	NotifyUtil::Popup(BeanNotifyContent::GetRoot());

	MemDB* memDB = BeanModel::Runtime::GetMemDB();
	QString beanGUID = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
	Bean::CardComponent* card = dynamic_cast<Bean::CardComponent*>(wallComponent_->FindChild(beanGUID));
	assert(card);

	NEW_ACTION(BeanActions::CompileBeanAction, action);
	action->guid = beanGUID;
	action->onFinish = std::bind(&Bean::PanelComponent::readyInstallBean, this, std::placeholders::_1);
	INVOKE_ACTION(BeanActions::CompileBeanAction, action);
}

//-----------------------------------------------------------------------
void Bean::PanelComponent::HandleOpenPage(IAction* _action)
{
	DYN_CAST(PanelView, view_, view);
	BeanActions::OpenPageAction* action = static_cast<BeanActions::OpenPageAction*>(_action);
	if (BeanActions::OpenPageAction::Page::DASHBOARD == action->page) {
		view->OpenDashboardPage();
	}
	else if (BeanActions::OpenPageAction::Page::ARCHETYPE == action->page) {
		view->OpenPickPage();
		pickerComponent_->SwitchArchetypePage();
	}
	else if (BeanActions::OpenPageAction::Page::CLONE == action->page) {
		view->OpenPickPage();
		pickerComponent_->SwitchClonePage();
	}

}

//-----------------------------------------------------------------------
void Bean::PanelComponent::onPublishFinish(const QByteArray& _bytes)
{
	qDebug() << _bytes;
	MemDB* memDB = BeanModel::Runtime::GetMemDB();
	QString beanGUID = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
	Bean::CardComponent* card = dynamic_cast<Bean::CardComponent*>(wallComponent_->FindChild(beanGUID));
	card->FinishPublish();
}

//-----------------------------------------------------------------------
void Bean::PanelComponent::onPublishErrro(const QString& _error)
{
	qDebug() << _error;
	MemDB* memDB = BeanModel::Runtime::GetMemDB();
	QString beanGUID = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
	Bean::CardComponent* card = dynamic_cast<Bean::CardComponent*>(wallComponent_->FindChild(beanGUID));
	card->AbortPublish();
}

//-----------------------------------------------------------------------
void Bean::PanelComponent::readyInstallBean(const QByteArray& _bytes)
{
	QString filepath = QString("%1/latest.vx").arg(PC_DEVICE_PERSISTENT_PATH);
	QFile file(filepath);
	if (file.open(QIODevice::WriteOnly))
	{
		file.write(_bytes);
		file.flush();
	}
	file.close();


	QTimer* timer = new QTimer();
	timer->setInterval(1000);
	QObject::connect(timer, &QTimer::timeout, [=] {
		int result = DeviceUtil::PushFile(APP_BUNDLE, filepath, "latest.vx");
		if (result == 0)
			BeanNotifyContent::SwitchOK();
		else if (1 == result)
			BeanNotifyContent::SwitchNoneDevice();
		else
			BeanNotifyContent::SwitchCopyFailed();

		NotifyUtil::ActivateOKButton();
		QFile::remove(filepath);
		timer->stop();
		delete timer;
	});
	timer->start();
}

//-----------------------------------------------------------------------
void Bean::PanelComponent::readyPublishBean(const QByteArray& _bytes)
{
	MemDB* memDB = BeanModel::Runtime::GetMemDB();
	QString beanGUID = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
	
	auto onPublishFinish = std::bind(&Bean::PanelComponent::onPublishFinish, this, std::placeholders::_1);
	auto onPublishError = std::bind(&Bean::PanelComponent::onPublishErrro, this, std::placeholders::_1);

	QString remote = "";
	if (!ConfigUtil::FindString("remote.location", remote))
			remote = "www";

	int uploadPort = 0;
	if (!ConfigUtil::FindInt("remote.port.upload", uploadPort))
		uploadPort = 8000;

	QString uploadUri = 0;
	if (!ConfigUtil::FindString("remote.uri.upload", uploadUri))
		uploadUri = "/storage/pushstring";

	QByteArray form;
	form.append("bucket=" + HttpUtil::Base64Encode(STORAGE_BUCKET));
	form.append("&key=" + HttpUtil::Base64Encode(beanGUID.toUtf8()));
	form.append("&value=" + HttpUtil::Base64Encode(_bytes));
	HttpUtil::POST(STORAGE_URI_PUSHSTRING, form, onPublishFinish, onPublishError);
}
