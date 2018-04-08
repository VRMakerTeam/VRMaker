#include <QUuid>
#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLayout>
#include "DragDropWidget.h"
#include "AppCore.h"
#include "Utils.h"
#include "HandleBus.hxx"
#include "BlocksPipeLayout.h"
#include "DiagramCompositeCore.h"
#include "DiagramCompositeBlock.h"
#include "DiagramCompositeGroup.h"
#include "PluginDiagram/DiagramActions.h"
#include "PluginDiagram/DiagramModel.h"
#include "PluginDiagramLogger.h"
#include "DiagramCompositePipe.h"

#define DD_TAG_PIPE "pipe"

namespace Diagram
{
	class PipeUI
	{
	public:
		~PipeUI()
		{
			//SAFE_DELETE(root);
		}
		QWidget* root;
		DragDropWidget* container;
		QWidget* settingBox;
		QCheckBox* toggleSetting;
		QPushButton* btnRemove;
		BlocksPipeLayout* pipeLayout;
	};
}

//-----------------------------------------------------------------------
Diagram::PipeView::~PipeView()
{
	SAFE_DELETE(ui_->pipeLayout);
	SAFE_DELETE(ui_);
}

QWidget* Diagram::PipeView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Diagram::PipeView::setupUI()
{
	ui_ = new PipeUI();
	ui_->root = AppCore::CreateUI("diagramPipe");
	ui_->container = ui_->root->findChild<DragDropWidget*>("__container");
	ui_->settingBox = ui_->root->findChild<QWidget*>("__settingBox");
	ui_->toggleSetting = ui_->root->findChild<QCheckBox*>("__toggleSetting");
	ui_->btnRemove = ui_->root->findChild<QPushButton*>("__btnRemove");
	

	ui_->container->SetMode(DragDropWidget::DRAGDROP);
	ui_->container->AddTag(DD_TAG_PIPE);
	ui_->container->AppendCloneDelegate =
		std::bind(&Diagram::PipeView::onAppendClone, this, std::placeholders::_1);
	ui_->container->OnSelectDelegate =
		std::bind(&Diagram::PipeView::onSelect, this, std::placeholders::_1);
	ui_->container->OnDropSelfDelegate =
		std::bind(&Diagram::PipeView::onDropSelf, this, std::placeholders::_1);

	ui_->pipeLayout = new BlocksPipeLayout(ui_->container, -12);
	ui_->container->setLayout(ui_->pipeLayout);

	ui_->pipeLayout->OnIndexUpdateDelegate =
		std::bind(&Diagram::PipeView::onIndexUpdate, this, std::placeholders::_1, std::placeholders::_2);

	QObject::connect(ui_->toggleSetting, &QCheckBox::toggled, [=](bool _toggled) {
		ui_->settingBox->setVisible(_toggled);
	});
	QObject::connect(ui_->btnRemove, &QPushButton::clicked, [=] {
		NEW_ACTION(DiagramActions::RemovePipeAction, action);
		action->uuidPipe = componentUUID_;
		INVOKE_ACTION(DiagramActions::RemovePipeAction, action);
	});
	ui_->toggleSetting->setChecked(false);
	ui_->settingBox->setVisible(false);
}

//-----------------------------------------------------------------------
void Diagram::PipeView::onAppendClone(const DragDropWidgeDelegate::AppendCloneParams* _params)
{
	//禁止Pipe之间的复制
	DragDropWidget* ddWidget = _params->source;
	if (ddWidget->HasTag(DD_TAG_PIPE) && !(ui_->container))
		return;

	QWidget* item = _params->item;

	BlockView::UIUserData* userData = dynamic_cast<BlockView::UIUserData*>(item->userData(BlockView::UserDataRole));
	assert(userData);
	
	NEW_ACTION(DiagramActions::NewBlockAction, action);
	action->group = userData->group;
	action->tpl = userData->tpl;
	action->uuidPipe = componentUUID_;
	INVOKE_ACTION(DiagramActions::NewBlockAction, action);
}

