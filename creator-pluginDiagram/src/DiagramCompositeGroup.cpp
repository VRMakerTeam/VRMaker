#include <QRadioButton>
#include <QUuid>
#include "DragDropWidget.h"
#include "Utils.h"
#include <QWidget>
#include <QLayout>
#include "AppCore.h"
#include "DiagramCompositeCore.h"
#include "Dispatcher.h"
#include "LogX.h"
#include "SVM.h"
#include "HandleBus.hxx"
#include "PluginDiagramLogger.h"
#include "DiagramCompositeBlock.h"
#include "PluginDiagram/DiagramActions.h"
#include "PluginDiagram/DiagramModel.h"
#include "DiagramCompositeGroup.h"

namespace Diagram
{
	class GroupUI
	{
	public:
		QWidget* root;
		QWidget* containerTab;
		DragDropWidget* containerBlock;
	};
}


QWidget* Diagram::GroupView::GetRoot()
{
	return ui_->root;
}

Diagram::GroupView::~GroupView()
{
	SAFE_DELETE(ui_);
}

void Diagram::GroupView::setupUI()
{
	ui_ = new GroupUI();
	ui_->root = AppCore::CreateUI("diagramGroup");

	ui_->containerBlock = ui_->root->findChild<DragDropWidget*>("__containerBlock");
	ui_->containerTab = ui_->root->findChild<QWidget*>("__containerTab");

	ui_->containerBlock->SetMode(DragDropWidget::ONLY_DRAG);
}

void Diagram::GroupView::onUpdate(const QString& _tag)
{
	if (0 == _tag.compare(DiagramModel::Runtime::Tags::blocks_register))
	{
		for each (View* view in children_)
		{
			DYN_CAST(BlockView, view, block);
			ui_->containerBlock->layout()->addWidget(block->GetRoot());
		}
	}

	if (0 == _tag.compare(DiagramModel::Runtime::Tags::blocks_groups))
	{
		for (auto itr = tabs.constBegin(); itr != tabs.constEnd(); ++itr)
		{
			ui_->containerTab->layout()->removeWidget(*itr);
			(*itr)->disconnect();
			(*itr)->deleteLater();
		}
		tabs.clear();

		MemDB* memDB = DiagramModel::Runtime::GetMemDB();
		QList<QString> groups = DiagramModel::Runtime::Sets::Query::diagram_groups(memDB);

		//把Entry放到第一个，Exit放到第二个，第三个留空
		groups.insert(0, "");
		groups.removeAll(GROUP_EXIT);
		groups.insert(0, GROUP_EXIT);
		groups.removeAll(GROUP_ENTRY);
		groups.insert(0, GROUP_ENTRY);

		QGridLayout* gridLayout = dynamic_cast<QGridLayout*>(ui_->containerTab->layout());

		for (int i = 0; i < groups.size(); i++)
		{
			QString group = groups[i];
			if (group.isNull() || group.isEmpty())
				continue;
			QRadioButton* btn = new QRadioButton();
			btn->setAccessibleName("group:tab");
			QString alias = group;
			alias = DiagramModel::Runtime::KV::Query::diagram_group_1_alias(memDB, group);
			btn->setText(alias);
			QObject::connect(btn, &QRadioButton::clicked, [=] {
				NEW_ACTION(DiagramActions::FilterGroupAction, action);
				action->group = group;
				INVOKE_ACTION(DiagramActions::FilterGroupAction, action);
			});
			gridLayout->addWidget(btn, i / 3, i % 3);
			tabs.append(btn);
		}
	}

}

