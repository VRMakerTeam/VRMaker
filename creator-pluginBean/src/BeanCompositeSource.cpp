#include <QUuid>
#include <QWidget>	   
#include <QLabel>
#include "ClickWidget.h"
#include "Utils.h"
#include "Dispatcher.h"
#include "HandleBus.hxx"
#include "PluginBeanLogger.h"
#include "BeanCompositeCore.h"
#include "AppCore.h"
#include "PluginBean/BeanActions.h"
#include "PluginBean/BeanModel.h"
#include "BeanCompositePanel.h"
#include "BeanCompositeSource.h"

namespace Bean
{
	class SourceUI
	{
	public:
		~SourceUI()
		{
			SAFE_DELETE(root);
		}
		QWidget* root;
		ClickWidget* clickWidget;
		QLabel* thumb;
		QLabel* selectMark;
	};
}

Bean::SourceView::~SourceView()
{
	SAFE_DELETE(ui_);
}

//-----------------------------------------------------------------------
QWidget* Bean::SourceView::GetRoot()
{
	return ui_->root;
}

void Bean::SourceView::setRadioGroup(const QString& _group)
{
	ui_->clickWidget->setGroup(_group);
}
//-----------------------------------------------------------------------
void Bean::SourceView::setupUI()
{
	ui_ = new SourceUI();

	ui_->root = AppCore::CreateUI("beanSource");

	ui_->clickWidget = ui_->root->findChild<ClickWidget*>("__clickwidget");
	ui_->thumb = ui_->root->findChild<QLabel*>("__imgThumbnail");
	ui_->selectMark = ui_->root->findChild<QLabel*>("__selectedMark");
	ui_->selectMark->setVisible(false);

	QObject::connect(ui_->clickWidget, &ClickWidget::toggled, [=](bool _toggled) {
		ui_->selectMark->setVisible(_toggled);
		NEW_ACTION(BeanActions::SelectSourceAction, action);
		action->uuid = componentUUID_;
		action->toggled = _toggled;
		INVOKE_ACTION(BeanActions::SelectSourceAction, action);
	});
}

//-----------------------------------------------------------------------
void Bean::SourceView::onUpdate(const QString& _tag)
{
	if (0 != _tag.compare(BeanModel::Runtime::Tags::bean_source))
		return;

	MemDB* memDB = BeanModel::Runtime::GetMemDB();
	QString alias = BeanModel::Runtime::KV::Query::bean_source_guid_1_alias(memDB, componentUUID_);
	QString category = "";
	if (this->HasTag("Extend"))
		category = "Extend";
	if (this->HasTag("Template"))
		category = "Template";
	QString imgPath = QString("%1/%2@%3.png").arg(category).arg(alias).arg(AppCore::Language());
	ui_->thumb->setPixmap(QPixmap(imgPath));
}

//-----------------------------------------------------------------------
void Bean::SourceView::reset()
{
	if (this->HasTag("Template"))
		ui_->clickWidget->setGroup("bean:source:template");
	ui_->selectMark->setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
Bean::SourceComponent::SourceComponent(Bean::SourceView* _view, const QString& _uuid)
	: Composite::MVCComponent(_view, _uuid)
{
}

//-----------------------------------------------------------------------
void Bean::SourceComponent::initialize(Composite::Core* _core)
{
	//为Template设置单选组
	ResetUI();
}
