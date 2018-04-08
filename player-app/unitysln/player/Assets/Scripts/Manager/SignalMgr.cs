/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Steven Sun
*********************************************************************/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine.UI;
using Fangs.Lib.Core;
using Fangs.Lib.Logger;
using VRXX.Manager;

namespace VRXX.Manager
{
    public class SignalMgr
    {
        public delegate void HandlerDelegate(string _signal);
        public static HandlerDelegate Handler;

        private static Transform root_ { get; set; }

        public static void Preload()
        {
            root_ = new GameObject("__SignalMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);

            Log.Info("SignalMgr", "Preload SignalMgr finish");
        }

        public static void Handle(string _signal)
        {
            if (null != Handler)
                Handler(_signal);
        }
    }
}