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
using SimpleJSON;


namespace VRXX.Manager
{
    namespace Editor
    {
        public class Image : VRXX.State
        {
            public List<string> objs = new List<string>(0);
            public List<string> triggers = new List<string>(0);
            public Material skybox = null;

            private Vector3 cameraPosition = Vector3.zero;
            private Vector3 cameraRotation = Vector3.zero;
            #region Override
            public override void Initialize()
            {
                base.Initialize();
            }

            public override void Release()
            {
                base.Release();
            }

            public override void OnEnter()
            {
                base.OnEnter();
                CameraMgr.Place(cameraPosition);
                CameraMgr.Euler(cameraRotation);
                Debug.Log(skybox);
                if (null != skybox)
                    CameraMgr.ApplySkybox(skybox);
                ResourceMgr.ToggleGameObjects(objs.ToArray(), true);
                foreach (string cloneUUID in triggers)
                    TriggerMgr.ToggleTrigger(cloneUUID, true);
            }

            public override void OnUpdate()
            {
                base.OnUpdate();
            }

            public override void OnExit()
            {
                base.OnExit();
                cameraPosition = CameraMgr.position;
                cameraRotation = CameraMgr.rotation;
                ResourceMgr.ToggleAllGameObjects(false);
                CameraMgr.UseDefaultSkybox();
                foreach (string cloneUUID in triggers)
                    TriggerMgr.ToggleTrigger(cloneUUID, false);
            }
            #endregion

           
        }

        public class ImageFSM : VRXX.FSM<Image>
        {

        }
    }
    

    public static class EditorMgr
    {
        private static Transform root_ { get; set; }
        private static Editor.ImageFSM fsm = new Editor.ImageFSM();

        private static List<string> images = new List<string>(0);

        public delegate void OnImportReadyDelegate();
        public delegate void OnImportErrorDelegate(string _error);
        public delegate void OnImportFinishDelegate();

        
        public delegate void onBatchPreloadFinishDelegate();

