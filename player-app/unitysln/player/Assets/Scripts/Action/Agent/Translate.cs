/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Steven Sun
*********************************************************************/

using UnityEngine;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using VRXX.Manager;

namespace VRXX.Action.Agent
{
    public static class Translate
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string uuid = "";
            if (!_params.TryGetValue("uuid", out uuid))
            {
                Log.Error("TranslateAgent", "need params uuid");
                return;
            }

            string x = "";
            if (!_params.TryGetValue("x", out x))
            {
                Log.Error("TranslateAgent", "need params x");
                return;
            }

            string y = "";
            if (!_params.TryGetValue("y", out y))
            {
                Log.Error("TranslateAgent", "need params y");
                return;
            }

            string z = "";
            if (!_params.TryGetValue("z", out z))
            {
                Log.Error("TranslateAgent", "need params z");
                return;
            }

            float _x = float.Parse(x);
            float _y = float.Parse(y);
            float _z = float.Parse(z);

            try
            {
                AgentMgr.Translate(uuid, _x, _y, _z, (_err) =>
                {
                    Log.Error("TranslateAgent", _err);
                });
                _onFinish();
            }
            catch (System.Exception e)
            {
                Log.Error("TranslateAgent", "Parse json has error: " + e.Message);
            }
        }
    }
}