/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using System.Collections.Generic;

namespace VRXX.Action.Entry
{
    public static class Startup
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            _onFinish();
        }
    }
}

    