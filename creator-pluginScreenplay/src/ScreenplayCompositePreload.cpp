#include <assert.h>
#include <QUuid>
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include "AppCore.h"
#include "IPlugin.h"
#include "HandleBus.hxx"
#include "Dispatcher.h"
#include "Utils.h"
#include "HttpUtil.h"
#include "PathUtil.h"
#include "PluginScreenplayLogger.h"
#include "pluginBucket/pluginBucket.h"
#include "pluginBucket/BucketActions.h"
#include "pluginBucket/BucketUtils.h"
#include "PluginDiagram/DiagramModel.h"
#include "PluginUnity/UnityActions.h"
#include "pluginBucket/BucketUtils.h"
#include "PluginScreenplay/ScreenplayActions.h"
#include "PluginScreenplay/ScreenplayModel.h"
#include "ScreenplayCompositeCore.h"
#include "ScreenplayCompositePanel.h"
#include "ScreenplayCompositePreloadCard.h"
#include "ScreenplaycompositePreload.h"


namespace Screenplay
{
	class PreloadUI
	{
	public:
		QWidget* root;
		QWidget* container;
		QPushButton* add;
		QPushButton* del;
		QPushButton* pick;
		QPushButton* fastAdd;
	};
}

//-----------------------------------------------------------------------
Screenplay::PreloadView::~PreloadView()
{
}

QWidget* Screenplay::PreloadView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Screenplay::PreloadView::setupUI()
{
	ui_ = new PreloadUI();
	ui_->root = AppCore::CreateUI("screenplayPreload");
	ui_->container = ui_->root->findChild<QWidget*>("__container");
	ui_->add = ui_->root->findChild<QPushButton*>("__add");
	ui_->del = ui_->root->findChild<QPushButton*>("__delete");
	ui_->pick = ui_->root->findChild<QPushButton*>("__pickSkybox");
	ui_->fastAdd = ui_->root->findChild<QPushButton*>("__fastAdd");

	ptrDefaultSkyboxIcon_ = new QIcon(ui_->pick->icon());

	QObject::connect(ui_->add, &QPushButton::clicked, [=] {
		LOGX_DEBUG(XC_PluginScreenplay, XL_LOW_PluginScreenplay, "click add preload");
		NEW_ACTION(BucketActions::PopupPickerAction, action);
		action->filter = QStringList("agent/");
		action->onOK = std::bind(&Screenplay::PreloadView::onPickAgentOK, this, std::placeholders::_1);
		INVOKE_ACTION(BucketActions::PopupPickerAction, action);
	});
	QObject::connect(ui_->fastAdd, &QPushButton::clicked, [=] {
		LOGX_DEBUG(XC_PluginScreenplay, XL_LOW_PluginScreenplay, "click add preload");
		NEW_ACTION(BucketActions::PopupPickerAction, action);
		action->filter = QStringList("agent/");
		action->onOK = std::bind(&Screenplay::PreloadView::onPickAgentOK, this, std::placeholders::_1);
		INVOKE_ACTION(BucketActions::PopupPickerAction, action);
	});
	QObject::connect(ui_->pick, &QPushButton::clicked, [=] {
		LOGX_DEBUG(XC_PluginScreenplay, XL_LOW_PluginScreenplay, "click skybox picker");
		NEW_ACTION(BucketActions::PopupPickerAction, action);
		action->filter << QStringList("skybox/") << QStringList("ugc/image");
		action->onOK = std::bind(&Screenplay::PreloadView::onPickSkyboxOK, this, std::placeholders::_1);
		INVOKE_ACTION(BucketActions::PopupPickerAction, action);
	});
}

//-----------------------------------------------------------------------
void Screenplay::PreloadView::onUpdate(const QString& _tag)
{
	if (0 == _tag.compare(ScreenplayModel::Persistent::Tags::slate_update_skybox))
	{
		MemDB* runtimeDB = ScreenplayModel::Runtime::GetMemDB();
		QString slateUUID = ScreenplayModel::Runtime::KV::Query::screenplay_slate_alive_uuid(runtimeDB);

		MemDB* persistentDB = ScreenplayModel::Persistent::GetMemDB();
		QString assetGUID = ScreenplayModel::Persistent::KV::Query::screenplay_slate_guid_1_preload_skybox_file(persistentDB, slateUUID);
		if (assetGUID.isNull() || assetGUID.isEmpty())
		{
			ui_->pick->setIcon(*ptrDefaultSkyboxIcon_);
		}
		else
		{
			if (assetGUID.contains(".jpg") || assetGUID.contains(".png"))
			{
				QString imgPath = PathUtil::PackageLocation() + "/ugc/" + assetGUID;
				ui_->pick->setIcon(QPixmap(imgPath));
			}
			else
			{
				QString imgPath = BucketUtils::QueryFileThumbPath(assetGUID);
				ui_->pick->setIcon(QPixmap(imgPath));
			}
		}
	}

	if (0 == _tag.compare(ScreenplayModel::Persistent::Tags::slate_update_agent))
	{
		for each (View* view in children_)
		{
			DYN_CAST(PreloadCardView, view, card);
			if (NULL == card)
				continue;
			ui_->container->layout()->addWidget(card->GetRoot());
		}
	}
}
void Screenplay::PreloadView::onPickSkyboxOK(const QString& _guid)
{
	LOGX_DEBUG(XC_PluginScreenplay, XL_LOW_PluginScreenplay, QString("on pick asset [%1]").arg(_guid));
	NEW_ACTION(ScreenplayActions::OnPickAssetAction, action);
	action->assetGUID = _guid;
	action->invoker = QString("%1#skybox").arg(componentUUID_);
	INVOKE_ACTION(ScreenplayActions::OnPickAssetAction, action);
}


