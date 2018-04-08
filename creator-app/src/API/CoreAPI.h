#pragma once

struct lua_State;
class AppCore;

class CoreAPI
{
public:
	static void RegisterModule(lua_State* _state);
};
