#pragma once

struct lua_State;
class AppCore;

class DiagramAPI
{
public:
	static void RegisterModule(lua_State* _state);
};