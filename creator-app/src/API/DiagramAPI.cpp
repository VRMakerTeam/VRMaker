extern "C" {
#include "3rd/lua/lua.h"
#include "3rd/lua/lualib.h"
#include "3rd/lua/lauxlib.h"
}
#include <QDir>
#include <QDebug>
#include "LuaUtil.h"
#include "AppCore.h"
#include "SVM.h"
#include "ConfigUtil.h"
#include "Composite.h"
#include "pluginDiagram/DiagramModel.h"
#include "pluginDiagram/DiagramActions.h"
#include "DiagramAPI.h"

#define SVM_DIAGRAM "svm:diagram"

static int lua_registerActions(lua_State* _state)
{
	QDir dir = QDir::current();
	if (!dir.cd("svm"))
		return 0;

	if (!dir.cd("actions"))
		return 0;

	QString svmformat = "";
	if (!ConfigUtil::FindString("svm.format", svmformat))
		svmformat = "lua";
	QStringList filters;
	filters << QString("*.%1").arg(svmformat);

	foreach(QString directory, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
	{
		QDir actionDir = dir;
		if (!actionDir.cd(directory))
			continue;

		MemDB* memDB = DiagramModel::Runtime::GetMemDB();
		DiagramModel::Runtime::Sets::Update::Add::diagram_groups(memDB, directory);
		
		QStringList files = actionDir.entryList(filters);
		//将root放在列表最前，第一个执行
		QString rootfile = QString("root.%1").arg(svmformat);
		if (files.contains(rootfile))
		{
			files.removeAll(rootfile);
			files.insert(0, rootfile);
		}

		foreach(QString file, files)
		{
			QString filepath = QDir::current().relativeFilePath(actionDir.absoluteFilePath(file));

			if (luaL_dofile(_state, filepath.toStdString().data()))
				SVM::LogError(lua_tostring(_state, -1));
		}
	}
	Composite::Store::Update(DiagramModel::Runtime::Tags::blocks_groups);

	return 0;
}

int lua_registerBlock(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 3)
		return 0;

	NEW_ACTION(DiagramActions::RegisterBlockAction, action);
	action->extend = lua_tostring(_state, 1);
	action->group = lua_tostring(_state, 2);
	action->name = lua_tostring(_state, 3);
	INVOKE_ACTION(DiagramActions::RegisterBlockAction, action);
	return 0;
}

int lua_refreshActions(lua_State* _state)
{
	Composite::Store::Update(DiagramModel::Runtime::Tags::blocks_register);
	return 0;
}


int lua_setGroupAlias(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 2)
		return 0;

	QString group = lua_tostring(_state, 1);
	QString alias = lua_tostring(_state, 2);

	MemDB* memDB = DiagramModel::Runtime::GetMemDB();
	DiagramModel::Runtime::KV::Update::diagram_group_1_alias(memDB, group, alias);
	return 0;
}


int lua_inspectorBindCallback(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 4)
		return 0;

	QString group = lua_tostring(_state, 1);
	QString tpl = lua_tostring(_state, 2);
	QString method = lua_tostring(_state, 3);
	int lua_callback = luaL_ref(_state, LUA_REGISTRYINDEX);
	auto callback = [=](QHash<QString, QVariant> _params) {
		lua_rawgeti(_state, LUA_REGISTRYINDEX, lua_callback);
		//创建一个表，放在栈顶
		lua_newtable(_state);
		foreach(QString key, _params.keys())
		{
			//压入key
			lua_pushstring(_state, key.toStdString().data());
			//压入value
			LuaUtil::PushLua(_state, _params.value(key));
			//弹出key和value，放在表中
			lua_rawset(_state, -3);
		}
		//1个参数，0个返回值
		if (lua_pcall(_state, 1, 0, 0))
			SVM::LogError(lua_tostring(_state, -1));
	};

	NEW_ACTION(DiagramActions::PropertyBindCallbackAction, action);
	action->group = group;
	action->tpl = tpl;
	action->method = method;
	action->callback = callback;
	INVOKE_ACTION(DiagramActions::PropertyBindCallbackAction, action);
	return 1;
}



int lua_blockRegisterProperty(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 4)
		return 0;

	NEW_ACTION(DiagramActions::BlockRegisterPropertyAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->property = lua_tostring(_state, 3);
	action->defaultValue = LuaUtil::toVariant(_state, 4);
	INVOKE_ACTION(DiagramActions::BlockRegisterPropertyAction, action);
	return 0;
}

