#include <QWidget>
#include "Utils.h"
#include "HandleBus.hxx"
#include "MemDB.h"
#include "AppCore.h"
#include "DiagramCompositeCore.h"
#include "BlockUiTemplateBuilder.h"
#include "DiagramCompositeGroup.h"
#include "PluginDiagram/DiagramModel.h"
#include "DiagramCompositeBlock.h"

namespace Diagram
{
	class BlockUI
	{
	public:
		BlockUI()
		{
			root = NULL;
		}
		~BlockUI()
		{
			//SAFE_DELETE(root);
		}
		QWidget* root;
	};
}

//-----------------------------------------------------------------------
Diagram::BlockView::BlockView()
{
}

void Diagram::BlockView::Show()
{
	ui_->root->show();
}

void Diagram::BlockView::Hide()
{
	ui_->root->hide();
}

//-----------------------------------------------------------------------
Diagram::BlockView::~BlockView()
{
	SAFE_DELETE(builder_);
	SAFE_DELETE(userData_);
	SAFE_DELETE(ui_);
}

void Diagram::BlockView::setupUI()
{
	ui_ = new BlockUI();
	ui_->root = AppCore::CreateUI("diagramBlock");
}

//-----------------------------------------------------------------------
QWidget* Diagram::BlockView::GetRoot()
{
	return ui_->root;
}
//-----------------------------------------------------------------------
void Diagram::BlockView::BuildArchetype(const QString& _group, const QString& _template)
{
	userData_ = new UIUserData();
	userData_->tpl = _template;
	userData_->group = _group;
	userData_->component = componentUUID_;
	ui_->root->setUserData(UserDataRole, userData_);

	builder_ = BlockUiArchetype::New(ui_->root, componentUUID_, _group, _template);
}

//-----------------------------------------------------------------------
void Diagram::BlockView::BuildClone(const QString& _group, const QString& _template)
{
	userData_ = new UIUserData();
	userData_->tpl = _template;
	userData_->group = _group;
	userData_->component = componentUUID_;
	ui_->root->setUserData(UserDataRole, userData_);

	builder_ = BlockUiClone::New(ui_->root, componentUUID_, _group, _template);
}
//-----------------------------------------------------------------------
QHash<QString, QVariant> Diagram::BlockView::GetParams()
{
	return builder_->Properties();
}

Diagram::BlockView::UIUserData Diagram::BlockView::GetUserData()
{
	return *userData_;
}

void Diagram::BlockView::onUpdate(const QString& _tag)
{
	if (0 != _tag.compare(DiagramModel::Persistent::Tags::digram_block_update))
		return;

	if (this->HasTag("tpl"))
		return;

	DYN_CAST(BlockUiClone, builder_, builder);
	if (NULL == builder)
		return;

	MemDB* memDB = DiagramModel::Persistent::GetMemDB();
	QList<QString> fields = DiagramModel::Persistent::Hash::List::diagram_block_guid_1_fields(memDB, componentUUID_);
	QHash<QString, QVariant> params;
	for each (QString field in fields)
	{
		QVariant val = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(memDB, componentUUID_, field);
		params.insert(field, val);
	}
	builder->Refresh(params);
}

void Diagram::BlockView::reset()
{
	builder_->Reset();
}

//-----------------------------------------------------------------------
Diagram::BlockComponent::~BlockComponent()
{
}

//-----------------------------------------------------------------------
void Diagram::BlockComponent::initialize(Composite::Core* _core)
{
}

void Diagram::BlockComponent::release()
{
}

void Diagram::BlockComponent::buildArchetype(const QString& _group, const QString& _template)
{
	DYN_CAST(BlockView, view_, view);
	view->BuildArchetype(_group, _template);
}

void Diagram::BlockComponent::buildClone(const QString& _group, const QString& _template)
{
	DYN_CAST(BlockView, view_, view);
	view->BuildClone(_group, _template);
}

Diagram::BlockComponent* Diagram::BlockComponent::NewArchetype(CompositeCore::Core* _core,  const QString& _uuid, const QString& _group, const QString& _template)
{
	BlockComponent* componet = new BlockComponent(new BlockView(), _uuid);
	componet->Initialize(_core);
	componet->buildArchetype(_group, _template);
	return componet;
}

Diagram::BlockComponent* Diagram::BlockComponent::NewClone(Composite::Core* _core, const QString& _uuid, const QString& _group, const QString& _template)
{
	BlockComponent* componet = new BlockComponent(new BlockView(), _uuid);
	componet->Initialize(_core);
	componet->buildClone(_group, _template);
	return componet;
}

//-----------------------------------------------------------------------
void Diagram::BlockComponent::Destroy(BlockComponent* _componet)
{
	SAFE_DELETE(_componet);
}

//-----------------------------------------------------------------------
Diagram::BlockComponent::BlockComponent(BlockView* _view)
	: Composite::MVCComponent(_view)
{

}

Diagram::BlockComponent::BlockComponent(BlockView* _view, const QString& _uuid)
	: Composite::MVCComponent(_view, _uuid)
{

}
