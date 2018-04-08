/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Steven Sun
*********************************************************************/

using UnityEngine;
using UnityEngine.EventSystems;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine.UI;
using Fangs.Lib.Core;
using Fangs.Lib.Logger;
using VRXX.Manager;

namespace VRXX.Manager
{

    public class Slate
    {
        public class Asset
        {
            public string slate { get; set; }
            public string group { get; set; }
            public string package { get; set; }
            public string file { get; set; }
            public string guid { get; set; }
            public float px { get; set; }
            public float py { get; set; }
            public float pz { get; set; }
            public float rx { get; set; }
            public float ry { get; set; }
            public float rz { get; set; }
            public float sx { get; set; }
            public float sy { get; set; }
            public float sz { get; set; }

            public bool gaze { get; set; }
            public string gazeAlias { get; set; }
        }

        public string alias { get; set; }
        public List<string> agents { get; private set; }
        public Dictionary<string, Asset> assets { get; private set; }
        private List<string> blocks = new List<string>();
        private List<string> triggers = new List<string>();

        public Slate()
        {
            agents = new List<string>(0);
            assets = new Dictionary<string, Asset>(0);
        }

        public void LinkBlocks()
        {
            //Link blocks
            List<string> startupBlocks = new List<string>(0);
            for(int i = 0; i < blocks.Count; ++i)
            {
                string guid = blocks[i];
                Block block = BlockMgr.FindBlock(guid);
                if (block.action.Equals("Entry:Startup", System.StringComparison.InvariantCultureIgnoreCase))
                    startupBlocks.Add(guid);
                if (block.action.Contains("Exit"))
                    continue;
                if(i+1 < blocks.Count)
                {
                    string next = blocks[i + 1];
                    Block nextblock = BlockMgr.FindBlock(next);
                    if(!nextblock.action.Contains("Entry"))
                        block.nexts.Add(next);
                }
            }
        }

        public void AppendBlock(string _blockGUID)
        {
            if (blocks.Contains(_blockGUID))
                return;
            blocks.Add(_blockGUID);
        }

        public Asset RegisterAsset(string _guid)
        {
            if (assets.ContainsKey(_guid))
                return null;
            Log.Debug("SlateMgr", "register asset, guid:{0}", _guid);
            Asset asset = new Asset();
            asset.guid = _guid;
            assets.Add(_guid, asset);
            return asset;
        }

        public void RegisterTrigger(string _uuid)
        {
            if (triggers.Contains(_uuid))
                return;
            triggers.Add(_uuid);
            //hide the trigger, show it when enter slate
            TriggerMgr.ToggleTrigger(_uuid, false);
        }

        public void DoEnter()
        {
            //preload assets
            foreach (Asset asset in assets.Values)
            {
                // asset has format like as .wav
                if(asset.file.Contains("."))
                {
                    byte[] data = UGCMgr.Take(asset.file);
                    if(null == data)
                    {
                        this.LogError("Missing asset, group:{0} package:{1} file:{2} ", asset.group, asset.package, asset.file);
                        continue;
                    }
                    if (asset.group.Equals("skybox"))
                    {
                        Material skybox = UGCMgr.BuildSkybox(data);
                        CameraMgr.ApplySkybox(skybox);
                    }
                }
                else
                {
                    Object obj = ResourceMgr.FindFromPreload(asset.package, asset.file);
                    if (null == obj)
                    {
                        this.LogError("Missing asset, group:{0} package:{1} file:{2} ", asset.group, asset.package, asset.file);
                        continue;
                    }

                    if (asset.group.Equals("agent"))
                    {
                        GameObject clone = ResourceMgr.CloneGameObject(asset.package, asset.file, asset.guid);
                        agents.Add(asset.guid);
                        clone.transform.position = new Vector3(asset.px, asset.py, asset.pz);
                        clone.transform.rotation = Quaternion.Euler(asset.rx, asset.ry, asset.rz);
                        clone.transform.localScale = new Vector3(asset.sx, asset.sy, asset.sz);

                        if(asset.gaze)
                        {
                            ReticleHandler handler = clone.AddComponent<ReticleHandler>();
                            handler.path = asset.slate;
                            handler.trigger = asset.gazeAlias;
                            EventTrigger et = clone.AddComponent<EventTrigger>();

                            EventTrigger.Entry entryEnter = new EventTrigger.Entry();
                            entryEnter.eventID = EventTriggerType.PointerEnter;
                            entryEnter.callback.AddListener((data) => {handler.onPointerEnter(); });
                            et.triggers.Add(entryEnter);

                            EventTrigger.Entry entryExit = new EventTrigger.Entry();
                            entryExit.eventID = EventTriggerType.PointerExit;
                            entryExit.callback.AddListener((data) => { handler.onPointerExit(); });
                            et.triggers.Add(entryExit);

                            EventTrigger.Entry entryClick = new EventTrigger.Entry();
                            entryClick.eventID = EventTriggerType.PointerClick;
                            entryClick.callback.AddListener((data) => { handler.onPointerClick(); });
                            et.triggers.Add(entryClick);

                            Collider collider = clone.GetComponent<Collider>();
                            if (null != collider)
                                collider.enabled = true;
                        }
                    }
                    else if (asset.group.Equals("skybox"))
                    {
                        CameraMgr.ApplySkybox(obj as Material);
                    }
                }
            }

            //hide all triggers
            TriggerMgr.ToggleAllTrigger(false);
            //show trigger in current slate
            foreach (string trigger in triggers)
            {
                TriggerMgr.ToggleTrigger(trigger, true);
            }

            // execute startup blocks
            foreach(string guid in blocks)
            {
                Block block = BlockMgr.FindBlock(guid);
                if (block.action.Equals("Entry:Startup"))
                    block.Execute();
            }
        }

