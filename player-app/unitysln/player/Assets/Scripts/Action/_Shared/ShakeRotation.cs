﻿/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Steven Sun
*********************************************************************/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using VRXX.Manager;

namespace VRXX.Action.Shared
{
    public static class ShakeRotation
    {

        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string targetGameObject = "";
            if (!_params.TryGetValue("targetgameobject", out targetGameObject))
            {
                Log.Error("ShakeRotation", "need params targetGameObject");
                return;
            }
            string x = "";
            if (!_params.TryGetValue("x", out x))
            {
                Log.Error("ShakeRotation", "need params x");
                return;
            }

            string y = "";
            if (!_params.TryGetValue("y", out y))
            {
                Log.Error("ShakeRotation", "need params y");
                return;
            }

            string z = "";
            if (!_params.TryGetValue("z", out z))
            {
                Log.Error("ShakeRotation", "need params z");
                return;
            }
            string duration = "";
            if (!_params.TryGetValue("duration", out duration))
            {
                Log.Error("ShakeRotation", "need params time");
                return;
            }

            Vector3 position = new Vector3(float.Parse(x), float.Parse(y), float.Parse(z));
            float time = float.Parse(duration);

            try
            {
                GameObject go = null;
                if (targetGameObject.Equals("camera"))
                    go = CameraMgr.GetMainCamera();
                else
                    go = ResourceMgr.FindGameObject(targetGameObject);

                if (go != null)
                {
                    shakeRotation(go, position, time, () =>
                    {
                        _onFinish();
                        return;
                    });
                }
                else
                    Log.Error("ShakeRotation", "[{0}] GameObject is not exist...", targetGameObject);
            }
            catch (System.Exception e)
            {
                Log.Error("ShakeRotation", "Parse json has error: " + e.Message);
            }
        }


        private static void shakeRotation(GameObject _go, Vector3 _position, float _duration, ActionDelegate _onFinish)
        {
            Hashtable hash = new Hashtable();
            hash.Add("amount", _position);
            hash.Add("time", _duration);

            iTween.ShakeRotation(_go, hash);

            CoroutineMgr.Start(wait(_duration, () =>
            {
                _onFinish();
            }));
        }

        private static IEnumerator wait(float _duration, ActionDelegate _onFinish)
        {
            yield return new WaitForSeconds(_duration);
            _onFinish();
        }
    }
}
