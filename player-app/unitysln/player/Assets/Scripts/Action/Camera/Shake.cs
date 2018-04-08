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
    public static class Shake
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string param_duration = "";
            if (!_params.TryGetValue("duration", out param_duration))
            {
                Log.Error("setCameraFadeIn", "need params duration");
                return;
            }

            try
            {
                float duration = float.Parse(param_duration);

                CameraMgr.Shake(duration, () =>
                {
                    _onFinish();
                });
            }
            catch (System.Exception e)
            {
                Log.Error("Shake", "error: " + e.Message);
            }
        }
	}
}
