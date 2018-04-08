#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QColorDialog>
#include <QPalette>
#include "Utils.h"
#include "AppCore.h"
#include "HandleBus.hxx"
#include "Dispatcher.h"
#include "PluginDiagramLogger.h"
#include "PluginDiagram/DiagramActions.h"
#include "PropertyUiTemplateBuilder.h"
#include "DiagramCompositeCore.h"
#include "DiagramCompositeGroup.h"
#include "DiagramCompositeBlock.h"
#include "PluginDiagram/DiagramModel.h"
#include "PluginUnity/UnityActions.h"
#include "DiagramCompositeProperty.h"


namespace Diagram
{
	class PropertyUI
	{
	public:
		QWidget* root;
		QStackedWidget* pages;
		QWidget* blcokToolbar;
		QWidget* blockInspector;
		QPushButton* blockDelete;
		QLabel* triggerUUID;
		QLabel* triggerAliasField;

		QLineEdit* triggerAliasInput;
		QWidget* groupGaze;
		QLabel* gazeIconField;
		QComboBox* gazeIconSelect;
		QLabel* gazeColorField;
		QPushButton* gazeColorPicker;
	};
}

Diagram::PropertyView::~PropertyView()
{
	SAFE_DELETE(ui_);
}

void Diagram::PropertyView::setupUI()
{
	ui_ = new PropertyUI();
	ui_->root = AppCore::CreateUI("diagramProperty");
	ui_->pages = ui_->root->findChild<QStackedWidget*>("__pages");
	ui_->blcokToolbar = ui_->root->findChild<QWidget*>("__blockToolbar");
	ui_->blockInspector = ui_->root->findChild<QWidget*>("__blockInspector");
	ui_->blockDelete = ui_->root->findChild<QPushButton*>("__blockDelete");
	ui_->triggerAliasInput = ui_->root->findChild<QLineEdit*>("__triggerAliasInput");
	ui_->triggerUUID = ui_->root->findChild<QLabel*>("__triggerUUID");
	ui_->triggerAliasField = ui_->root->findChild<QLabel*>("__triggerAliasField");
	ui_->groupGaze = ui_->root->findChild<QWidget*>("__groupGaze");
	ui_->gazeIconField = ui_->root->findChild<QLabel*>("__gazeIconField");
	ui_->gazeIconSelect = ui_->root->findChild<QComboBox*>("__gazeIconSelect");
	ui_->gazeColorField = ui_->root->findChild<QLabel*>("__gazeColorField");
	ui_->gazeColorPicker = ui_->root->findChild<QPushButton*>("__gazeColorPicker");

	ui_->triggerAliasField->setText(AppCore::Translate("ui.diagramProperty.__triggerAliasField"));
	ui_->gazeIconField->setText(AppCore::Translate("ui.diagramProperty.__gazeIconField"));
	ui_->gazeColorField->setText(AppCore::Translate("ui.diagramProperty.__gazeColorField"));

	ui_->gazeColorPicker->setAutoFillBackground(true);
	ui_->gazeColorPicker->setFlat(true);
	QPalette palette = ui_->gazeColorPicker->palette();
	palette.setColor(QPalette::ColorRole::Button, QColor::fromRgb(255,255,255));
	ui_->gazeColorPicker->setPalette(palette);

	//用作存储数据，不需要显示
	ui_->triggerUUID->setVisible(false);

	QObject::connect(ui_->blockDelete, &QPushButton::clicked, [=] {
		NEW_ACTION(DiagramActions::DeleteBlockAction, action);
		action->uuid = PropertyUiBuilder::Alive();
		INVOKE_ACTION(DiagramActions::DeleteBlockAction, action);
		ToogleBlockToolbar(false);
		reset();
	});

	QObject::connect(ui_->triggerAliasInput, &QLineEdit::textChanged, [=](const QString& _text) {
		QString uuidTrigger = ui_->triggerUUID->text();

		MemDB* memDB = DiagramModel::Persistent::GetMemDB();
		DiagramModel::Persistent::KV::Update::diagram_trigger_1_alias(memDB, uuidTrigger, _text );

		//refresh trigger card
		Composite::Store::Update(DiagramModel::Persistent::Tags::digram_triggers_update);

		//刷新Unity中的触发器
		QString alias = DiagramModel::Persistent::KV::Query::diagram_trigger_1_alias(memDB, uuidTrigger);
		QString type = DiagramModel::Persistent::KV::Query::diagram_trigger_1_type(memDB, uuidTrigger);

		NEW_ACTION(UnityActions::API::ModifyTriggerAction, unityAction);
		unityAction->alias = alias;
		unityAction->uuid = uuidTrigger;
		unityAction->type = type;
		INVOKE_ACTION(UnityActions::API::ModifyTriggerAction, unityAction);
	});

	QObject::connect(ui_->gazeIconSelect, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int _index) {
		QString uuidTrigger = ui_->triggerUUID->text();

		MemDB* memDB = DiagramModel::Persistent::GetMemDB();
		DiagramModel::Persistent::KV::Update::diagram_gaze_1_icon(memDB, ui_->triggerUUID->text(), _index);

		//refresh trigger card
		Composite::Store::Update(DiagramModel::Persistent::Tags::digram_triggers_update);

		//刷新Unity中的触发器
		QString alias = DiagramModel::Persistent::KV::Query::diagram_trigger_1_alias(memDB, uuidTrigger);
		QString type = DiagramModel::Persistent::KV::Query::diagram_trigger_1_type(memDB, uuidTrigger);
		int icon = DiagramModel::Persistent::KV::Query::diagram_gaze_1_icon(memDB, uuidTrigger).toInt();

		NEW_ACTION(UnityActions::API::ModifyGazeIconAction, unityAction);
		unityAction->icon = icon;
		unityAction->uuid = uuidTrigger;
		INVOKE_ACTION(UnityActions::API::ModifyGazeIconAction, unityAction);
	});


	QObject::connect(ui_->gazeColorPicker, &QPushButton::clicked, [=] {
		QColor color = QColorDialog::getColor(Qt::white, NULL, "Color");
		if (!color.isValid())
			return;

		QString uuidTrigger = ui_->triggerUUID->text();

		MemDB* memDB = DiagramModel::Persistent::GetMemDB();
		DiagramModel::Persistent::KV::Update::diagram_gaze_1_color_r(memDB, uuidTrigger, color.red());
		DiagramModel::Persistent::KV::Update::diagram_gaze_1_color_g(memDB, uuidTrigger, color.green());
		DiagramModel::Persistent::KV::Update::diagram_gaze_1_color_b(memDB, uuidTrigger, color.blue());
		DiagramModel::Persistent::KV::Update::diagram_gaze_1_color_a(memDB, uuidTrigger, color.alpha());
		QPalette palette = ui_->gazeColorPicker->palette();
		palette.setColor(QPalette::ColorRole::Button, color);
		ui_->gazeColorPicker->setPalette(palette);

		NEW_ACTION(UnityActions::API::ModifyGazeColorAction, unityAction);
		unityAction->uuid = uuidTrigger;
		unityAction->r = color.red();
		unityAction->g = color.green();
		unityAction->b = color.blue();
		unityAction->a = color.alpha();
		INVOKE_ACTION(UnityActions::API::ModifyGazeColorAction, unityAction);
	});

	builder_ = new PropertyUiBuilder();
}

