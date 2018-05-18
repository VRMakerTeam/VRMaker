#include <assert.h>
#include <QWidget>
#include <QRadioButton>
#include <QStackedWidget>
#include <QLayout>
#include "Utils.h"
#include "MemDB.h"
#include "HttpUtil.h"
#include "AppCore.h"
#include "Dispatcher.h"
#include "HandleBus.hxx"
#include "PluginScreenplaylogger.h"
#include "PluginDiagram/PluginDiagram.h"
#include "PluginScreenplay/ScreenplayActions.h"
#include "PluginScreenplay/ScreenplayModel.h"
#include "PluginUnity/UnityActions.h"
#include "PluginDiagram/DiagramActions.h"
#include "ScreenplayCompositeCore.h"
#include "ScreenplayCompositePlayer.h"
#include "ScreenplayCompositePreload.h"
#include "ScreenplayCompositeSlate.h"
#include "ScreenplayCompositeAgentInspector.h"
#include "ScreenplayCompositePanel.h"


namespace Screenplay
{
	class PanelUI
	{
	public:
		QWidget* root;
		QWidget* slotDiagramProperty;
		QWidget* slotAgentProperty;
		QWidget* slotDiagramFlow;
		QWidget* slotPlayer;
		QWidget* slotSlate;
		QWidget* slotPreload;
		QWidget* slotBlock;
		QWidget* slotTrigger;
		QStackedWidget* inspector;
		QRadioButton* tabSlate;
		QRadioButton* tabPreload;
		QRadioButton* tabBlock;
		QRadioButton* tabTrigger;
		QStackedWidget* pages;
	};
}

//-----------------------------------------------------------------------
Screenplay::PanelView::PanelView()
{
}

//-----------------------------------------------------------------------
Screenplay::PanelView::~PanelView()
{
	SAFE_DELETE(ui_);
}

//-----------------------------------------------------------------------
void Screenplay::PanelView::setupUI()
{
	ui_ = new PanelUI();
	ui_->root = AppCore::CreateUI("screenplayPanel");

	ui_->inspector = ui_->root->findChild<QStackedWidget*>("__inspector");
	ui_->slotDiagramProperty = ui_->root->findChild<QWidget*>("__slotDiagramProperty");
	ui_->slotAgentProperty = ui_->root->findChild<QWidget*>("__slotAgentProperty");
	ui_->slotDiagramFlow = ui_->root->findChild<QWidget*>("__slotDiagramFlow");
	ui_->slotPlayer = ui_->root->findChild<QWidget*>("__slotPlayer");
	ui_->slotSlate = ui_->root->findChild<QWidget*>("__slotSlate");
	ui_->slotPreload = ui_->root->findChild<QWidget*>("__slotPreload");
	ui_->slotBlock = ui_->root->findChild<QWidget*>("__slotBlock");
	ui_->slotTrigger = ui_->root->findChild<QWidget*>("__slotTrigger");
	ui_->tabBlock = ui_->root->findChild<QRadioButton*>("__blocksTab");
	ui_->tabPreload = ui_->root->findChild<QRadioButton*>("__preloadsTab");
	ui_->tabSlate = ui_->root->findChild<QRadioButton*>("__slatesTab");
	ui_->tabTrigger = ui_->root->findChild<QRadioButton*>("__triggersTab");
	ui_->pages = ui_->root->findChild<QStackedWidget*>("__pages");

	ui_->pages->setCurrentIndex(0);
	ui_->tabBlock->setEnabled(false);
	ui_->tabPreload->setEnabled(false);
	ui_->tabTrigger->setEnabled(false);

	QObject::connect(ui_->tabSlate, &QRadioButton::clicked, [=] {
		ui_->pages->setCurrentIndex(0);
	});
	QObject::connect(ui_->tabPreload, &QRadioButton::clicked, [=] {
		ui_->pages->setCurrentIndex(1);
	});
	QObject::connect(ui_->tabBlock, &QRadioButton::clicked, [=] {
		ui_->pages->setCurrentIndex(2);
	});
	QObject::connect(ui_->tabTrigger, &QRadioButton::clicked, [=] {
		ui_->pages->setCurrentIndex(3);
	});
}

void Screenplay::PanelView::onAttach(View* _view)
{
	{
		DYN_CAST(PlayerView, _view, view);
		if (NULL != view)
			ui_->slotPlayer->layout()->addWidget(view->GetRoot());
	}

	{
		DYN_CAST(SlateView, _view, view);
		if (NULL != view)
			ui_->slotSlate->layout()->addWidget(view->GetRoot());
	}

	{
		DYN_CAST(PreloadView, _view, view);
		if (NULL != view)
			ui_->slotPreload->layout()->addWidget(view->GetRoot());
	}

	{
		DYN_CAST(AgentInspectorView, _view, view);
		if (NULL != view)
			ui_->slotAgentProperty->layout()->addWidget(view->GetRoot());
	}
}

