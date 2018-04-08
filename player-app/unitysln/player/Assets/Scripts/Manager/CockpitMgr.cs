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
    public static class CockpitMgr
    {
        private static Transform root_ { get; set; }

        private static bool pitch_ = false;
        private static bool yaw_ = false;
        private static bool roll_ = false;
        private static bool zoom_ = false;

        public static void Preload()
        {
            root_ = new GameObject("__CockpitMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("CockpitMgr", "Preload CockpitMgr finish");
        }
        public static void UseStandardFPS()
        {
            EnablePitch();
            EnableYaw();
            DisableRoll();
            DisableZoom();
        }

        public static void EnablePitch()
        {
            pitch_ = true;
        }

        public static void DisablePitch()
        {
            pitch_ = false;
        }

        public static void EnableYaw()
        {
            yaw_ = true;
        }

        public static void DisableYaw()
        {
            yaw_ = false;
        }

        public static void EnableRoll()
        {
            roll_ = true;
        }

        public static void DisableRoll()
        {
            roll_ = false;
        }

        public static void EnableZoom()
        {
            zoom_ = true;
        }

        public static void DisableZoom()
        {
            zoom_ = false;
        }

        public static void UpdateAxis(Vector3 _axis)
        {
            if (!pitch_)
                _axis.x = 0;
            if (!yaw_)
                _axis.y = 0;
            if (!roll_)
                _axis.z = 0;
            CameraMgr.Rotate(_axis);
        }

        public static void UpdateZoom(float _speed)
        {
            if (!zoom_)
                return;

            CameraMgr.Zoom(_speed);
        }
    }
}
