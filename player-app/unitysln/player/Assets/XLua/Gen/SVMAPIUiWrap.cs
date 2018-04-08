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
    public class SVMAPIUiWrap
    {
        public static void __Register(RealStatePtr L)
        {
			ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			Utils.BeginObjectRegister(typeof(SVMAPI.Ui), L, translator, 0, 0, 0, 0);
			
			
			
			
			
			Utils.EndObjectRegister(typeof(SVMAPI.Ui), L, translator, null, null,
			    null, null, null);

		    Utils.BeginClassRegister(typeof(SVMAPI.Ui), L, __CreateInstance, 12, 0, 0);
			Utils.RegisterFunc(L, Utils.CLS_IDX, "New3DText", _m_New3DText_xlua_st_);
            Utils.RegisterFunc(L, Utils.CLS_IDX, "Modify3DTextPosition", _m_Modify3DTextPosition_xlua_st_);
            Utils.RegisterFunc(L, Utils.CLS_IDX, "Modify3DTextRotation", _m_Modify3DTextRotation_xlua_st_);
            Utils.RegisterFunc(L, Utils.CLS_IDX, "Modify3DTextScale", _m_Modify3DTextScale_xlua_st_);
            Utils.RegisterFunc(L, Utils.CLS_IDX, "Modify3DTextColor", _m_Modify3DTextColor_xlua_st_);
            Utils.RegisterFunc(L, Utils.CLS_IDX, "Modify3DTextSize", _m_Modify3DTextSize_xlua_st_);
            Utils.RegisterFunc(L, Utils.CLS_IDX, "Modify3DTextText", _m_Modify3DTextText_xlua_st_);
            Utils.RegisterFunc(L, Utils.CLS_IDX, "Modify3DTextFontSize", _m_Modify3DTextFontSize_xlua_st_);
            Utils.RegisterFunc(L, Utils.CLS_IDX, "Modify3DTextFontColor", _m_Modify3DTextFontColor_xlua_st_);
            Utils.RegisterFunc(L, Utils.CLS_IDX, "Show3DText_Vertical", _m_Show3DText_Vertical_xlua_st_);
            Utils.RegisterFunc(L, Utils.CLS_IDX, "Show3DText_Horizontal", _m_Show3DText_Horizontal_xlua_st_);
            
			
            
            Utils.RegisterObject(L, translator, Utils.CLS_IDX, "UnderlyingSystemType", typeof(SVMAPI.Ui));
			
			
			Utils.EndClassRegister(typeof(SVMAPI.Ui), L, translator);
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int __CreateInstance(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
			try {
				if(LuaAPI.lua_gettop(L) == 1)
				{
					
					SVMAPI.Ui __cl_gen_ret = new SVMAPI.Ui();
					translator.Push(L, __cl_gen_ret);
                    
					return 1;
				}
				
			}
			catch(System.Exception __gen_e) {
				return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
			}
            return LuaAPI.luaL_error(L, "invalid arguments to SVMAPI.Ui constructor!");
            
        }
        
		
        
		
        
        
        
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_New3DText_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
            try {
                
                {
                    string _uuid = LuaAPI.lua_tostring(L, 1);
                    
                        UnityEngine.GameObject __cl_gen_ret = SVMAPI.Ui.New3DText( _uuid );
                        translator.Push(L, __cl_gen_ret);
                    
                    
                    
                    return 1;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Modify3DTextPosition_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
            try {
                
                {
                    UnityEngine.GameObject _3dText = (UnityEngine.GameObject)translator.GetObject(L, 1, typeof(UnityEngine.GameObject));
                    float _x = (float)LuaAPI.lua_tonumber(L, 2);
                    float _y = (float)LuaAPI.lua_tonumber(L, 3);
                    float _z = (float)LuaAPI.lua_tonumber(L, 4);
                    
                    SVMAPI.Ui.Modify3DTextPosition( _3dText, _x, _y, _z );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Modify3DTextRotation_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
            try {
                
                {
                    UnityEngine.GameObject _3dText = (UnityEngine.GameObject)translator.GetObject(L, 1, typeof(UnityEngine.GameObject));
                    float _x = (float)LuaAPI.lua_tonumber(L, 2);
                    float _y = (float)LuaAPI.lua_tonumber(L, 3);
                    float _z = (float)LuaAPI.lua_tonumber(L, 4);
                    
                    SVMAPI.Ui.Modify3DTextRotation( _3dText, _x, _y, _z );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Modify3DTextScale_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
            try {
                
                {
                    UnityEngine.GameObject _3dText = (UnityEngine.GameObject)translator.GetObject(L, 1, typeof(UnityEngine.GameObject));
                    float _x = (float)LuaAPI.lua_tonumber(L, 2);
                    float _y = (float)LuaAPI.lua_tonumber(L, 3);
                    float _z = (float)LuaAPI.lua_tonumber(L, 4);
                    
                    SVMAPI.Ui.Modify3DTextScale( _3dText, _x, _y, _z );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Modify3DTextColor_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
            try {
                
                {
                    UnityEngine.GameObject _3dText = (UnityEngine.GameObject)translator.GetObject(L, 1, typeof(UnityEngine.GameObject));
                    int _r = LuaAPI.xlua_tointeger(L, 2);
                    int _g = LuaAPI.xlua_tointeger(L, 3);
                    int _b = LuaAPI.xlua_tointeger(L, 4);
                    int _a = LuaAPI.xlua_tointeger(L, 5);
                    
                    SVMAPI.Ui.Modify3DTextColor( _3dText, _r, _g, _b, _a );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Modify3DTextSize_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
            try {
                
                {
                    UnityEngine.GameObject _3dText = (UnityEngine.GameObject)translator.GetObject(L, 1, typeof(UnityEngine.GameObject));
                    float _width = (float)LuaAPI.lua_tonumber(L, 2);
                    float _high = (float)LuaAPI.lua_tonumber(L, 3);
                    
                    SVMAPI.Ui.Modify3DTextSize( _3dText, _width, _high );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Modify3DTextText_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
            try {
                
                {
                    UnityEngine.GameObject _3dtext = (UnityEngine.GameObject)translator.GetObject(L, 1, typeof(UnityEngine.GameObject));
                    string _text = LuaAPI.lua_tostring(L, 2);
                    
                    SVMAPI.Ui.Modify3DTextText( _3dtext, _text );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Modify3DTextFontSize_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
            try {
                
                {
                    UnityEngine.GameObject _3dText = (UnityEngine.GameObject)translator.GetObject(L, 1, typeof(UnityEngine.GameObject));
                    int _size = LuaAPI.xlua_tointeger(L, 2);
                    
                    SVMAPI.Ui.Modify3DTextFontSize( _3dText, _size );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Modify3DTextFontColor_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
            try {
                
                {
                    UnityEngine.GameObject _3dText = (UnityEngine.GameObject)translator.GetObject(L, 1, typeof(UnityEngine.GameObject));
                    int _r = LuaAPI.xlua_tointeger(L, 2);
                    int _g = LuaAPI.xlua_tointeger(L, 3);
                    int _b = LuaAPI.xlua_tointeger(L, 4);
                    int _a = LuaAPI.xlua_tointeger(L, 5);
                    
                    SVMAPI.Ui.Modify3DTextFontColor( _3dText, _r, _g, _b, _a );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Show3DText_Vertical_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
			int __gen_param_count = LuaAPI.lua_gettop(L);
            
            try {
                if(__gen_param_count == 6&& translator.Assignable<UnityEngine.GameObject>(L, 1)&& (LuaAPI.lua_isnil(L, 2) || LuaAPI.lua_type(L, 2) == LuaTypes.LUA_TSTRING)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 3)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 4)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 5)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 6)) 
                {
                    UnityEngine.GameObject _3dText = (UnityEngine.GameObject)translator.GetObject(L, 1, typeof(UnityEngine.GameObject));
                    string _text = LuaAPI.lua_tostring(L, 2);
                    int _fontsize = LuaAPI.xlua_tointeger(L, 3);
                    float _vec_x = (float)LuaAPI.lua_tonumber(L, 4);
                    float _vec_y = (float)LuaAPI.lua_tonumber(L, 5);
                    int _Max_x = LuaAPI.xlua_tointeger(L, 6);
                    
                    SVMAPI.Ui.Show3DText_Vertical( _3dText, _text, _fontsize, _vec_x, _vec_y, _Max_x );
                    
                    
                    
                    return 0;
                }
                if(__gen_param_count == 5&& translator.Assignable<UnityEngine.GameObject>(L, 1)&& (LuaAPI.lua_isnil(L, 2) || LuaAPI.lua_type(L, 2) == LuaTypes.LUA_TSTRING)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 3)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 4)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 5)) 
                {
                    UnityEngine.GameObject _3dText = (UnityEngine.GameObject)translator.GetObject(L, 1, typeof(UnityEngine.GameObject));
                    string _text = LuaAPI.lua_tostring(L, 2);
                    int _fontsize = LuaAPI.xlua_tointeger(L, 3);
                    float _vec_x = (float)LuaAPI.lua_tonumber(L, 4);
                    float _vec_y = (float)LuaAPI.lua_tonumber(L, 5);
                    
                    SVMAPI.Ui.Show3DText_Vertical( _3dText, _text, _fontsize, _vec_x, _vec_y );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
            return LuaAPI.luaL_error(L, "invalid arguments to SVMAPI.Ui.Show3DText_Vertical!");
            
        }
        
        [MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
        static int _m_Show3DText_Horizontal_xlua_st_(RealStatePtr L)
        {
            
            ObjectTranslator translator = ObjectTranslatorPool.Instance.Find(L);
            
            
            
			int __gen_param_count = LuaAPI.lua_gettop(L);
            
            try {
                if(__gen_param_count == 6&& translator.Assignable<UnityEngine.GameObject>(L, 1)&& (LuaAPI.lua_isnil(L, 2) || LuaAPI.lua_type(L, 2) == LuaTypes.LUA_TSTRING)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 3)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 4)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 5)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 6)) 
                {
                    UnityEngine.GameObject _3dText = (UnityEngine.GameObject)translator.GetObject(L, 1, typeof(UnityEngine.GameObject));
                    string _text = LuaAPI.lua_tostring(L, 2);
                    int _fontsize = LuaAPI.xlua_tointeger(L, 3);
                    float _vec_x = (float)LuaAPI.lua_tonumber(L, 4);
                    float _vec_y = (float)LuaAPI.lua_tonumber(L, 5);
                    int _Max_y = LuaAPI.xlua_tointeger(L, 6);
                    
                    SVMAPI.Ui.Show3DText_Horizontal( _3dText, _text, _fontsize, _vec_x, _vec_y, _Max_y );
                    
                    
                    
                    return 0;
                }
                if(__gen_param_count == 5&& translator.Assignable<UnityEngine.GameObject>(L, 1)&& (LuaAPI.lua_isnil(L, 2) || LuaAPI.lua_type(L, 2) == LuaTypes.LUA_TSTRING)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 3)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 4)&& LuaTypes.LUA_TNUMBER == LuaAPI.lua_type(L, 5)) 
                {
                    UnityEngine.GameObject _3dText = (UnityEngine.GameObject)translator.GetObject(L, 1, typeof(UnityEngine.GameObject));
                    string _text = LuaAPI.lua_tostring(L, 2);
                    int _fontsize = LuaAPI.xlua_tointeger(L, 3);
                    float _vec_x = (float)LuaAPI.lua_tonumber(L, 4);
                    float _vec_y = (float)LuaAPI.lua_tonumber(L, 5);
                    
                    SVMAPI.Ui.Show3DText_Horizontal( _3dText, _text, _fontsize, _vec_x, _vec_y );
                    
                    
                    
                    return 0;
                }
                
            } catch(System.Exception __gen_e) {
                return LuaAPI.luaL_error(L, "c# exception:" + __gen_e);
            }
            
            return LuaAPI.luaL_error(L, "invalid arguments to SVMAPI.Ui.Show3DText_Horizontal!");
            
        }
        
        
        
        
        
        
		
		
		
		
    }
}
