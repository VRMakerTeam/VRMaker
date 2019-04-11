extern "C" {
#include "3rd/lua/lua.h"
#include "3rd/lua/lualib.h"
#include "3rd/lua/lauxlib.h"
}
#include <QLocale>
#include "ConfigUtil.h"
#include "SVM.h"
#include "CoreAPI.h"
#include "AppCore.h"

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
#if ONLY_CHINA_VER
	lang = "zh_CN";
#else
	if (!ConfigUtil::FindString("language", lang))
	{
		lang = QLocale::system().name();
	}
#endif

	QHash<QString, QString> language;

	lua_pushnil(_state);
	// ���ڵ�ջ��-1 => nil; 2 => table
	while (lua_next(_state, 1))
	{
		// ���ڵ�ջ��-1 => value; -2 => key; 2 => table
		// ����һ�� key ��ջ����Ȼ������� lua_tostring �Ͳ���ı�ԭʼ�� key ֵ��
		lua_pushvalue(_state, -2);
		// ���ڵ�ջ��-1 => key; -2 => value; -3 => key; index => table

		if (lua_isstring(_state, -1) && lua_isstring(_state, -2))
		{
			const char* key = lua_tostring(_state, -1);
			const char* value = lua_tostring(_state, -2);

			language.insert(QString(key).toLower(), QString(value));
		}
		
		// ���� value �Ϳ����� key������ԭʼ�� key ��Ϊ��һ�� lua_next �Ĳ���
		lua_pop(_state, 2);
		// ���ڵ�ջ��-1 => key; index => table
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

