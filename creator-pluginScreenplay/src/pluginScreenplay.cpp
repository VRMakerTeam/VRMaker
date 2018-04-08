#include <assert.h>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QWidget>
#include <QPushButton>
#include "Utils.h"
#include "HandleBus.hxx"
#include "Dispatcher.h"
#include "AppCore.h"
#include "defines.h"
#include "PluginUnity/UnityActions.h"
#include "PluginUnity/UnityMessages.h"
#include "pluginBucket/BucketActions.h"
#include "PluginBean/BeanActions.h"
#include "PluginBean/BeanModel.h"
#include "PluginDiagram/DiagramActions.h"

#include "PluginScreenplayLogger.h"
#include "ScreenplayCompositeCore.h"
#include "ScreenplayCompositePanel.h"
#include "PluginScreenplay/ScreenplayActions.h"
#include "PluginScreenplay/ScreenplayModel.h"
#include "PluginScreenplay/PluginScreenplay.h"

class PluginScreenplayInternal
{
public:
	Screenplay::CompositeCore* compositeCore;
	Screenplay::PanelComponent* panelComponent;
	QWidget* toolbar;

	void HandleMessageFromUnity(IAction* _action);
};

void PluginScreenplayInternal::HandleMessageFromUnity(IAction* _action)
{
	CAST_ACTION(UnityActions::WSMessageFromUnityAction, _action, action);
	if (0 == action->method.compare("__ready__"))
	{
		compositeCore->GetAppCore()->NotifyLoadingTip(AppCore::Translate("ui.loadingtip.unityready"));
		compositeCore->GetAppCore()->EnterHomePage();
		compositeCore->GetAppCore()->FinishModelTask();
		compositeCore->GetAppCore()->Maximum();
	}
	else if (0 == action->method.compare(JSEditorOnImportFinish))
	{
		HandleBus<Screenplay::SlateComponent>::Handle<ScreenplayActions::OnVXImportFinishReceiver>(_action);
	}
	else if (0 == action->method.compare(JSEditorOnClonePositionUpdate))
	{
		LOGX_DEBUG(XC_PluginScreenplay, XL_MEDIMU_PluginScreenplay, action->param);
		NEW_ACTION(ScreenplayActions::UpdateClonePositionAction, updateAction);
		QJsonDocument doc = QJsonDocument::fromJson(action->param.toUtf8());
		QJsonArray aryClones = doc.array();
		foreach(QJsonValue varClone, aryClones)
		{
			QJsonObject objClone = varClone.toObject();
			updateAction->uuid = objClone.take("uuid").toString();
			updateAction->x = objClone.take("x").toString().toDouble();
			updateAction->y = objClone.take("y").toString().toDouble();
			updateAction->z = objClone.take("z").toString().toDouble();
		}
		INVOKE_ACTION(ScreenplayActions::UpdateClonePositionAction, updateAction);
	}
	else if (0 == action->method.compare(JSEditorOnCloneRotationUpdate))
	{
		LOGX_DEBUG(XC_PluginScreenplay, XL_MEDIMU_PluginScreenplay, action->param);
		NEW_ACTION(ScreenplayActions::UpdateCloneRotationAction, updateAction);
		QJsonDocument doc = QJsonDocument::fromJson(action->param.toUtf8());
		QJsonArray aryClones = doc.array();
		foreach(QJsonValue varClone, aryClones)
		{
			QJsonObject objClone = varClone.toObject();
			updateAction->uuid = objClone.take("uuid").toString();
			updateAction->x = objClone.take("x").toString().toDouble();
			updateAction->y = objClone.take("y").toString().toDouble();
			updateAction->z = objClone.take("z").toString().toDouble();
		}
		INVOKE_ACTION(ScreenplayActions::UpdateCloneRotationAction, updateAction);
	}
	else if (0 == action->method.compare(JSEditorOnCloneScaleUpdate))
	{
		LOGX_DEBUG(XC_PluginScreenplay, XL_MEDIMU_PluginScreenplay, action->param);
		NEW_ACTION(ScreenplayActions::UpdateCloneScaleAction, updateAction);
		QJsonDocument doc = QJsonDocument::fromJson(action->param.toUtf8());
		QJsonArray aryClones = doc.array();
		foreach(QJsonValue varClone, aryClones)
		{
			QJsonObject objClone = varClone.toObject();
			updateAction->uuid = objClone.take("uuid").toString();
			updateAction->x = objClone.take("x").toString().toDouble();
			updateAction->y = objClone.take("y").toString().toDouble();
			updateAction->z = objClone.take("z").toString().toDouble();
		}
		INVOKE_ACTION(ScreenplayActions::UpdateCloneScaleAction, updateAction);
	}
	
}

