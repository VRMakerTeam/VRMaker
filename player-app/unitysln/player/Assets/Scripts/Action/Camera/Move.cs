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
    public static class Move
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string posx = "";
            if (!_params.TryGetValue("posx", out posx))
            {
                Log.Error("Move", "need params posx");
                return;
            }

            string posy = "";
            if (!_params.TryGetValue("posy", out posy))
            {
                Log.Error("Move", "need params posy");
                return;
            }

            string posz = "";
            if (!_params.TryGetValue("posz", out posz))
            {
                Log.Error("Move", "need params posz");
                return;
            }

            float px = float.Parse(posx);
            float py = float.Parse(posy);
            float pz = float.Parse(posz);

            string rotx = "";
            if (!_params.TryGetValue("rotx", out rotx))
            {
                Log.Error("Move", "need params rotx");
                return;
            }

            string roty = "";
            if (!_params.TryGetValue("roty", out roty))
            {
                Log.Error("Move", "need params roty");
                return;
            }

            string rotz = "";
            if (!_params.TryGetValue("rotz", out rotz))
            {
                Log.Error("Move", "need params rotz");
                return;
            }

            string durationStr = "";
            if (!_params.TryGetValue("duration", out durationStr))
            {
                Log.Error("Move", "need params duration");
                return;
            }

            float rx = float.Parse(rotx);
            float ry = float.Parse(roty);
            float rz = float.Parse(rotz);
            float duration = float.Parse(durationStr);

            try
            {
                CameraMgr.Move(new Vector3(px, py, pz), new Vector3(rx, ry, rz), duration, () =>
                {
                    _onFinish();
                });
            }
            catch (System.Exception e)
            {
                Log.Error("moveCamera", "Parse json has error: " + e.Message);
            }
        }
    }







    public static class VRPushByTime
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string durationStr = "";
            if (!_params.TryGetValue("duration", out durationStr))
            {
                Log.Error("Move", "need params duration");
                return;
            }
            float duration = float.Parse(durationStr);

            try
            {
                CoroutineMgr.Start(move(duration, _onFinish));
            }
            catch (System.Exception e)
            {
                Log.Error("VRPushByTime", "Parse json has error: " + e.Message);
            }
        }

        private static System.Collections.IEnumerator move(float _duration, ActionDelegate _onFinish)
        {
            Vector3 posCamera = CameraMgr.camera.position;
            Vector3 posGaze = XGaze.instance.target.position;
            float time = 0;

            while ((time += UnityEngine.Time.deltaTime) < _duration)
            {
                CameraMgr.camera.position = Vector3.Lerp(posCamera, posGaze, time / _duration);
                yield return new WaitForEndOfFrame();
            }

            _onFinish();
        }
    }

    
}

    