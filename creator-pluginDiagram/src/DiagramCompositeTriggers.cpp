#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLayout>
#include "HandleBus.hxx"
#include "Utils.h"
#include "AppCore.h"
#include "PluginDiagramLogger.h"
#include "DiagramCompositeCore.h"
#include "PluginUnity/UnityActions.h"
#include "PluginBean/BeanModel.h"
#include "PluginDiagram/DiagramActions.h"
#include "PluginDiagram/DiagramModel.h"
#include "DiagramCompositeTrigger.h"
#include "DiagramCompositeTriggers.h"


namespace Diagram
{
	class TriggersUI
	{
	public:
		QWidget* root;
		QWidget* container;
		QPushButton* btnNewGazeTrigger;
		QCheckBox* toggleTrash;
	};
}

void Diagram::TriggersView::setupUI()
{
	ui_ = new TriggersUI();
	ui_->root = AppCore::CreateUI("diagramTrigger");
	ui_->container = ui_->root->findChild<QWidget*>("__container");
	ui_->btnNewGazeTrigger = ui_->root->findChild<QPushButton*>("__btnNewGazeTrigger");
	ui_->toggleTrash = ui_->root->findChild<QCheckBox*>("__toggleTrash");
	

	QObject::connect(ui_->btnNewGazeTrigger, &QPushButton::clicked, [=] {
		NEW_ACTION(DiagramActions::NewTriggerAction, action);
		action->type = "sight";
		INVOKE_ACTION(DiagramActions::NewTriggerAction, action);
	});

	QObject::connect(ui_->toggleTrash, &QCheckBox::clicked, [=](bool _clicked) {
		NEW_ACTION(DiagramActions::ToggleTriggersTrashAction, action);
		action->toggle = _clicked;
		INVOKE_ACTION(DiagramActions::ToggleTriggersTrashAction, action);
	});
	

}

void Diagram::TriggersView::reset()
{
	
}

void Diagram::TriggersView::onUpdate(const QString& _tag)
{
	if (0 != _tag.compare(DiagramModel::Persistent::Tags::digram_triggers_update))
		return;

	while (QLayoutItem* item = ui_->container->layout()->itemAt(0))
	{
		ui_->container->layout()->removeItem(item);
	}

	for each (Composite::View* view in children_)
	{
		DYN_CAST(TriggerView, view, trigger);
		ui_->container->layout()->addWidget(trigger->GetRoot());
	}
}

void Diagram::TriggersView::ToogleNewGaze(bool _visiable)
{
	ui_->btnNewGazeTrigger->setVisible(_visiable);
}

