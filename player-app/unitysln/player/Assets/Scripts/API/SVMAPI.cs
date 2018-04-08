using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using VRXX.Manager;
using XLua;
using Fangs.Lib.Logger;


namespace SVMAPI
{
    [CSharpCallLua]
    public class Parser
    {
        public static bool has(Dictionary<string, string> _param, string _key)
        {
            return _param.ContainsKey(_key);
        }

        public static double toNumber(Dictionary<string, string> _param, string _field, double _default)
        {
            if (!_param.ContainsKey(_field))
                return _default;

            string v = _param[_field];

            double value = 0;
            if (!double.TryParse(v, out value))
                return _default;

            return value;
        }

        public static int toInteger(Dictionary<string, string> _param, string _field, int _default)
        {
            if (!_param.ContainsKey(_field))
                return _default;

            string v = _param[_field];

            int value = 0;
            if (!int.TryParse(v, out value))
                return _default;

            return value;
        }

        public static bool toBoolean(Dictionary<string, string> _param, string _field, bool _default)
        {
            if (!_param.ContainsKey(_field))
                return _default;

            string v = _param[_field];

            bool value = false;
            if (!bool.TryParse(v, out value))
                return _default;

            return value;
        }

        public static string toString(Dictionary<string, string> _param, string _field, string _default)
        {
            if (!_param.ContainsKey(_field))
                return _default;

            return _param[_field];
        }
    }

    [CSharpCallLua]
    public delegate void FinishDelegate();

    [CSharpCallLua]
    public delegate void OnExecute(Dictionary<string, string> _params, FinishDelegate _onFinish);

    [CSharpCallLua]
    public delegate void OnUpdate();

    [LuaCallCSharp]
    public class Math
    {
        public static Vector3 LerpVector3(Vector3 _a, Vector3 _b, float _t)
        {
            return Vector3.Lerp(_a, _b, _t);
        }

        public static int RoundToInt(float _value)
        {
            return Mathf.RoundToInt(_value);
        }

        public static Vector3 NewVector3(float _x, float _y, float _z)
        {
            return new Vector3(_x, _y, _z);
        }

        public static Quaternion EulerQuaternion(float _x, float _y, float _z)
        {
            return Quaternion.Euler(_x, _y, _z);
        }

        public static Quaternion EulerQuaternion(Vector3 _vector3)
        {
            return Quaternion.Euler(_vector3);
        }

        public static Quaternion LerpQuaternion(Quaternion _a, Quaternion _b, float _t)
        {
           return Quaternion.Lerp(_a, _b, _t);
        }

        public static Vector3 GetRandomVector3()
        {
            return Random.insideUnitSphere;
        }
    }

    [LuaCallCSharp]
    public class Action
    {
        public static Block[] List(string _path)
        {
            List<Block> blocks = new List<Block>();
            foreach (string uuid in BlockMgr.ListWithPath(_path))
            {
                Block block = BlockMgr.FindBlock(uuid);
                if (null == block)
                    continue;

                blocks.Add(block);
            }
            return blocks.ToArray();
        }

        public static string GetName(Block _block)
        {
            if (null == _block)
                return "";

            return _block.action;
        }

        public static string GetParam(Block _block, string _param)
        {
            if (null == _block)
                return "";
            if (!_block.param.ContainsKey(_param))
                return "";
            return _block.param[_param];
        }

        public static void Execute(Block _block)
        {
            if (null == _block)
                return;

            _block.Execute();
        }
    }

    [LuaCallCSharp]
    public class Slate
    {
        public static void Switch(string _alias)
        {
            string uuid = SlateMgr.AliasToUUID(_alias);
            Log.Trace("Invoke::SwitchSlate", "Switch slate, alias:{0} uuid:{1}", _alias, uuid);
            SlateMgr.SwithSlate(uuid);
        }
    }

    [LuaCallCSharp]
    public class VR
    {
        public static Transform reticleTarget()
        {
            return XGaze.instance.target;
        }
    }

    [LuaCallCSharp]
    public class Camera
    {
        public static Transform transform()
        {
            return CameraMgr.camera;
        }

