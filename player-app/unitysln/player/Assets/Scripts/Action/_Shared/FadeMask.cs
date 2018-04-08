/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Steven Sun
*********************************************************************/

using UnityEngine;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using VRXX.Manager;

namespace VRXX.Action.Shared
{
    public static class FadeMask
    {
        public static void Execute(float _duration, int _mode, Color _color, ActionDelegate _onFinish)
        {
            try
            {
                CanvasMgr.SetMaskColor(_color);
                CanvasMgr.FadeMask(_duration, _mode, () =>
                {
                    _onFinish();
                });
            }
            catch (System.Exception e)
            {
                Log.Error("FadeMask", "Parse json has error: " + e.Message);
            }
        }
    }
}