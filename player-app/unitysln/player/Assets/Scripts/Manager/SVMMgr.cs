using UnityEngine;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using System.IO;
using XLua;

namespace VRXX.Manager
{
    //Script VM
    public static class SVMMgr
    {
        public class Agent
        {
            public delegate void OnUpdateDelegate();
            public OnUpdateDelegate OnUpdate;

            private LuaEnv env = null;
            public Agent()
            {
                env = new LuaEnv();
                env.AddLoader((ref string _filename) =>
                {
                    string path = System.IO.Path.Combine(VRXX.Platform.GetStreamingAssetsPath(), string.Format("svm/{0}.out", _filename));
                    //string path = System.IO.Path.Combine(VRXX.Platform.GetPersistentDataPath(), string.Format("svm/{0}.out", _filename));
                    WWW www = new WWW(path);
                    if (null != www.error)
                    {
                        Log.Error("SVMMgr", "load file {0} error: {1}", _filename, www.error);
                        return null;
                    }

                    while (!www.isDone)
                        continue;

                    return www.bytes;
                });

                Log.Info("SVMMgr", "New agent finish");
            }

            ~Agent()
            {
                env.Dispose();
                env = null;
            }

            public void Exec(string _file)
            {
                if (null == env)
                    return;

                env.DoString(string.Format("require('{0}')", _file));
            }

            public void Update()
            {
                if (null == env)
                    return;

                env.Tick();
                if (null != OnUpdate)
                    OnUpdate();
            }

            public T var<T>(string _var)
            {
                return env.Global.Get<T>(_var);
            }
        }

        private static Transform root_ { get; set; }
        private static List<Agent> agents = new List<Agent>();
        private static List<Agent> trash = new List<Agent>();

        public static void Preload()
        {
            if (Application.platform == RuntimePlatform.WebGLPlayer)
                return;

            root_ = new GameObject("__SVMMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);

            Log.Info("SVMMgr", "Preload SVMManager finish");
        }

        public static Agent NewAgent()
        {
            Agent agent = new Agent();
            agents.Add(agent);
            return agent;
        }

        public static void Destroy(Agent _agent)
        {
            trash.Add(_agent);
        }

        public static void UpdateFrame()
        {
            if (Application.platform == RuntimePlatform.WebGLPlayer)
                return;
            foreach (Agent agent in trash)
            {
                agents.Remove(agent);
            }
            trash.Clear();
            foreach (Agent agent in agents)
            {
                agent.Update();
            }
        }

        public static void Dispose()
        {
            if (Application.platform == RuntimePlatform.WebGLPlayer)
                return;

            agents.Clear();
        }
    } //class
}//namespace