        public void DoExit()
        {
            foreach (string trigger in triggers)
            {
                TriggerMgr.ToggleTrigger(trigger, false);
            }
            ResourceMgr.CleanAllGameObjects();
        }

    }

    public class SlateState : VRXX.State
    {
        public Slate slate {get; private set;} 

        #region Override
        public override void Initialize()
        {
            base.Initialize();
            slate = new Slate();
        }

        public override void Release()
        {
            base.Release();
        }

        public override void OnEnter()
        {
            base.OnEnter();
            CameraMgr.UseDefaultSkybox();
            CameraMgr.Place(Vector3.zero);
            CameraMgr.Euler(Vector3.zero);
            slate.DoEnter();
        }

        public override void OnUpdate()
        {
            base.OnUpdate();
        }

        public override void OnExit()
        {
            base.OnExit();
            slate.DoExit();
        }
        #endregion
    }
    public class SlateFSM : VRXX.FSM<SlateState>
    {

    }

    public static class SlateMgr
    {
        private delegate void CallBack();

        static private SlateFSM fsm = new SlateFSM();

        private static Transform root_ { get; set; }

        private static List<string> slates = new List<string>(0);

        public static void Preload()
        {
            root_ = new GameObject("__SlateMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("SlateMgr", "Preload SlateMgr finish");
        }

        public static void Run()
        {
            if (slates.Count <= 0)
                return;
            List<string> assets = new List<string>(0);
            foreach(string slateUUID in slates)
            {
                SlateState state = fsm.FindState(slateUUID) as SlateState;
                foreach(Slate.Asset asset in state.slate.assets.Values)
                {
                    string assetID = asset.package + "@" + asset.file;
                    if (assets.Contains(assetID))
                        continue;
                    assets.Add(assetID);
                }
            }
            Log.Debug("SlateMgr", "ready load {0} assets", assets.Count);
            preloadAssets(assets, ()=>
            {
                Log.Debug("SlateMgr", "ready run slate");
                SlateState firstSlate = fsm.FindState(slates[0]) as SlateState;
                fsm.SwitchState(firstSlate);
            });
        }

        public static void Stop()
        {
            fsm.Clean();
            slates.Clear();
        }

        public static Slate NewSlate(string _uuid)
        {
            if (slates.Contains(_uuid))
                return null;

            State state = fsm.NewState(new VRXX.State.Param(_uuid));
            slates.Add(_uuid);
            return (state as SlateState).slate;
        }

        public static void DeleteSlate(string _uuid)
        {
            State state = fsm.FindState(_uuid);
            if (null == state)
                return;
            fsm.DeleteState(state);
        }

        public static void SwithSlate(string _uuid)
        {
            fsm.SwitchState(_uuid);
        }

        public static void SwithSlate(int _index)
        {
            if (_index >= slates.Count)
                return;

            fsm.SwitchState(slates[_index]);
        }


        public static string AliasToUUID(string _alias)
        {
            foreach(string slateUUID in slates)
            {
                State state = fsm.FindState(slateUUID);
                if (null == state)
                    continue;
                SlateState slateState = state as SlateState;
                if (slateState.slate.alias.Equals(_alias))
                    return slateUUID;
            }
            return "";
        }

        private static void preloadAssets(List<string> assets, CallBack _onFinish)
        {
            if (assets.Count <= 0)
            {
                _onFinish();
                return;
            }

            string assetID = assets[0];
            assets.RemoveAt(0);
            string package = assetID.Split('@')[0];
            string file = assetID.Split('@')[1];
            // has '.' is ugc file
            if(!file.Contains("."))
            {
                Log.Trace("SlateMgr", "preload asset package:{0} file:{1}", package, file);
                ResourceMgr.PreloadAsset(package, file
                 , () =>
                 {
                     Log.Trace("SlateMgr", "ready preload asset package:{0} file:{1}", package, file);
                 }
                 , (_obj) =>
                 {
                     Log.Trace("SlateMgr", "finish preload asset package:{0} file:{1}", package, file);
                     preloadAssets(assets, _onFinish);
                 }
                 , (_error) =>
                 {
                     Log.Error("SlateMgr", "preload asset package:{0} file:{1} has error:{2}", package, file, _error);
                     preloadAssets(assets, _onFinish);
                 });
            }
            else
            {
                Log.Trace("SlateMgr", "skip preload ugcfile :{0}", file);
                _onFinish();
            }
        }
    }
}