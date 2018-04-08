/********************************************************************
     Copyright (c) zyncho.
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using System.Collections;
using Fangs.Lib.Core;
using Fangs.Lib.Logger;

using VRXX.Manager;

namespace VRXX
{
	public class Engine
	{

		public Error Preload ()
		{
			this.LogInfo ("Preload VRXX Engine ...");

            AnyRes.Unpack.DebugMode = true;
            AnyRes.Unpack.Initialize();
            if (Application.platform == RuntimePlatform.WebGLPlayer)
                AnyRes.Unpack.StoragePath = Application.streamingAssetsPath;
            else
                AnyRes.Unpack.StoragePath = Application.persistentDataPath;

            this.LogInfo(AnyRes.Unpack.StoragePath);

            VRXX.Core.ExtPlugin.Initialize();
            
            preloadNotifyManager();
			preloadCoroutineManager ();
			preloadResourceManager ();
			preloadCanvasManager ();
			preloadMixerManager ();
            preloadAgentManager ();
			preloadCameraManager ();
			//preloadAnimatorManager ();
            preloadBeanManager();
            preloadCockpitMgnager();
            preloadInputManager();
            preloadVRManager();
            preloadHttpManager();
            preloadSlateManager();
            preloadBlockManager();
            preloadLightManager();
            preloadTriggerManager();
            preloadUGCManager();
            preloadSignalManager();

            Action.ActionHub.RegisterActions();

            preloadSVMManager();
            preloadUIManager(); 

            return Error.OK;
		}

		public void Update ()
		{
            //InputMgr.UpdateFrame();
            SVMMgr.UpdateFrame();
            VRMgr.UpdateFrame();
            TriggerMgr.UpdateFrame();
		}

        public void Dispose()
        {
            SVMMgr.Dispose();
        }

        private void preloadSVMManager()
        {
            this.LogTrace("preload svm manager ...");
            SVMMgr.Preload();
        }

        private void preloadNotifyManager()
        {
            this.LogTrace("preload notify manager ...");
            NotifyMgr.Preload();
        }

        private void preloadCoroutineManager()
        {
            this.LogTrace("preload coroutine manager ...");
            CoroutineMgr.Preload();
        }

        private void preloadResourceManager()
        {
            this.LogTrace("preload resource manager ...");
            ResourceMgr.Preload();
        }

        private void preloadCanvasManager ()
		{
			this.LogTrace ("preload canvas manager ...");
            CanvasMgr.Preload ();
		}

		private void preloadMixerManager ()
		{
			this.LogTrace ("preload mixer manager...");
            MixerMgr.Preload();
		}

        private void preloadAgentManager()
        {
            this.LogTrace("preload agent manager...");
            AgentMgr.Preload();
        }

        private void preloadCameraManager()
        {
            this.LogTrace("preload Camera manager...");
            CameraMgr.Preload();
        }

        private void preloadBeanManager()
        {
            this.LogTrace("preload Bean manager...");
            BeanMgr.Preload();
        }

        private void preloadCockpitMgnager()
        {
            this.LogTrace("preload Cockpit manager...");
            CockpitMgr.Preload();
        }

        private void preloadInputManager()
        {
            this.LogTrace("preload Input manager...");
            InputMgr.Preload();
        }

        private void preloadVRManager()
        {
            this.LogTrace("preload VR manager...");
            VRMgr.Preload();
        }

        private void preloadHttpManager()
        {
            this.LogTrace("preload Http manager...");
            HttpMgr.Preload();
        }

        private void preloadSlateManager()
        {
            this.LogTrace("preload Slate manager...");
            SlateMgr.Preload();
        }

        private void preloadBlockManager()
        {
            this.LogTrace("preload Block manager...");
            BlockMgr.Preload();
        }

        private void preloadLightManager()
        {
            this.LogTrace("preload Light manager...");
            LightMgr.Preload();
        }

        private void preloadTriggerManager()
        {
            this.LogTrace("preload Trigger manager...");
            TriggerMgr.Preload();
        }

        private void preloadUGCManager()
        {
            this.LogTrace("preload UGC manager...");
            UGCMgr.Preload();
        }

        private void preloadSignalManager()
        {
            this.LogTrace("preload signal manager...");
            SignalMgr.Preload();
        }

        private void preloadUIManager()
        {
            this.LogTrace("preload UI manager...");
            UIMgr.Preload();
        }
    }//class
}//namespace