void Diagram::PropertyView::reset()
{
	while (QLayoutItem* item = ui_->blockInspector->layout()->itemAt(0))
	{
		ui_->blockInspector->layout()->removeItem(item);
		item->widget()->setParent(NULL);
	}
}

void Diagram::PropertyView::onUpdate(const QString& _tag)
{

}

QWidget* Diagram::PropertyView::GetRoot()
{
	return ui_->root;
}

void Diagram::PropertyView::ToogleBlockToolbar(bool _visiable)
{
	ui_->blcokToolbar->setVisible(_visiable);
}

void Diagram::PropertyView::SwitchTriggerProperty()
{
	ui_->pages->setCurrentIndex(1);
}

void Diagram::PropertyView::SwitchBlockProperty()
{
	ui_->pages->setCurrentIndex(0);
}

void Diagram::PropertyView::UpdateTrigger(const QString& _triggerUUID, const QString& _alias)
{
	ui_->triggerUUID->setText(_triggerUUID);
	ui_->triggerAliasInput->setText(_alias);
}

void Diagram::PropertyView::UpdateGazeIcon(const QString& _triggerUUID, int _icon)
{
	ui_->gazeIconSelect->setCurrentIndex(_icon);
}

void Diagram::PropertyView::UpdateGazeColor(const QString& _triggerUUID, int _r, int _g, int _b, int _a)
{
	QPalette palette = ui_->gazeColorPicker->palette();
	palette.setColor(QPalette::ColorRole::Button, QColor(_r, _g, _b, _a));
	ui_->gazeColorPicker->setPalette(palette);
}

void Diagram::PropertyView::Build(const QString& _group, const QString& _tpl, const QString& _invoker)
{
	builder_->Build(ui_->blockInspector, _group, _tpl, _invoker);
}