//-----------------------------------------------------------------------
QWidget* Screenplay::PanelView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Screenplay::PanelView::TogglePreloadRadio(bool _toggled)
{
	ui_->tabPreload->setEnabled(_toggled);
	if (!_toggled)
	{
		ui_->pages->setCurrentIndex(0);
		ui_->tabSlate->setChecked(true);
	}
}

void Screenplay::PanelView::ToggleBlockRadio(bool _toggled)
{
	ui_->tabBlock->setEnabled(_toggled);
	if (!_toggled)
	{
		ui_->pages->setCurrentIndex(0);
		ui_->tabSlate->setChecked(true);
	}
}


void Screenplay::PanelView::ToggleTriggerRadio(bool _toggled)
{
	ui_->tabTrigger->setEnabled(_toggled);
	if (!_toggled)
	{
		ui_->pages->setCurrentIndex(0);
		ui_->tabSlate->setChecked(true);
	}
}

void Screenplay::PanelView::AttachDiagramFlow(QWidget* _root)
{
	ui_->slotDiagramFlow->layout()->addWidget(_root);
}

void Screenplay::PanelView::AttachDiagramGroup(QWidget* _root)
{
	ui_->slotBlock->layout()->addWidget(_root);
}

void Screenplay::PanelView::AttachDiagramTrigger(QWidget* _root)
{
	ui_->slotTrigger->layout()->addWidget(_root);
}

void Screenplay::PanelView::AttachDiagramProperty(QWidget* _root)
{
	ui_->slotDiagramProperty->layout()->addWidget(_root);
}


void Screenplay::PanelView::SwitchBlankInspector()
{
	ui_->inspector->setCurrentIndex(0);
}

void Screenplay::PanelView::SwitchAgentInspector()
{
	ui_->inspector->setCurrentIndex(2);
}

