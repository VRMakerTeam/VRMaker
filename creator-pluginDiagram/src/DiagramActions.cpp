#include "HandleBus.hxx"
#include "DiagramCompositeGroup.h"
#include "DiagramCompositeFlow.h"
#include "DiagramCompositeTriggers.h"
#include "DiagramCompositeTrigger.h"
#include "DiagramCompositeProperty.h"
#include "DiagramCompositePipe.h"
#include "PropertyUiTemplateBuilder.h"
#include "PluginDiagramLogger.h"
#include "PluginBean/BeanModel.h"
#include "PluginBean/BeanActions.h"
#include "PluginUnity/UnityActions.h"
#include "BlockUiTemplateBuilder.h"
#include "PluginDiagram/PluginDiagram.h"
#include "PluginDiagram/DiagramModel.h"
#include "PluginDiagram/DiagramActions.h"

//-----------------------------------------------------------------------
void DiagramActions::FilterGroupReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::GroupComponent>::Handle<DiagramActions::FilterGroupReceiver>(_action);
}

//-----------------------------------------------------------------------
void DiagramActions::FilterExtendReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::GroupComponent>::Handle<DiagramActions::FilterExtendReceiver>(_action);
}

//-----------------------------------------------------------------------
void DiagramActions::AddPipeReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::FlowComponent>::Handle<DiagramActions::AddPipeReceiver>(_action);
}

//-----------------------------------------------------------------------
void DiagramActions::OpenBlockPropertyPanelReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::PropertyComponent>::Handle<DiagramActions::OpenBlockPropertyPanelReceiver>(_action);
}

void DiagramActions::OpenTriggerPropertyPanelReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::PropertyComponent>::Handle<DiagramActions::OpenTriggerPropertyPanelReceiver>(_action);
}

void DiagramActions::ClosePropertyPanelReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::PropertyComponent>::Handle<DiagramActions::ClosePropertyPanelReceiver>(_action);
}

//-----------------------------------------------------------------------
void DiagramActions::MessageFromUnityReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginDiagram, XL_LOW_PluginDiagram, "receiver is triggered");
	HandleBus<PluginDiagramInternal>::Handle<DiagramActions::MessageFromUnityReceiver>(_action);
}

void DiagramActions::RemovePipeReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::FlowComponent>::Handle<DiagramActions::RemovePipeReceiver>(_action);
}

void DiagramActions::NewBlockReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::PipeComponent>::Handle<DiagramActions::NewBlockReceiver>(_action);
}

void DiagramActions::DeleteBlockReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::PipeComponent>::Handle<DiagramActions::DeleteBlockReceiver>(_action);
}


void DiagramActions::RefreshPipeReceiver::Receive(IAction* _action)
{

}

void DiagramActions::ToggleFlowToolbarReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::FlowComponent>::Handle<DiagramActions::ToggleFlowToolbarReceiver>(_action);
}

void DiagramActions::CleanFlowReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::FlowComponent>::Handle<DiagramActions::CleanFlowReceiver>(_action);
}

void DiagramActions::RefreshFlowReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::FlowComponent>::Handle<DiagramActions::RefreshFlowReceiver>(_action);
}

void DiagramActions::OnReloadBeanReceiver::Receive(IAction* _action)
{
	CAST_ACTION(BeanActions::EditBeanAction, _action, action);
	//close flow toolbar
	{
		NEW_ACTION(ToggleFlowToolbarAction, action);
		action->visible = false;
		INVOKE_ACTION(ToggleFlowToolbarAction, action);
	}
	//clean flow
	{
		NEW_ACTION(CleanFlowAction, action);
		INVOKE_ACTION(CleanFlowAction, action);
	}
	//Close Property
	{
		NEW_ACTION(ClosePropertyPanelAction, action);
		INVOKE_ACTION(ClosePropertyPanelAction, action);
	}

	//根据bean的扩展包过滤行为
	MemDB* memDB = BeanModel::Persistent::GetMemDB(action->guid);
	QList<QString> extends = BeanModel::Persistent::Sets::Query::bean_extends(memDB);
	NEW_ACTION(DiagramActions::FilterExtendAction, filterAction);
	foreach(QString extend, extends)
	{
		filterAction->extends.append(extend);
	}
	INVOKE_ACTION(DiagramActions::FilterExtendAction, filterAction);

	HandleBus<Diagram::TriggersComponent>::Handle<DiagramActions::OnReloadBeanReceiver>(_action);
}

void DiagramActions::NewTriggerReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::TriggersComponent>::Handle<DiagramActions::NewTriggerReceiver>(_action);
}

void DiagramActions::DeleteTriggerReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::TriggersComponent>::Handle<DiagramActions::DeleteTriggerReceiver>(_action);
}

void DiagramActions::RefreshTriggersReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::TriggersComponent>::Handle<DiagramActions::RefreshTriggersReceiver>(_action);
}

void DiagramActions::ToggleTrigersTrashReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::TriggerComponent>::Handle<DiagramActions::ToggleTrigersTrashReceiver>(_action);
}