int lua_blockUiAddText(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 3)
		return 0;

	NEW_ACTION(DiagramActions::BlockUiAddTextAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->text = lua_tostring(_state, 3);
	INVOKE_ACTION(DiagramActions::BlockUiAddTextAction, action);
	return 0;
}

int lua_blockUiBindProperty(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 3)
		return 0;

	NEW_ACTION(DiagramActions::BlockUiBindPropertyAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->property = lua_tostring(_state, 3);
	INVOKE_ACTION(DiagramActions::BlockUiBindPropertyAction, action);
	return 0;
}

int lua_blockUiUseImage(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 3)
		return 0;

	NEW_ACTION(DiagramActions::BlockUiUseImageAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->image = lua_tostring(_state, 3);
	INVOKE_ACTION(DiagramActions::BlockUiUseImageAction, action);
	return 0;
}

int lua_inspectorUiBindPicker(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 5)
		return 0;

	NEW_ACTION(DiagramActions::PropertyUiBindPickerAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->property = lua_tostring(_state, 3);
	action->butText = lua_tostring(_state, 4);

	//取出数组的长度
	int size = luaL_len(_state, 5);
	for (int i = 0; i < size; ++i)
	{
		lua_rawgeti(_state, 5, i + 1);
		action->filter.append(QString(lua_tostring(_state, -1)));
		lua_pop(_state, 1);
	}
	INVOKE_ACTION(DiagramActions::PropertyUiBindPickerAction, action);
	return 0;
}

int lua_inspectorUiBindRecorder(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 3)
		return 0;

	NEW_ACTION(DiagramActions::PropertyUiBindRecorderAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->property = lua_tostring(_state, 3);
	INVOKE_ACTION(DiagramActions::PropertyUiBindRecorderAction, action);
	return 0;
}

int lua_inspectorUiBindLineEdit(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 4)
		return 0;

	NEW_ACTION(DiagramActions::PropertyUiBindLineEditAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->property = lua_tostring(_state, 3);
	action->label = lua_tostring(_state, 4);
	INVOKE_ACTION(DiagramActions::PropertyUiBindLineEditAction, action);
	return 0;
}

int lua_inspectorUiBindNumberSpin(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 6)
		return 0;

	NEW_ACTION(DiagramActions::PropertyUiBindNumberSpinAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->property = lua_tostring(_state, 3);
	action->label = lua_tostring(_state, 4);
	action->min = lua_tonumber(_state, 5);
	action->max = lua_tonumber(_state, 6);
	INVOKE_ACTION(DiagramActions::PropertyUiBindNumberSpinAction, action);
	return 0;
}

int lua_inspectorUiBindIntegerSpin(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 6)
		return 0;

	NEW_ACTION(DiagramActions::PropertyUiBindIntegerSpinAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->property = lua_tostring(_state, 3);
	action->label = lua_tostring(_state, 4);
	action->min = lua_tointeger(_state, 5);
	action->max = lua_tointeger(_state, 6);
	INVOKE_ACTION(DiagramActions::PropertyUiBindIntegerSpinAction, action);
	return 0;
}

int lua_inspectorUiBindButton(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 4)
		return 0;

	NEW_ACTION(DiagramActions::PropertyUiBindButtonAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->label = lua_tostring(_state, 3);
	int lua_callback = luaL_ref(_state, LUA_REGISTRYINDEX);
	action->onClick = [=] {
		lua_rawgeti(_state, LUA_REGISTRYINDEX, lua_callback);
		if (lua_pcall(_state, 0, 0, 0))
			SVM::LogError(lua_tostring(_state, -1));
	};
	INVOKE_ACTION(DiagramActions::PropertyUiBindButtonAction, action);

	return 0;
}

int lua_inspectorUiBindTextEdit(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 4)
		return 0;

	NEW_ACTION(DiagramActions::PropertyUiBindTextEditAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->property = lua_tostring(_state, 3);
	action->label = lua_tostring(_state, 4);
	INVOKE_ACTION(DiagramActions::PropertyUiBindTextEditAction, action);
	return 0;
}

