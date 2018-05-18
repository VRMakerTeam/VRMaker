#include <QPushButton>
#include "AppCore.h"
#include "Utils.h"
#include "HandleBus.hxx"
#include "Dispatcher.h"
#include "PluginBeanLogger.h"
#include "BeanCompositeCore.h"
#include "PluginBean/BeanActions.h"
#include "PluginBean/BeanModel.h"
#include "BeanCompositePanel.h"
#include "BeanCompositeToolbar.h"

namespace Bean
{
	class ToolbarUI
	{
	public:
		QWidget* root;
		QPushButton* btnAddBean;
		QPushButton* btnDeleteBean;
		QPushButton* btnEditBean;
		QPushButton* btnInstallBean;
		QPushButton* btnPlayBean;
		QPushButton* btnPublishBean;
		QPushButton* btnExport;
	};
}

void Bean::ToolbarView::setupUI()
{
	ui_ = new ToolbarUI();
	ui_->root = AppCore::CreateUI("beanToolbar");

	ui_->btnAddBean = ui_->root->findChild<QPushButton*>("__btnAddBean");
	ui_->btnDeleteBean = ui_->root->findChild<QPushButton*>("__btnDeleteBean");
	ui_->btnEditBean = ui_->root->findChild<QPushButton*>("__btnEditBean");
	ui_->btnInstallBean = ui_->root->findChild<QPushButton*>("__btnInstallBean");
	ui_->btnPlayBean = ui_->root->findChild<QPushButton*>("__btnPlayBean");
	ui_->btnPublishBean = ui_->root->findChild<QPushButton*>("__btnPublishBean");
	ui_->btnExport = ui_->root->findChild<QPushButton*>("__btnExport");
	
	ui_->btnExport->setVisible(false);

	ToggleEditor(false);

	QObject::connect(ui_->btnAddBean, &QPushButton::clicked, [=] {
		LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "button add bean clicked");
		NEW_ACTION(BeanActions::OpenPageAction, action);
		action->page = BeanActions::OpenPageAction::ARCHETYPE;
		INVOKE_ACTION(BeanActions::OpenPageAction, action);
	});
	QObject::connect(ui_->btnEditBean, &QPushButton::clicked, [=] {
		LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "button edit bean clicked");
		MemDB* memDB = BeanModel::Runtime::GetMemDB();
		QString guid = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
		NEW_ACTION(BeanActions::EditBeanAction, action);
		action->guid = guid;
		INVOKE_ACTION(BeanActions::EditBeanAction, action);
	});
	QObject::connect(ui_->btnDeleteBean, &QPushButton::clicked, [=] {
		LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "button delete bean clicked");
		MemDB* memDB = BeanModel::Runtime::GetMemDB();
		QString guid = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
		NEW_ACTION(BeanActions::DeleteBeanAction, action);
		action->guid = guid;
		INVOKE_ACTION(BeanActions::DeleteBeanAction, action);
	});
	QObject::connect(ui_->btnPlayBean, &QPushButton::clicked, [=] {
		LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "button play bean clicked");
		MemDB* memDB = BeanModel::Runtime::GetMemDB();
		QString guid = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
		NEW_ACTION(BeanActions::PlayBeanAction, action);
		action->guid = guid;
		INVOKE_ACTION(BeanActions::PlayBeanAction, action);
	});
	QObject::connect(ui_->btnPublishBean, &QPushButton::clicked, [=] {
		LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "button publish bean clicked");
		MemDB* memDB = BeanModel::Runtime::GetMemDB();
		QString guid = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
		NEW_ACTION(BeanActions::PublishBeanAction, action);
		action->guid = guid;
		INVOKE_ACTION(BeanActions::PublishBeanAction, action);
	});
	QObject::connect(ui_->btnInstallBean, &QPushButton::clicked, [=] {
		LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "button install bean clicked");
		MemDB* memDB = BeanModel::Runtime::GetMemDB();
		QString guid = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
		NEW_ACTION(BeanActions::InstallBeanAction, action);
		action->guid = guid;
		INVOKE_ACTION(BeanActions::InstallBeanAction, action);
	});
	QObject::connect(ui_->btnExport, &QPushButton::clicked, [=] {
		LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "button export bean clicked");
		MemDB* memDB = BeanModel::Runtime::GetMemDB();
		QString guid = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
		NEW_ACTION(BeanActions::ExportBeanAction, action);
		action->guid = guid;
		INVOKE_ACTION(BeanActions::ExportBeanAction, action);
	});
}

QWidget* Bean::ToolbarView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Bean::ToolbarView::ToggleEditor(bool _visible)
{
	ui_->btnEditBean->setVisible(_visible);
	ui_->btnDeleteBean->setVisible(_visible);
	ui_->btnPlayBean->setVisible(_visible);
	ui_->btnPublishBean->setVisible(_visible);
	ui_->btnInstallBean->setVisible(_visible);
}
//////////////////////////////////////////////////////////////////////////
//
// Cmponent
//
//////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------
Bean::ToolbarComponent::ToolbarComponent(ToolbarView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
void Bean::ToolbarComponent::initialize(Composite::Core* _core)
{
	HandleBus<Bean::ToolbarComponent>::Route<BeanActions::ToggleBeanEditorReceiver>(this, &Bean::ToolbarComponent::HandleToggleEditorBar);
}

//-----------------------------------------------------------------------
void Bean::ToolbarComponent::HandleToggleEditorBar(IAction* _action)
{
	CAST_ACTION(BeanActions::ToggleBeanEditorAction, _action, action);
	DYN_CAST(ToolbarView, view_, view);
	view->ToggleEditor(action->visible);
}



