#if USE_UNI_LUA
using LuaAPI = UniLua.Lua;
using RealStatePtr = UniLua.ILuaState;
using LuaCSFunction = UniLua.CSharpFunctionDelegate;
#else
using LuaAPI = XLua.LuaDLL.Lua;
using RealStatePtr = System.IntPtr;
using LuaCSFunction = XLua.LuaDLL.lua_CSFunction;
#endif

using XLua;
using System.Collections.Generic;


namespace XLua.CSObjectWrap
{
    using Utils = XLua.Utils;
    public class SVMAPICanvasWrap
    {
        public static void __Register(RealStatePtr L)
        {
			ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			Utils.BeginObjectRegister(typeof(SVMAPI.Canvas), L, translator, 0, 0, 0, 0);
			
			
			
			
			
			Utils.EndObjectRegister(typeof(SVMAPI.Canvas), L, translator, null, null,
			    null, null, null);

		    Utils.BeginClassRegister(typeof(SVMAPI.Canvas), L, __CreateInstance, 3, 0, 0);
			Utils.RegisterFunc(L, Utils.CLS_IDX, "ToggleMask", _m_ToggleMask_xlua_st_);
            Utils.RegisterFunc(L, Utils.CLS_IDX, "ModifyMaskColor", _m_ModifyMaskColor_xlua_st_);
            
			
            
            Utils.RegisterObject(L, translator, Utils.CLS_IDX, "UnderlyingSystemType", typeof(SVMAPI.Canvas));
			
			
			Utils.EndClassRegister(typeof(SVMAPI.Canvas), L, translator);
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int __CreateInstance(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			try {
				if(LuaAPI.lua_gettop(L) == 1)
				{
					
					SVMAPI.Canvas __cl_gen_ret = new SVMAPI.Canvas();
					translator.Push(L, __cl_gen_ret);
                    
					return 1;
				}
				
			}
			catch(System.Exception __gen_e) {
				return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
			}
            return LuaAPI.luaL_error(L, "invalid arguments to SVMAPI.Canvas constructor!");
            
        }
        
		
        
		
        
        
        
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_ToggleMask_xlua_st_(RealStatePtr L)
        {
            
            
            
            try {
                
                {
                    bool _visible = LuaAPI.lua_toboolean(L, 1);
                    
                    SVMAPI.Canvas.ToggleMask( _visible );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_ModifyMaskColor_xlua_st_(RealStatePtr L)
        {
            
            
            
            try {
                
                {
                    int _r = LuaAPI.xlua_tointeger(L, 1);
                    int _g = LuaAPI.xlua_tointeger(L, 2);
                    int _b = LuaAPI.xlua_tointeger(L, 3);
                    int _a = LuaAPI.xlua_tointeger(L, 4);
                    
                    SVMAPI.Canvas.ModifyMaskColor( _r, _g, _b, _a );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        
        
        
        
        
		
		
		
		
    }
}