void Screenplay::PreloadView::onPickAgentOK(const QString& _guid)
{
	LOGX_DEBUG(XC_PluginScreenplay, XL_LOW_PluginScreenplay, QString("on pick asset [%1]").arg(_guid));
	NEW_ACTION(ScreenplayActions::OnPickAssetAction, action);
	action->assetGUID = _guid;
	action->invoker = QString("%1#agent").arg(componentUUID_);
	INVOKE_ACTION(ScreenplayActions::OnPickAssetAction, action);
}


//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
Screenplay::PreloadComponent::PreloadComponent(PreloadView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
Screenplay::PreloadComponent::~PreloadComponent()
{

}

//-----------------------------------------------------------------------
void Screenplay::PreloadComponent::initialize(Composite::Core* _core)
{
	ptrCore_ = _core;
	HandleBus<Screenplay::PreloadComponent>::Route<ScreenplayActions::OnPickAssetReceiver>(this, &Screenplay::PreloadComponent::HandleOnPickAssetOK);

	HandleBus<Screenplay::PreloadComponent>::Route<ScreenplayActions::DeletePreloadReceiver>(this, &Screenplay::PreloadComponent::HandleDeletePreload);
	HandleBus<Screenplay::PreloadComponent>::Route<ScreenplayActions::MessageFromUnityReceiver>(this, &Screenplay::PreloadComponent::HandleMessageFromUnity);
	HandleBus<Screenplay::PreloadComponent>::Route<ScreenplayActions::SelectSlateReceiver>(this, &Screenplay::PreloadComponent::HandleRefresh);
}
//-----------------------------------------------------------------------
void Screenplay::PreloadComponent::HandleOnPickAssetOK(IAction* _action)
{
	CAST_ACTION(ScreenplayActions::OnPickAssetAction, _action, action);
	if (action->invoker.contains(this->GetUUID()))
	{
		MemDB* runtimeDB = ScreenplayModel::Runtime::GetMemDB();
		QString slateUUID = ScreenplayModel::Runtime::KV::Query::screenplay_slate_alive_uuid(runtimeDB);
		MemDB* persistentDB = ScreenplayModel::Persistent::GetMemDB();

		QString fileUUID = action->assetGUID;
		QString packageUUID = BucketUtils::QueryFilePackage(fileUUID);

		if (action->invoker.contains("skybox"))
		{
			ScreenplayModel::Persistent::KV::Update::screenplay_slate_guid_1_preload_skybox_file(persistentDB, slateUUID, fileUUID);
			ScreenplayModel::Persistent::KV::Update::screenplay_slate_guid_1_preload_skybox_package(persistentDB, slateUUID, packageUUID);

			Composite::Store::Update(ScreenplayModel::Persistent::Tags::slate_update_skybox);
			if (action->assetGUID.contains(".png") || action->assetGUID.contains(".jpg"))
			{

				NEW_ACTION(UnityActions::WSMessageToUnityAction, actionMessage);
				actionMessage->method = "JSEditorUseSkybox";
				actionMessage->activity = "0";
				actionMessage->param = QString("{\"source\":\"ugc\", \"file\":\"%1\"}").arg(fileUUID);
				INVOKE_ACTION(UnityActions::WSMessageToUnityAction, actionMessage);
			}
			else
			{
				NEW_ACTION(UnityActions::WSMessageToUnityAction, actionMessage);
				actionMessage->method = "JSEditorUseSkybox";
				actionMessage->activity = "0";
				actionMessage->param = QString("{\"source\":\"asset\", \"package\":\"%1\", \"file\":\"%2\"}").arg(packageUUID).arg(fileUUID);
				INVOKE_ACTION(UnityActions::WSMessageToUnityAction, actionMessage);
			}
			
		}
		else if (action->invoker.contains("agent"))
		{
			newPreload(QUuid::createUuid().toString().remove("{").remove("}"), fileUUID);
		}
	}
}

//-----------------------------------------------------------------------
void Screenplay::PreloadComponent::HandleDeletePreload(IAction* _action)
{
	CAST_ACTION(ScreenplayActions::DeletePreloadAction, _action, action);
	deletePreload(action->uuid);
}

//-----------------------------------------------------------------------
void Screenplay::PreloadComponent::HandleMessageFromUnity(IAction* _action)
{
	CAST_ACTION(UnityActions::WSMessageFromUnityAction, _action, action);
}

void Screenplay::PreloadComponent::HandleRefresh(IAction* _action)
{
	Composite::Store::Update(ScreenplayModel::Persistent::Tags::slate_update_skybox);
	for each (QString uuid in this->ListChildren())
	{
		removePreload(uuid);
	}

	MemDB* runtimeDB = ScreenplayModel::Runtime::GetMemDB();
	QString aliveSlate = ScreenplayModel::Runtime::KV::Query::screenplay_slate_alive_uuid(runtimeDB);
	MemDB* persistentDB = ScreenplayModel::Persistent::GetMemDB();
	QList<QString> preloads = ScreenplayModel::Persistent::Sets::Query::screenplay_slate_guid_1_preloads_guids(persistentDB, aliveSlate);
	for each (QString guid in preloads)
	{
		QString assetGUID = ScreenplayModel::Persistent::KV::Query::screenplay_slate_guid_1_preload_guid_2_asset_file(persistentDB, aliveSlate, guid);
		addPreload(guid, assetGUID);
	}

}

void Screenplay::PreloadComponent::newPreload(const QString& _guid, const QString& _assetGUID)
{
	QString package = BucketUtils::QueryFilePackage(_assetGUID);

	MemDB* runtimeMemDB = ScreenplayModel::Runtime::GetMemDB();
	QString aliveSlate = ScreenplayModel::Runtime::KV::Query::screenplay_slate_alive_uuid(runtimeMemDB);

	MemDB* persistentMemDB = ScreenplayModel::Persistent::GetMemDB();
	ScreenplayModel::Persistent::Sets::Update::Add::screenplay_slate_guid_1_preloads_guids(persistentMemDB, aliveSlate, _guid);
	ScreenplayModel::Persistent::KV::Update::screenplay_slate_guid_1_preload_guid_2_asset_file(persistentMemDB, aliveSlate, _guid, _assetGUID);
	ScreenplayModel::Persistent::KV::Update::screenplay_slate_guid_1_preload_guid_2_asset_package(persistentMemDB, aliveSlate, _guid, package);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_px(persistentMemDB, _guid, 0);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_py(persistentMemDB, _guid, 0);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_pz(persistentMemDB, _guid, 0);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_rx(persistentMemDB, _guid, 0);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_ry(persistentMemDB, _guid, 0);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_rz(persistentMemDB, _guid, 0);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_sx(persistentMemDB, _guid, 1);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_sy(persistentMemDB, _guid, 1);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_sz(persistentMemDB, _guid, 1);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_gaze(persistentMemDB, _guid, false);

	int index = DiagramModel::Persistent::KV::Query::diagram_triggers_index(persistentMemDB).toInt();
	DiagramModel::Persistent::KV::Update::diagram_triggers_index(persistentMemDB, index + 1);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_gaze_alias(persistentMemDB, _guid, QString("%1").arg(index));

	addPreload(_guid, _assetGUID);

	NEW_ACTION(UnityActions::WSMessageToUnityAction, action);
	action->method = "JSEditorNewClone";
	action->activity = "0";
	action->param = QString("{\"package\":\"%1\", \"file\":\"%2\", \"uuid\":\"%3\"}").arg(package).arg(_assetGUID).arg(_guid);
	INVOKE_ACTION(UnityActions::WSMessageToUnityAction, action);
}

void Screenplay::PreloadComponent::deletePreload(const QString& _uuid)
{
	MemDB* runtimeMemDB = ScreenplayModel::Runtime::GetMemDB();
	QString aliveSlate = ScreenplayModel::Runtime::KV::Query::screenplay_slate_alive_uuid(runtimeMemDB);
	MemDB* persistentMemDB = ScreenplayModel::Persistent::GetMemDB();
	ScreenplayModel::Persistent::Sets::Update::Remove::screenplay_slate_guid_1_preloads_guids(persistentMemDB, aliveSlate, _uuid);
	ScreenplayModel::Persistent::KV::Remove::screenplay_slate_guid_1_preload_guid_2_asset_file(runtimeMemDB, aliveSlate, _uuid);

	removePreload(_uuid);

	NEW_ACTION(UnityActions::WSMessageToUnityAction, action);
	action->method = "JSEditorDeleteClone";
	action->activity = "0";
	action->param = QString("{\"uuid\":\"%1\"}").arg(_uuid);
	INVOKE_ACTION(UnityActions::WSMessageToUnityAction, action);
}

void Screenplay::PreloadComponent::addPreload(const QString& _guid, const QString& _assetGUID)
{
	PreloadCardComponent* card = PreloadCardComponent::New(_guid);
	card->Initialize(ptrCore_);
	this->AddMVCChild(card);
	Composite::Store::Update(ScreenplayModel::Persistent::Tags::slate_update_agent);
}

void Screenplay::PreloadComponent::removePreload(const QString& _uuid)
{
	Composite::Component* component = this->FindChild(_uuid);
	assert(component);
	Screenplay::PreloadCardComponent* card = dynamic_cast<Screenplay::PreloadCardComponent*>(component);
	card->Release();
	//TODO delete the card when receive message from unity is better.
	this->RemoveMVCChild(card);
	delete card;
	Composite::Store::Update(ScreenplayModel::Persistent::Tags::slate_update_agent);
}