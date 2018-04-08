#include <QWidget>
#include <QPushButton>
#include "Utils.h"
#include "AppCore.h"
#include "ScreenplayCompositeCore.h"
#include "pluginBucket/BucketUtils.h"
#include "PluginUnity/UnityActions.h"
#include "PluginScreenplay/ScreenplayModel.h"
#include "PluginScreenplay/ScreenplayActions.h"
#include "ScreenplayCompositePreloadCard.h"

namespace Screenplay
{
	class PreloadCardUI
	{
	public:
		~PreloadCardUI()
		{
			SAFE_DELETE(root);
		}
		QWidget* root;
		QPushButton* thumb;
		QPushButton* del;
	};
}

//-----------------------------------------------------------------------
Screenplay::PreloadCardView::PreloadCardView()
{
}

//-----------------------------------------------------------------------
Screenplay::PreloadCardView::~PreloadCardView()
{
	SAFE_DELETE(ui_);
}

//-----------------------------------------------------------------------
QWidget* Screenplay::PreloadCardView::GetRoot()
{
	return ui_->root;
}
//-----------------------------------------------------------------------
void Screenplay::PreloadCardView::setupUI()
{
	ui_ = new PreloadCardUI();
	ui_->root = AppCore::CreateUI("screenplayPreloadCard");
	ui_->del = ui_->root->findChild<QPushButton*>("__delete");
	ui_->thumb = ui_->root->findChild<QPushButton*>("__thumb");

	QObject::connect(ui_->del, &QPushButton::clicked, [=] {
		NEW_ACTION(ScreenplayActions::DeletePreloadAction, action);
		action->uuid = componentUUID_;
		INVOKE_ACTION(ScreenplayActions::DeletePreloadAction, action);
	});

	QObject::connect(ui_->thumb, &QPushButton::clicked, [=] {
		//focus agent
		{
			NEW_ACTION(UnityActions::WSMessageToUnityAction, action);
			action->method = "JSEditorSelect";
			action->activity = "0";
			action->param = QString("{\"uuid\":\"%1\"}").arg(componentUUID_);
			INVOKE_ACTION(UnityActions::WSMessageToUnityAction, action);
		}

		//switch to inspector of agent
		{
			NEW_ACTION(ScreenplayActions::SwitchInspectorToAgentAction, action);
			action->invoker = componentUUID_;
			INVOKE_ACTION(ScreenplayActions::SwitchInspectorToAgentAction, action);
		}
	});
}
//-----------------------------------------------------------------------
void Screenplay::PreloadCardView::onUpdate(const QString& _tag)
{
	if (0 != _tag.compare(ScreenplayModel::Persistent::Tags::slate_update_agent))
		return;

	MemDB* persistentDB = ScreenplayModel::Persistent::GetMemDB();
	MemDB* runtimeDB = ScreenplayModel::Runtime::GetMemDB();
	QString slate = ScreenplayModel::Runtime::KV::Query::screenplay_slate_alive_uuid(runtimeDB);
	QString asset = ScreenplayModel::Persistent::KV::Query::screenplay_slate_guid_1_preload_guid_2_asset_file(persistentDB, slate, componentUUID_);
	QString imgPath = BucketUtils::QueryFileThumbPath(asset);
	ui_->thumb->setIcon(QPixmap(imgPath));
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------
Screenplay::PreloadCardComponent* Screenplay::PreloadCardComponent::New()
{
	PreloadCardComponent* card = new PreloadCardComponent(new PreloadCardView());
	return card;
}

//-----------------------------------------------------------------------
Screenplay::PreloadCardComponent* Screenplay::PreloadCardComponent::New(const QString& _uuid)
{
	PreloadCardComponent* card = new PreloadCardComponent(new PreloadCardView(), _uuid);
	return card;
}
//-----------------------------------------------------------------------
void Screenplay::PreloadCardComponent::Destroy(PreloadCardComponent* _componet)
{
	//SAFE_DELETE(_componet);
}

//-----------------------------------------------------------------------
Screenplay::PreloadCardComponent::PreloadCardComponent(PreloadCardView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
Screenplay::PreloadCardComponent::PreloadCardComponent(PreloadCardView* _view, const QString& _uuid)
	: Composite::MVCComponent(_view, _uuid)
{

}
//-----------------------------------------------------------------------
void Screenplay::PreloadCardComponent::initialize(Composite::Core* _core)
{
	ptrCore_ = _core;
}

