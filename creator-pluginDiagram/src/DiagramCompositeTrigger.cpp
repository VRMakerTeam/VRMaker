#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QDateTime>
#include "HandleBus.hxx"
#include "Utils.h"
#include "ClickWidget.h"
#include "AppCore.h"
#include "ClickWidget.h"
#include "PluginDiagramLogger.h"
#include "DiagramCompositeCore.h"
#include "PluginUnity/UnityActions.h"
#include "PluginDiagram/DiagramModel.h"
#include "PluginDiagram/DiagramActions.h"
#include "DiagramCompositeTrigger.h"


namespace Diagram
{
	class TriggerUI
	{
	public:
		QWidget* root;
		ClickWidget* clickwidget;
		QCheckBox* toggleTrash;
		QLabel* alias;
		QLabel* thumb;
		QPushButton* btnDelete;
	};
}

void Diagram::TriggerView::setupUI()
{
	ui_ = new TriggerUI();
	ui_->root = AppCore::CreateUI("diagramTriggerCard");
	ui_->clickwidget = ui_->root->findChild<ClickWidget*>("__clickwidget");
	ui_->toggleTrash = ui_->root->findChild<QCheckBox*>("__toggleTrash");
	ui_->alias = ui_->root->findChild<QLabel*>("__alias");
	ui_->thumb = ui_->root->findChild<QLabel*>("__thumb");
	ui_->btnDelete = ui_->root->findChild<QPushButton*>("__btnDelete");

	ui_->clickwidget->setGroup("triggers");

	QObject::connect(ui_->clickwidget, &ClickWidget::clicked, [=]() {
		//打开属性面板
		{
			NEW_ACTION(DiagramActions::OpenTriggerPropertyPanelAction, action);
			action->invoker = componentUUID_;
			INVOKE_ACTION(DiagramActions::OpenTriggerPropertyPanelAction, action);
		}
		//在Unity中定位
		{
			MemDB* memDB = DiagramModel::Persistent::GetMemDB();
			QString type = DiagramModel::Persistent::KV::Query::diagram_trigger_1_type(memDB, componentUUID_);
			NEW_ACTION(UnityActions::WSMessageToUnityAction, action);
			action->activity = "0";
			action->method = "JSEditorFocusTrigger";
			action->param = QString("{\"uuid\":\"%1\", \"type\":\"%2\"}").arg(componentUUID_).arg(type);
			INVOKE_ACTION(UnityActions::WSMessageToUnityAction, action);
		}
	});

	QObject::connect(ui_->clickwidget, &ClickWidget::toggled, [=](bool _toggled) {
		if (_toggled)
		{
			ui_->root->setAccessibleName("bg:light");
		}
		else
		{
			ui_->root->setAccessibleName("bg:normal");
		}
		ui_->root->setStyleSheet(ui_->root->styleSheet());
		
	});

	QObject::connect(ui_->toggleTrash, &QCheckBox::clicked, [=](bool _clicked) {
		ui_->btnDelete->setVisible(_clicked);
	});

	QObject::connect(ui_->btnDelete, &QPushButton::clicked, [=] {
		NEW_ACTION(DiagramActions::DeleteTriggerAction, action);
		action->guid = componentUUID_;
		INVOKE_ACTION(DiagramActions::DeleteTriggerAction, action);
	});


	ui_->toggleTrash->setVisible(false);
	ui_->btnDelete->setVisible(false);
}

void Diagram::TriggerView::onUpdate(const QString& _tag)
{
	if (0 == _tag.compare(DiagramModel::Persistent::Tags::digram_triggers_update))
	{
		MemDB* memDB = DiagramModel::Persistent::GetMemDB();
		QString alias = DiagramModel::Persistent::KV::Query::diagram_trigger_1_alias(memDB, componentUUID_);
		ui_->alias->setText(alias);

		int icon = DiagramModel::Persistent::KV::Query::diagram_gaze_1_icon(memDB, componentUUID_).toInt();
		ui_->thumb->setPixmap(QPixmap(QString(":/pluginDiagram/icons/vr-trigger-%1").arg(icon)));
	}
}

QWidget* Diagram::TriggerView::GetRoot()
{
	return ui_->root;
}

void Diagram::TriggerView::ToggleTrash(bool _toggle)
{
	ui_->toggleTrash->setVisible(_toggle);
	ui_->toggleTrash->setChecked(false);
	ui_->btnDelete->setVisible(false);
}


//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
Diagram::TriggerComponent::~TriggerComponent()
{
}

Diagram::TriggerComponent* Diagram::TriggerComponent::New()
{
	return new TriggerComponent(new TriggerView());
}

void Diagram::TriggerComponent::HandleToggleTrigersTrash(IAction* _action)
{
	CAST_ACTION(DiagramActions::ToggleTriggersTrashAction, _action, action);

	DYN_CAST(TriggerView, view_, view);
	view->ToggleTrash(action->toggle);
}


Diagram::TriggerComponent* Diagram::TriggerComponent::New(const QString& _guid)
{
	return new TriggerComponent(new TriggerView(), _guid);
}

Diagram::TriggerComponent::TriggerComponent(TriggerView* _view)
	: Composite::MVCComponent(_view)
{

}

Diagram::TriggerComponent::TriggerComponent(TriggerView* _view, const QString& _guid)
	: Composite::MVCComponent(_view, _guid)
{

}

//-----------------------------------------------------------------------
void Diagram::TriggerComponent::initialize(Composite::Core* _core)
{
	HandleBus<TriggerComponent>::Route<DiagramActions::ToggleTrigersTrashReceiver>(this, &TriggerComponent::HandleToggleTrigersTrash);
}
