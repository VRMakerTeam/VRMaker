#include <algorithm>
#include <QDebug>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QLayoutItem>
#include <QUuid>
#include "Utils.h"
#include "AppCore.h"
#include "ClickWidget.h"
#include "HandleBus.hxx"
#include "PluginScreenplayLogger.h"
#include "PluginScreenplay/ScreenplayActions.h"
#include "PluginDiagram/DiagramActions.h"
#include "PluginUnity/UnityActions.h"
#include "PluginScreenplay/ScreenplayModel.h"
#include "pluginBucket/BucketUtils.h"
#include "PluginBean/BeanModel.h"
#include "PluginBean/BeanActions.h"
#include "ScreenplayCompositeCore.h"
#include "ScreenplayCompositeSlateCard.h"
#include "ScreenplayCompositeSlate.h"

namespace Screenplay
{
	class SlateUI
	{
	public:
		QWidget* root;
		QPushButton* fastAdd;
		QWidget* container;
		QPushButton* add;
		QPushButton* del;
		QWidget* toolbar;
	};
}

QWidget* Screenplay::SlateView::GetRoot()
{
	return ui_->root;
}

void Screenplay::SlateView::ToggleDeleteButton(bool _visible)
{
	ui_->del->setVisible(_visible);
}

void Screenplay::SlateView::setupUI()
{
	ui_ = new SlateUI();
	ui_->root = AppCore::CreateUI("screenplaySlate");
	ui_->fastAdd = ui_->root->findChild<QPushButton*>("__fastAdd");
	ui_->container = ui_->root->findChild<QWidget*>("__container");
	ui_->toolbar = ui_->root->findChild<QWidget*>("__toolbar");
	ui_->add = ui_->root->findChild<QPushButton*>("__add");
	ui_->del = ui_->root->findChild<QPushButton*>("__delete");

	ui_->del->setVisible(false);

	QObject::connect(ui_->fastAdd, &QPushButton::clicked, [=] {
		NEW_ACTION(ScreenplayActions::NewSlateAction, action);
		INVOKE_ACTION(ScreenplayActions::NewSlateAction, action);
	});

	QObject::connect(ui_->add, &QPushButton::clicked, [=] {
		NEW_ACTION(ScreenplayActions::NewSlateAction, action);
		INVOKE_ACTION(ScreenplayActions::NewSlateAction, action);
	});
	QObject::connect(ui_->del, &QPushButton::clicked, [=] {
		MemDB* memDB = ScreenplayModel::Runtime::GetMemDB();
		QString uuid = ScreenplayModel::Runtime::KV::Query::screenplay_slate_alive_uuid(memDB);
		NEW_ACTION(ScreenplayActions::DeleteSlateAction, action);
		action->uuid = uuid;
		INVOKE_ACTION(ScreenplayActions::DeleteSlateAction, action);
	});
}



void Screenplay::SlateView::onUpdate(const QString& _tag)
{
	if (0 == _tag.compare(ScreenplayModel::Persistent::Tags::slates_update))
	{
		QLayout* layout = ui_->container->layout();
		QLayoutItem* item = NULL;
		while (item = layout->takeAt(0))
		{
			layout->removeItem(item);
		}

		for each (View* view in children_)
		{
			DYN_CAST(SlateCardView, view, card);
			layout->addWidget(card->GetRoot());
		}
	}
	

}