        public static void ModifyPosition(float _x, float _y, float _z)
        {
            transform().position = new Vector3(_x, _y, _z);
        }

        public static void ModifyRotation(float _x, float _y, float _z)
        {
            transform().rotation = Quaternion.Euler(new Vector3(_x, _y, _z));
        }

        public static void ModifyFOV(float _value)
        {
            CameraMgr.FOV(_value);
        }

    }

    [LuaCallCSharp]
    public class Canvas
    {
        public static void ToggleMask(bool _visible)
        {
            if (CanvasMgr.isActiveCanvas3D())
                CanvasMgr.ToggleMask3D(_visible);

            if (CanvasMgr.isActiveCanvas2D())
                CanvasMgr.ToggleMask2D(_visible);
        }

        public static void ModifyMaskColor(int _r, int _g, int _b, int _a)
        {
            if (CanvasMgr.isActiveCanvas3D())
                CanvasMgr.SetMaskColor3D(new Color(_r, _g, _b, _a / 255f));

            if (CanvasMgr.isActiveCanvas2D())
                CanvasMgr.SetMaskColor(new Color(_r, _g, _b, _a / 255f));
        }
    }

    [LuaCallCSharp]
    public class Ui
    {
        #region 3DText
        public static GameObject New3DText(string _uuid)
        {
            return UIMgr.New3DText(_uuid);
        }

        public static void Modify3DTextPosition(GameObject _3dText, float _x, float _y, float _z)
        {
            UIMgr.Modify3DText_Position(_3dText, new Vector3(_x, _y, _z));
        }

        public static void Modify3DTextRotation(GameObject _3dText, float _x, float _y, float _z)
        {
            UIMgr.Modify3DText_Rotation(_3dText, new Vector3(_x, _y, _z));
        }

        public static void Modify3DTextScale(GameObject _3dText, float _x, float _y, float _z)
        {
            UIMgr.Modify3DText_Scale(_3dText, new Vector3(_x, _y, _z));
        }

        public static void Modify3DTextColor(GameObject _3dText, int _r, int _g, int _b, int _a)
        {
            UIMgr.Modify3DText_Color(_3dText, new Color(_r / 255.0f, _g / 255.0f, _b / 255.0f, _a / 255.0f));
        }

        public static void Modify3DTextSize(GameObject _3dText, float _width, float _high)
        {
            UIMgr.Modify3DText_Size(_3dText, new Vector2(_width, _high));
        }

        public static void Modify3DTextText(GameObject _3dtext, string _text)
        {
            UIMgr.Modify3DText_Text(_3dtext, _text);
        }

        public static void Modify3DTextFontSize(GameObject _3dText, int _size)
        {
            UIMgr.Modify3DText_FontSize(_3dText, _size);
        }

        public static void Modify3DTextFontColor(GameObject _3dText, int _r, int _g, int _b, int _a)
        {
            UIMgr.Modify3DText_FontColor(_3dText, new Color(_r / 255.0f, _g / 255.0f, _b / 255.0f, _a / 255.0f));
        }

        public static void Show3DText_Vertical(GameObject _3dText, string _text, int _fontsize, float _vec_x, float _vec_y, int _Max_x = 600)
        {
            UIMgr.Show3DText_Vertical(_3dText, _text, _fontsize, new Vector2(_vec_x, _vec_y), _Max_x);
        }
        public static void Show3DText_Horizontal(GameObject _3dText, string _text, int _fontsize, float _vec_x, float _vec_y, int _Max_y = 500)
        {
            UIMgr.Show3DText_Horizontal(_3dText, _text, _fontsize, new Vector2(_vec_x, _vec_y), _Max_y);
        }

        #endregion
    }

    [LuaCallCSharp]
    public class Sound
    {
        public static void Play2D(string _package, string _file, int _track, FinishDelegate _finish)
        {
            VRXX.Action.Sound.Play2D.Play(_package, _file, _track, _finish);
        }

        public static void Play2dUGC(string _cache_asset, int _track, SVMAPI.FinishDelegate _onFinish)
        {
            VRXX.Action.Sound.Play2DUGC.Play(_cache_asset, _track, _onFinish);
        }
    }


}//namespace
