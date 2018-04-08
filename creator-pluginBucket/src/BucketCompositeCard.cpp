#include <QFileInfo>
#include <QWidget>
#include <QLabel>
#include "Utils.h"
#include "ClickWidget.h"
#include "Dispatcher.h"
#include "PathUtil.h"
#include "BucketCompositeCore.h"
#include "AppCore.h"
#include "pluginBucket/BucketActions.h"
#include "pluginBucket/BucketModel.h"
#include "PackageManager.h"
#include "BucketCompositeCard.h"

namespace Bucket
{
	class CardUI
	{
	public:
		~CardUI()
		{
			SAFE_DELETE(root);
		}
		QWidget* root;
		QLabel* alias;
		QLabel* thumb;
		QLabel* selectMark;
		ClickWidget* clickWidget;
	};
}

QWidget* Bucket::CardView::GetRoot()
{
	return ui_->root;
}

void Bucket::CardView::setRadioGroup(const QString& _group)
{
	ui_->clickWidget->setGroup(_group);
}
//-----------------------------------------------------------------------
Bucket::CardView::~CardView()
{
	SAFE_DELETE(ui_);
}

//-----------------------------------------------------------------------
void Bucket::CardView::setupUI()
{
	ui_ = new CardUI();
	ui_->root = AppCore::CreateUI("bucketAssetCard");
	ui_->alias = ui_->root->findChild<QLabel*>("__alias");
	ui_->thumb = ui_->root->findChild<QLabel*>("__thumb");
	ui_->selectMark = ui_->root->findChild<QLabel*>("__selectedMark");
	ui_->clickWidget = ui_->root->findChild<ClickWidget*>("__clickWidget");

	ui_->selectMark->setVisible(false);

	QObject::connect(ui_->clickWidget, &ClickWidget::clicked, [=] {
		if (NULL != onSelect)
			onSelect(componentUUID_);
	});

	QObject::connect(ui_->clickWidget, &ClickWidget::toggled, [=](bool _toggled) {
		ui_->selectMark->setVisible(_toggled);
	});
}

//-----------------------------------------------------------------------
void Bucket::CardView::onUpdate(const QString& _tag)
{
	if (0 != _tag.compare(BucketModel::Runtime::Tags::filter))
		return;

	if (this->HasTag("ugc:image"))
	{
		ui_->thumb->setPixmap(QPixmap(QString("%1/%2").arg(PathUtil::ThumbLocation()).arg(componentUUID_)));
		ui_->alias->setText(componentUUID_);
	}
	else if (this->HasTag("ugc:audio"))
	{
		ui_->thumb->setPixmap(QPixmap(":/pluginBucket/simple/img/ugc_audio.png"));
		ui_->alias->setText(componentUUID_);
	}
	else
	{
		const QString fileUUID = componentUUID_;
		QString imgPath = PackageManager::QueryFileThumbPath(fileUUID);
		if(!imgPath.isNull() && !imgPath.isEmpty())
			ui_->thumb->setPixmap(QPixmap(imgPath));
		QString alias = PackageManager::QueryFileAlias(componentUUID_);
		QString translator = PackageManager::TranslateResource(alias);
		if (0 == translator.compare(componentUUID_))
			translator = alias;
		ui_->alias->setText(translator);
	}

}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////
Bucket::CardComponent::CardComponent(CardView* _view, const QString& _uuid)
	: Composite::MVCComponent(_view, _uuid)
{

}

void Bucket::CardComponent::initialize(Composite::Core* _core)
{
	DYN_CAST(CardView, view_, view);
	view->setRadioGroup(this->GetParent()->GetUUID());
}
