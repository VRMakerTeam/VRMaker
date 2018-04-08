#include <QResource>
#include <QCheckBox>
#include "Utils.h"
#include "AppCore.h"
#include "Dispatcher.h"
#include "HandleBus.hxx"
#include "MemDB.h"
#include "PluginBean/BeanModel.h"
#include "PluginBean/BeanActions.h"
#include "BeanCompositeCore.h"
#include "BeanCompositePanel.h"
#include "PluginBean/PluginBean.h"

class BeanLauncherUI
{
public:
	QWidget* root;
	QCheckBox* launch;
};

class PluginBeanInternal
{
public:
	Bean::CompositeCore* compositeCore;
	Bean::PanelComponent* panelComponent;

	BeanLauncherUI* launcherUI;

	void HandleEditBean(IAction* _action)
	{
		IPlugin* plugin = compositeCore->GetAppCore()->FindCachePlugin("PluginScreenplay");
		assert(plugin);
		plugin->Attach();
	}
};

//-----------------------------------------------------------------------
void PluginBean::Initialize(AppCore* _appCore)
{
	AppCore::LoadTS("pluginBean");
	AppCore::LoadRCC("pluginBean");

	BeanModel::Runtime::InitializeModel();

	internal_ = new PluginBeanInternal();

	internal_->compositeCore = new Bean::CompositeCore();
	internal_->compositeCore->SetAppCore(_appCore);

	internal_->panelComponent = new Bean::PanelComponent(new Bean::PanelView());
	internal_->panelComponent->Initialize(internal_->compositeCore);

	internal_->launcherUI = new BeanLauncherUI();
	internal_->launcherUI->root = AppCore::CreateUI("beanLauncher");

	internal_->launcherUI->launch = internal_->launcherUI->root->findChild<QCheckBox*>("__launch");

	internal_->compositeCore->GetAppCore()->AppendTitlebarLauncher(internal_->launcherUI->root);
	QObject::connect(internal_->launcherUI->launch, &QCheckBox::clicked, [=](bool _clicked) {
		if (!_clicked)
		{
			internal_->launcherUI->launch->setChecked(true);
			return;
		}
		internal_->compositeCore->GetAppCore()->LaunchPlugin(this);
		//NEW_ACTION(BeanActions::ParseDiskAction, action);
		//INVOKE_ACTION(BeanActions::ParseDiskAction, action);
	});

	Dispatcher::Subscribe<BeanActions::ParseDiskReceiver, BeanActions::ParseDiskAction>();
	Dispatcher::Subscribe<BeanActions::OpenPageReceiver, BeanActions::OpenPageAction>();
	// Picker 
	Dispatcher::Subscribe<BeanActions::RegisterSourceReceiver, BeanActions::RegisterSourceAction>();
	Dispatcher::Subscribe<BeanActions::SelectSourceReceiver, BeanActions::SelectSourceAction>();
	// Dashboard
	Dispatcher::Subscribe<BeanActions::NewBeanReceiver, BeanActions::NewBeanAction>();
	Dispatcher::Subscribe<BeanActions::DeleteBeanReceiver, BeanActions::DeleteBeanAction>();
	Dispatcher::Subscribe<BeanActions::ModifyBeanReceiver, BeanActions::ModifyBeanAction>();
	Dispatcher::Subscribe<BeanActions::EditBeanReceiver, BeanActions::EditBeanAction>();
	Dispatcher::Subscribe<BeanActions::DumpBeanReceiver, BeanActions::DumpBeanAction>();
	Dispatcher::Subscribe<BeanActions::PlayBeanReceiver, BeanActions::PlayBeanAction>();
	Dispatcher::Subscribe<BeanActions::PublishBeanReceiver, BeanActions::PublishBeanAction>();
	Dispatcher::Subscribe<BeanActions::InstallBeanReceiver, BeanActions::InstallBeanAction>();
	Dispatcher::Subscribe<BeanActions::ExportBeanReceiver, BeanActions::ExportBeanAction>();
	Dispatcher::Subscribe<BeanActions::SelectBeanReceiver, BeanActions::SelectBeanAction>();
	Dispatcher::Subscribe<BeanActions::ToggleBeanEditorReceiver, BeanActions::ToggleBeanEditorAction>();

	HandleBus<PluginBeanInternal>::Route<BeanActions::EditBeanReceiver>(internal_, &PluginBeanInternal::HandleEditBean);

	NEW_ACTION(BeanActions::ParseDiskAction, action);
	INVOKE_ACTION(BeanActions::ParseDiskAction, action);
}

//-----------------------------------------------------------------------
void PluginBean::Attach()
{
	DYN_CAST(Bean::PanelView, internal_->panelComponent->GetView(), view);
	internal_->compositeCore->GetAppCore()->AttachPanel(view->GetRoot());
	internal_->launcherUI->launch->setChecked(true);
}

//-----------------------------------------------------------------------
void PluginBean::Show()
{
	internal_->panelComponent->GetView()->Show();
}

//-----------------------------------------------------------------------
void PluginBean::Hide()
{
	internal_->panelComponent->GetView()->Hide();
}

//-----------------------------------------------------------------------
void PluginBean::Detach()
{
	internal_->launcherUI->launch->setChecked(false);
}

//-----------------------------------------------------------------------
void PluginBean::Release()
{
	SAFE_DELETE(internal_->compositeCore);
	SAFE_DELETE(internal_->panelComponent);
	SAFE_DELETE(internal_);
}

