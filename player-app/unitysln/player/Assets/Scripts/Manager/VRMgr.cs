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

            //SightTrigger.SupportCardboard();
#endif
        }

        public static void SupportIdealens()
        {
#if __SDK_VR_IDEALENS__
            Log.Info("VRMgr", "SupportIdealens");

            if (null != device_)
                return;

            CanvasMgr.Toggle2D(false);
            CanvasMgr.ToggleEvent(false);

            device_ = new GameObject("__idealensvr__").transform;
            device_.SetParent(root_);
            device_.localPosition = Vector3.zero;

            GameObject camera = new GameObject("Camera");
            camera.transform.SetParent(device_);
            camera.transform.localPosition = Vector3.zero;

            GameObject leftEye = new GameObject("LeftEye");
            leftEye.transform.SetParent(camera.transform);
            Camera leftEyeCamera = leftEye.AddComponent<Camera>();
            leftEyeCamera.nearClipPlane = 0.3f;
            leftEyeCamera.fieldOfView = 90;
            leftEyeCamera.allowHDR = false;
            leftEyeCamera.gameObject.AddComponent<FlareLayer>();

            GameObject rightEye = new GameObject("RightEye");
            rightEye.transform.SetParent(camera.transform);
            Camera rightEyeCamera = rightEye.AddComponent<Camera>();
            rightEyeCamera.nearClipPlane = 0.3f;
            rightEyeCamera.fieldOfView = 90;
            rightEyeCamera.allowHDR = false;
            rightEyeCamera.gameObject.AddComponent<FlareLayer>();

            GameObject anchor = new GameObject("Anchor");
            anchor.transform.SetParent(camera.transform);
            Camera anchorCamera = anchor.AddComponent<Camera>();
            anchor.AddComponent<IVRPhysicsRaycaster>();
            anchorCamera.nearClipPlane = 0.3f;
            anchorCamera.fieldOfView = 60;
            anchorCamera.enabled = false;

            camera.AddComponent<IVRManager>();
            camera.AddComponent<NeedIVRManager>();

            CameraMgr.InjectOuterCamera(camera.transform);

            event_ = new GameObject("EventSystem");
            event_.transform.SetParent(device_.transform);
            IVRInputModule input = event_.AddComponent<IVRInputModule>();
            input.rayTransform = anchor.transform;
            input.allowActivationOnMobileDevice = true;

            reticle_ = Object.Instantiate<GameObject>(Resources.Load<GameObject>("VR/ReticleCanvas"));
            reticle_.name = "ReticleCanvas";
            reticle_.transform.SetParent(anchor.transform);

            CanvasMgr.Canvas3D.gameObject.AddComponent<IVRRaycaster>();
            GraphicRaycaster raycaster = CanvasMgr.Canvas3D.gameObject.GetComponent<GraphicRaycaster>();
            Canvas canvas = CanvasMgr.Canvas3D.gameObject.GetComponent<Canvas>();
            canvas.worldCamera = anchorCamera;
            Object.Destroy(raycaster);

            IVRTouchPad.TouchEvent_onLongPress += ()=> {
                IVR.UnityPluginSender.IVR_ResetPose();
            };


            Log.Info("VRMgr", "new java class: com.ainck.vplayer.IdealensPlugin");
            VRXX.Core.ExtPlugin.InitAndroidPlugin("com.ainck.vplayer.IdealensPlugin");
            string sn = VRXX.Core.ExtPlugin.InvokeStaticMethod<string>("com.ainck.vplayer.IdealensPlugin", "QuerySerialNumber");
            Log.Info("VRMgr", "SN is {0}", sn);
#endif
            
            VRXX.Platform.RedirectPersistentDataPath("Android", "file:////sdcard/Android/data/com.ainck.vplayer/files");
        }

        public static void SupportViulux()
        {
#if __SDK_VR_VIULUX__
            Log.Info("VRMgr", "SupportViulux");

            if (null != device_)
                return;
                
             //close 2d canvas
            CanvasMgr.Toggle2D(false);
            //close standalone input event   
            CanvasMgr.ToggleEvent(false);

            //step 1: create object - Controller
            device_ = new GameObject("__viuluxsvr__").transform;
            device_.SetParent(root_);
            device_.localPosition = Vector3.zero;

            //step 2: create object - Left Camera
            GameObject leftEye = new GameObject("IVRCameraLeft");
            leftEye.transform.SetParent(device_.transform);
            leftEye.transform.localPosition = new Vector3(-0.084f, 0f, 0f);
            Camera leftEyeCamera = leftEye.AddComponent<Camera>();
            Object.Destroy(leftEye.GetComponent<FlareLayer>());
            leftEyeCamera.clearFlags = CameraClearFlags.Skybox;
            leftEyeCamera.backgroundColor = Color.black;
            leftEyeCamera.nearClipPlane = 0.15f;
            leftEyeCamera.farClipPlane = 1000;
            leftEyeCamera.fieldOfView = 90;
            leftEyeCamera.rect = new Rect(0f, 0f, 1f, 1f);
            leftEyeCamera.renderingPath = RenderingPath.Forward;
            leftEyeCamera.allowHDR = false;
            leftEyeCamera.stereoTargetEye = StereoTargetEyeMask.Left;

            //step 3: create object - Right Camera
            GameObject rightEye = new GameObject("IVRCameraRight");
            rightEye.transform.SetParent(device_.transform);
            rightEye.transform.localPosition = new Vector3(0.084f, 0f, 0f);
            Camera rightEyeCamera = rightEye.AddComponent<Camera>();
            Object.Destroy(rightEye.GetComponent<FlareLayer>());
            rightEyeCamera.clearFlags = CameraClearFlags.Skybox;
            rightEyeCamera.backgroundColor = Color.black;
            rightEyeCamera.nearClipPlane = 0.15f;
            rightEyeCamera.farClipPlane = 1000;
            rightEyeCamera.fieldOfView = 90;
            rightEyeCamera.rect = new Rect(0f, 0f, 1f, 1f);
            rightEyeCamera.renderingPath = RenderingPath.Forward;
            rightEyeCamera.allowHDR = false;
            rightEyeCamera.stereoTargetEye = StereoTargetEyeMask.Right;

            //step 4: add Controller Script
            IVRCameraController cc = device_.gameObject.AddComponent<IVRCameraController>();
            cc.PositionTrack = false;
            cc.RotationTrack = true;
            cc.OutputToScreen = true;
            cc.AntiBluRay = false;
            cc.FadeParam = 1;   
            cc.BlueRayParam = 0.95f;
            cc.NearClipPlane = 0.015f;
            cc.FarCliPlane = 1000f;
            cc.FieldOfView = 90;
            cc.IPDprecent = 0.084f;
            cc.BackgroundColor = Color.black;

            //step 5: add Camera Script
            IVRCamera ivrLeftCamera = leftEye.AddComponent<IVRCamera>();
            ivrLeftCamera.RightState = false;

            //step 6: add Camera Script
            IVRCamera ivrRightCamera = rightEye.AddComponent<IVRCamera>();
            ivrRightCamera.RightState = true;

            //step 7: add NV Script
            NVDirectMode nvMode = device_.gameObject.AddComponent<NVDirectMode>();
            nvMode.deviceType = DeviceType.AUTO;
            nvMode.AntiAliasing = AntiAliasing.AA2;
            
            addXRectiel(device_.transform);

            CameraMgr.InjectOuterCamera(device_);
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
