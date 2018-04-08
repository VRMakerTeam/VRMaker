#include <assert.h>
#include <QDebug>
#include <QVariantHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "HandleBus.hxx"
#include "Dispatcher.h"
#include "PluginUnity/PluginUnity.h"
#include "PluginUnity/UnityMessages.h"
#include "PluginUnity/UnityActions.h"

namespace __UnityActions
{
	void Invoke(const QString& _activity, const QString& _method, const QVariantHash& _param)
	{
		QJsonDocument doc;
		doc.setObject(QJsonObject::fromVariantHash(_param));

		NEW_ACTION(UnityActions::WSMessageToUnityAction, msgAction);
		msgAction->activity = "0";
		msgAction->method = _method;
		msgAction->param = doc.toJson();
		INVOKE_ACTION(UnityActions::WSMessageToUnityAction, msgAction);
	}

}

//-----------------------------------------------------------------------
void UnityActions::WSMessageToUnityReceiver::Receive(IAction* _action)
{
	HandleBus<PluginUnityInternal>::Handle<UnityActions::WSMessageToUnityReceiver>(_action);
}


void UnityActions::API::CleanReceiver::Receive(IAction* _action)
{
	CAST_ACTION(UnityActions::API::CleanAction, _action, action);

	QVariantHash param;
	__UnityActions::Invoke("0", JSEditorClean, param);
}


void UnityActions::API::ModifyTriggerReceiver::Receive(IAction* _action)
{
	CAST_ACTION(UnityActions::API::ModifyTriggerAction, _action, action);

	QVariantHash param;
	param["uuid"] = action->uuid;
	param["type"] = action->type;
	param["alias"] = action->alias;
	__UnityActions::Invoke("0", JSEditorModifyTrigger, param);
}

void UnityActions::API::NewTriggerReceiver::Receive(IAction* _action)
{
	CAST_ACTION(UnityActions::API::NewTriggerAction, _action, action);

	QVariantHash param;
	param["uuid"] = action->uuid;
	param["type"] = action->type;
	__UnityActions::Invoke("0", JSEditorNewTrigger, param);
}

void UnityActions::API::DeleteTriggerReceiver::Receive(IAction* _action)
{
	CAST_ACTION(UnityActions::API::DeleteTriggerAction, _action, action);

	QVariantHash param;
	param["uuid"] = action->uuid;
	__UnityActions::Invoke("0", JSEditorDeleteTrigger, param);
}

void UnityActions::API::DispatchAll()
{
	Dispatcher::Subscribe<UnityActions::API::CleanReceiver, UnityActions::API::CleanAction>();
	Dispatcher::Subscribe<UnityActions::API::ImportReceiver, UnityActions::API::ImportAction>();

	Dispatcher::Subscribe<UnityActions::API::NewTriggerReceiver, UnityActions::API::NewTriggerAction>();
	Dispatcher::Subscribe<UnityActions::API::DeleteTriggerReceiver, UnityActions::API::DeleteTriggerAction>();
	Dispatcher::Subscribe<UnityActions::API::ModifyTriggerReceiver, UnityActions::API::ModifyTriggerAction>();
	Dispatcher::Subscribe<UnityActions::API::ModifyGazeIconReceiver, UnityActions::API::ModifyGazeIconAction>();
	Dispatcher::Subscribe<UnityActions::API::ModifyGazeColorReceiver, UnityActions::API::ModifyGazeColorAction>();
}

void UnityActions::RunPlayerReceiver::Receive(IAction* _action)
{
	HandleBus<PluginUnityInternal>::Handle<UnityActions::RunPlayerReceiver>(_action);
}

void UnityActions::API::ImportReceiver::Receive(IAction* _action)
{
	CAST_ACTION(UnityActions::API::ImportAction, _action, action);

	QVariantHash param;
	param["source"] = action->source;
	__UnityActions::Invoke("0", JSEditorImport, param);
}

void UnityActions::API::ModifyGazeColorReceiver::Receive(IAction* _action)
{
	CAST_ACTION(UnityActions::API::ModifyGazeColorAction, _action, action);

	QVariantHash param;
	param["uuid"] = action->uuid;
	param["r"] = action->r;
	param["g"] = action->g;
	param["b"] = action->b;
	param["a"] = action->a;
	__UnityActions::Invoke("0", JSEditorModifyGazeColor, param);
}

void UnityActions::API::ModifyGazeIconReceiver::Receive(IAction* _action)
{
	CAST_ACTION(UnityActions::API::ModifyGazeIconAction, _action, action);

	QVariantHash param;
	param["uuid"] = action->uuid;
	param["icon"] = action->icon;
	__UnityActions::Invoke("0", JSEditorModifyGazeIcon, param);
}
