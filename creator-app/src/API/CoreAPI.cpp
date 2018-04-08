extern "C" {
#include "3rd/lua/lua.h"
#include "3rd/lua/lualib.h"
#include "3rd/lua/lauxlib.h"
}
#include <QLocale>
#include "ConfigUtil.h"
#include "SVM.h"
#include "CoreAPI.h"

int lua_translate(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 1)
	{
		SVM::LogError("translate(table) need 1 argv");
		lua_pushstring(_state, "");
		return 1;
	}
	
	if (!lua_istable(_state, 1))
	{
		SVM::LogError("argv 1 translate(table)");
		lua_pushstring(_state, "");
		return 1;
	}

	QString lang = "";
	if (!ConfigUtil::FindString("language", lang))
	{
		lang = QLocale::system().name();
	}

	QHash<QString, QString> language;

	lua_pushnil(_state);
	// 现在的栈：-1 => nil; 2 => table
	while (lua_next(_state, 1))
	{
		// 现在的栈：-1 => value; -2 => key; 2 => table
		// 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
		lua_pushvalue(_state, -2);
		// 现在的栈：-1 => key; -2 => value; -3 => key; index => table

		if (lua_isstring(_state, -1) && lua_isstring(_state, -2))
		{
			const char* key = lua_tostring(_state, -1);
			const char* value = lua_tostring(_state, -2);

			language.insert(QString(key).toLower(), QString(value));
		}
		
		// 弹出 value 和拷贝的 key，留下原始的 key 作为下一次 lua_next 的参数
		lua_pop(_state, 2);
		// 现在的栈：-1 => key; index => table
	}

	if (language.contains(lang.toLower()))
		lua_pushstring(_state, language.value(lang.toLower()).toStdString().data());
	else if (language.contains("en_uk"))
		lua_pushstring(_state, language.value("en_uk").toStdString().data());
	else
		lua_pushstring(_state, "");

	return 1;
}

static const struct luaL_Reg core_lib[] = {
	{ "translate", lua_translate },
	{ NULL, NULL },
};


int luaopen_core(lua_State* _state)
{
	luaL_newlib(_state, core_lib);
	return 1;
}

void CoreAPI::RegisterModule(lua_State* _state)
{
	luaL_requiref(_state, "core", luaopen_core, 1);
	lua_pop(_state, 1);
}