void DiagramActions::OnNewBeanReceiver::Receive(IAction* _action)
{
	CAST_ACTION(BeanActions::OnNewBeanAction, _action, action);
	MemDB* persistentDB = BeanModel::Persistent::GetMemDB(action->guid);

	// 将beand的所有源加入到扩展列表中，在action列表的组筛选时需要用到
	QStringList extends;
	foreach(QString extend, BeanModel::Persistent::Sets::Query::bean_extends(persistentDB))
		extends.append(extend);

	NEW_ACTION(FilterExtendAction, filterAction);
	filterAction->extends = extends;
	INVOKE_ACTION(FilterExtendAction, filterAction);

	MemDB* db = DiagramModel::Persistent::GetMemDB(action->guid);
	DiagramModel::Persistent::KV::Update::diagram_triggers_index(db, 1);
}

void DiagramActions::OnDeleteBeanReceiver::Receive(IAction* _action)
{
	NEW_ACTION(UnityActions::API::CleanAction, action);
	INVOKE_ACTION(UnityActions::API::CleanAction, action);
}

void DiagramActions::RegisterBlockReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::GroupComponent>::Handle<DiagramActions::RegisterBlockReceiver>(_action);
}

void DiagramActions::BlockUiAddTextReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::BlockUiAddTextAction, _action, action);
	BlockUiArchetype* builder = BlockUiArchetype::Find(action->group, action->tpl);
	if (NULL == builder)
		return;
	builder->AddText(action->text);
}

void DiagramActions::BlockUiBindPropertyReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::BlockUiBindPropertyAction, _action, action);
	BlockUiArchetype* builder = BlockUiArchetype::Find(action->group, action->tpl);
	if (NULL == builder)
		return;
	builder->BindProperty(action->property);
}

void DiagramActions::BlockRegisterPropertyReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::BlockRegisterPropertyAction, _action, action);
	BlockUiArchetype* builder = BlockUiArchetype::Find(action->group, action->tpl);
	if (NULL == builder)
		return;
	builder->RegisterProperty(action->property, action->defaultValue);
}

void DiagramActions::BlockUiUseImageReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::BlockUiUseImageAction, _action, action);
	BlockUiArchetype* builder = BlockUiArchetype::Find(action->group, action->tpl);
	if (NULL == builder)
		return;
	builder->UseImage(action->image);
}

void DiagramActions::PropertyUiBindPickerReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::PropertyUiBindPickerAction, _action, action);
	PropertyUiBuilder::BindPicker(action->group, action->tpl, action->property, action->butText, action->filter);
}

void DiagramActions::PropertyUiBindLineEditReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::PropertyUiBindLineEditAction, _action, action);
	PropertyUiBuilder::BindLineEdit(action->group, action->tpl, action->property, action->label);
}

void DiagramActions::PropertyUiBindNumberSpinReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::PropertyUiBindNumberSpinAction, _action, action);
	PropertyUiBuilder::BindNumberSpin(action->group, action->tpl, action->property, action->label, action->min, action->max);
}

void DiagramActions::PropertyUiBindIntegerSpinReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::PropertyUiBindIntegerSpinAction, _action, action);
	PropertyUiBuilder::BindIntegerSpin(action->group, action->tpl, action->property, action->label, action->min, action->max);
}

void DiagramActions::PropertyUiBindRecorderReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::PropertyUiBindRecorderAction, _action, action);
	PropertyUiBuilder::BindRecorder(action->group, action->tpl, action->property);
}

void DiagramActions::PropertyUiBindButtonReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::PropertyUiBindButtonAction, _action, action);
	PropertyUiBuilder::BindButton(action->group, action->tpl, action->label, action->onClick);
}

void DiagramActions::PropertyBindCallbackReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::PropertyBindCallbackAction, _action, action);
	PropertyUiBuilder::BindCallback(action->group, action->tpl, action->method, action->callback);
}

void DiagramActions::PropertyUiUpdatePropertyReceiver::Receive(IAction* _action)
{
	HandleBus<Diagram::PropertyComponent>::Handle<DiagramActions::PropertyUiUpdatePropertyReceiver>(_action);
}



void DiagramActions::PropertyUiBindTextEditReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::PropertyUiBindTextEditAction, _action, action);
	PropertyUiBuilder::BindTextEdit(action->group, action->tpl, action->property, action->label);
}


void DiagramActions::PropertyUiBindComboBoxReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::PropertyUiBindComboBoxAction, _action, action);
	PropertyUiBuilder::BindCombobox(action->group, action->tpl, action->property, action->label, action->lis);

}

void DiagramActions::PropertyUiBindColorPickerReceiver::Receive(IAction* _action)
{
	CAST_ACTION(DiagramActions::PropertyUiBindColorPickerAction, _action, action);
	PropertyUiBuilder::BindColorPicker(action->group, action->tpl, action->property, action->label, action->r, action->g, action->b, action->a);
}


