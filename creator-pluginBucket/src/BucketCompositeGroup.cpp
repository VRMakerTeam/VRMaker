#include <QWidget>
#include <QRadioButton>
#include <QPushbutton>
#include <QTreeWidget>
#include "AppCore.h"
#include "Utils.h"
#include "PackageManager.h"
#include "pluginBucket/BucketActions.h"
#include "pluginBucket/BucketModel.h"
#include "BucketCompositeAssets.h"
#include "BucketCompositeGroup.h"

namespace Bucket
{
	class GroupUI
	{
	public:
		QWidget* root;
		QRadioButton* tpl;
		QTreeWidget* container;
		QRadioButton* ugcSound;
		QRadioButton* ugcImage;
		QPushButton* importImage;
		QPushButton* importSound;
	};
}

//-----------------------------------------------------------------------
void Bucket::GroupView::setupUI()
{
	if (ui_ != NULL) {
		return;
	}

	ui_ = new GroupUI();
	ui_->root = AppCore::CreateUI("bucketGroup");

	ui_->tpl = ui_->root->findChild<QRadioButton*>("__tpl");
	ui_->container = ui_->root->findChild<QTreeWidget*>("__container");
	ui_->ugcImage = ui_->root->findChild<QRadioButton*>("__ugcsound");
	ui_->ugcSound = ui_->root->findChild<QRadioButton*>("__ugcimage");
	ui_->importImage = ui_->root->findChild<QPushButton*>("__import_image");
	ui_->importSound = ui_->root->findChild<QPushButton*>("__import_sound");

	ui_->ugcImage->setText(AppCore::Translate("ui.bucketGroup.__ugcimage"));
	ui_->ugcSound->setText(AppCore::Translate("ui.bucketGroup.__ugcsound"));
	
	ui_->tpl->hide();
	ui_->container->clear();


	QObject::connect(ui_->ugcSound, &QRadioButton::clicked, [=] {
		{
			NEW_ACTION(BucketActions::FilterUGCAudioAction, action);
			INVOKE_ACTION(BucketActions::FilterUGCAudioAction, action);
			ui_->container->clearSelection();
		}
	});

	QObject::connect(ui_->ugcImage, &QRadioButton::clicked, [=] {
		{
			NEW_ACTION(BucketActions::FilterUGCImageAction, action);
			INVOKE_ACTION(BucketActions::FilterUGCImageAction, action);
			ui_->container->clearSelection();
		}
	});

	QObject::connect(ui_->importSound, &QRadioButton::clicked, [=] {
		{
			//打开文件窗口导入音频
			NEW_ACTION(BucketActions::ImportAudioAction, action);
			INVOKE_ACTION(BucketActions::ImportAudioAction, action);
			//刷新UGC音频界面
			ui_->ugcSound->click();
		}
	});

	QObject::connect(ui_->importImage, &QRadioButton::clicked, [=] {
		{
			//打开文件窗口导入音频
			NEW_ACTION(BucketActions::ImportImageAction, action);
			INVOKE_ACTION(BucketActions::ImportImageAction, action);
			//刷新UGC图片界面
			ui_->ugcImage->click();
		}
	});


	QObject::connect(ui_->container, &QTreeWidget::itemClicked, [=](QTreeWidgetItem* _item, int _column) {
		if (_item->childCount() == 0)
		{
			{
				NEW_ACTION(BucketActions::FilterAssetsGroupAction, action);
				action->group = groupMap_[_item];
				INVOKE_ACTION(BucketActions::FilterAssetsGroupAction, action);
			}
			{
				NEW_ACTION(BucketActions::FilterTagsGroupAction, action);
				action->group = groupMap_[_item];
				INVOKE_ACTION(BucketActions::FilterTagsGroupAction, action);
			}
			//选中隐藏的模板按钮，间接清除其他radiobutton的选中状态
			ui_->tpl->setChecked(true);
		}
		else
		{
			_item->setExpanded(!_item->isExpanded());
		}
	});
}

//-----------------------------------------------------------------------
void Bucket::GroupView::onUpdate(const QString& _tag)
{
	if (0 != _tag.compare(BucketModel::Runtime::Tags::package))
		return;

	reset();

	QList<QString>& groups = PackageManager::AllGroups();
	for each (QString group in groups)
	{
		QString translator = PackageManager::TranslateGroup(group);
		append(group, translator);
	}
}

//-----------------------------------------------------------------------
void Bucket::GroupView::reset()
{
	ui_->ugcImage->setText(PackageManager::TranslateGroup(ui_->ugcImage->text()));
	ui_->ugcSound->setText(PackageManager::TranslateGroup(ui_->ugcSound->text()));
}

void Bucket::GroupView::append(const QString& _group, const QString& _alias)
{
	QStringList fields = _alias.split('/');
	QTreeWidgetItem* parent = ui_->container->invisibleRootItem();
	for (int i = 0; i < fields.count(); ++i)
	{
		QString field = fields[i];
		bool exists = false;
		int childCount = parent->childCount();
		for (int j = 0; j < childCount; ++j)
		{
			QTreeWidgetItem* child = parent->child(j);
			if (child->text(0).compare(field) == 0)
			{
				parent = child;
				exists = true;
				break;
			}
		}
		if (!exists)
		{
			QTreeWidgetItem* item = new QTreeWidgetItem(parent);
			groupMap_.insert(item, _group);
			item->setText(0, field);
			parent->addChild(item);
			parent = item;
		}
	}


}

QWidget* Bucket::GroupView::GetRoot()
{
	return ui_->root;
}

void Bucket::GroupView::Filter(const QStringList& _groups)
{
	ui_->container->collapseAll();
	for (auto itr = groupMap_.cbegin(); itr != groupMap_.cend(); ++itr)
	{
		itr.key()->setHidden(true);
		for each (QString group in _groups)
		{
			 if(itr.value().contains(group))
				 itr.key()->setHidden(false);
		}
	}

	ui_->ugcSound->setVisible(_groups.contains("ugc/audio"));
	ui_->ugcImage->setVisible(_groups.contains("ugc/image"));
	ui_->importImage->setVisible(false);
	ui_->importSound->setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
//
// Composite
//
//////////////////////////////////////////////////////////////////////////

Bucket::GroupComponent::GroupComponent(GroupView* _view)
	: Composite::MVCComponent(_view)
{

}

void Bucket::GroupComponent::FilterGroups(const QStringList& _groups)
{
	DYN_CAST(GroupView, view_, view);
	view->Filter(_groups);
}

void Bucket::GroupComponent::initialize(Composite::Core* _core)
{
}