        public static void Preload()
        {
            root_ = new GameObject("__EditorMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("CameraMgr", "Preload EditorMgr finish");
        }

        public static void ImportVCB(OnImportReadyDelegate _onReady, OnImportFinishDelegate _onFinish, OnImportErrorDelegate _onError)
        {
            VRXX.Manager.CoroutineMgr.Start(importVCB(_onReady, _onFinish, _onError));
        }

        public static void NewImage(string _uuid)
        {
            if (images.Contains(_uuid))
                return;
            fsm.NewState(new VRXX.State.Param(_uuid));
            images.Add(_uuid);
        }

        public static void DeleteImage(string _uuid)
        {
            if(images.Contains(_uuid))
                images.Remove(_uuid);

            State state = fsm.FindState(_uuid);
            if (null == state)
                return;
            fsm.DeleteState(state);
        }

        public static void SwithImage(string _uuid)
        {
            fsm.SwitchState(_uuid);
        }

        public static void AppendGameObject(string _guid)
        {
            Editor.Image img = (fsm.current as Editor.Image);
            if(null == img)
            {
                Log.Error("EditorMgr", "current state is null");
                return;
            }
            if (img.objs.Contains(_guid))
                return;
            img.objs.Add(_guid);
        }

        public static void AppendTrigger(string _guid)
        {
            Editor.Image img = (fsm.current as Editor.Image);
            if (null == img)
            {
                Log.Error("EditorMgr", "current state is null");
                return;
            }
            if (img.triggers.Contains(_guid))
                return;
            img.triggers.Add(_guid);
        }

        public static void RemoveTrigger(string _guid)
        {
            Editor.Image img = (fsm.current as Editor.Image);
            if (null == img)
            {
                Log.Error("EditorMgr", "current state is null");
                return;
            }
            if (!img.triggers.Contains(_guid))
                return;
            img.triggers.Remove(_guid);
        }

        public static void UseSkybox(Material _skybox)
        {
            Editor.Image img = (fsm.current as Editor.Image);
            img.skybox = _skybox;
            CameraMgr.ApplySkybox(_skybox);
        }

        public static void UseUGCSkybox(string _file)
        {
            UGCMgr.Res res = UGCMgr.Find(_file);
            Editor.Image img = (fsm.current as Editor.Image);
            img.skybox = res.data as Material;
            CameraMgr.ApplySkybox(res.data as Material);
        }

        public static void Reset()
        {
            CameraMgr.UseDefaultSkybox();
            CameraMgr.Place(Vector3.zero);
            CameraMgr.Euler(Vector3.zero);
            ResourceMgr.ToggleAllGameObjects(false);
            TriggerMgr.ToggleAllTrigger(false);
            fsm.Reset();
        }

        public static void Clean()
        {
            CameraMgr.UseDefaultSkybox();
            CameraMgr.Place(Vector3.zero);
            CameraMgr.Euler(Vector3.zero);
            ResourceMgr.CleanAllGameObjects();
            TriggerMgr.Clean();
            fsm.Clean();
            images.Clear();
        }

        public static void OnPositionHandleDrop(Battlehub.RTHandles.BaseHandle _handle)
        {
            JSONArray aryClones = new JSONArray();
            foreach(Transform transform in _handle.Targets)
            {
                JSONClass clone = new JSONClass();
                clone.Add("uuid", transform.name.Replace("(Clone)",""));
                clone.Add("x", transform.position.x.ToString());
                clone.Add("y", transform.position.y.ToString());
                clone.Add("z", transform.position.z.ToString());
                aryClones.Add(clone);
            }
            Debug.Log(aryClones.ToJSON(0));
            JSAPI.MessageToJS(JSAPI.MSG.JSEditorOnClonePositionUpdate, "0", aryClones.ToJSON(0));
        }

        public static void OnRotationHandleDrop(Battlehub.RTHandles.BaseHandle _handle)
        {
            JSONArray aryClones = new JSONArray();
            foreach (Transform transform in _handle.Targets)
            {
                JSONClass clone = new JSONClass();
                clone.Add("uuid", transform.name.Replace("(Clone)", ""));
                clone.Add("x", transform.rotation.eulerAngles.x.ToString());
                clone.Add("y", transform.rotation.eulerAngles.y.ToString());
                clone.Add("z", transform.rotation.eulerAngles.z.ToString());
                aryClones.Add(clone);
            }
            Debug.Log(aryClones.ToJSON(0));
            JSAPI.MessageToJS(JSAPI.MSG.JSEditorOnCloneRotationUpdate, "0", aryClones.ToJSON(0));
        }

        public static void OnScaleHandleDrop(Battlehub.RTHandles.BaseHandle _handle)
        {
            JSONArray aryClones = new JSONArray();
            foreach (Transform transform in _handle.Targets)
            {
                JSONClass clone = new JSONClass();
                clone.Add("uuid", transform.name.Replace("(Clone)", ""));
                clone.Add("x", transform.localScale.x.ToString());
                clone.Add("y", transform.localScale.y.ToString());
                clone.Add("z", transform.localScale.z.ToString());
                aryClones.Add(clone);
            }
            Debug.Log(aryClones.ToJSON(0));
            JSAPI.MessageToJS(JSAPI.MSG.JSEditorOnCloneScaleUpdate, "0", aryClones.ToJSON(0));
        }

        private static IEnumerator importVX()
        {
            
            string path = System.IO.Path.Combine(VRXX.Platform.GetPersistentDataPath(), "latest.vx");
            WWW www = new WWW(path);
            yield return www;
            if (null != www.error)
            {
                Log.Error("EditorMgr::importVX", www.error);
                yield break;
            }

            try
            {

                //Read json
                int jsonSize = VRXX.Convert.ByteToInt(www.bytes, 0);
                string json = VRXX.Convert.ByteToString(www.bytes, 4, jsonSize);

                Log.Info("EditorMgr::importVX", "parse bean success");

                JSONNode root = JSON.Parse(json);
                JSONArray slateAry = root["slates"].AsArray;

                foreach (JSONNode slateNode in slateAry)
                {  
                    string slateUUID = slateNode["guid"].Value;
                    Log.Trace("EditorMgr:importVX", "new a image {0}", slateUUID);
                    NewImage(slateUUID);
                    Editor.Image image = fsm.FindState(slateUUID) as Editor.Image;

                    JSONArray preloadAry = slateNode["preloads"].AsArray;
                    foreach (JSONNode node in preloadAry)
                    {
                        string group = node["group"].Value;
                        string package = node["package"].Value;
                        string file = node["file"].Value;
                        string guid = node["guid"].Value;
                        //string id = package + "@" + file;

                        if (group.Equals("skybox"))
                        {
                            ResourceMgr.PreloadAsset(package, file
                            , () =>
                            {
                                //startLoading();
                            }
                            , (_obj) =>
                            {
                                image.skybox = _obj as Material;
                            }
                            , (_err) =>
                            {
                                //onNotify(_err);
                            });
                        }
                    }

                    JSONArray triggerAry = slateNode["triggers"].AsArray;
                    foreach (JSONNode node in triggerAry)
                    {
                        string cloneUUID = node["uuid"].Value;
                        string alias = node["alias"].Value;
                        float px = node["px"].AsFloat;
                        float py = node["py"].AsFloat;
                        float pz = node["pz"].AsFloat;
                        float rx = node["rx"].AsFloat;
                        float ry = node["ry"].AsFloat;
                        float rz = node["rz"].AsFloat;

                    }
                }

                //offset
                int count_ugcfile = VRXX.Convert.ByteToInt(www.bytes, 4+ jsonSize);
                Log.Info("EditorMgr", "has {0} ugcfiles", count_ugcfile);

                int offset = 4 + jsonSize +4;
                for(int i = 0; i < count_ugcfile; ++i)
                {
                    int filenamesize = VRXX.Convert.ByteToInt(www.bytes, offset);
                    offset += 4;

                    string filename = VRXX.Convert.ByteToString(www.bytes, offset, filenamesize);
                    offset +=filenamesize;

                    Log.Info("EditorMgr", "ugcfile is {0}", filename);

                    int filesize = VRXX.Convert.ByteToInt(www.bytes, offset);
                    offset += 4;

                    byte[] bytes = new byte[filesize];
                    System.Array.Copy(www.bytes, offset, bytes, 0, filesize);
                    offset += filesize;
                    UGCMgr.Cache(filename, bytes);
                }
            }
            catch(System.Exception e)
            {
                Log.Exception("EditorMgr::importVX", e);
            }
            JSAPI.MessageToJS(JSAPI.MSG.JSEditorOnImportFinish, "", "");
        }

        private static IEnumerator importVCB(OnImportReadyDelegate _onReady, OnImportFinishDelegate _onFinish, OnImportErrorDelegate _onError)
        {

            string path = System.IO.Path.Combine(VRXX.Platform.GetStreamingAssetsPath(), "latest.vcb");
            WWW www = new WWW(path);
            yield return www;
            if (null != www.error)
            {
                _onError(www.error);
                Log.Error("EditorMgr::importVCB", www.error);
                yield break;
            }

            _onReady();
            yield return 1;

            try
            {
                //Read json
                Log.Info("EditorMgr::importVCB", "parse bean success");

                JSONNode root = JSON.Parse(www.text);
                JSONNode nodeSets = root["sets"];
                JSONNode nodeKV = root["kv"];
                JSONNode nodeHash = root["hash"];

                //parse slates
                JSONArray arySlates = nodeSets["screenplay_slates_guids"].AsArray;

                List<Dictionary<string, string>> assets = new List<Dictionary<string, string>>();

                //create slate
                foreach (JSONNode slateNode in arySlates)
                {
                    string slateUUID = slateNode.Value;
                    Log.Trace("EditorMgr:importVCB", "new a image {0}", slateUUID);
                    NewImage(slateUUID);

                    //parse skybox
                    string skyboxFile = nodeKV[string.Format("screenplay_slate_guid_1_preload_skybox_file:[{0}]", slateUUID)].Value;
                    string skyboxPackage = nodeKV[string.Format("screenplay_slate_guid_1_preload_skybox_package:[{0}]", slateUUID)].Value;

                    Dictionary<string, string> assetSkybox = new Dictionary<string, string>();
                    assetSkybox["package"] = skyboxPackage;
                    assetSkybox["file"] = skyboxFile;
                    assetSkybox["image"] = slateUUID;
                    assetSkybox["uuid"] = "";
                    assetSkybox["type"] = "skybox";

                    assets.Add(assetSkybox);

                    //parse agent
                    JSONArray aryAgents = nodeSets[string.Format("screenplay_slate_guid_1_preloads_guids:[{0}]", slateUUID)].AsArray;
                    foreach (JSONNode agentNode in aryAgents)
                    {
                        string agentUUID = agentNode.Value;
                        Log.Trace("EditorMgr:importVCB", "import a agent {0}", agentUUID);

                        string agentFile = nodeKV[string.Format("screenplay_slate_guid_1_preload_guid_2_asset_file:[{0}]:[{1}]", slateUUID, agentUUID)].Value;
                        string agentPackage = nodeKV[string.Format("screenplay_slate_guid_1_preload_guid_2_asset_package:[{0}]:[{1}]", slateUUID, agentUUID)].Value;

                        float posX = nodeKV[string.Format("screenplay_preload_guid_1_px:[{0}]", agentUUID)].AsFloat;
                        float posY = nodeKV[string.Format("screenplay_preload_guid_1_py:[{0}]", agentUUID)].AsFloat;
                        float posZ = nodeKV[string.Format("screenplay_preload_guid_1_pz:[{0}]", agentUUID)].AsFloat;
                        float rotX = nodeKV[string.Format("screenplay_preload_guid_1_rx:[{0}]", agentUUID)].AsFloat;
                        float rotY = nodeKV[string.Format("screenplay_preload_guid_1_ry:[{0}]", agentUUID)].AsFloat;
                        float rotZ = nodeKV[string.Format("screenplay_preload_guid_1_rz:[{0}]", agentUUID)].AsFloat;
                        float scaleX = nodeKV[string.Format("screenplay_preload_guid_1_sx:[{0}]", agentUUID)].AsFloat;
                        float scaleY = nodeKV[string.Format("screenplay_preload_guid_1_sy:[{0}]", agentUUID)].AsFloat;
                        float scaleZ = nodeKV[string.Format("screenplay_preload_guid_1_sz:[{0}]", agentUUID)].AsFloat;

                        Dictionary<string, string> assetAgent = new Dictionary<string, string>();
                        assetAgent["package"] = agentPackage;
                        assetAgent["file"] = agentFile;
                        assetAgent["image"] = slateUUID;
                        assetAgent["uuid"] = agentUUID;
                        assetAgent["type"] = "agent";
                        assetAgent["pos.x"] = posX.ToString();
                        assetAgent["pos.y"] = posY.ToString();
                        assetAgent["pos.z"] = posZ.ToString();
                        assetAgent["rot.x"] = rotX.ToString();
                        assetAgent["rot.y"] = rotY.ToString();
                        assetAgent["rot.z"] = rotZ.ToString();
                        assetAgent["scale.x"] = scaleX.ToString();
                        assetAgent["scale.y"] = scaleY.ToString();
                        assetAgent["scale.z"] = scaleZ.ToString();

                        assets.Add(assetAgent);
                    } //foreach (JSONNode agentNode in aryAgents)

                    //parse trigger
                    JSONArray aryTriggers = nodeSets[string.Format("screenplay_slate_guid_1_triggers_guids:[{0}]", slateUUID)].AsArray;
                    foreach (JSONNode triggerNode in aryTriggers)
                    {
                        string triggerUUID = triggerNode.Value;
                        Log.Trace("EditorMgr:importVCB", "import a trigger {0}", triggerUUID);

                        float posX = nodeKV[string.Format("diagram_trigger_1_px:[{0}]", triggerUUID)].AsFloat;
                        float posY = nodeKV[string.Format("diagram_trigger_1_py:[{0}]", triggerUUID)].AsFloat;
                        float posZ = nodeKV[string.Format("diagram_trigger_1_pz:[{0}]", triggerUUID)].AsFloat;
                        float rotX = nodeKV[string.Format("diagram_trigger_1_rx:[{0}]", triggerUUID)].AsFloat;
                        float rotY = nodeKV[string.Format("diagram_trigger_1_ry:[{0}]", triggerUUID)].AsFloat;
                        float rotZ = nodeKV[string.Format("diagram_trigger_1_rz:[{0}]", triggerUUID)].AsFloat;
                        string alias = nodeKV[string.Format("diagram_trigger_1_alias:[{0}]", triggerUUID)].Value;

                        string type = nodeKV[string.Format("diagram_trigger_1_type:[{0}]", triggerUUID)].Value;
                        if (type.Equals("sight"))
                        {
                            Log.Trace("EditorMgr:importVCB", "update sight property {0}", triggerUUID);
                            int icon = nodeKV[string.Format("diagram_gaze_1_icon:[{0}]", triggerUUID)].AsInt;
                            int colorR = nodeKV[string.Format("diagram_gaze_1_color_r:[{0}]", triggerUUID)].AsInt;
                            int colorG = nodeKV[string.Format("diagram_gaze_1_color_g:[{0}]", triggerUUID)].AsInt;
                            int colorB = nodeKV[string.Format("diagram_gaze_1_color_b:[{0}]", triggerUUID)].AsInt;
                            int colorA = nodeKV[string.Format("diagram_gaze_1_color_a:[{0}]", triggerUUID)].AsInt;
                            TriggerMgr.NewEditorSightTrigger(triggerUUID);
                            TriggerMgr.AdjustSightTrigger(triggerUUID, new Vector3(posX, posY, posZ), new Vector3(rotX, rotY, rotZ));
                            TriggerMgr.ModifyGazeAlias(triggerUUID, alias);
                            TriggerMgr.ModifyGazeIcon(triggerUUID, icon);
                            TriggerMgr.ModifyGazeColor(triggerUUID, colorR, colorG, colorB, colorA);

                            Editor.Image image = fsm.FindState(slateUUID) as Editor.Image;
                            image.triggers.Add(triggerUUID);
                        }
                    }
                }// foreach (JSONNode slateNode in arySlates)
                TriggerMgr.ToggleAllTrigger(false);
                Log.Debug("EditorMgr:importVCB", "Batch preload asset");

                batchPreloadAsset(assets, ()=>
                {
                    _onFinish();
                });
            }
            catch (System.Exception e)
            {
                Log.Exception("EditorMgr::importVX", e);
            }
        }


        private static void batchPreloadAsset(List<Dictionary<string, string>> _assets, onBatchPreloadFinishDelegate _onFinish)
        {
            if (_assets.Count == 0)
            {
                _onFinish();
                return;
            }

            Dictionary<string, string> asset = _assets[0];
            string packageUUID = asset["package"];
            string fileUUID = asset["file"];
            string imageUUID = asset["image"];
            string cloneUUID = asset["uuid"];
            string type = asset["type"];

           
            _assets.RemoveAt(0);

            if(type.Equals("skybox"))
            {
                Log.Debug("EditorMgr:importVCB", "preload skybox {0}", fileUUID);
                ResourceMgr.PreloadAsset(packageUUID, fileUUID
                            , () =>
                            {
                                Log.Debug("EditorMgr::importVCB", "ready load skybox {0}@{1}", packageUUID, fileUUID);
                            }
                            , (_obj) =>
                            {
                                Editor.Image image = fsm.FindState(imageUUID) as Editor.Image;
                                image.skybox = _obj as Material;
                                Log.Debug("EditorMgr::importVCB", "load skybox {0}@{1} success", packageUUID, fileUUID);
                                batchPreloadAsset(_assets, _onFinish);
                            }
                            , (_err) =>
                            {
                                //onNotify(_err);
                                Log.Error("EditorMgr::importVCB", "load skybox {0}@{1} failed: {2}", packageUUID, fileUUID, _err);
                                batchPreloadAsset(_assets, _onFinish);
                            });
            }
            else if(type.Equals("agent"))
            {
                float posX = float.Parse(asset["pos.x"]);
                float posY = float.Parse(asset["pos.y"]);
                float posZ = float.Parse(asset["pos.z"]);
                float rotX = float.Parse(asset["rot.x"]);
                float rotY = float.Parse(asset["rot.y"]);
                float rotZ = float.Parse(asset["rot.z"]);
                float scaleX = float.Parse(asset["scale.x"]);
                float scaleY = float.Parse(asset["scale.y"]);
                float scaleZ = float.Parse(asset["scale.z"]);
                Log.Debug("EditorMgr:importVCB", "preload agent {0}", fileUUID);
                ResourceMgr.PreloadAsset(packageUUID, fileUUID
                           , () =>
                           {
                               Log.Debug("EditorMgr::importVCB", "ready load agent {0}@{1}", packageUUID, fileUUID);
                           }
                           , (_obj) =>
                           {
                               Editor.Image image = fsm.FindState(imageUUID) as Editor.Image;

                               GameObject go = ResourceMgr.CloneGameObject(packageUUID, fileUUID, cloneUUID);
                               go.SetActive(false);

                               go.transform.position = new Vector3(posX, posY, posZ);
                               go.transform.rotation = Quaternion.Euler(rotX, rotY, rotZ);
                               go.transform.localScale = new Vector3(scaleX, scaleY, scaleZ);

                               Collider collider = go.GetComponent<Collider>();
                               if (null != collider)
                                   collider.enabled = true;

                               Battlehub.RTEditor.ExposeToEditor script = go.GetComponent<Battlehub.RTEditor.ExposeToEditor>();
                               if (null == script)
                               {
                                   script = go.AddComponent<Battlehub.RTEditor.ExposeToEditor>();
                                   //Debug.Log(script);
                               }
                               if (!image.objs.Contains(cloneUUID))
                                   image.objs.Add(cloneUUID);
                               Log.Debug("EditorMgr::importVCB", "load agent {0}@{1} success", packageUUID, fileUUID);
                               batchPreloadAsset(_assets, _onFinish);
                           }
                           , (_err) =>
                           {
                               Log.Error("EditorMgr::importVCB", "load agent {0}@{1} failed: {2}", packageUUID, fileUUID, _err);
                               batchPreloadAsset(_assets, _onFinish);
                           });
            }
        }

    }//class
}//namespace
    