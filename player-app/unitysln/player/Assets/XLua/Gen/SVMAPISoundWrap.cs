﻿#if USE_UNI_LUA
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
    public class SVMAPISoundWrap
    {
        public static void __Register(RealStatePtr L)
        {
			ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			Utils.BeginObjectRegister(typeof(SVMAPI.Sound), L, translator, 0, 0, 0, 0);
			
			
			
			
			
			Utils.EndObjectRegister(typeof(SVMAPI.Sound), L, translator, null, null,
			    null, null, null);

		    Utils.BeginClassRegister(typeof(SVMAPI.Sound), L, __CreateInstance, 3, 0, 0);
			Utils.RegisterFunc(L, Utils.CLS_IDX, "Play2D", _m_Play2D_xlua_st_);
            Utils.RegisterFunc(L, Utils.CLS_IDX, "Play2dUGC", _m_Play2dUGC_xlua_st_);
            
			
            
            Utils.RegisterObject(L, translator, Utils.CLS_IDX, "UnderlyingSystemType", typeof(SVMAPI.Sound));
			
			
			Utils.EndClassRegister(typeof(SVMAPI.Sound), L, translator);
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int __CreateInstance(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			try {
				if(LuaAPI.lua_gettop(L) == 1)
				{
					
					SVMAPI.Sound __cl_gen_ret = new SVMAPI.Sound();
					translator.Push(L, __cl_gen_ret);
                    
					return 1;
				}
				
			}
			catch(System.Exception __gen_e) {
				return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
			}
            return LuaAPI.luaL_error(L, "invalid arguments to SVMAPI.Sound constructor!");
            
        }
        
		
        
		
        
        
        
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Play2D_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
            try {
                
                {
                    string _package = LuaAPI.lua_tostring(L, 1);
                    string _file = LuaAPI.lua_tostring(L, 2);
                    int _track = LuaAPI.xlua_tointeger(L, 3);
                    SVMAPI.FinishDelegate _finish = translator.GetDelegate<SVMAPI.FinishDelegate>(L, 4);
                    
                    SVMAPI.Sound.Play2D( _package, _file, _track, _finish );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Play2dUGC_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
            try {
                
                {
                    string _cache_asset = LuaAPI.lua_tostring(L, 1);
                    int _track = LuaAPI.xlua_tointeger(L, 2);
                    SVMAPI.FinishDelegate _onFinish = translator.GetDelegate<SVMAPI.FinishDelegate>(L, 3);
                    
                    SVMAPI.Sound.Play2dUGC( _cache_asset, _track, _onFinish );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        
        
        
        
        
		
		
		
		
    }
}
