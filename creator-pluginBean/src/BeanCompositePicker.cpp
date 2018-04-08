#include <QUuid>
#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QLabel>
#include <QStackedWidget>
#include <QLayout>
#include "AppCore.h"
#include "Utils.h"
#include "PluginBeanLogger.h"
#include "Dispatcher.h"
#include "HandleBus.hxx"
#include "BeanCompositeSource.h"
#include "PluginBean/BeanActions.h"
#include "PluginBean/BeanModel.h"
#include "BeanCompositeCore.h"
#include "BeanCompositePanel.h"
#include "BeanCompositePicker.h"

namespace Bean
{
	class PickerUI
	{
	public:
		QWidget* root;
		QPushButton* btnOK;
		QPushButton* btnCancel;
		QRadioButton* clone;
		QRadioButton* archetype;
		QLineEdit* aliasInput;
		QLabel* error;
		QWidget* clonesContainer;
		QWidget* archetypeContainer;
		QStackedWidget* sourceSwither;
	};
}

//-----------------------------------------------------------------------
void Bean::PickerView::setupUI()
{
	ui_ = new PickerUI();
	ui_->root = AppCore::CreateUI("beanPicker");

	ui_->btnOK = ui_->root->findChild<QPushButton*>("__btnOK");
	ui_->btnCancel = ui_->root->findChild<QPushButton*>("__btnCancel");
	ui_->clone = ui_->root->findChild<QRadioButton*>("__rbClone");
	ui_->archetype = ui_->root->findChild<QRadioButton*>("__rbArchetype");
	ui_->archetypeContainer = ui_->root->findChild<QWidget*>("__archetypesContainer");
	ui_->clonesContainer = ui_->root->findChild<QWidget*>("__clonesContainer");
	ui_->aliasInput = ui_->root->findChild<QLineEdit*>("__aliasInput");
	ui_->error = ui_->root->findChild<QLabel*>("__error");
	ui_->sourceSwither = ui_->root->findChild<QStackedWidget*>("__sourcesSwitcher");

	ui_->archetype->setText(AppCore::Translate("ui.beanPicker.__rbArchetype"));
	ui_->clone->setText(AppCore::Translate("ui.beanPicker.__rbClone"));
	ui_->aliasInput->setPlaceholderText(AppCore::Translate("ui.beanPicker.__aliasInput"));
	ui_->btnOK->setText(AppCore::Translate("ui.beanPicker.__btnOK"));
	ui_->btnCancel->setText(AppCore::Translate("ui.beanPicker.__btnCancel"));

	ui_->btnOK->setDisabled(true);

	QObject::connect(ui_->archetype, &QRadioButton::clicked, [=] {
		NEW_ACTION(BeanActions::OpenPageAction, action);
		action->page = BeanActions::OpenPageAction::ARCHETYPE;
		INVOKE_ACTION(BeanActions::OpenPageAction, action);
	});
	QObject::connect(ui_->clone, &QRadioButton::clicked, [=] {
		NEW_ACTION(BeanActions::OpenPageAction, action);
		action->page = BeanActions::OpenPageAction::CLONE;
		INVOKE_ACTION(BeanActions::OpenPageAction, action);
	});
	QObject::connect(ui_->btnOK, &QRadioButton::clicked, [=] {
		QString alias = ui_->aliasInput->text();
		// 检测alias是否存在
		MemDB* runtimeDB = BeanModel::Runtime::GetMemDB();
		if (BeanModel::Runtime::Sets::Query::bean_alias(runtimeDB).contains(alias))
		{
			ui_->error->setText(AppCore::Translate("ui.beanPicker.__error"));
			return;
		}
		//清空错误提示
		ui_->error->setText("");

		QString uuid = QUuid::createUuid().toString().remove("{").remove("}");
		NEW_ACTION(BeanActions::NewBeanAction, addBeanAction);
		addBeanAction->guid = uuid;
		addBeanAction->alias = alias;
		INVOKE_ACTION(BeanActions::NewBeanAction, addBeanAction);
		
		NEW_ACTION(BeanActions::OpenPageAction, action);
		action->page = BeanActions::OpenPageAction::DASHBOARD;
		INVOKE_ACTION(BeanActions::OpenPageAction, action);

		//打开编辑页面
		NEW_ACTION(BeanActions::SelectBeanAction, actionSelect);
		actionSelect->uuid = uuid;
		INVOKE_ACTION(BeanActions::SelectBeanAction, actionSelect);

		NEW_ACTION(BeanActions::EditBeanAction, actionEdit);
		actionEdit->guid = uuid;
		INVOKE_ACTION(BeanActions::EditBeanAction, actionEdit);
	});
	QObject::connect(ui_->btnCancel, &QRadioButton::clicked, [=] {
		NEW_ACTION(BeanActions::OpenPageAction, action);
		action->page = BeanActions::OpenPageAction::DASHBOARD;
		INVOKE_ACTION(BeanActions::OpenPageAction, action);;
	});
	QObject::connect(ui_->aliasInput, &QLineEdit::textChanged, [=](const QString& _value) {
		ui_->btnOK->setDisabled(_value.isEmpty());
	});
}

