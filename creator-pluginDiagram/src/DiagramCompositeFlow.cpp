#include <QUuid>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include "Utils.h"
#include "MemDB.h"
#include "HandleBus.hxx"
#include "Dispatcher.h"
#include "AppCore.h"
#include "PluginDiagramLogger.h"
#include "PluginDiagram/DiagramActions.h"
#include "PluginDiagram/DiagramModel.h"
#include "DiagramCompositeBlock.h"
#include "DiagramCompositeGroup.h"
#include "DiagramCompositeCore.h"
#include "DiagramCompositePipe.h"
#include "DiagramCompositeFlow.h"

namespace Diagram
{
	class FlowUI
	{
	public:
		QWidget* root;
		QWidget* container;
		QPushButton* btnAddPipe;
	};
}

//-----------------------------------------------------------------------
Diagram::FlowView::FlowView()
{
}

//-----------------------------------------------------------------------
Diagram::FlowView::~FlowView()
{
	SAFE_DELETE(ui_);
}

QWidget* Diagram::FlowView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Diagram::FlowView::ToggleToolbar(bool _toggled)
{
	ui_->btnAddPipe->setVisible(_toggled);
}


//-----------------------------------------------------------------------
void Diagram::FlowView::setupUI()
{
	ui_ = new FlowUI();
	ui_->root = AppCore::CreateUI("diagramFlow");
	ui_->btnAddPipe = ui_->root->findChild<QPushButton*>("__btnAddPipe");
	ui_->container = ui_->root->findChild<QWidget*>("__container");

	ui_->btnAddPipe->setVisible(false);
	QObject::connect(ui_->btnAddPipe, &QPushButton::clicked, [=] {
		NEW_ACTION(DiagramActions::AddPipeAction, action);
		INVOKE_ACTION(DiagramActions::AddPipeAction, action);
	});
}


void Diagram::FlowView::onUpdate(const QString& _tag)
{
	if (0 == _tag.compare(DiagramModel::Persistent::Tags::digram_pipes_update))
	{
		QLayout* layout = ui_->container->layout();
		QLayoutItem* item = NULL;
		while (item = layout->takeAt(0))
		{
			layout->removeItem(item);
		}

		for each (View* view in children_)
		{
			DYN_CAST(PipeView, view, pipe);
			layout->addWidget(pipe->GetRoot());
		}
	}
}

void Diagram::FlowView::reset()
{
	QLayout* layout = ui_->container->layout();
	QLayoutItem* item = NULL;
	while (item = layout->takeAt(0))
	{
		layout->removeItem(item);
	}
}

//-----------------------------------------------------------------------
Diagram::FlowComponent::FlowComponent(FlowView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
void Diagram::FlowComponent::initialize(Composite::Core* _core)
{
	ptrCore_ = _core;
	HandleBus<Diagram::FlowComponent>::Route<DiagramActions::ToggleFlowToolbarReceiver>(this, &Diagram::FlowComponent::HandleToggleFlowToolbar);
	HandleBus<Diagram::FlowComponent>::Route<DiagramActions::AddPipeReceiver>(this, &Diagram::FlowComponent::HandleAddPipe);
	HandleBus<Diagram::FlowComponent>::Route<DiagramActions::RemovePipeReceiver>(this, &Diagram::FlowComponent::HandleRemovePipe);
	HandleBus<Diagram::FlowComponent>::Route<DiagramActions::CleanFlowReceiver>(this, &Diagram::FlowComponent::HandleCleanFlow);
	HandleBus<Diagram::FlowComponent>::Route<DiagramActions::RefreshFlowReceiver>(this, &Diagram::FlowComponent::HandleRefreshFlow);
}

void Diagram::FlowComponent::release()
{
}

//-----------------------------------------------------------------------
void Diagram::FlowComponent::HandleToggleFlowToolbar(IAction* _action)
{
	CAST_ACTION(DiagramActions::ToggleFlowToolbarAction, _action, action);
	DYN_CAST(FlowView, view_, view);
	view->ToggleToolbar(action->visible);
}

//-----------------------------------------------------------------------
void Diagram::FlowComponent::HandleAddPipe(IAction* _action)
{
	QString guid = QUuid::createUuid().toString().remove("{").remove("}");
	newPipe(guid);
}

//-----------------------------------------------------------------------
void Diagram::FlowComponent::HandleCleanFlow(IAction* _action)
{
	for each (QString uuid in this->ListChildren())
	{
		removePipe(uuid);
	}
	Composite::Store::Update(DiagramModel::Persistent::Tags::digram_pipes_update);
}

//-----------------------------------------------------------------------
void Diagram::FlowComponent::HandleRefreshFlow(IAction* _action)
{
	CAST_ACTION(DiagramActions::RefreshFlowAction, _action, action);
	
	for each (QString uuid in this->ListChildren())
	{
		removePipe(uuid);
	}
	
	for each (QString guid in action->pipesGUID)
	{
		addPipe(guid);
		refreshPipe(guid);
		//DiagramModel::Persistent::Sets::Query::diagram_pipe_guid_1_blocks_guids(memDB, )
	}
	Composite::Store::Update(DiagramModel::Persistent::Tags::digram_pipes_update);
}

//-----------------------------------------------------------------------
void Diagram::FlowComponent::HandleRemovePipe(IAction* _action)
{
	CAST_ACTION(DiagramActions::RemovePipeAction, _action, action);
	LOGX_DEBUG(XC_PluginDiagram, XL_MEDIMU_PluginDiagram, QString("remove the pipe %1").arg(action->uuidPipe));

	deletePipe(action->uuidPipe);
}
//-----------------------------------------------------------------------
void Diagram::FlowComponent::newPipe(const QString& _guid)
{
	// update model layer
	MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
	DiagramModel::Persistent::Sets::Update::Add::diagram_pipes_guids(persistentMemDB, _guid);

	addPipe(_guid);
	Composite::Store::Update(DiagramModel::Persistent::Tags::digram_pipes_update);

	NEW_ACTION(DiagramActions::OnPipeAddAction, action);
	action->uuidPipe = _guid;
	INVOKE_ACTION(DiagramActions::OnPipeAddAction, action);
}

//-----------------------------------------------------------------------
void Diagram::FlowComponent::addPipe(const QString& _guid)
{
	PipeComponent* pipe = new PipeComponent(new PipeView(), _guid);
	pipe->Initialize(ptrCore_);
	this->AddMVCChild(pipe);
}
//-----------------------------------------------------------------------
void Diagram::FlowComponent::deletePipe(const QString& _guid)
{
	// update model layer
	MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
	DiagramModel::Persistent::Sets::Update::Remove::diagram_pipes_guids(persistentMemDB, _guid);

	removePipe(_guid);
	Composite::Store::Update(DiagramModel::Persistent::Tags::digram_pipes_update);

	NEW_ACTION(DiagramActions::OnPipeRemoveAction, actionRemove);
	actionRemove->uuidPipe = _guid;
	INVOKE_ACTION(DiagramActions::OnPipeRemoveAction, actionRemove);
}

//-----------------------------------------------------------------------
void Diagram::FlowComponent::removePipe(const QString& _guid)
{
	Component* component = this->FindChild(_guid);
	component->Release();
	this->RemoveMVCChild(_guid);
	SAFE_DELETE(component);
}

void Diagram::FlowComponent::refreshPipe(const QString& _guid)
{
	Component* component = this->FindChild(_guid);
	DYN_CAST(PipeComponent, component, pipe);
	pipe->Refresh(true);
}
