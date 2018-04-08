#include <QLabel>
#include <QTimer>
#include <QLineEdit>
#include <QPushButton>
#include "ClickWidget.h"
#include "Utils.h"
#include "Dispatcher.h"
#include "HandleBus.hxx"
#include "BeanCompositeCore.h"
#include "AppCore.h"
#include "PluginBean/BeanActions.h"
#include "PluginBean/BeanModel.h"
#include "PluginBeanLogger.h"
#include "BeanCompositeCard.h"

namespace Bean
{
	class CardUI
	{
	public:
		~CardUI()
		{
			SAFE_DELETE(root);
		}
		QWidget* root;
		QLabel* selectMark;
		QPushButton* publishMark;
		QLineEdit* inputAlias;
		ClickWidget* clickwidget;
	};
}


//-----------------------------------------------------------------------
Bean::CardView::~CardView()
{
	SAFE_DELETE(ui_);
}
//-----------------------------------------------------------------------
void Bean::CardView::setupUI()
{
	ui_ = new CardUI();
	publishTimer_ = new QTimer(NULL);
	publishTimer_->setInterval(1000);
	QObject::connect(publishTimer_, &QTimer::timeout, [=] {
		ui_->publishMark->setVisible(false);
		publishTimer_->stop();
	});

	ui_->root = AppCore::CreateUI("beanCard");
	ui_->inputAlias = ui_->root->findChild<QLineEdit*>("__input_alias");
	ui_->selectMark = ui_->root->findChild<QLabel*>("__selectedMark");
	ui_->selectMark->setVisible(false);
	ui_->publishMark = ui_->root->findChild<QPushButton*>("__publishMark");
	ui_->publishMark->setVisible(false);
	ui_->clickwidget = ui_->root->findChild<ClickWidget*>("__clickwidget");

	QObject::connect(ui_->clickwidget, &ClickWidget::clicked, [=] {
		NEW_ACTION(BeanActions::SelectBeanAction, action);
		action->uuid = componentUUID_;
		INVOKE_ACTION(BeanActions::SelectBeanAction, action);
	});

	QObject::connect(ui_->clickwidget, &ClickWidget::toggled, [=](bool _toggled) {
		ui_->selectMark->setVisible(_toggled);
	});

	QObject::connect(ui_->inputAlias, &QLineEdit::editingFinished, [=] {
		NEW_ACTION(BeanActions::ModifyBeanAction, action);
		action->guid = componentUUID_;
		action->alias = ui_->inputAlias->text();
		INVOKE_ACTION(BeanActions::ModifyBeanAction, action);
	});

}

//-----------------------------------------------------------------------
QWidget* Bean::CardView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Bean::CardView::setRadioGroup(const QString& _group)
{
	ui_->clickwidget->setGroup(_group);
}

//-----------------------------------------------------------------------
void Bean::CardView::TogglePublishMark(bool _visible)
{
	if (_visible)
		ui_->publishMark->setVisible(true);
	else
		publishTimer_->start();
}
				 
//-----------------------------------------------------------------------
void Bean::CardView::onUpdate(const QString& _tag)
{
	if (0 != _tag.compare(BeanModel::Persistent::Tags::bean_update))
		return;
	MemDB* memDB = BeanModel::Persistent::GetMemDB(componentUUID_);
	QString alias = BeanModel::Persistent::KV::Query::bean_alias(memDB);
	ui_->inputAlias->setText(alias);
}



//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////



Bean::CardComponent::CardComponent(CardView* _view, const QString& _uuid)
	:MVCComponent(_view, _uuid)
{

}

//-----------------------------------------------------------------------
void Bean::CardComponent::HandleModifyBean(IAction* _action)
{
	CAST_ACTION(BeanActions::ModifyBeanAction, _action, action);

	if (0 != action->guid.compare(this->GetUUID()))
		return;

	MemDB* memDB = BeanModel::Persistent::GetMemDB(this->GetUUID());
	BeanModel::Persistent::KV::Update::bean_alias(memDB, action->alias);
	Composite::Store::Update(BeanModel::Persistent::Tags::bean_update);
}

//-----------------------------------------------------------------------
void Bean::CardComponent::initialize(Composite::Core* _core)
{
	DYN_CAST(CardView, view_, view);
	view->setRadioGroup(this->GetParent()->GetUUID());
	HandleBus<Bean::CardComponent>::Route<BeanActions::ModifyBeanReceiver>(this, &CardComponent::HandleModifyBean);
}
//-----------------------------------------------------------------------
void Bean::CardComponent::StartPublish()
{
	DYN_CAST(CardView, view_, view);
	view->TogglePublishMark(true);
}

//-----------------------------------------------------------------------
void Bean::CardComponent::FinishPublish()
{
	DYN_CAST(CardView, view_, view);
	view->TogglePublishMark(false);
}

//-----------------------------------------------------------------------
void Bean::CardComponent::AbortPublish()
{
	DYN_CAST(CardView, view_, view);
	view->TogglePublishMark(false);
}