void Bean::PickerView::reset()
{
	ui_->aliasInput->clear();
	ui_->error->setText("");
}

void Bean::PickerView::onUpdate(const QString& _tag)
{
	if (0 != _tag.compare(BeanModel::Runtime::Tags::bean_source))
		return;

	//QLayoutItem* item = NULL;
	//while (item = ptrUI_->ui->archetypesContainer->layout()->takeAt(0))
	//{
	//	ptrUI_->ui->archetypesContainer->layout()->removeItem(item);
	//}

	for each (Composite::View* view in this->children_)
	{
		DYN_CAST(Bean::SourceView, view, source);
		if(source->HasTag("Extend"))
			ui_->archetypeContainer->layout()->addWidget(source->GetRoot());
		else if(source->HasTag("Template"))
			ui_->clonesContainer->layout()->addWidget(source->GetRoot());
	}
}

QWidget* Bean::PickerView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Bean::PickerView::SwitchArchetypePage()
{
	ui_->sourceSwither->setCurrentIndex(0);
}

//-----------------------------------------------------------------------
void Bean::PickerView::SwitchClonePage()
{
	ui_->sourceSwither->setCurrentIndex(1);
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////
Bean::PickerComponent::PickerComponent(PickerView* _view)
	: Composite::MVCComponent(_view)
{

}
//-----------------------------------------------------------------------
void Bean::PickerComponent::initialize(Composite::Core* _core)
{
	ptrCore_ = _core;
	HandleBus<Bean::PickerComponent>::Route<BeanActions::RegisterSourceReceiver>(this, &PickerComponent::HandleRegisterSource);
	HandleBus<Bean::PickerComponent>::Route<BeanActions::SelectSourceReceiver>(this, &PickerComponent::HandleSelectSource);
}

//-----------------------------------------------------------------------
void Bean::PickerComponent::HandleRegisterSource(IAction* _action)
{
	CAST_ACTION(BeanActions::RegisterSourceAction, _action, action);
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, QString("register %1 to %2").arg(action->alias).arg(action->category));
	SourceComponent* source = new SourceComponent(new SourceView(), action->guid);
	source->AddTag(action->category);
	this->AddMVCChild(source);
	source->Initialize(ptrCore_);

	MemDB* memDB = BeanModel::Runtime::GetMemDB();
	BeanModel::Runtime::Sets::Update::Add::bean_sources_guids(memDB, action->guid);
	BeanModel::Runtime::KV::Update::bean_source_guid_1_alias(memDB, action->guid, action->alias);
	BeanModel::Runtime::KV::Update::bean_source_guid_1_category(memDB, action->guid, action->category);
	BeanModel::Runtime::KV::Update::bean_source_guid_1_extends(memDB, action->guid, action->extends);

	Composite::Store::Update(BeanModel::Runtime::Tags::bean_source);
}
//-----------------------------------------------------------------------
void Bean::PickerComponent::HandleSelectSource(IAction* _action)
{
	CAST_ACTION(BeanActions::SelectSourceAction, _action, action);
	LOGX_DEBUG(XC_PluginBean, XL_MEDIMU_PluginBean, QString("select source %1").arg(action->uuid));
	
	MemDB* memDB = BeanModel::Runtime::GetMemDB();
	if (action->toggled)
		BeanModel::Runtime::Sets::Update::Add::bean_source_selected_guid(memDB, action->uuid);
	else
		BeanModel::Runtime::Sets::Update::Remove::bean_source_selected_guid(memDB, action->uuid);
}

//-----------------------------------------------------------------------
void Bean::PickerComponent::SwitchArchetypePage()
{
	DYN_CAST(PickerView, view_, view);
	view->SwitchArchetypePage();
	pickedSource_ = "";
	
	ResetUI(true);

	MemDB* memDB = BeanModel::Runtime::GetMemDB();
	BeanModel::Runtime::Sets::Remove::bean_source_selected_guid(memDB);
}

//-----------------------------------------------------------------------
void Bean::PickerComponent::SwitchClonePage()
{
	DYN_CAST(PickerView, view_, view);
	view->SwitchClonePage();
	pickedSource_ = "";
	
	ResetUI(true);

	MemDB* memDB = BeanModel::Runtime::GetMemDB();
	BeanModel::Runtime::Sets::Remove::bean_source_selected_guid(memDB);
}
