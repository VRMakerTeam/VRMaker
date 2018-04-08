/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Steven Sun
*********************************************************************/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Core;
using Fangs.Lib.Logger;

namespace VRXX.Manager
{
    public static class InputMgr
    {
        private static Transform root_ { get; set; }
        private static Vector3 axis_ = Vector3.zero;
        private static bool enable_ { get; set; }

        public static void Preload()
        {
            enable_ = false;
            root_ = new GameObject("__InputMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("CockpitMgr", "Preload CockpitMgr finish");
        }

        public static void UpdateFrame()
        {
            if (!enable_)
                return;

            updateCockpit();
        }

        public static void Toggle(bool _flag)
        {
            enable_ = _flag;
        }

        private static void updateCockpit()
        {
            axis_.y = -Input.GetAxis("Mouse X");
            axis_.x = Input.GetAxis("Mouse Y");
            axis_.z = Input.GetAxis("Mouse Z");
            CockpitMgr.UpdateAxis(axis_);

            float speed = Input.GetAxis("Mouse ScrollWheel");
            CockpitMgr.UpdateZoom(speed);
        }
    }
	    
}