QWidget* Diagram::TriggersView::GetRoot()
{
	return ui_->root;
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
Diagram::TriggersComponent::~TriggersComponent()
{
}

Diagram::TriggersComponent::TriggersComponent(TriggersView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
void Diagram::TriggersComponent::initialize(Composite::Core* _core)
{
	HandleBus<TriggersComponent>::Route<DiagramActions::NewTriggerReceiver>(this, &TriggersComponent::HandleNewTrigger);
	HandleBus<TriggersComponent>::Route<DiagramActions::DeleteTriggerReceiver>(this, &TriggersComponent::HandleDeleteTrigger);
	HandleBus<TriggersComponent>::Route<DiagramActions::OnReloadBeanReceiver>(this, &TriggersComponent::HandleOnReloadBean);
	HandleBus<TriggersComponent>::Route<DiagramActions::RefreshTriggersReceiver>(this, &TriggersComponent::HandleRefreshTriggers);

	ptrCore_ = _core;
}

QString Diagram::TriggersComponent::newTrigger(const QString& _type)
{
	TriggerComponent* trigger = TriggerComponent::New();
	trigger->Initialize(ptrCore_);
	this->AddMVCChild(trigger);

	MemDB* memDB = DiagramModel::Persistent::GetMemDB();
	DiagramModel::Persistent::Sets::Update::Add::diagram_triggers_guids(memDB, trigger->GetUUID());
	int index = DiagramModel::Persistent::KV::Query::diagram_triggers_index(memDB).toInt();
	DiagramModel::Persistent::KV::Update::diagram_triggers_index(memDB, index + 1);
	DiagramModel::Persistent::KV::Update::diagram_trigger_1_alias(memDB, trigger->GetUUID(), QString("%1").arg(index));
	DiagramModel::Persistent::KV::Update::diagram_trigger_1_type(memDB, trigger->GetUUID(), _type);
	DiagramModel::Persistent::KV::Update::diagram_gaze_1_icon(memDB, trigger->GetUUID(), 0);
	DiagramModel::Persistent::KV::Update::diagram_gaze_1_color_r(memDB, trigger->GetUUID(), 255);
	DiagramModel::Persistent::KV::Update::diagram_gaze_1_color_g(memDB, trigger->GetUUID(), 255);
	DiagramModel::Persistent::KV::Update::diagram_gaze_1_color_b(memDB, trigger->GetUUID(), 255);
	DiagramModel::Persistent::KV::Update::diagram_gaze_1_color_a(memDB, trigger->GetUUID(), 255);

	Composite::Store::Update(DiagramModel::Persistent::Tags::digram_triggers_update);

	NEW_ACTION(DiagramActions::OnTriggerNewAction, triggerAction);
	triggerAction->uuid = trigger->GetUUID();
	INVOKE_ACTION(DiagramActions::OnTriggerNewAction, triggerAction);


	return trigger->GetUUID();
}

void Diagram::TriggersComponent::deleteTrigger(const QString& _guid)
{
	this->RemoveMVCChild(_guid);

	MemDB* memDB = DiagramModel::Persistent::GetMemDB();
	DiagramModel::Persistent::Sets::Update::Remove::diagram_triggers_guids(memDB, _guid);

	Composite::Store::Update(DiagramModel::Persistent::Tags::digram_triggers_update);

	NEW_ACTION(DiagramActions::OnTriggerDeleteAction, triggerAction);
	triggerAction->uuid = _guid;
	INVOKE_ACTION(DiagramActions::OnTriggerDeleteAction, triggerAction);
}

//-----------------------------------------------------------------------
void Diagram::TriggersComponent::HandleNewTrigger(IAction* _action)
{
	CAST_ACTION(DiagramActions::NewTriggerAction, _action, action);

	QString guid = newTrigger(action->type);

	//在Unity中创建触发器
	{
		NEW_ACTION(UnityActions::API::NewTriggerAction, unityAction);
		unityAction->uuid = guid;
		unityAction->type = action->type;
		INVOKE_ACTION(UnityActions::API::NewTriggerAction, unityAction);
	}
	
	//刷新Unity中触发器的名称和图标
	{
		MemDB* memDB = DiagramModel::Persistent::GetMemDB();
		QString alias = DiagramModel::Persistent::KV::Query::diagram_trigger_1_alias(memDB, guid);
		QString type = DiagramModel::Persistent::KV::Query::diagram_trigger_1_type(memDB, guid);

		{
			NEW_ACTION(UnityActions::API::ModifyTriggerAction, unityAction);
			unityAction->uuid = guid;
			unityAction->type = action->type;
			unityAction->alias = alias;
			INVOKE_ACTION(UnityActions::API::ModifyTriggerAction, unityAction);
		}

		{
			int icon = DiagramModel::Persistent::KV::Query::diagram_gaze_1_icon(memDB, guid).toInt();
			NEW_ACTION(UnityActions::API::ModifyGazeIconAction, unityAction);
			unityAction->uuid = guid;
			unityAction->icon = icon;
			INVOKE_ACTION(UnityActions::API::ModifyGazeIconAction, unityAction);
		}

		{
			int r = DiagramModel::Persistent::KV::Query::diagram_gaze_1_color_r(memDB, guid).toInt();
			int g = DiagramModel::Persistent::KV::Query::diagram_gaze_1_color_g(memDB, guid).toInt();
			int b = DiagramModel::Persistent::KV::Query::diagram_gaze_1_color_b(memDB, guid).toInt();
			int a = DiagramModel::Persistent::KV::Query::diagram_gaze_1_color_a(memDB, guid).toInt();

			NEW_ACTION(UnityActions::API::ModifyGazeColorAction, unityAction);
			unityAction->uuid = guid;
			unityAction->r = r;
			unityAction->g = g;
			unityAction->b = b;
			unityAction->a = a;
			INVOKE_ACTION(UnityActions::API::ModifyGazeColorAction, unityAction);
		}
		
	}

}

void Diagram::TriggersComponent::HandleDeleteTrigger(IAction* _action)
{
	CAST_ACTION(DiagramActions::DeleteTriggerAction, _action, action);

	deleteTrigger(action->guid);

	//在Unity中删除触发器
	{
		NEW_ACTION(UnityActions::API::DeleteTriggerAction, unityAction);
		unityAction->uuid = action->guid;
		INVOKE_ACTION(UnityActions::API::DeleteTriggerAction, unityAction);
	}
}

void Diagram::TriggersComponent::HandleOnReloadBean(IAction* _action)
{
	MemDB* memDB = DiagramModel::Persistent::GetMemDB();
	QList<QString> extends = BeanModel::Persistent::Sets::Query::bean_extends(memDB);

	DYN_CAST(TriggersView, view_, view);
	view->ToogleNewGaze(extends.contains("VR"));

	foreach(QString guid, this->ListChildren())
	{
		this->RemoveMVCChild(guid);
	}
	Composite::Store::Update(DiagramModel::Persistent::Tags::digram_triggers_update);
}

void Diagram::TriggersComponent::HandleRefreshTriggers(IAction* _action)
{
	CAST_ACTION(DiagramActions::RefreshTriggersAction, _action, action);
	foreach(QString guid, this->ListChildren())
	{
		this->RemoveMVCChild(guid);
	}
	foreach(QString guid, action->triggersGUID)
	{
		TriggerComponent* trigger = TriggerComponent::New(guid);
		trigger->Initialize(NULL);
		this->AddMVCChild(trigger);
	}
	Composite::Store::Update(DiagramModel::Persistent::Tags::digram_triggers_update);
}