//-----------------------------------------------------------------------
void Diagram::GroupComponent::HandleFilterGroup(IAction* _action)
{
	CAST_ACTION(DiagramActions::FilterGroupAction, _action, action);
	QString group = action->group;
	LOGX_DEBUG(XC_PluginDiagram, XL_LOW_PluginDiagram, QString("Filter group %1").arg(group));
	
	//Hide all blocks
	for each (QString uuidPipe in this->ListChildren())
	{
		Composite::MVCComponent* component = dynamic_cast<Composite::MVCComponent*>(this->FindChild(uuidPipe));
		component->GetView()->Hide();
	}
	

	MemDB* memDB = DiagramModel::Runtime::GetMemDB();
	//获取bean的扩展中包含的blocks
	QStringList extendBlocks;
	QList<QString> filterExtends = DiagramModel::Runtime::Sets::Query::diagram_extends_filter(memDB);
	foreach(QString filterExtend, filterExtends)
	{
		foreach(QString block, DiagramModel::Runtime::Sets::Query::diagram_extend_1_blocks(memDB, filterExtend))
		{
			extendBlocks.append(block);
		}
	}

	QList<QString> groupBlocks = DiagramModel::Runtime::Sets::Query::diagram_group_1_blocks(memDB, group);
	//show blocks of the group
	for each (QString uuidBlock in groupBlocks)
	{
		if (!extendBlocks.contains(uuidBlock))
			continue;

		Composite::Component* component = this->FindChild(uuidBlock);
		if (NULL != component)
		{
			Composite::MVCComponent* component = dynamic_cast<Composite::MVCComponent*>(this->FindChild(uuidBlock));
			component->GetView()->Show();
		}
	}
}

void Diagram::GroupComponent::HandleFilterExtend(IAction* _action)
{
	CAST_ACTION(DiagramActions::FilterExtendAction, _action, action);

	MemDB* memDB = DiagramModel::Runtime::GetMemDB();
	foreach(QString extend, action->extends)
	{
		LOGX_DEBUG(XC_PluginDiagram, XL_LOW_PluginDiagram, QString("Filter extends %1").arg(extend));
		DiagramModel::Runtime::Sets::Update::Add::diagram_extends_filter(memDB, extend);
	}
}

void Diagram::GroupComponent::HandleRegisterBlock(IAction* _action)
{
	CAST_ACTION(DiagramActions::RegisterBlockAction, _action, action);
	registerBlock(action->extend, action->group, action->name);
}

//-----------------------------------------------------------------------
void Diagram::GroupComponent::registerBlock(const QString& _extend, const QString& _group, const QString& _template)
{
	LOGX_DEBUG(XC_PluginDiagram, XL_LOW_PluginDiagram, QString("Register block with group:%1").arg(_group));
	QString uuid = QUuid::createUuid().toString().remove("{").remove("}");
	BlockComponent* block = BlockComponent::NewArchetype(ptrCore_, uuid, _group, _template);
	this->AddMVCChild(block);
	block->AddTag("tpl");
	block->GetView()->Hide();

	MemDB* memDB = DiagramModel::Runtime::GetMemDB();
	DiagramModel::Runtime::Sets::Update::Add::diagram_group_1_blocks(memDB, _group, block->GetUUID());
	DiagramModel::Runtime::Sets::Update::Add::diagram_extend_1_blocks(memDB, _extend, block->GetUUID());
}

//-----------------------------------------------------------------------
void Diagram::GroupComponent::initialize(Composite::Core* _core)
{
	ptrCore_ = _core;
	HandleBus<Diagram::GroupComponent>::Route<DiagramActions::FilterGroupReceiver>(this, &Diagram::GroupComponent::HandleFilterGroup);
	HandleBus<Diagram::GroupComponent>::Route<DiagramActions::FilterExtendReceiver>(this, &Diagram::GroupComponent::HandleFilterExtend);
	HandleBus<Diagram::GroupComponent>::Route<DiagramActions::RegisterBlockReceiver>(this, &Diagram::GroupComponent::HandleRegisterBlock);
}
//-----------------------------------------------------------------------
Diagram::GroupComponent::~GroupComponent()
{
	
}

Diagram::GroupComponent::GroupComponent(GroupView* _view)
	: Composite::MVCComponent(_view)
{

}
