#include <QDebug>
#include <QWidget>
#include <QLabel>
#include "Utils.h"
#include "ClickWidget.h"
#include "flowlayout.h"
#include "HandleBus.hxx"
#include "Dispatcher.h"
#include "PackageManager.h"
#include "BucketCompositeCore.h"
#include "AppCore.h"
#include "pluginBucket/BucketActions.h"
#include "pluginBucket/BucketUtils.h"
#include "pluginBucket/BucketModel.h"
#include "BucketCompositeCore.h"
#include "BucketCompositeGroup.h"
#include "BucketCompositeTag.h"
#include "BucketCompositeWall.h"
#include "BucketCompositeCard.h"
#include "BucketCompositeAssets.h"

namespace Bucket
{
	class AssetsUI
	{
	public:
		QWidget* root;
		QWidget* slotGroup;
		QWidget* slotTag;
		QWidget* slotWall;
	};

}
//-----------------------------------------------------------------------
Bucket::AssetsView::~AssetsView()
{
	SAFE_DELETE(ui_);
}

//-----------------------------------------------------------------------
QWidget* Bucket::AssetsView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Bucket::AssetsView::setupUI()
{
	ui_ = new AssetsUI();
	ui_->root = AppCore::CreateUI("bucketAssets");
	ui_->slotGroup = ui_->root->findChild<QWidget*>("__slotGroup");
	ui_->slotTag = ui_->root->findChild<QWidget*>("__slotTag");
	ui_->slotWall = ui_->root->findChild<QWidget*>("__slotWall");

}

void Bucket::AssetsView::onAttach(View* _view)
{
	// attach group
	{
		DYN_CAST(GroupView, _view, view);
		if (NULL != view)
			ui_->slotGroup->layout()->addWidget(view->GetRoot());
	}
	//attach tag
	{
		DYN_CAST(TagView, _view, view);
		if (NULL != view)
			ui_->slotTag->layout()->addWidget(view->GetRoot());
	}
	//attach wall
	{
		DYN_CAST(WallView, _view, view);
		if (NULL != view)
			ui_->slotWall->layout()->addWidget(view->GetRoot());
	}
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
Bucket::AssetsComponent::AssetsComponent(AssetsView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
void Bucket::AssetsComponent::initialize(Composite::Core* _core)
{
	DYN_CAST(AssetsView, view_, view);
	GroupComponent* groupComponent = new GroupComponent(new GroupView());
	this->AddMVCChild(groupComponent);

	TagComponent* tagComponent = new TagComponent(new TagView());
	this->AddMVCChild(tagComponent);

	WallComponent* wallComponent = new WallComponent(new WallView());
	this->AddMVCChild(wallComponent);
	
	DYN_CAST(WallView, wallComponent->GetView(), wallView);
	DYN_CAST(AssetsView, view_, assetView);

	wallView->onSelect = assetView->onSelect;
}