void Screenplay::PanelView::SwitchDiagramInspector()
{
	ui_->inspector->setCurrentIndex(1);
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------
Screenplay::PanelComponent::PanelComponent(PanelView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
Screenplay::PanelComponent::~PanelComponent()
{
	//TODO
}

//-----------------------------------------------------------------------
void Screenplay::PanelComponent::initialize(Composite::Core* _core)
{
	DYN_CAST(Screenplay::CompositeCore, _core, core);
	IPlugin* plugin = core->GetAppCore()->FindCachePlugin("PluginDiagram");
	assert(plugin);
	PluginDiagram* pluginDiagram = dynamic_cast<PluginDiagram*>(plugin);

	DYN_CAST(Screenplay::PanelView, view_, view);

	view->AttachDiagramGroup(pluginDiagram->GetGroupUi());
	view->AttachDiagramTrigger(pluginDiagram->GetTriggerUi());
	view->AttachDiagramFlow(pluginDiagram->GetFlowUi());
	view->AttachDiagramProperty(pluginDiagram->GetPropertyUi());

	playerComponent_ = new Screenplay::PlayerComponent(new PlayerView());
	this->AddMVCChild(playerComponent_);

	slateComponent_ = new Screenplay::SlateComponent(new SlateView());
	this->AddMVCChild(slateComponent_);

	preloadComponent_ = new Screenplay::PreloadComponent(new PreloadView());
	this->AddMVCChild(preloadComponent_);

	agentInspectorComponent_ = new AgentInspectorComponent(new AgentInspectorView());
	this->AddMVCChild(agentInspectorComponent_);

	HandleBus<Screenplay::PanelComponent>::Route<ScreenplayActions::SelectSlateReceiver>(this, &Screenplay::PanelComponent::HandleOnSelectSlate);
	HandleBus<Screenplay::PanelComponent>::Route<ScreenplayActions::DeleteSlateReceiver>(this, &Screenplay::PanelComponent::HandleOnDeleteSlate);
	HandleBus<Screenplay::PanelComponent>::Route<ScreenplayActions::OnTriggerAddReceiver>(this, &Screenplay::PanelComponent::HandleOnTriggerNew);
	HandleBus<Screenplay::PanelComponent>::Route<ScreenplayActions::OnTriggerRemoveReceiver>(this, &Screenplay::PanelComponent::HandleOnTriggerDelete);
	HandleBus<Screenplay::PanelComponent>::Route<ScreenplayActions::OnPipeAddReceiver>(this, &Screenplay::PanelComponent::HandleOnPipeAdd);
	HandleBus<Screenplay::PanelComponent>::Route<ScreenplayActions::OnPipeRemoveReceiver>(this, &Screenplay::PanelComponent::HandleOnPipeRemove);
	HandleBus<Screenplay::PanelComponent>::Route<ScreenplayActions::OnReloadBeanReceiver>(this, &Screenplay::PanelComponent::HandleOnReloadBean);
	HandleBus<Screenplay::PanelComponent>::Route<ScreenplayActions::SwitchInspectorToAgentReceiver>(this, &Screenplay::PanelComponent::HandleSwitchAgentInspctor);
	HandleBus<Screenplay::PanelComponent>::Route<ScreenplayActions::SwitchInspectorToDiagramReceiver>(this, &Screenplay::PanelComponent::HandleSwitchDiagramInspctor);
}
/*
void Astronomy::PanelComponent::HandleReloadSlates(IAction* _action)
{
	MemDB* persistentDB = AstronomyModel::Persistent::GetMemDB();
	QByteArray bytes = persistentDB->EncodeBucket(persistentDB->GetBucket());
	QByteArray b64 = HttpUtil::Base64Encode(bytes);
	NEW_ACTION(UnityActions::WSMessageToUnityAction, action);
	action->method = "JSEditorReload";
	action->activity = "0";
	action->param = QString("{\"data\":\"%1\"}").arg(QString::fromUtf8(b64));
	INVOKE_ACTION(UnityActions::WSMessageToUnityAction, action);
}
*/
void Screenplay::PanelComponent::HandleOnTriggerNew(IAction* _action)
{
	CAST_ACTION(DiagramActions::OnTriggerNewAction, _action, action);

	MemDB* runtimeDB = ScreenplayModel::Runtime::GetMemDB();
	const QString& uuidSlate = ScreenplayModel::Runtime::KV::Query::screenplay_slate_alive_uuid(runtimeDB);
	MemDB* persistentDB = ScreenplayModel::Persistent::GetMemDB();
	ScreenplayModel::Persistent::Sets::Update::Add::screenplay_slate_guid_1_triggers_guids(persistentDB, uuidSlate, action->uuid);
}


void Screenplay::PanelComponent::HandleOnTriggerDelete(IAction* _action)
{
	CAST_ACTION(DiagramActions::OnTriggerDeleteAction, _action, action);

	MemDB* runtimeDB = ScreenplayModel::Runtime::GetMemDB();
	const QString& uuidSlate = ScreenplayModel::Runtime::KV::Query::screenplay_slate_alive_uuid(runtimeDB);
	MemDB* persistentDB = ScreenplayModel::Persistent::GetMemDB();
	ScreenplayModel::Persistent::Sets::Update::Remove::screenplay_slate_guid_1_triggers_guids(persistentDB, uuidSlate, action->uuid);
}

void Screenplay::PanelComponent::HandleOnPipeAdd(IAction* _action)
{
	CAST_ACTION(DiagramActions::OnPipeAddAction, _action, action);

	MemDB* runtimeDB = ScreenplayModel::Runtime::GetMemDB();
	const QString& uuidSlate = ScreenplayModel::Runtime::KV::Query::screenplay_slate_alive_uuid(runtimeDB);
	MemDB* persistentDB = ScreenplayModel::Persistent::GetMemDB();
	ScreenplayModel::Persistent::Sets::Update::Add::screenplay_slate_guid_1_pipes_guids(persistentDB, uuidSlate, action->uuidPipe);
}

void Screenplay::PanelComponent::HandleOnPipeRemove(IAction* _action)
{
	CAST_ACTION(DiagramActions::OnPipeRemoveAction, _action, action);

	MemDB* runtimeDB = ScreenplayModel::Runtime::GetMemDB();
	const QString& uuidSlate = ScreenplayModel::Runtime::KV::Query::screenplay_slate_alive_uuid(runtimeDB);
	MemDB* persistentDB = ScreenplayModel::Persistent::GetMemDB();
	ScreenplayModel::Persistent::Sets::Update::Remove::screenplay_slate_guid_1_pipes_guids(persistentDB, uuidSlate, action->uuidPipe);
}

void Screenplay::PanelComponent::HandleOnSelectSlate(IAction* _action)
{
	DYN_CAST(PanelView, view_, view);
	view->TogglePreloadRadio(true);
	view->ToggleBlockRadio(true);
	view->ToggleTriggerRadio(true);
}

void Screenplay::PanelComponent::HandleOnDeleteSlate(IAction* _action)
{
	DYN_CAST(PanelView, view_, view);
	view->TogglePreloadRadio(false);
	view->ToggleBlockRadio(false);
	view->ToggleTriggerRadio(false);
}

void Screenplay::PanelComponent::HandleOnReloadBean(IAction* _action)
{
	DYN_CAST(PanelView, view_, view);
	view->TogglePreloadRadio(false);
	view->ToggleBlockRadio(false);
	view->ToggleTriggerRadio(false);
	//根据bean的扩展情况过滤选项
	agentInspectorComponent_->ResetUI(false);
	view->SwitchBlankInspector();
}

void Screenplay::PanelComponent::HandleSwitchDiagramInspctor(IAction* _action)
{
	DYN_CAST(PanelView, view_, view);
	view->SwitchDiagramInspector();
}

void Screenplay::PanelComponent::HandleSwitchAgentInspctor(IAction* _action)
{
	DYN_CAST(PanelView, view_, view);
	CAST_ACTION(ScreenplayActions::SwitchInspectorToAgentAction, _action, action);
	view->SwitchAgentInspector();
	agentInspectorComponent_->SetInvoker(action->invoker);
}

