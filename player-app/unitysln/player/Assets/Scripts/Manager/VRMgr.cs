using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Core;
using Fangs.Lib.Logger;



namespace VRXX.Manager
{
    public static class VRMgr
    {
        public delegate void OnHomeBtnClickDelegate();
        public static OnHomeBtnClickDelegate onHomeBtnClick;

        private static Transform root_ { get; set; }
        private static Transform device_ { get; set; }
        private static GameObject reticle_ { get; set; }
        private static GameObject xgaze_ { get; set; }
        private static GameObject event_ { get; set; }

        private static Camera centerEyeCamera_ { get; set; }

        public static void Preload()
        {
            device_ = null;
            reticle_ = null;
            event_ = null;

            root_ = new GameObject("__VRMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);

            GameObject objXGaze = Resources.Load<GameObject>("VR/XGaze");
            xgaze_ = GameObject.Instantiate<GameObject>(objXGaze);
            CanvasMgr.Attach3DChild(xgaze_.transform);

            Log.Info("VRMgr", "Preload VRManager finish");
        }

        public static void RestoreCardboard()
        {
            GameObject.Destroy(device_.gameObject);
        }

        public static void SupportMI()
        {
#if __SDK_VR_XIAOMI__
            if (null != device_)
                return;

            //close 2d canvas
            CanvasMgr.Toggle2D(false);
            //close standalone input event   
            CanvasMgr.ToggleEvent(false);

            device_ = new GameObject("__mivr__").transform;
            device_.SetParent(root_);

            GameObject leftEye = new GameObject("LeftEye");
            leftEye.transform.SetParent(device_);
            Camera leftEyeCamera = leftEye.AddComponent<Camera>();
            leftEyeCamera.nearClipPlane = 0.1f;

            GameObject rightEye = new GameObject("RightEye");
            rightEye.transform.SetParent(device_);
            Camera rightEyeCamera = rightEye.AddComponent<Camera>();
            rightEyeCamera.nearClipPlane = 0.1f;

            addXRectiel(device_);

            device_.gameObject.AddComponent<MiCamera>();
            device_.gameObject.AddComponent<VrManager>();
            device_.gameObject.AddComponent<MIInputManager>();

            CameraMgr.InjectOuterCamera(device_);

            ///XGaze.instance.transform.SetParent(reticle.transform);
            XGaze.instance.transform.localPosition = Vector3.zero;
            XGaze.instance.transform.localRotation = Quaternion.identity;
            XGaze.instance.transform.localScale = Vector3.one;
#endif
        }

        public static void SupportCardboard()
        {
#if __SDK_VR_GOOGLE__
            if (null != device_)
                return;

            CanvasMgr.Toggle2D(false);
            CanvasMgr.ToggleEvent(false);
            CameraMgr.ToggleCamera(false);

            device_ = new GameObject("__gvr__").transform;
            device_.SetParent(root_);


            Transform tsHead = new GameObject("head").transform;
            tsHead.SetParent(device_);

            Transform tsCamera = new GameObject("camera").transform;
            tsCamera.tag = "MainCamera";
            tsCamera.SetParent(tsHead);

            CameraMgr.InjectOuterCamera(tsHead);

            /*
           // Transform tsCamera = CameraMgr.camera;
            GvrViewer viewer = device_.gameObject.AddComponent<GvrViewer>();
            viewer.VRModeEnabled = true;
            viewer.DistortionCorrection = GvrViewer.DistortionCorrectionMethod.Native;
            viewer.StereoScreenScale = 1f;
            viewer.NeckModelScale = 0f;

            reticle_ = Object.Instantiate<GameObject>(Resources.Load<GameObject>("VR/XReticle"));
            reticle_.name = "Reticle";
            reticle_.transform.SetParent(device_);

            GvrReticlePointer reticle = reticle_.AddComponent<GvrReticlePointer>();
            reticle.reticleSegments = 20;
            reticle.reticleGrowthSpeed = 8;
            reticle_.transform.SetParent(tsCamera);

            event_ = new GameObject("EventSystem");
            event_.transform.SetParent(device_.transform);
            event_.AddComponent<GvrPointerManager>();
            event_.AddComponent<GvrPointerInputModule>();
            tsCamera.gameObject.AddComponent<PhysicsRaycaster>();

            XGaze.instance.transform.SetParent(reticle.transform);
                        */
            XGaze.instance.transform.localPosition = Vector3.zero;
            XGaze.instance.transform.localRotation = Quaternion.identity;
            XGaze.instance.transform.localScale = Vector3.one;

            //SightTrigger.SupportCardboard();
#endif
        }

        public static void UpdateFrame()
        {
            if (Input.GetKeyDown(KeyCode.Escape))
            {
                if(null != onHomeBtnClick)
                    onHomeBtnClick();
            }
#if __SDK_VR_XIAOMI__
            if (MIInputManager.HmdButtonUp)
            {
                if(null != onHomeBtnClick)
                    onHomeBtnClick(); 
            }
#endif
        }

        public static void ToogleReticle(bool _visible)
        {
            reticle_.SetActive(_visible);
        }

        private static void addXRectiel(Transform _parent)
        {
            GameObject centerEye = new GameObject("CenterEye");
            centerEye.tag = "MainCamera";
            centerEye.transform.SetParent(_parent);
            
            centerEyeCamera_ = centerEye.AddComponent<Camera>();
            centerEyeCamera_.clearFlags = CameraClearFlags.Nothing;

            centerEye.AddComponent<XCenterCamera>();
            centerEye.AddComponent<PhysicsRaycaster>();

            reticle_ = Object.Instantiate<GameObject>(Resources.Load<GameObject>("VR/XReticle"));
            reticle_.name = "Reticle";
            reticle_.transform.SetParent(_parent);
            XReticle cpt = reticle_.AddComponent<XReticle>();

            event_ = new GameObject("EventSystem");
            event_.transform.SetParent(device_);
            event_.AddComponent<XGazeInputModule>();

            CoroutineMgr.Start(EndOfFrame());
        }

        private static IEnumerator EndOfFrame()
        {
            while (true)
            {
                if (centerEyeCamera_ != null)
                {
                    centerEyeCamera_.enabled = true;
                }

                yield return new WaitForEndOfFrame();
            }
        }

    }//class
}//namespave