//-----------------------------------------------------------------------
void Diagram::PipeView::onSelect(QWidget* _widget)
{
	BlockView::UIUserData* userData = dynamic_cast<BlockView::UIUserData*>(_widget->userData(BlockView::UserDataRole));
	assert(userData);

	NEW_ACTION(DiagramActions::OpenBlockPropertyPanelAction, action);
	action->group = userData->group;
	action->tpl = userData->tpl;
	action->invoker = userData->component;
	INVOKE_ACTION(DiagramActions::OpenBlockPropertyPanelAction, action);
}

//-----------------------------------------------------------------------
void Diagram::PipeView::onDropSelf(QWidget* _widget)
{
	ui_->pipeLayout->relayout();
}

void Diagram::PipeView::onIndexUpdate(QWidget* _widget, int _index)
{
	BlockView::UIUserData* userData = dynamic_cast<BlockView::UIUserData*>(_widget->userData(BlockView::UserDataRole));
	QString blokUUID = userData->component;

	MemDB* memDB = DiagramModel::Persistent::GetMemDB();
	DiagramModel::Persistent::KV::Update::diagram_block_1_index(memDB, blokUUID, _index);
}

//-----------------------------------------------------------------------
void Diagram::PipeView::onUpdate(const QString& _tag)
{
	if (0 != _tag.compare(DiagramModel::Persistent::Tags::digram_blocks_update))
		return;

	BlocksPipeLayout* layout = dynamic_cast<BlocksPipeLayout*>(ui_->container->layout());

	layout->clean();

	for each (View* view in children_)
	{
		DYN_CAST(BlockView, view, block);
		if (view->HasTag(GROUP_ENTRY))
			layout->addEntry(block->GetRoot());
		else if (view->HasTag(GROUP_EXIT))
			layout->addExit(block->GetRoot());
		else
			layout->addBond(block->GetRoot());
	}
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
Diagram::PipeComponent::PipeComponent(PipeView* _view, const QString& _uuid)
	: Composite::MVCComponent(_view, _uuid)
{

}

Diagram::PipeComponent::~PipeComponent()
{

}

//-----------------------------------------------------------------------
void Diagram::PipeComponent::initialize(Composite::Core* _core)
{
	ptrCore_ = _core;
	HandleBus<PipeComponent>::Route<DiagramActions::NewBlockReceiver>(this, &PipeComponent::HandleNewBlock);
	HandleBus<PipeComponent>::Route<DiagramActions::DeleteBlockReceiver>(this, &PipeComponent::HandleDeleteBlock);
}

//-----------------------------------------------------------------------
void Diagram::PipeComponent::release()
{
	HandleBus<PipeComponent>::DisRoute<DiagramActions::NewBlockReceiver>(this, &PipeComponent::HandleNewBlock);
	HandleBus<PipeComponent>::DisRoute<DiagramActions::DeleteBlockReceiver>(this, &PipeComponent::HandleDeleteBlock);

	QList<QString> blocks = this->ListChildren();
	foreach(QString block, blocks)
	{
		removeBlock(block);
	}
}

//-----------------------------------------------------------------------
void Diagram::PipeComponent::refresh()
{
	for each (QString guid in this->ListChildren())
	{
		removeBlock(guid);
	}

	MemDB* memDB = DiagramModel::Persistent::GetMemDB();
	QList<QString> blocks = DiagramModel::Persistent::Sets::Query::diagram_pipe_guid_1_blocks_guids(memDB, this->GetUUID());
	for each (QString guid in blocks)
	{
		QString group = DiagramModel::Persistent::KV::Query::diagram_block_1_group(memDB, guid);
		QString tpl = DiagramModel::Persistent::KV::Query::diagram_block_1_template(memDB, guid);
		addBlock(guid, group, tpl);
	}

	Composite::Store::Update(DiagramModel::Persistent::Tags::digram_blocks_update);
	Composite::Store::Update(DiagramModel::Persistent::Tags::digram_block_update);
}

//-----------------------------------------------------------------------
void Diagram::PipeComponent::HandleNewBlock(IAction* _action)
{
	CAST_ACTION(DiagramActions::NewBlockAction, _action, action);
	if (0 != action->uuidPipe.compare(this->GetUUID()))
		return;

	// pipe only has one entry and one exit
	if (action->group.compare(GROUP_ENTRY) == 0)
		newEntryBlock(action->tpl);
	else if (action->group.compare(GROUP_EXIT) == 0)
		newExitBlock(action->tpl);
	else
		newBlock(action->group, action->tpl);
	Composite::Store::Update(DiagramModel::Persistent::Tags::digram_blocks_update);
}

void Diagram::PipeComponent::HandleDeleteBlock(IAction* _action)
{
	CAST_ACTION(DiagramActions::DeleteBlockAction, _action, action);
	Composite::Component* c = this->FindChild(action->uuid);
	if (NULL == c)
		return;

	deleteBlock(action->uuid);
	Composite::Store::Update(DiagramModel::Persistent::Tags::digram_blocks_update);
}

//-----------------------------------------------------------------------
void Diagram::PipeComponent::newEntryBlock(const QString& _template)
{
	MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();

	QList<QString> children = this->ListChildren();
	for each (QString uuidBlock in children)
	{
		Composite::Component* component = this->FindChild(uuidBlock);
		QString group = DiagramModel::Persistent::KV::Query::diagram_block_1_group(persistentMemDB, component->GetUUID());
		if (0 != group.compare(GROUP_ENTRY))
			continue;
		deleteBlock(uuidBlock);
	}
	newBlock(GROUP_ENTRY, _template);
}

void Diagram::PipeComponent::newExitBlock(const QString& _template)
{
	MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();

	QList<QString> children = this->ListChildren();
	for each (QString uuidPipe in children)
	{
		Composite::Component* component = this->FindChild(uuidPipe);
		QString group = DiagramModel::Persistent::KV::Query::diagram_block_1_group(persistentMemDB, component->GetUUID());
		if (0 != group.compare(GROUP_EXIT))
			continue;

		deleteBlock(uuidPipe);
	}
	newBlock(GROUP_EXIT, _template);
}

void Diagram::PipeComponent::newBlock(const QString& _group, const QString& _template)
{
	QString uuid = QUuid::createUuid().toString().remove("{").remove("}");
	addBlock(uuid, _group, _template);

	MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
	DiagramModel::Persistent::Sets::Update::Add::diagram_pipe_guid_1_blocks_guids(persistentMemDB, this->GetUUID(), uuid);
	DiagramModel::Persistent::KV::Update::diagram_block_1_group(persistentMemDB, uuid, _group);
	DiagramModel::Persistent::KV::Update::diagram_block_1_template(persistentMemDB, uuid, _template);

	Composite::Component* c = this->FindChild(uuid);
	DYN_CAST(BlockComponent, c, component);
	Composite::View* v = component->GetView();
	DYN_CAST(BlockView, v, view);
	
	QHash<QString, QVariant> fields = view->GetParams();
	for each (QString key in fields.keys())
	{
		DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, uuid, key, fields[key]);
	};

}