void Diagram::PropertyView::UpdateProperty(const QString& _invoker, const QHash<QString, QVariant>& _properties)
{
	builder_->UpdateProperty(_invoker, _properties);
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
Diagram::PropertyComponent::~PropertyComponent()
{
}

Diagram::PropertyComponent::PropertyComponent(PropertyView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
void Diagram::PropertyComponent::initialize(Composite::Core* _core)
{
	HandleBus<Diagram::PropertyComponent>::Route<DiagramActions::OpenBlockPropertyPanelReceiver>(this, &Diagram::PropertyComponent::HandleOpenBlockPropertyPanel);
	HandleBus<Diagram::PropertyComponent>::Route<DiagramActions::OpenTriggerPropertyPanelReceiver>(this, &Diagram::PropertyComponent::HandleOpenTriggerPropertyPanel);
	HandleBus<Diagram::PropertyComponent>::Route<DiagramActions::ClosePropertyPanelReceiver>(this, &Diagram::PropertyComponent::HandleClosePropertyPanel);
	HandleBus<Diagram::PropertyComponent>::Route<DiagramActions::PropertyUiUpdatePropertyReceiver>(this, &Diagram::PropertyComponent::HandleUpdateProperty);
}

//-----------------------------------------------------------------------
void Diagram::PropertyComponent::HandleOpenBlockPropertyPanel(IAction* _action)
{
	DiagramActions::OpenBlockPropertyPanelAction* action = static_cast<DiagramActions::OpenBlockPropertyPanelAction*>(_action);
	assert(action);
	PropertyView* view = dynamic_cast<PropertyView*>(view_);
	view->Build(action->group, action->tpl, action->invoker);

	view->ToogleBlockToolbar(true);
	view->SwitchBlockProperty();
}

void Diagram::PropertyComponent::HandleOpenTriggerPropertyPanel(IAction* _action)
{
	PropertyView* view = dynamic_cast<PropertyView*>(view_);
	view->SwitchTriggerProperty();

	CAST_ACTION(DiagramActions::OpenTriggerPropertyPanelAction, _action, action);
	action->invoker;

	MemDB* memDB = DiagramModel::Persistent::GetMemDB();
	QString alias = DiagramModel::Persistent::KV::Query::diagram_trigger_1_alias(memDB, action->invoker);
	view->UpdateTrigger(action->invoker, alias);

	int icon = DiagramModel::Persistent::KV::Query::diagram_gaze_1_icon(memDB, action->invoker).toInt();
	view->UpdateGazeIcon(action->invoker, icon);

	int r = DiagramModel::Persistent::KV::Query::diagram_gaze_1_color_r(memDB, action->invoker).toInt();
	int g = DiagramModel::Persistent::KV::Query::diagram_gaze_1_color_g(memDB, action->invoker).toInt();
	int b = DiagramModel::Persistent::KV::Query::diagram_gaze_1_color_b(memDB, action->invoker).toInt();
	int a = DiagramModel::Persistent::KV::Query::diagram_gaze_1_color_a(memDB, action->invoker).toInt();
	view->UpdateGazeColor(action->invoker, r, g, b, a);
}

void Diagram::PropertyComponent::HandleClosePropertyPanel(IAction* _action)
{
	ResetUI(false);

	PropertyView* view = dynamic_cast<PropertyView*>(view_);
	view->ToogleBlockToolbar(false);
	view->SwitchBlockProperty();
}

void Diagram::PropertyComponent::ProcessMessageFromUnity(const QString& _method, const QString& _activity, const QString& _param)
{
	if (0 == _method.compare("JSOnEditorAdjustTrigger"))
	{
		QJsonDocument doc = QJsonDocument::fromJson(_param.toUtf8());
		QJsonObject obj = doc.object();
		QString uuid = obj.take("uuid").toString();
		double px = obj.take("px").toDouble();
		double py = obj.take("py").toDouble();
		double pz = obj.take("pz").toDouble();
		double rx = obj.take("rx").toDouble();
		double ry = obj.take("ry").toDouble();
		double rz = obj.take("rz").toDouble();

		MemDB* memDB = DiagramModel::Persistent::GetMemDB();
		DiagramModel::Persistent::KV::Update::diagram_trigger_1_px(memDB, uuid, px);
		DiagramModel::Persistent::KV::Update::diagram_trigger_1_py(memDB, uuid, py);
		DiagramModel::Persistent::KV::Update::diagram_trigger_1_pz(memDB, uuid, pz);
		DiagramModel::Persistent::KV::Update::diagram_trigger_1_rx(memDB, uuid, rx);
		DiagramModel::Persistent::KV::Update::diagram_trigger_1_ry(memDB, uuid, ry);
		DiagramModel::Persistent::KV::Update::diagram_trigger_1_rz(memDB, uuid, rz);
	}
}

void Diagram::PropertyComponent::HandleUpdateProperty(IAction* _action)
{
	CAST_ACTION(DiagramActions::PropertyUiUpdatePropertyAction, _action, action);

	PropertyView* view = dynamic_cast<PropertyView*>(view_);
	view->UpdateProperty(action->invoker, action->properties);
}
