#pragma once
extern "C" {
#include "3rd/lua/lua.h"
#include "3rd/lua/lualib.h"
#include "3rd/lua/lauxlib.h"
}
#include <QVariant>

class LuaUtil
{
public:
	static QVariant toVariant(lua_State* _state, int _index)
	{
		if (lua_isboolean(_state, _index))
			return QVariant::fromValue(lua_toboolean(_state, _index));
		if (lua_isinteger(_state, _index))
			return QVariant::fromValue(lua_tointeger(_state, _index));
		if (lua_isnumber(_state, _index))
			return QVariant::fromValue(lua_tonumber(_state, _index));
		if (lua_isstring(_state, _index))
			return QVariant::fromValue(QString(lua_tostring(_state, _index)));

		return QVariant();
	}

	static void PushLua(lua_State* _state, const QVariant& _value)
	{
		if (QVariant::Type::Bool == _value.type())
			lua_toboolean(_state, _value.toBool());
		else if (QVariant::Type::Int == _value.type() || QVariant::Type::UInt == _value.type() || QVariant::Type::LongLong == _value.type() || QVariant::Type::ULongLong == _value.type())
			lua_pushinteger(_state, _value.toLongLong());
		else if (QVariant::Type::Double == _value.type())
			lua_pushnumber(_state, _value.toDouble());
		else if (QVariant::Type::String == _value.type())
			lua_pushstring(_state, _value.toString().toStdString().data());
		else
			lua_pushnil(_state);
	}
};