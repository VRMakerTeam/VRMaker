/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Steven Sun
*********************************************************************/

using UnityEngine;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using VRXX.Manager;

namespace VRXX.Action.Camera
{
    public static class FOV
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string value = "";
            if (!_params.TryGetValue("fov", out value))
            {
                Log.Error("setCameraFOV", "need params fov");
                return;
            }

            float fov = float.Parse(value);

            try
            {
                CameraMgr.FOV(fov);
                _onFinish();
                
            }
            catch (System.Exception e)
            {
                Log.Error("setCameraFOV", "Parse json has error: " + e.Message);
            }
        }
	}
}
