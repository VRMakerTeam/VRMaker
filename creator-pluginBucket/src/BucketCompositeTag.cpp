#include <QWidget>
#include <QCheckBox>
#include <QLayout>
#include "AppCore.h"
#include "Utils.h"
#include "HandleBus.hxx"
#include "PackageManager.h"
#include "pluginBucket/BucketActions.h"
#include "pluginBucket/BucketModel.h"
#include "BucketCompositeAssets.h"
#include "BucketCompositeTag.h"

namespace Bucket
{
	class TagUI
	{
	public:
		QWidget* root;
		QCheckBox* tpl;
		QWidget* container;
	};
}

QWidget* Bucket::TagView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Bucket::TagView::setupUI()
{
	ui_ = new TagUI();
	ui_->root = AppCore::CreateUI("bucketTag");
	ui_->tpl = ui_->root->findChild<QCheckBox*>("__tpl");
	ui_->container = ui_->root->findChild<QWidget*>("__container");
	ui_->tpl->hide();
}

//-----------------------------------------------------------------------
void Bucket::TagView::onUpdate(const QString& _tag)
{
	if (0 != _tag.compare(BucketModel::Runtime::Tags::tag))
		return;

	reset();

	MemDB* memDB = BucketModel::Runtime::GetMemDB();
	QList<QString>& tags = BucketModel::Runtime::Sets::Query::assets_tags_filter(memDB);
	for each (QString tag in tags)
	{
		QString translator = PackageManager::TranslateTag(tag);
		append(tag, translator);
	}
}

//-----------------------------------------------------------------------
void Bucket::TagView::reset()
{
	for each (QCheckBox* cb in tagMap_.keys())
	{
		cb->disconnect();
		ui_->container->layout()->removeWidget(cb);
		delete cb;
	}
	tagMap_.clear();
}

void Bucket::TagView::append(const QString& _tag, const QString& _alias)
{
	QCheckBox* tpl = ui_->tpl;
	QCheckBox* cb = new QCheckBox();
	cb->setAccessibleName(tpl->accessibleName());
	cb->setMaximumSize(tpl->maximumSize());
	cb->setMinimumSize(tpl->minimumSize());
	cb->setText(_alias);
	ui_->container->layout()->addWidget(cb);

	tagMap_.insert(cb, _tag);

	QObject::connect(cb, &QCheckBox::clicked, [=] {
		MemDB* memDB = BucketModel::Runtime::GetMemDB();
		NEW_ACTION(BucketActions::FilterAssetsGroupTagsAction, action);
		action->group = BucketModel::Runtime::KV::Query::assets_group_filter(memDB);
		for each (QCheckBox* tag in tagMap_.keys())
		{
			if (tag->isChecked())
				action->tags.append(tagMap_.value(tag));
		}
		INVOKE_ACTION(BucketActions::FilterAssetsGroupTagsAction, action);
	});
}

//////////////////////////////////////////////////////////////////////////
//
// Composite
//
//////////////////////////////////////////////////////////////////////////

Bucket::TagComponent::TagComponent(TagView* _view)
	: Composite::MVCComponent(_view)
{

}

void Bucket::TagComponent::HandleFilterTagsGroup(IAction* _action)
{
	CAST_ACTION(BucketActions::FilterTagsGroupAction, _action, action);
	MemDB* memDB = BucketModel::Runtime::GetMemDB();

	QStringList tags = PackageManager::ListTagsInGroup(action->group);
	BucketModel::Runtime::Sets::Remove::assets_tags_filter(memDB);
	BucketModel::Runtime::KV::Update::assets_group_filter(memDB, action->group);
	for each (QString tag in tags)
	{
		BucketModel::Runtime::Sets::Update::Add::assets_tags_filter(memDB, tag);
	}
	Composite::Store::Update(BucketModel::Runtime::Tags::tag);
}

void Bucket::TagComponent::initialize(Composite::Core* _core)
{
	HandleBus<Bucket::TagComponent>::Route<BucketActions::FilterTagsGroupReceiver>(this, &Bucket::TagComponent::HandleFilterTagsGroup);
}
