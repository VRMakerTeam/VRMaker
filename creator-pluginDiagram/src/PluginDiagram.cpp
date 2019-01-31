#include "Utils.h"
#include "Dispatcher.h"
#include "HandleBus.hxx"
#include "AppCore.h"
#include "SVM.h"
#include "DiagramCompositeCore.h"
#include "DiagramCompositeGroup.h"
#include "DiagramCompositeTriggers.h"
#include "DiagramCompositeFlow.h"
#include "DiagramCompositeProperty.h"
#include "PluginDiagramLogger.h"
#include "PluginUnity/UnityActions.h"
#include "PluginUnity/UnityMessages.h"
#include "PluginBean/BeanActions.h"
#include "PluginDiagram/DiagramActions.h"
#include "PluginDiagram/DiagramModel.h"
#include "PluginDiagram/PluginDiagram.h"

class PluginDiagramInternal
{
public:
	Diagram::CompositeCore* compositeCore;
	Diagram::GroupComponent* groupComponent;
	Diagram::TriggersComponent* triggerComponent;
	Diagram::FlowComponent* flowComponent;
	Diagram::PropertyComponent* propertyComponent;

	void HandleMessageFromUnity(IAction* _action)
	{
		CAST_ACTION(UnityActions::WSMessageFromUnityAction, _action, action);
		LOGX_DEBUG(XC_PluginDiagram, XL_MEDIMU_PluginDiagram, QString("message from unity %1").arg(action->method));
		propertyComponent->ProcessMessageFromUnity(action->method, action->activity, action->param);
	}
};

