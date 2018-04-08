/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using VRXX.Manager;

namespace VRXX.Action.Camera
{
    public static class Place
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string x = "";
            if (!_params.TryGetValue("x", out x))
            {
                Log.Error("Place", "need params x");
                return;
            }

            string y = "";
            if (!_params.TryGetValue("y", out y))
            {
                Log.Error("Place", "need params y");
                return;
            }

            string z = "";
            if (!_params.TryGetValue("z", out z))
            {
                Log.Error("Place", "need params z");
                return;
            }

            float _x = float.Parse(x);
            float _y = float.Parse(y);
            float _z = float.Parse(z);

            try
            {
                CameraMgr.Place(new Vector3(_x, _y, _z));
                _onFinish();
            }
            catch (System.Exception e)
            {
                Log.Error("setCameraPosition", "Parse json has error: " + e.Message);
            }
        }
    }
}

    