int lua_inspectorUiBindComboBox(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 5)
		return 0;

	NEW_ACTION(DiagramActions::PropertyUiBindComboBoxAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->property = lua_tostring(_state, 3);
	action->label = lua_tostring(_state, 4);

	int size = luaL_len(_state, 5);
	for (int i = 0; i < size; ++i)
	{
		lua_rawgeti(_state, 5, i + 1);
		action->lis.append(QString(lua_tostring(_state, -1)));
		lua_pop(_state, 1);
	}
	INVOKE_ACTION(DiagramActions::PropertyUiBindComboBoxAction, action);
	return 0;
}

int lua_inspectorUiBindColorPicker(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 8)
		return 0;

	NEW_ACTION(DiagramActions::PropertyUiBindColorPickerAction, action);
	action->group = lua_tostring(_state, 1);
	action->tpl = lua_tostring(_state, 2);
	action->property = lua_tostring(_state, 3);
	action->label = lua_tostring(_state, 4);
	action->r = lua_tointeger(_state, 5);
	action->g = lua_tointeger(_state, 6);
	action->b = lua_tointeger(_state, 7);
	action->a = lua_tointeger(_state, 8);
	INVOKE_ACTION(DiagramActions::PropertyUiBindColorPickerAction, action);
	return 0;
}



int lua_inspectorUiUpdateProperty(lua_State* _state)
{
	int number = lua_gettop(_state);
	if (number != 2)
		return 0;

	if (!lua_isstring(_state, 1))
	{
		SVM::LogError("argv 1 inspectorUiUpdateProperty(string, table)");
		return 0;
	}
	if (!lua_istable(_state, 2))
	{
		SVM::LogError("argv 2 inspectorUiUpdateProperty(string, table)");
		return 0;
	}

	QString invoker = lua_tostring(_state, 1);

	QHash<QString, QVariant> properties;;

	lua_pushnil(_state);
	// 现在的栈：-1 => nil; 2 => table
	while (lua_next(_state, 2))
	{
		// 现在的栈：-1 => value; -2 => key; 2 => table
		// 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
		lua_pushvalue(_state, -2);
		// 现在的栈：-1 => key; -2 => value; -3 => key; index => table

		const char* key = lua_tostring(_state, -1);
		QVariant value = LuaUtil::toVariant(_state, -2);

		properties.insert(QString(key), value);
		// 弹出 value 和拷贝的 key，留下原始的 key 作为下一次 lua_next 的参数
		lua_pop(_state, 2);
		// 现在的栈：-1 => key; index => table
	}
	
	NEW_ACTION(DiagramActions::PropertyUiUpdatePropertyAction, action);
	action->invoker = invoker;
	action->properties = properties;
	INVOKE_ACTION(DiagramActions::PropertyUiUpdatePropertyAction, action);
	
	return 0;
}


static const struct luaL_Reg diagram_lib[] = {
	{ "registerActions", lua_registerActions },
	{ "registerBlock", lua_registerBlock },
	{ "refreshActions", lua_refreshActions },
	{ "setGroupAlias", lua_setGroupAlias },

	{ "blockRegisterProperty", lua_blockRegisterProperty },
	{ "blockUiAddText", lua_blockUiAddText },
	{ "blockUiBindProperty", lua_blockUiBindProperty },
	{ "blockUiUseImage", lua_blockUiUseImage },

	{ "inspectorBindCallback", lua_inspectorBindCallback },

	{ "inspectorUiBindPicker", lua_inspectorUiBindPicker },
	{ "inspectorUiBindRecorder", lua_inspectorUiBindRecorder },
	{ "inspectorUiBindLineEdit", lua_inspectorUiBindLineEdit },
	{ "inspectorUiBindNumberSpin", lua_inspectorUiBindNumberSpin },
	{ "inspectorUiBindIntegerSpin", lua_inspectorUiBindIntegerSpin },
	{ "inspectorUiBindButton", lua_inspectorUiBindButton },
	{ "inspectorUiUpdateProperty", lua_inspectorUiUpdateProperty },

	{ "inspectorUiBindTextEdit", lua_inspectorUiBindTextEdit },
	{ "inspectorUiBindComboBox", lua_inspectorUiBindComboBox },
	{ "inspectorUiBindColorPicker", lua_inspectorUiBindColorPicker },

	{ NULL, NULL },
};

int luaopen_diagram(lua_State* _state)
{
	luaL_newlib(_state, diagram_lib);
	return 1;
}

void DiagramAPI::RegisterModule(lua_State* _state)
{
	luaL_requiref(_state, "diagram", luaopen_diagram, 1);
	lua_pop(_state, 1);
}
