﻿/********************************************************************
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
    public static class SetPosition
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string uuid = "";
            if (!_params.TryGetValue("uuid", out uuid))
            {
                Log.Error("setAgentPosition", "need params uuid");
                return;
            }

            string x = "";
            if (!_params.TryGetValue("x", out x))
            {
                Log.Error("setAgentPosition", "need params x");
                return;
            }

            string y = "";
            if (!_params.TryGetValue("y", out y))
            {
                Log.Error("setAgentPosition", "need params y");
                return;
            }

            string z = "";
            if (!_params.TryGetValue("z", out z))
            {
                Log.Error("setAgentPosition", "need params z");
                return;
            }

            float _x = float.Parse(x);
            float _y = float.Parse(y);
            float _z = float.Parse(z);

            try
            {
                AgentMgr.Place(uuid, _x, _y, _z, (_err) =>
                {
                    Log.Error("setAgentPosition", _err);
                });
                _onFinish();
            }
            catch (System.Exception e)
            {
                Log.Error("setAgentPosition", "Parse json has error: " + e.Message);
            }
        }
	}
}
