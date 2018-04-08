#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include "Utils.h"
#include "HandleBus.hxx"
#include "AppCore.h"
#include "PluginBucketLogger.h"
#include "pluginBucket/BucketActions.h"
#include "BucketCompositeCore.h"
#include "BucketCompositeAssets.h"
#include "BucketCompositeGroup.h"
#include "BucketCompositeWall.h"
#include "BucketCompositeTag.h"
#include "BucketCompositePicker.h"

namespace Bucket
{
	class PickerUI
	{
	public:
		QWidget* root;
		QPushButton* btnOK;
		QPushButton* btnCancel;
		QWidget* slotAssets;
	};
}
//-----------------------------------------------------------------------
Bucket::PickerView::PickerView()
{
	ui_ = new PickerUI();
}
//-----------------------------------------------------------------------
Bucket::PickerView::~PickerView()
{
	SAFE_DELETE(ui_);
}
//-----------------------------------------------------------------------
void Bucket::PickerView::setupUI()
{
	ui_->root = AppCore::CreateUI("bucketPicker");
	ui_->btnOK = ui_->root->findChild<QPushButton*>("__btnOK");
	ui_->btnCancel = ui_->root->findChild<QPushButton*>("__btnCancel");
	ui_->slotAssets = ui_->root->findChild<QWidget*>("__slotAssets");

	QObject::connect(ui_->btnCancel, &QPushButton::clicked, [=] {
		ui_->root->hide();
	});

	QObject::connect(ui_->btnOK, &QPushButton::clicked, [=] {
		ui_->root->hide();
		if (NULL != onOK)
			onOK(selectUUID_);
	});
}
//-----------------------------------------------------------------------
void Bucket::PickerView::onUpdate(const QString& _tag)
{
}

void Bucket::PickerView::onAttach(View* _view)
{
	DYN_CAST(AssetsView, _view, view);
	if (NULL != view)
		ui_->slotAssets->layout()->addWidget(view->GetRoot());
}

void Bucket::PickerView::reset()
{
	selectUUID_ = "";
	ui_->btnOK->setDisabled(true);
}

//-----------------------------------------------------------------------
void Bucket::PickerView::OnCardSelect(const QString& _uuid)
{
	selectUUID_ = _uuid;
	ui_->btnOK->setDisabled(false);
}

void Bucket::PickerView::Popup()
{
	ui_->root->show();
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////

Bucket::PickerComponent::PickerComponent(PickerView* _view)
	: Composite::MVCComponent(_view)
{

}
//-----------------------------------------------------------------------
Bucket::PickerComponent::~PickerComponent()
{
	this->RemoveChild(assetsComponent_->GetUUID());
	SAFE_DELETE(assetsComponent_);
}

//-----------------------------------------------------------------------
void Bucket::PickerComponent::initialize(Composite::Core* _core)
{
	assetsComponent_ = new AssetsComponent(new AssetsView());
	this->AddMVCChild(assetsComponent_);
	DYN_CAST(AssetsView, assetsComponent_->GetView(), assetsView);
	DYN_CAST(PickerView, view_, pickerView);
	assetsView->onSelect = std::bind(&PickerView::OnCardSelect, pickerView, std::placeholders::_1);
	HandleBus<Bucket::PickerComponent>::Route<BucketActions::PopupPickerReceiver>(this, &Bucket::PickerComponent::HandlePopupPicker);
}

//-----------------------------------------------------------------------
void Bucket::PickerComponent::HandlePopupPicker(IAction* _action)
{
	CAST_ACTION(BucketActions::PopupPickerAction, _action, action);
	LOGX_DEBUG(XC_PluginBucket, XL_MEDIMU_PluginBucket, "popup picker");
	Bucket::GroupComponent* groupComponent = assetsComponent_->FindChild<GroupComponent>();
	assert(groupComponent);
	groupComponent->ResetUI(false);
	groupComponent->FilterGroups(action->filter);

	Bucket::TagComponent* tagComponent = assetsComponent_->FindChild<TagComponent>();
	assert(tagComponent);
	tagComponent->ResetUI();

	Bucket::WallComponent* wallComponent = assetsComponent_->FindChild<WallComponent>();
	assert(wallComponent);
	wallComponent->ResetUI();

	DYN_CAST(PickerView, view_, view);
	view->onOK = action->onOK;
	this->ResetUI();
	view->Popup();

	
}