Screenplay::SlateComponent::SlateComponent(SlateView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
Screenplay::SlateComponent::~SlateComponent()
{
	//TODO
	//SAFE_DELETE(toolbarComponent_);
	//SAFE_DELETE(scrollComponent_);
}

//-----------------------------------------------------------------------
void Screenplay::SlateComponent::initialize(Composite::Core* _core)
{
	ptrCore_ = _core;
	HandleBus<SlateComponent>::Route<ScreenplayActions::NewSlateReceiver>(this, &Screenplay::SlateComponent::HandleNewSlateCard);
	HandleBus<SlateComponent>::Route<ScreenplayActions::DeleteSlateReceiver>(this, &Screenplay::SlateComponent::HandleDeleteSlateCard);
	HandleBus<SlateComponent>::Route<ScreenplayActions::SelectSlateReceiver>(this, &Screenplay::SlateComponent::HandleSelectSlateCard);
	HandleBus<SlateComponent>::Route<ScreenplayActions::OnReloadBeanReceiver>(this, &Screenplay::SlateComponent::HandleReloadSlates);

}

void Screenplay::SlateComponent::newSlateCard()
{
	QString guid = QUuid::createUuid().toString().remove("{").remove("}");
	newSlateCard(guid);
}

void Screenplay::SlateComponent::deleteSlateCard(const QString& _uuid)
{
	MemDB* persistentMemDB = ScreenplayModel::Persistent::GetMemDB();
	ScreenplayModel::Persistent::Sets::Update::Remove::screenplay_slates_guids(persistentMemDB, _uuid);
	removeSlateCard(_uuid);

	NEW_ACTION(UnityActions::WSMessageToUnityAction, actionMessageToUnity);
	actionMessageToUnity->method = "JSEditorDeleteImage";
	actionMessageToUnity->activity = "0";
	actionMessageToUnity->param = QString("{\"guid\":\"%1\"}").arg(_uuid);
	INVOKE_ACTION(UnityActions::WSMessageToUnityAction, actionMessageToUnity);
}

void Screenplay::SlateComponent::addSlateCard(const QString& _uuid)
{
	SlateCardComponent* card = new SlateCardComponent(new SlateCardView(), _uuid);
	this->AddMVCChild(card);
	card->Initialize(ptrCore_);
	card->ResetUI(false);

	LOGX_DEBUG(XC_PluginScreenplay, XL_MEDIMU_PluginScreenplay, QString("new a slate card, uuid is %1").arg(_uuid));

	Composite::Store::Update(ScreenplayModel::Persistent::Tags::slates_update);
}

void Screenplay::SlateComponent::removeSlateCard(const QString& _uuid)
{
	LOGX_DEBUG(XC_PluginScreenplay, XL_MEDIMU_PluginScreenplay, QString("delete a slate card, uuid is %1").arg(_uuid));

	Composite::Component* component = this->FindChild(_uuid);
	DYN_CAST(Screenplay::SlateCardComponent, component, card);
	component->Release();
	this->RemoveMVCChild(card);
	SAFE_DELETE(component);

	Composite::Store::Update(ScreenplayModel::Persistent::Tags::slates_update);
}

void Screenplay::SlateComponent::newSlateCard(const QString& _uuid)
{
	MemDB* persistentMemDB = ScreenplayModel::Persistent::GetMemDB();
	ScreenplayModel::Persistent::Sets::Update::Add::screenplay_slates_guids(persistentMemDB, _uuid);
	//addSlatecard方法将调动Reset，使用数据库中的alias更新UI上的显示,所以需要先更新数据库中的alias
	int index = ScreenplayModel::Persistent::KV::Query::screenplay_slate_index(persistentMemDB).toInt();
	index += 1;
	ScreenplayModel::Persistent::KV::Update::screenplay_slate_index(persistentMemDB, index);

	ScreenplayModel::Persistent::KV::Update::screenplay_slate_guid_1_alias(persistentMemDB, _uuid, QString("%1").arg(index));
	ScreenplayModel::Persistent::KV::Update::screenplay_slate_guid_1_preload_skybox_file(persistentMemDB, _uuid, "");
	ScreenplayModel::Persistent::KV::Update::screenplay_slate_guid_1_preload_skybox_package(persistentMemDB, _uuid, "");

	addSlateCard(_uuid);

	NEW_ACTION(UnityActions::WSMessageToUnityAction, actionMessageToUnity);
	actionMessageToUnity->method = "JSEditorNewImage";
	actionMessageToUnity->activity = "0";
	actionMessageToUnity->param = QString("{\"guid\":\"%1\"}").arg(_uuid);
	INVOKE_ACTION(UnityActions::WSMessageToUnityAction, actionMessageToUnity);
}

void Screenplay::SlateComponent::HandleNewSlateCard(IAction* _action)
{
	newSlateCard();
}

void Screenplay::SlateComponent::HandleDeleteSlateCard(IAction* _action)
{
	CAST_ACTION(ScreenplayActions::DeleteSlateAction, _action, action);
	deleteSlateCard(action->uuid);

	DYN_CAST(SlateView, view_, view);
	view->ToggleDeleteButton(false);

	MemDB* runtimeMemDB = ScreenplayModel::Runtime::GetMemDB();
	ScreenplayModel::Runtime::KV::Update::screenplay_slate_alive_uuid(runtimeMemDB, "");
}


void Screenplay::SlateComponent::HandleSelectSlateCard(IAction* _action)
{
	CAST_ACTION(ScreenplayActions::SelectSlateAction, _action, action);
	DYN_CAST(SlateView, view_, view);
	view->ToggleDeleteButton(true);

	MemDB* memDB = ScreenplayModel::Runtime::GetMemDB();
	ScreenplayModel::Runtime::KV::Update::screenplay_slate_alive_uuid(memDB, action->uuid);

	memDB = ScreenplayModel::Persistent::GetMemDB();

	//switch image
	{
		NEW_ACTION(UnityActions::WSMessageToUnityAction, actionMessageToUnity);
		actionMessageToUnity->method = "JSEditorSwitchImage";
		actionMessageToUnity->activity = "0";
		actionMessageToUnity->param = QString("{\"guid\":\"%1\"}").arg(action->uuid);
		INVOKE_ACTION(UnityActions::WSMessageToUnityAction, actionMessageToUnity);
	}
}

void Screenplay::SlateComponent::HandleReloadSlates(IAction* _action)
{
	LOGX_DEBUG(XC_PluginScreenplay, XL_MEDIMU_PluginScreenplay, "reload bean data");

	//移除所有Slate
	for each (QString uuid in this->ListChildren())
	{
		removeSlateCard(uuid);
	}

	MemDB* persistenDB = ScreenplayModel::Persistent::GetMemDB();


	//重建新的slate
	QList<QString> slates = ScreenplayModel::Persistent::Sets::Query::screenplay_slates_guids(persistenDB);
	for each (QString uuid in slates)
	{
		addSlateCard(uuid);
	}
	//刷新slate
	Composite::Store::Update(ScreenplayModel::Persistent::Tags::slates_update);
}
