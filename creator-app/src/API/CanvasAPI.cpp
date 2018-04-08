extern "C" {
#include "3rd/lua/lua.h"
#include "3rd/lua/lualib.h"
#include "3rd/lua/lauxlib.h"
}
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "SVM.h"
#include "LuaUtil.h"
#include "AppCore.h"
#include "Composite.h"
#include "PluginUnity/UnityActions.h"
#include "CanvasAPI.h"

#define SVM_CANVAS "svm:canvas"

namespace API
{
	namespace Canvas
	{
		QList< std::function< void(const QString& _message, const QString& _activity, const QHash<QString, QVariant>& _params)> > handlers;

		class OnWSMessageFromUnityReceiver : public IReceiver
		{
		public:
			virtual void Receive(IAction* _action)
			{
				CAST_ACTION(UnityActions::WSMessageFromUnityAction, _action, action);
				QHash<QString, QVariant> params;
				QJsonDocument jsonDoc = QJsonDocument::fromJson(action->param.toUtf8());
				QJsonObject jsonObj = jsonDoc.object();
				foreach(QString key, jsonObj.keys())
				{
					params.insert(key, jsonObj.value(key).toVariant());
				}

				for (auto itr = handlers.constBegin(); itr != handlers.constEnd(); ++itr)
				{
					(*itr)(action->method, action->activity, params);
				}
			}
		};
	}
}

int lua_canvasBindHandler(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 1)
		return 0;

	int lua_callback = luaL_ref(_state, LUA_REGISTRYINDEX);
	auto handler = [=](const QString& _message, const QString& _invoker, const QHash<QString, QVariant>& _params) {
		lua_rawgeti(_state, LUA_REGISTRYINDEX, lua_callback);
		lua_pushstring(_state, _message.toStdString().data());
		lua_pushstring(_state, _invoker.toStdString().data());
		//创建一个表，放在栈顶
		lua_newtable(_state);
		foreach(QString key, _params.keys())
		{
			//压入key
			lua_pushstring(_state, key.toStdString().data());
			//压入value
			LuaUtil::PushLua(_state, _params.value(key));
			//弹出key和value，放到表中
			lua_rawset(_state, -3);
		}
		//3个参数，0个返回值
		if (lua_pcall(_state, 3, 0, 0))
			SVM::LogError(lua_tostring(_state, -1));
	};
	API::Canvas::handlers.append(handler);
	return 0;
}


int lua_canvasInvokeMessage(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 3)
		return 0;

	QString msg = lua_tostring(_state, 1);
	QString invoker = lua_tostring(_state, 2);	

	QHash<QString, QVariant> paramList;

	QJsonObject root;
	foreach(QString key, paramList.keys())
	{
		root.insert(key, QJsonValue::fromVariant(paramList.value(key)));
	}
	QJsonDocument jsonDoc;
	jsonDoc.setObject(root);

	NEW_ACTION(UnityActions::WSMessageToUnityAction, action);
	action->method = msg;
	action->activity = invoker;
	action->param = jsonDoc.toJson();
	INVOKE_ACTION(UnityActions::WSMessageToUnityAction, action);
	return 0;
}

static const struct luaL_Reg canvas_lib[] = {
	{ "bindHandler", lua_canvasBindHandler },
	{ "invokeMessage", lua_canvasInvokeMessage},
	{ NULL, NULL },
};


int luaopen_canvas(lua_State* _state)
{
	luaL_newlib(_state, canvas_lib);
	return 1;
}

void CanvasAPI::RegisterModule(lua_State* _state)
{
	luaL_requiref(_state, "canvas", luaopen_canvas, 1);
	lua_pop(_state, 1);

	Dispatcher::Subscribe<API::Canvas::OnWSMessageFromUnityReceiver, UnityActions::WSMessageFromUnityAction>();
}