//-----------------------------------------------------------------------
void PluginDiagram::Initialize(AppCore* _appCore)
{
	AppCore::LoadTS("pluginDiagram");
	QString language = _appCore->Language();
	if (language.compare("de") == 0) {
		AppCore::LoadRCC("pluginDiagram_de");
	}
	else if (language.compare("jp") == 0) {
		AppCore::LoadRCC("pluginDiagram_jp");
	}
	else if (language.compare("kr") == 0) {
		AppCore::LoadRCC("pluginDiagram_kr");
	}
	else if (language.compare("it") == 0) {
		AppCore::LoadRCC("pluginDiagram_it");
	}
	else if (language.compare("pt") == 0) {
		AppCore::LoadRCC("pluginDiagram_pt");
	}
	else if (language.compare("sp") == 0) {
		AppCore::LoadRCC("pluginDiagram_sp");
	}
	else if (language.compare("fr") == 0) {
		AppCore::LoadRCC("pluginDiagram_fr");
	}
	else if (language.compare("ru") == 0) {
		AppCore::LoadRCC("pluginDiagram_ru");
	}
	else if (language.compare("zh_CN") == 0) {
		AppCore::LoadRCC("pluginDiagram_zh_CN");
	}
	else {
		AppCore::LoadRCC("pluginDiagram");
	}


	DiagramModel::Runtime::InitializeModel();

	//Flow
	Dispatcher::Subscribe<DiagramActions::ToggleFlowToolbarReceiver, DiagramActions::ToggleFlowToolbarAction>();
	Dispatcher::Subscribe<DiagramActions::CleanFlowReceiver, DiagramActions::CleanFlowAction>();
	Dispatcher::Subscribe<DiagramActions::RefreshFlowReceiver, DiagramActions::RefreshFlowAction>();

	//Block
	Dispatcher::Subscribe<DiagramActions::RegisterBlockReceiver, DiagramActions::RegisterBlockAction>();
	Dispatcher::Subscribe<DiagramActions::NewBlockReceiver, DiagramActions::NewBlockAction>();
	Dispatcher::Subscribe<DiagramActions::DeleteBlockReceiver, DiagramActions::DeleteBlockAction>();
	Dispatcher::Subscribe<DiagramActions::BlockRegisterPropertyReceiver, DiagramActions::BlockRegisterPropertyAction>();
	Dispatcher::Subscribe<DiagramActions::BlockUiAddTextReceiver, DiagramActions::BlockUiAddTextAction>();
	Dispatcher::Subscribe<DiagramActions::BlockUiBindPropertyReceiver, DiagramActions::BlockUiBindPropertyAction>();
	Dispatcher::Subscribe<DiagramActions::BlockUiUseImageReceiver, DiagramActions::BlockUiUseImageAction>();

	Dispatcher::Subscribe<DiagramActions::FilterGroupReceiver, DiagramActions::FilterGroupAction>();
	Dispatcher::Subscribe<DiagramActions::FilterExtendReceiver, DiagramActions::FilterExtendAction>();

	//property
	Dispatcher::Subscribe<DiagramActions::PropertyBindCallbackReceiver, DiagramActions::PropertyBindCallbackAction>();
	Dispatcher::Subscribe<DiagramActions::PropertyUiBindPickerReceiver, DiagramActions::PropertyUiBindPickerAction>();
	Dispatcher::Subscribe<DiagramActions::PropertyUiBindRecorderReceiver, DiagramActions::PropertyUiBindRecorderAction>();
	Dispatcher::Subscribe<DiagramActions::PropertyUiBindLineEditReceiver, DiagramActions::PropertyUiBindLineEditAction>();
	Dispatcher::Subscribe<DiagramActions::PropertyUiBindNumberSpinReceiver, DiagramActions::PropertyUiBindNumberSpinAction>();
	Dispatcher::Subscribe<DiagramActions::PropertyUiBindIntegerSpinReceiver, DiagramActions::PropertyUiBindIntegerSpinAction>();
	Dispatcher::Subscribe<DiagramActions::PropertyUiBindButtonReceiver, DiagramActions::PropertyUiBindButtonAction>();
	Dispatcher::Subscribe<DiagramActions::PropertyUiUpdatePropertyReceiver, DiagramActions::PropertyUiUpdatePropertyAction>();

	Dispatcher::Subscribe<DiagramActions::PropertyUiBindTextEditReceiver, DiagramActions::PropertyUiBindTextEditAction>();
	Dispatcher::Subscribe<DiagramActions::PropertyUiBindComboBoxReceiver, DiagramActions::PropertyUiBindComboBoxAction>();
	Dispatcher::Subscribe<DiagramActions::PropertyUiBindColorPickerReceiver, DiagramActions::PropertyUiBindColorPickerAction>();

	//Trigger
	Dispatcher::Subscribe<DiagramActions::NewTriggerReceiver, DiagramActions::NewTriggerAction>();
	Dispatcher::Subscribe<DiagramActions::DeleteTriggerReceiver, DiagramActions::DeleteTriggerAction>();
	Dispatcher::Subscribe<DiagramActions::RefreshTriggersReceiver, DiagramActions::RefreshTriggersAction>();
	Dispatcher::Subscribe<DiagramActions::ToggleTrigersTrashReceiver, DiagramActions::ToggleTriggersTrashAction>();

	Dispatcher::Subscribe<DiagramActions::AddPipeReceiver, DiagramActions::AddPipeAction>();
	Dispatcher::Subscribe<DiagramActions::RemovePipeReceiver, DiagramActions::RemovePipeAction>();
	Dispatcher::Subscribe<DiagramActions::OpenBlockPropertyPanelReceiver, DiagramActions::OpenBlockPropertyPanelAction>();
	Dispatcher::Subscribe<DiagramActions::OpenTriggerPropertyPanelReceiver, DiagramActions::OpenTriggerPropertyPanelAction>();
	Dispatcher::Subscribe<DiagramActions::ClosePropertyPanelReceiver, DiagramActions::ClosePropertyPanelAction>();
	Dispatcher::Subscribe<DiagramActions::MessageFromUnityReceiver, UnityActions::WSMessageFromUnityAction>();

	Dispatcher::Subscribe<DiagramActions::OnNewBeanReceiver, BeanActions::OnNewBeanAction>();
	Dispatcher::Subscribe<DiagramActions::OnDeleteBeanReceiver, BeanActions::OnDeleteBeanAction>();
	Dispatcher::Subscribe<DiagramActions::OnReloadBeanReceiver, BeanActions::EditBeanAction>();

	internal_ = new PluginDiagramInternal();
	internal_->compositeCore = new Diagram::CompositeCore();
	internal_->compositeCore->SetAppCore(_appCore);

	internal_->groupComponent = new Diagram::GroupComponent(new Diagram::GroupView());
	internal_->groupComponent->Initialize(internal_->compositeCore);

	internal_->triggerComponent = new Diagram::TriggersComponent(new Diagram::TriggersView());
	internal_->triggerComponent->Initialize(internal_->compositeCore);

	internal_->flowComponent = new Diagram::FlowComponent(new Diagram::FlowView());
	internal_->flowComponent->Initialize(internal_->compositeCore);

	internal_->propertyComponent = new Diagram::PropertyComponent(new Diagram::PropertyView());
	internal_->propertyComponent->Initialize(internal_->compositeCore);

	HandleBus<PluginDiagramInternal>::Route<DiagramActions::MessageFromUnityReceiver>(internal_, &PluginDiagramInternal::HandleMessageFromUnity);

	internal_->compositeCore->GetAppCore()->GetSVM()->Invoke("init_diagram");
}

//-----------------------------------------------------------------------
void PluginDiagram::Attach()
{
}

//-----------------------------------------------------------------------
void PluginDiagram::Show()
{
}

//-----------------------------------------------------------------------
void PluginDiagram::Hide()
{
}

//-----------------------------------------------------------------------
void PluginDiagram::Detach()
{
}

//-----------------------------------------------------------------------
void PluginDiagram::Release()
{
	SAFE_DELETE(internal_->compositeCore);
	SAFE_DELETE(internal_);
}

//-----------------------------------------------------------------------
QWidget* PluginDiagram::GetGroupUi()
{
	Diagram::GroupView* view = dynamic_cast<Diagram::GroupView*>(internal_->groupComponent->GetView());
	return view->GetRoot();
}

QWidget* PluginDiagram::GetTriggerUi()
{
	Diagram::TriggersView* view = dynamic_cast<Diagram::TriggersView*>(internal_->triggerComponent->GetView());
	return view->GetRoot();
}

//-----------------------------------------------------------------------
QWidget* PluginDiagram::GetFlowUi()
{
	Diagram::FlowView* view = dynamic_cast<Diagram::FlowView*>(internal_->flowComponent->GetView());
	return view->GetRoot();
}

//-----------------------------------------------------------------------
QWidget* PluginDiagram::GetPropertyUi()
{
	Diagram::PropertyView* view = dynamic_cast<Diagram::PropertyView*>(internal_->propertyComponent->GetView());
	return view->GetRoot();
}