void Diagram::PipeComponent::deleteBlock(const QString& _uuid)
{
	removeBlock(_uuid);

	//update model layer
	MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
	DiagramModel::Persistent::Sets::Update::Remove::diagram_pipe_guid_1_blocks_guids(persistentMemDB, this->GetUUID(), _uuid);
	DiagramModel::Persistent::KV::Remove::diagram_block_1_group(persistentMemDB, _uuid);
	DiagramModel::Persistent::KV::Remove::diagram_block_1_index(persistentMemDB, _uuid);
	DiagramModel::Persistent::KV::Remove::diagram_block_1_template(persistentMemDB, _uuid);
	DiagramModel::Persistent::Hash::Remove::diagram_block_guid_1_fields(persistentMemDB, _uuid);
}

void Diagram::PipeComponent::addBlock(const QString& _uuid, const QString& _group, const QString& _template)
{
	BlockComponent* component = BlockComponent::NewClone(ptrCore_, _uuid, _group, _template);
	this->AddMVCChild(component);
	component->AddTag(_group);
}

void Diagram::PipeComponent::removeBlock(const QString& _uuid)
{
	Composite::Component* component = this->FindChild(_uuid);
	BlockComponent* block = dynamic_cast<BlockComponent*>(component);
	if (NULL == block)
		return;
	block->Release();
	this->RemoveMVCChild(_uuid);
	BlockComponent::Destroy(block);
}
