#include <QWidget>
#include <QLayout>
#include "Utils.h"
#include "AppCore.h"
#include "BucketCompositeCore.h"
#include "BucketCompositeAssets.h"
#include "BucketCompositePanel.h"

namespace Bucket
{
	class PanelUI
	{
	public:
		QWidget* root;
		QWidget* slotAssets;
	};
}
//-----------------------------------------------------------------------
void Bucket::PanelView::setupUI()
{
	ui_ = new PanelUI();
	ui_->root = AppCore::CreateUI("bucketPanel");
	ui_->slotAssets = ui_->root->findChild<QWidget*>("__slotAssets");
}

void Bucket::PanelView::onAttach(View* _view)
{
	DYN_CAST(AssetsView, _view, view);
	if (NULL != view)
		ui_->slotAssets->layout()->addWidget(view->GetRoot());
}

QWidget* Bucket::PanelView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Bucket::PanelView::AttachAssets(QWidget* _widget)
{
	_widget->setParent(ui_->slotAssets);
	ui_->slotAssets->layout()->addWidget(_widget);
}

Bucket::PanelComponent::PanelComponent(PanelView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
void Bucket::PanelComponent::initialize(Composite::Core* _core)
{
	assetsComponent_ = new AssetsComponent(new AssetsView());
	this->AddMVCChild(assetsComponent_);
}
