/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/
using UnityEngine;
using System.Collections.Generic;
using Fangs.Lib.Logger;

namespace VRXX.Action
{
    public delegate void ActionDelegate();

    public class ActionHub
    {
        public delegate void ExecuteDelegate(Dictionary<string, string> _params, ActionDelegate _onFinish);
        private static Dictionary<string, ExecuteDelegate> functions = new Dictionary<string, ExecuteDelegate>(0);
        private static List<string> baned = new List<string>();

        public static void Invoke(string _action, Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            if (baned.Contains(_action))
                return;

            if(!functions.ContainsKey(_action))
            {
                VRXX.Manager.SVMMgr.Agent agent = VRXX.Manager.SVMMgr.NewAgent();
                agent.Exec("actions/" + _action.Replace(":", "/"));

                SVMAPI.OnExecute svmOnExecute = agent.var<SVMAPI.OnExecute>("onExecute");
                if (null == svmOnExecute)
                {
                    Log.Error("ActionHub", "missing function of {0}", _action);
                    return;
                }
                SVMAPI.OnUpdate svmOnUpdate = agent.var<SVMAPI.OnUpdate>("onUpdate");
                agent.OnUpdate = new Manager.SVMMgr.Agent.OnUpdateDelegate(() => { if (null != svmOnUpdate) svmOnUpdate(); });
                svmOnExecute(_params, () =>
                {
                    _onFinish();
                    Manager.SVMMgr.Destroy(agent);
                });
                return;
            }

            Log.Debug("ActionHub", "invoke action:{0}", _action);
            functions[_action](_params, _onFinish);
        }

        public static void Ban(string _action)
        {
            if (baned.Contains(_action))
                return;

            Log.Debug("ActionHub", "ban {0}", _action);
            baned.Add(_action);
        }

        //public static 
        public static void RegisterActions()
        {   
            registerAction("Exit:InvokeCommand", Exit.Invoke.Execute);
            registerAction("Exit:SwitchSlate", Exit.SwitchSlate.Execute);

            //registerAction("Time:Sleep", Time.Sleep.Execute);

            #region Sound
            //registerAction("Sound:Play2d", Sound.Play2D.Execute);
            //registerAction("Sound:Play2dUGC", Sound.Play2DUGC.Execute);
            #endregion

            #region Camera
            //registerAction("Camera:SetPosition", Camera.Place.Execute);
            //registerAction("Camera:SetRotation", Camera.Euler.Execute);
            //registerAction("Camera:Move", Camera.Move.Execute);
            //registerAction("Camera:SetFOV", Camera.FOV.Execute);

            //registerAction("Camera:FadeIn", Camera.FadeIn.Execute);
            //registerAction("Camera:FadeOut", Camera.FadeOut.Execute);
            //registerAction("Camera:Shake", Camera.Shake.Execute);
            //registerAction("Camera:VR_PushByTime", Camera.VRPushByTime.Execute);

            //registerAction("Camera:skybox", Camera.Skybox.Execute);
            #endregion

            #region Agent
            registerAction("Agent:position", Agent.SetPosition.Execute);
            registerAction("Agent:rotation", Agent.SetRotation.Execute);
            registerAction("Agent:scale", Agent.SetScale.Execute);
            registerAction("Agent:translate", Agent.Translate.Execute);
            registerAction("Agent:rotate", Agent.Rotate.Execute);
            #endregion

            #region itween  
            registerAction("itween:lookfrom", Shared.LookFrom.Execute);
            registerAction("itween:lookto", Shared.LookTo.Execute);

            registerAction("itween:moveadd", Shared.MoveAdd.Execute);
            registerAction("itween:movefrom", Shared.MoveFrom.Execute);
            registerAction("itween:moveto", Shared.MoveTo.Execute);

            registerAction("itween:rotateadd", Shared.RotateAdd.Execute);
            registerAction("itween:rotatefrom", Shared.RotateFrom.Execute);
            registerAction("itween:rotateto", Shared.RotateTo.Execute);

            registerAction("itween:scaleadd", Shared.ScaleAdd.Execute);
            registerAction("itween:scalefrom", Shared.ScaleFrom.Execute);
            registerAction("itween:scaleto", Shared.ScaleTo.Execute);

            registerAction("itween:punchposition", Shared.PunchPosition.Execute);
            registerAction("itween:punchrotation", Shared.PunchRotation.Execute);
            registerAction("itween:punchscale", Shared.PunchScale.Execute);

            registerAction("itween:shakeposition", Shared.ShakePosition.Execute);
            registerAction("itween:shakerotation", Shared.ShakeRotation.Execute);
            registerAction("itween:shakescale", Shared.ShakeScale.Execute);

            #endregion
        }

        private static void registerAction(string _action, ExecuteDelegate _function)
        {
            if (functions.ContainsKey(_action))
                return;
            functions.Add(_action, _function);
        }
    }
}
