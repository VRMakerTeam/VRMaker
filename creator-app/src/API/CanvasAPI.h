#pragma once


struct lua_State;
class AppCore;

class CanvasAPI
{
public:
	static void RegisterModule(lua_State* _state);
};