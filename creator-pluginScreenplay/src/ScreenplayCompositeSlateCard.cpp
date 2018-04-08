#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include "Utils.h"
#include "ClickWidget.h"
#include "Dispatcher.h"
#include "HandleBus.hxx"
#include "AppCore.h"
#include "ScreenplayCompositeCore.h"
#include "PluginScreenplay/ScreenplayActions.h"
#include "PluginScreenplay/ScreenplayModel.h"
#include "ScreenplayCompositeSlateCard.h"

namespace Screenplay
{
	class SlateCardUI
	{
	public:
		~SlateCardUI()
		{
			SAFE_DELETE(root);
		}
		QWidget* root;
		ClickWidget* clickwidget;
		QLineEdit* inputAlias;
		QLabel* thumb;
		QLabel* selectMark;
	};
}

//-----------------------------------------------------------------------
Screenplay::SlateCardView::~SlateCardView()
{
	SAFE_DELETE(ui_);
}

//-----------------------------------------------------------------------
QWidget* Screenplay::SlateCardView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Screenplay::SlateCardView::setupUI()
{
	ui_ = new SlateCardUI();
	ui_->root = AppCore::CreateUI("screenplaySlateCard");
	ui_->clickwidget = ui_->root->findChild<ClickWidget*>("__clickwidget");
	ui_->inputAlias = ui_->root->findChild<QLineEdit*>("__inputAlias");
	ui_->selectMark = ui_->root->findChild<QLabel*>("__selectMark");
	ui_->thumb = ui_->root->findChild<QLabel*>("__thumb");

	ui_->selectMark->setVisible(false);
	ui_->clickwidget->setGroup("slatecard");

	QObject::connect(ui_->clickwidget, &ClickWidget::clicked, [=] {
		NEW_ACTION(ScreenplayActions::SelectSlateAction, action);
		action->uuid = componentUUID_;
		INVOKE_ACTION(ScreenplayActions::SelectSlateAction, action);
	});
	QObject::connect(ui_->clickwidget, &ClickWidget::toggled, [=](bool _toggled) {
		ui_->selectMark->setVisible(_toggled);
	});
	QObject::connect(ui_->inputAlias, &QLineEdit::editingFinished, [=]() {
		NEW_ACTION(ScreenplayActions::RenameSlateAction, action);
		action->uuid = componentUUID_;
		action->alias = ui_->inputAlias->text();
		INVOKE_ACTION(ScreenplayActions::RenameSlateAction, action);
		ui_->inputAlias->clearFocus();
	});
}

void Screenplay::SlateCardView::reset()
{
	MemDB* memDB = ScreenplayModel::Persistent::GetMemDB();
	ui_->inputAlias->setText(ScreenplayModel::Persistent::KV::Query::screenplay_slate_guid_1_alias(memDB, componentUUID_));
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////
Screenplay::SlateCardComponent::SlateCardComponent(SlateCardView* _view)
	: Composite::MVCComponent(_view)
{

}

Screenplay::SlateCardComponent::SlateCardComponent(SlateCardView* _view, const QString& _uuid)
	: Composite::MVCComponent(_view, _uuid)
{

}

void Screenplay::SlateCardComponent::initialize(Composite::Core* _core)
{
	HandleBus<Screenplay::SlateCardComponent>::Route<ScreenplayActions::RenameSlateReceiver>(this, &Screenplay::SlateCardComponent::HandleRenameSlateCard);
}

void Screenplay::SlateCardComponent::HandleRenameSlateCard(IAction* _action)
{
	CAST_ACTION(ScreenplayActions::RenameSlateAction, _action, action);
	MemDB* memDB = ScreenplayModel::Persistent::GetMemDB();
	ScreenplayModel::Persistent::KV::Update::screenplay_slate_guid_1_alias(memDB, action->uuid, action->alias);
}