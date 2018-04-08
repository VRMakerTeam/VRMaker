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
    public static class FadeIn
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string duration = "";
            if (!_params.TryGetValue("duration", out duration))
            {
                Log.Error("setCameraFadeIn", "need params duration");
                return;
            }

            string r = "";
            if (!_params.TryGetValue("color:r", out r))
            {
                Log.Error("setCameraFadeIn", "need params color:r");
                return;
            }

            string g = "";
            if (!_params.TryGetValue("color:g", out g))
            {
                Log.Error("setCameraFadeIn", "need params color:g");
                return;
            }

            string b = "";
            if (!_params.TryGetValue("color:b", out b))
            {
                Log.Error("setCameraFadeIn", "need params color:b");
                return;
            }

            Color _color = new Color(float.Parse(r), float.Parse(g), float.Parse(b), 0);
            float _duration = float.Parse(duration);
            int _mode = 1;
            Shared.FadeMask.Execute(_duration, _mode, _color, _onFinish);
        }
	}
}
