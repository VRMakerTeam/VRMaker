#include <QWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVariant>
#include <QColorDialog>
#include "Utils.h"
#include "AppCore.h"
#include "pluginBean/BeanModel.h"
#include "pluginScreenplay/ScreenplayModel.h"
#include "pluginScreenplay/ScreenplayActions.h"
#include "ScreenplayCompositeAgentInspector.h"

namespace Screenplay
{
	class AgentInspectorUI
	{
	public:
		QWidget* root;
		QCheckBox* gaze;
		QWidget* gazeFieldBox;
		QLabel* gazeAliasField;
		QLineEdit* gazeAliasInput;
	};
}

QWidget* Screenplay::AgentInspectorView::GetRoot()
{
	return ui_->root;
}

void Screenplay::AgentInspectorView::UpdateInvoker(const QString& _invoker)
{
	ui_->root->setProperty("invoker",  _invoker);

	MemDB* memDB = ScreenplayModel::Persistent::GetMemDB();
	bool gaze = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_gaze(memDB, _invoker).toBool();
	ui_->gaze->setChecked(gaze);
	ui_->gazeFieldBox->setVisible(gaze);

	QString gazeAlias = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_gaze_alias(memDB, _invoker).toString();
	ui_->gazeAliasInput->setText(gazeAlias);
}

void Screenplay::AgentInspectorView::setupUI()
{
	ui_ = new AgentInspectorUI();
	ui_->root = AppCore::CreateUI("screenplayAgentInspector");

	ui_->gaze = ui_->root->findChild<QCheckBox*>("__gaze");
	ui_->gazeFieldBox = ui_->root->findChild<QWidget*>("__gazeFieldBox");
	ui_->gazeAliasField = ui_->root->findChild<QLabel*>("__gazeAliasField");
	ui_->gazeAliasInput = ui_->root->findChild<QLineEdit*>("__gazeAliasInput");

	ui_->gaze->setText(AppCore::Translate("ui.screenplayAgentInspector.__gaze"));
	ui_->gazeAliasField->setText(AppCore::Translate("ui.screenplayAgentInspector.__gazeAliasField"));

	ui_->gaze->setChecked(false);
	ui_->gazeFieldBox->setVisible(false);

	QObject::connect(ui_->gaze, &QCheckBox::clicked, [=](bool _clicked) {
		QString agentUUID = ui_->root->property("invoker").toString();
		MemDB* memDB = ScreenplayModel::Persistent::GetMemDB();
		ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_gaze(memDB, agentUUID, _clicked);
		ui_->gazeFieldBox->setVisible(_clicked);
	});

	QObject::connect(ui_->gazeAliasInput, &QLineEdit::textChanged, [=](const QString& _text) {
		QString agentUUID = ui_->root->property("invoker").toString();
		MemDB* memDB = ScreenplayModel::Persistent::GetMemDB();
		ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_gaze_alias(memDB, agentUUID, _text);
	});

}

void Screenplay::AgentInspectorView::onUpdate(const QString& _tag)
{
}

void Screenplay::AgentInspectorView::reset()
{
	MemDB* memDB = ScreenplayModel::Persistent::GetMemDB();
	QList<QString> extends = BeanModel::Persistent::Sets::Query::bean_extends(memDB);
	ui_->gaze->setVisible(extends.contains("VR"));
}

Screenplay::AgentInspectorComponent::AgentInspectorComponent(AgentInspectorView* _view)
	: MVCComponent(_view)
{

}

void Screenplay::AgentInspectorComponent::SetInvoker(const QString& _invoker)
{
	DYN_CAST(Screenplay::AgentInspectorView, view_, view);
	view->UpdateInvoker(_invoker);
}

void Screenplay::AgentInspectorComponent::initialize(Composite::Core* _core)
{

}
