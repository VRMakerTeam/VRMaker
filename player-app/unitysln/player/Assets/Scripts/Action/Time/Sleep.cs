/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using VRXX.Manager;

namespace VRXX.Action.Time
{
    public static class Sleep
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string duration = "0";
            if (!_params.TryGetValue("duration", out duration))
            {
                Log.Error("ActionMgr", "need params duration");
                return;
            }

            float timer = float.Parse(duration);

            CoroutineMgr.Start(sleep(timer, _onFinish));
        }

        private static IEnumerator sleep(float _duration, ActionDelegate _onFinish)
        {
            yield return new WaitForSeconds(_duration);
            _onFinish();
        }
    }
}

    