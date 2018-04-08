#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QVariant>
#include "AppCore.h"
#include "Utils.h"
#include "AudioUtil.h"
#include "flowlayout.h"
#include "HandleBus.hxx"
#include "Dispatcher.h"
#include "UGCManager.h"
#include "PackageManager.h"
#include "pluginBucket/BucketActions.h"
#include "pluginBucket/BucketModel.h"
#include "BucketCompositeAssets.h"
#include "BucketCompositeCard.h"
#include "BucketCompositeWall.h"

namespace Bucket
{
	class WallUI
	{
	public:
		QWidget* root;
		QPushButton* btnPlayAudio;
		QPushButton* btnStopAudio;
		QWidget* container;
		QWidget* toolbar;
	};
}

QWidget* Bucket::WallView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Bucket::WallView::setupUI()
{
	ui_ = new WallUI();
	ui_->root = AppCore::CreateUI("bucketWall");
	ui_->btnPlayAudio = ui_->root->findChild<QPushButton*>("__btnPlayAudio");
	ui_->btnStopAudio = ui_->root->findChild<QPushButton*>("__btnStopAudio");
	ui_->container = ui_->root->findChild<QWidget*>("__container");
	ui_->toolbar = ui_->root->findChild<QWidget*>("__toolbar");

	FlowLayout* layout = new FlowLayout(ui_->container);
	ui_->container->setLayout(layout);
	ui_->toolbar->setVisible(false);
	QObject::connect(ui_->btnPlayAudio, &QPushButton::clicked, [=] {
		QString uuid = ui_->btnPlayAudio->property("guid").toString();
		QString package = PackageManager::QueryFilePackage(uuid);
		AudioUtils::Player::PlayPKG(package, uuid);
	});
	QObject::connect(ui_->btnStopAudio, &QPushButton::clicked, [=] {
		AudioUtils::Player::Stop();
	});
}

void Bucket::WallView::onUpdate(const QString& _tag)
{
	if (0 != _tag.compare(BucketModel::Runtime::Tags::filter))
		return;

	QLayout* layout = ui_->container->layout();
	QLayoutItem* item = NULL;
	while (item = layout->takeAt(0))
	{
		delete item;
	}

	for each (View* child in children_)
	{
		DYN_CAST(CardView, child, view);
		layout->addWidget(view->GetRoot());
		view->onSelect = std::bind(&WallView::onCardSelect, this, std::placeholders::_1);
	}
}

void Bucket::WallView::reset()
{
	QLayout* layout = ui_->container->layout();
	for each (View* child in children_)
	{
		DYN_CAST(CardView, child, view);
		layout->removeWidget(view->GetRoot());
		view->GetRoot()->setParent(0);
	}
	ui_->toolbar->setVisible(false);
}

void Bucket::WallView::onCardSelect(const QString& _guid)
{
	ui_->toolbar->setVisible(false);
	ui_->btnPlayAudio->setVisible(false);
	ui_->btnStopAudio->setVisible(false);

	if (PackageManager::QueryFileGroup(_guid).contains("audio"))
	{
		ui_->toolbar->setVisible(true);
		ui_->btnPlayAudio->setVisible(true);
		ui_->btnStopAudio->setVisible(true);
		ui_->btnPlayAudio->setProperty("guid", _guid);
	}
	

	if (NULL != onSelect)
		onSelect(_guid);
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
Bucket::WallComponent::WallComponent(WallView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
void Bucket::WallComponent::initialize(Composite::Core* _core)
{
	core_ = _core;
	HandleBus<Bucket::WallComponent>::Route<BucketActions::FilterAssetsGroupReceiver>(this, &Bucket::WallComponent::HandleFilterAssetsGroup);
	HandleBus<Bucket::WallComponent>::Route<BucketActions::FilterAssetsGroupTagsReceiver>(this, &Bucket::WallComponent::HandleFilterAssetsGroupTags);
	HandleBus<Bucket::WallComponent>::Route<BucketActions::FilterUGCAudioReceiver>(this, &Bucket::WallComponent::HandleFilterUGCAudios);
	HandleBus<Bucket::WallComponent>::Route<BucketActions::FilterUGCImageReceiver>(this, &Bucket::WallComponent::HandleFilterUGCImages);
}

void Bucket::WallComponent::HandleFilterAssetsGroup(IAction* _action)
{
	CAST_ACTION(BucketActions::FilterAssetsGroupAction, _action, action);

	for each (QString uuid in this->ListChildren())
	{
		Composite::Component* component = this->FindChild(uuid);
		component->Release();
		this->RemoveMVCChild(uuid);
		SAFE_DELETE(component);
	}

	QStringList& assets = PackageManager::ListFilesInGroup(action->group);
	for each (QString asset in assets)
	{
		CardComponent* card = new CardComponent(new CardView(), asset);
		card->AddTag(action->group);
		this->AddMVCChild(card);
		card->Initialize(core_);
	}
	Composite::Store::Update(BucketModel::Runtime::Tags::filter);
}

void Bucket::WallComponent::HandleFilterAssetsGroupTags(IAction* _action)
{
	CAST_ACTION(BucketActions::FilterAssetsGroupTagsAction, _action, action);

	//remove all cards
	for each (QString uuid in this->ListChildren())
	{
		Composite::Component* component = this->FindChild(uuid);
		component->Release();
		this->RemoveMVCChild(uuid);
		SAFE_DELETE(component);
	}

	QStringList& assets = PackageManager::ListFilesInGroupTags(action->group, action->tags);
	for each (QString asset in assets)
	{
		CardComponent* card = new CardComponent(new CardView(), asset);
		card->AddTag(action->group);
		this->AddMVCChild(card);
		card->Initialize(core_);
	}
	Composite::Store::Update(BucketModel::Runtime::Tags::filter);
}

void Bucket::WallComponent::HandleFilterUGCAudios(IAction* _action)
{
	CAST_ACTION(BucketActions::FilterUGCAudioAction, _action, action);

	for each (QString uuid in this->ListChildren())
	{
		Composite::Component* component = this->FindChild(uuid);
		component->Release();
		this->RemoveMVCChild(uuid);
		SAFE_DELETE(component);
	}

	QStringList& assets = UGCManager::ListAudios();
	for each (QString asset in assets)
	{
		CardComponent* card = new CardComponent(new CardView(), asset);
		card->AddTag("ugc:audio");
		this->AddMVCChild(card);
		card->Initialize(core_);
	}
	Composite::Store::Update(BucketModel::Runtime::Tags::filter);
}

void Bucket::WallComponent::HandleFilterUGCImages(IAction* _action)
{
	CAST_ACTION(BucketActions::FilterUGCImageAction, _action, action);

	for each (QString uuid in this->ListChildren())
	{
		Composite::Component* component = this->FindChild(uuid);
		component->Release();
		this->RemoveMVCChild(uuid);
		SAFE_DELETE(component);
	}

	QStringList& assets = UGCManager::ListImages();
	for each (QString asset in assets)
	{
		CardComponent* card = new CardComponent(new CardView(), asset);
		card->AddTag("ugc:image");
		this->AddMVCChild(card);
		card->Initialize(core_);
	}
	Composite::Store::Update(BucketModel::Runtime::Tags::filter);
}
