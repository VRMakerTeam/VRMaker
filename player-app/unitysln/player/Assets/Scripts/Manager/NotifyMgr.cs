/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using Fangs.Lib.Core;

namespace VRXX.Manager
{
    public static class NotifyMgr
    {
        private static Transform root_ { get; set; }

        public static void Preload()
        {
            root_ = new GameObject("__NotifyMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("NotifyMgr", "Preload NotifyMgr finish");
        }

        public static void Alert(string _group, Error _error)
        {
            Log.Error("ErrorCenter::Push", _error);
        }

    }//class
}//namespace

