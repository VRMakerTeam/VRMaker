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
    public static class Skybox
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string package = "";
            if (!_params.TryGetValue("package", out package))
            {
                Log.Error("setSkyBox", "need params package");
                return;
            }

            string file = "";
            if (!_params.TryGetValue("file", out file))
            {
                Log.Error("setSkyBox", "need params file");
                return;
            }
            try
            {
                ResourceMgr.LoadSkybox(package, file
                    , () =>
                    {
                        Log.LogTrace("LoadSkyBox", "Start loading resources...");
                    }
                    , (_scene) =>
                    {
                        _onFinish();
                    }
                    , (_error) =>
                    {
                        Log.Error("LoadSkyBox", _error);
                    });
            }
            catch (System.Exception e)
            {
                Log.Error("setSkyBox", "Parse json has error: " + e.Message);
            }
        }
	}
}
