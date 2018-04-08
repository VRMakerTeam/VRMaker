using Fangs.Lib.Logger;
using UnityEngine;

namespace VRXX
{
    namespace Core
    {
        public class ExtPlugin
        {
            private static AndroidJavaObject objActivity_ = null;

            public static void Initialize()
            {
                if (Application.platform != RuntimePlatform.Android)
                    return;

                AndroidJavaClass jc = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
                objActivity_ = jc.GetStatic<AndroidJavaObject>("currentActivity");
            }

            public static void InitAndroidPlugin(string _class)
            {
                if (Application.platform != RuntimePlatform.Android)
                    return;

                using (AndroidJavaClass jc = new AndroidJavaClass(_class))
                {
                    Log.Info("Core", "inject activity");
                    jc.CallStatic("InjectActivity", objActivity_);
                    jc.CallStatic("Initialize");
                }
            }

            public static ReturnType InvokeStaticMethod<ReturnType>(string _class, string _method, params object[] _args)
            {
                if (Application.platform != RuntimePlatform.Android)
                    return default(ReturnType);

                using (AndroidJavaClass jc = new AndroidJavaClass(_class))
                {
                    Log.Info("Core", string.Format("InvokeStaticMethod {0}:{1}", _class, _method));
                    return jc.CallStatic<ReturnType>(_method, _args);
                }
                return default(ReturnType);
            }
        }
    } //namespace Core
}// namespace ExtPlugin