//-----------------------------------------------------------------------
void PluginScreenplay::Initialize(AppCore* _appCore)
{
	AppCore::LoadTS("pluginScreenplay");
	AppCore::LoadRCC("pluginScreenplay");

	ScreenplayModel::Runtime::InitializeModel();

	Dispatcher::Subscribe<ScreenplayActions::OnNewBeanReceiver, BeanActions::OnNewBeanAction>();
	Dispatcher::Subscribe<ScreenplayActions::CompileReceiver, BeanActions::CompileBeanAction>();

	Dispatcher::Subscribe<ScreenplayActions::OnPickAssetReceiver, ScreenplayActions::OnPickAssetAction>();


	Dispatcher::Subscribe<ScreenplayActions::NewSlateReceiver, ScreenplayActions::NewSlateAction>();
	Dispatcher::Subscribe<ScreenplayActions::DeleteSlateReceiver, ScreenplayActions::DeleteSlateAction>();
	Dispatcher::Subscribe<ScreenplayActions::RenameSlateReceiver, ScreenplayActions::RenameSlateAction>();
	Dispatcher::Subscribe<ScreenplayActions::SelectSlateReceiver, ScreenplayActions::SelectSlateAction>();
	Dispatcher::Subscribe<ScreenplayActions::DeletePreloadReceiver, ScreenplayActions::DeletePreloadAction>();
	Dispatcher::Subscribe<ScreenplayActions::OnReloadBeanReceiver, BeanActions::EditBeanAction>();
	Dispatcher::Subscribe <ScreenplayActions::OnVXImportFinishReceiver, ScreenplayActions::OnVXImportFinishAction> ();

	Dispatcher::Subscribe<ScreenplayActions::MessageFromUnityReceiver, UnityActions::WSMessageFromUnityAction>();
	Dispatcher::Subscribe<ScreenplayActions::OnTriggerAddReceiver, DiagramActions::OnTriggerNewAction>();
	Dispatcher::Subscribe<ScreenplayActions::OnTriggerRemoveReceiver, DiagramActions::OnTriggerDeleteAction>();
	Dispatcher::Subscribe<ScreenplayActions::OnPipeAddReceiver, DiagramActions::OnPipeAddAction>();
	Dispatcher::Subscribe<ScreenplayActions::OnPipeRemoveReceiver, DiagramActions::OnPipeRemoveAction>();

	Dispatcher::Subscribe<ScreenplayActions::UpdateClonePositionReceiver, ScreenplayActions::UpdateClonePositionAction>();
	Dispatcher::Subscribe<ScreenplayActions::UpdateCloneRotationReceiver, ScreenplayActions::UpdateCloneRotationAction>();
	Dispatcher::Subscribe<ScreenplayActions::UpdateCloneScaleReceiver, ScreenplayActions::UpdateCloneScaleAction>();

	Dispatcher::Subscribe<ScreenplayActions::SwitchInspectorToAgentReceiver, ScreenplayActions::SwitchInspectorToAgentAction>();
	Dispatcher::Subscribe<ScreenplayActions::SwitchInspectorToDiagramReceiver, ScreenplayActions::SwitchInspectorToDiagramAction>();
	Dispatcher::Subscribe<ScreenplayActions::SwitchInspectorToDiagramReceiver, DiagramActions::OpenBlockPropertyPanelAction>();
	Dispatcher::Subscribe<ScreenplayActions::SwitchInspectorToDiagramReceiver, DiagramActions::OpenTriggerPropertyPanelAction>();

	internal_ = new PluginScreenplayInternal();

	internal_->compositeCore = new Screenplay::CompositeCore();
	internal_->compositeCore->SetAppCore(_appCore);

	internal_->panelComponent = new Screenplay::PanelComponent(new Screenplay::PanelView());
	internal_->panelComponent->Initialize(internal_->compositeCore);

	internal_->toolbar = AppCore::CreateUI("screenplayToolbar");
	QPushButton* playBean = internal_->toolbar->findChild<QPushButton*>("__play");
	QPushButton* publishBean = internal_->toolbar->findChild<QPushButton*>("__publish");
	QPushButton* installBean = internal_->toolbar->findChild<QPushButton*>("__install");
	
	QObject::connect(playBean, &QPushButton::clicked, [=] {
		MemDB* memDB = BeanModel::Runtime::GetMemDB();
		QString guid = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
		NEW_ACTION(BeanActions::PlayBeanAction, action);
		action->guid = guid;
		INVOKE_ACTION(BeanActions::PlayBeanAction, action);
	});
	QObject::connect(publishBean, &QPushButton::clicked, [=] {
		MemDB* memDB = BeanModel::Runtime::GetMemDB();
		QString guid = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
		NEW_ACTION(BeanActions::PublishBeanAction, action);
		action->guid = guid;
		INVOKE_ACTION(BeanActions::PublishBeanAction, action);
	});
	QObject::connect(installBean, &QPushButton::clicked, [=] {
		MemDB* memDB = BeanModel::Runtime::GetMemDB();
		QString guid = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
		NEW_ACTION(BeanActions::InstallBeanAction, action);
		action->guid = guid;
		INVOKE_ACTION(BeanActions::InstallBeanAction, action);
	});

	HandleBus<PluginScreenplayInternal>::Route<ScreenplayActions::MessageFromUnityReceiver>(internal_, &PluginScreenplayInternal::HandleMessageFromUnity);
}

//-----------------------------------------------------------------------
void PluginScreenplay::Attach()
{
	DYN_CAST(Screenplay::PanelView, internal_->panelComponent->GetView(), view);
	internal_->compositeCore->GetAppCore()->EnterPluginPage();
	internal_->compositeCore->GetAppCore()->AttachPanel(view->GetRoot());

	internal_->compositeCore->GetAppCore()->AttachToolbar(internal_->toolbar, 2);
}

//-----------------------------------------------------------------------
void PluginScreenplay::Show()
{
	internal_->panelComponent->GetView()->Show();
}

//-----------------------------------------------------------------------
void PluginScreenplay::Hide()
{
	internal_->panelComponent->GetView()->Hide();
}

//-----------------------------------------------------------------------
void PluginScreenplay::Detach()
{
}

//-----------------------------------------------------------------------
void PluginScreenplay::Release()
{
	SAFE_DELETE(internal_->compositeCore);
	SAFE_DELETE(internal_->panelComponent);
	SAFE_DELETE(internal_);
}

//-----------------------------------------------------------------------
void PluginScreenplay::RegisterToBeanSource()
{
	LOGX_DEBUG(XC_PluginScreenplay, XL_LOW_PluginScreenplay, "RegisterToBeanSource");

	auto registerSource = [](const QString& _dir, const QString& _category) {
		QDir dir(_dir);
		dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
		QStringList files = dir.entryList(QStringList() << "*.json");

		foreach(QString file, files)
		{
			QFile f(dir.absoluteFilePath(file));
			if (!f.open(QIODevice::ReadOnly))
				continue;

			QJsonDocument jsonDoc = QJsonDocument::fromJson(f.readAll());
			f.close();

			if (!jsonDoc.isObject())
				continue;

			QJsonObject jsonRoot = jsonDoc.object();
			QString alias = jsonRoot["alias"].toString();
			if (alias.isNull() || alias.isEmpty())
				continue;

			QStringList extends;
			QJsonValue valProperties = jsonRoot["properties"];
			if (valProperties.isObject())
			{
				QJsonObject objProperties = valProperties.toObject();
				QJsonArray aryExtends = objProperties["extends"].toArray();
				foreach(QJsonValue val, aryExtends)
					extends.append(val.toString());
			}

			NEW_ACTION(BeanActions::RegisterSourceAction, action);
			action->alias = alias;
			action->category = _category;
			action->guid = QFileInfo(file).baseName();
			action->extends = extends;
			INVOKE_ACTION(BeanActions::RegisterSourceAction, action);
		}
	};

	registerSource("extend", "Extend");
	registerSource("template", "Template");
}
