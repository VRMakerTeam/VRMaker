/********************************************************************
     Copyright (c) zyncho.
     All rights reserved.

     author: Easlee Liu
*********************************************************************/
using UnityEngine;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using SimpleJSON;
using VRXX.Manager;
using Fangs.Lib.Logger;

namespace VRXX
{
    class JSAPI
    {
        public static class MSG
        {
            // From External
            public const string JSEditorReset = "JSEditorReset";
            public const string JSEditorClean = "JSEditorClean";
            public const string JSEditorImport = "JSEditorImport";

            public const string JSEditorNewImage = "JSEditorNewImage";
            public const string JSEditorDeleteImage = "JSEditorDeleteImage";
            public const string JSEditorSwitchImage = "JSEditorSwitchImage";

            public const string JSEditorNewTrigger = "JSEditorNewTrigger";
            public const string JSEditorDeleteTrigger = "JSEditorDeleteTrigger";
            public const string JSEditorModifyTrigger = "JSEditorModifyTrigger";
            public const string JSEditorFocusTrigger = "JSEditorFocusTrigger";
            public const string JSEditorAdjustTrigger = "JSEditorAdjustTrigger";

            public const string JSEditorModifyGazeIcon = "JSEditorModifyGazeIcon";
            public const string JSEditorModifyGazeColor = "JSEditorModifyGazeColor";


            public const string JSEditorUseSkybox = "JSEditorUseSkybox";
            public const string JSEditorNewClone = "JSEditorNewClone";
            public const string JSEditorDeleteClone = "JSEditorDeleteClone";
            public const string JSEditorSelect = "JSEditorSelect";
            public const string JSEditorFocus = "JSEditorFocus";

            public const string JSUnloadGameObject = "JSUnloadGameObject";
            public const string JSLoadScene = "JSLoadScene";
            public const string JSLoadSkybox = "JSLoadSkybox";
            public const string JSCleanSkybox = "JSCleanSkybox";

            public const string JSPlaceAgent = "JSPlaceAgent";
            public const string JSTranslateAgent = "JSTranslateAgent";
            public const string JSRotateAgent = "JSRotateAgent";
            public const string JSScaleAgent = "JSScaleAgent";

            public const string JSChangeViewport = "JSChangeViewport";
            public const string JSQueryCameraPosition = "JSQueryCameraPosition";
            public const string JSQueryCameraRotation = "JSQueryCameraRotation";
            public const string JSAlignGameObject = "JSAlignGameObject";

            public const string JSEditorExposeDirectionalLight = "JSEditorExposeDirectionalLight";
            public const string JSEditorDirectionalLightSelect = "JSEditorDirectionalLightSelect";
            public const string JSUnloadDirectionalLight = "JSUnloadDirectionalLight";
            public const string JSVRToggleReticle = "JSVRToggleReticle";

            // To External
            public const string __ready = "__ready";
            public const string JSEditorOnImportFinish = "JSEditorOnImportFinish";
            public const string JSEditorOnClonePositionUpdate = "JSEditorOnClonePositionUpdate";
            public const string JSEditorOnCloneRotationUpdate = "JSEditorOnCloneRotationUpdate";
            public const string JSEditorOnCloneScaleUpdate = "JSEditorOnCloneScaleUpdate";

        }



        public delegate void OnNotifyDelegate(string _message);
        public static OnNotifyDelegate onNotify;
        public delegate void OnCallbackDelegate();
        public static OnCallbackDelegate onLoadStart;
        public static OnCallbackDelegate onLoadFinish;

        public delegate void HandleDelegate(string _activity, string _param);
        private static Dictionary<string, HandleDelegate> handles = new Dictionary<string, HandleDelegate>(0);
        private static bool isLoading = false;
        private static Queue<string> messagePool = new Queue<string>();
        
        public static void Initialize()
        {
            RegisterHandle(MSG.JSEditorReset, JSEditorReset);
            RegisterHandle(MSG.JSEditorClean, JSEditorClean);
            RegisterHandle(MSG.JSEditorImport, JSEditorImport);

            RegisterHandle(MSG.JSEditorNewImage, JSEditorNewImage);
            RegisterHandle(MSG.JSEditorDeleteImage, JSEditorDeleteImage);
            RegisterHandle(MSG.JSEditorSwitchImage, JSEditorSwitchImage);

            RegisterHandle(MSG.JSEditorNewTrigger, JSEditorNewTrigger);
            RegisterHandle(MSG.JSEditorDeleteTrigger, JSEditorDeleteTrigger);
            RegisterHandle(MSG.JSEditorModifyTrigger, JSEditorModifyTrigger);
            RegisterHandle(MSG.JSEditorFocusTrigger, JSEditorFocusTrigger);
            RegisterHandle(MSG.JSEditorAdjustTrigger, JSEditorAdjustTrigger);
            RegisterHandle(MSG.JSEditorModifyGazeIcon, JSEditorModifyGazeIcon);
            RegisterHandle(MSG.JSEditorModifyGazeColor, JSEditorModifyGazeColor);

            RegisterHandle(MSG.JSEditorUseSkybox, JSEditorUseSkybox);
            RegisterHandle(MSG.JSEditorNewClone, JSEditorNewClone);
            RegisterHandle(MSG.JSEditorDeleteClone, JSEditorDeleteClone);
            RegisterHandle(MSG.JSEditorSelect, JSEditorSelect);
            RegisterHandle(MSG.JSEditorFocus, JSEditorFocus);

            RegisterHandle(MSG.JSUnloadGameObject, JSUnloadGameObject);
            RegisterHandle(MSG.JSLoadScene, JSLoadScene);
            RegisterHandle(MSG.JSLoadSkybox, JSLoadSkybox);
            RegisterHandle(MSG.JSCleanSkybox, JSCleanSkybox);

            RegisterHandle(MSG.JSPlaceAgent, JSPlaceAgent);
            RegisterHandle(MSG.JSTranslateAgent, JSTranslateAgent);
            RegisterHandle(MSG.JSRotateAgent, JSRotateAgent);
            RegisterHandle(MSG.JSScaleAgent, JSScaleAgent);

            RegisterHandle(MSG.JSChangeViewport, JSChangeViewport);
            RegisterHandle(MSG.JSQueryCameraPosition, JSQueryCameraPosition);
            RegisterHandle(MSG.JSQueryCameraRotation, JSQueryCameraRotation);
            RegisterHandle(MSG.JSAlignGameObject, JSAlignGameObject);

            RegisterHandle(MSG.JSEditorExposeDirectionalLight, JSEditorExposeDirectionalLight);
            RegisterHandle(MSG.JSEditorDirectionalLightSelect, JSEditorDirectionalLightSelect);
            RegisterHandle(MSG.JSUnloadDirectionalLight, JSUnloadDirectionalLight);

            RegisterHandle(MSG.JSVRToggleReticle, JSVRToggleReticle);

            Application.ExternalCall(MSG.__ready);
        }

        public static AppError HandleMessage(string _message)
        {
            Log.LogDebug("JSAPI", "cache a message {0}", _message);
            //messagePool.Enqueue(_message);

            string message = _message;
            // get method
            int pos = message.IndexOf('|');
            if (pos < 0)
                return new AppError(1, "_message is invaliad");
            string method = message.Substring(0, pos);
            //get activity
            message = pos < message.Length - 1 ? message.Substring(pos + 1, message.Length - pos - 1) : "";
            pos = message.IndexOf('|');
            if (pos < 0)
                return new AppError(1, "_message is invaliad");

            string activity = message.Substring(0, pos);
            //get param
            string param = pos < message.Length - 1 ? message.Substring(pos + 1, message.Length - pos - 1) : "";

            HandleDelegate handle = findHandle(method);
            if(null == handle)
                return new AppError(1, "handle is not exists");

            handle(activity, param);

            Resources.UnloadUnusedAssets();
            System.GC.Collect();
            return new AppError(0,"");
        }

        public static void MessageToJS(string _method, string _activity, string _param)
        {
            Log.LogDebug("JSAPI", "message to javascript {0}|{1}|{2}", _method, _activity, _param);
            Application.ExternalCall("__messageToQt", string.Format("{0}|{1}|{2}", _method, _activity, _param));
        }

        public static void RegisterHandle(string _method, HandleDelegate _dalegate)
        {
            handles.Add(_method, _dalegate);
        }

        private static HandleDelegate findHandle(string _method)
        {
            if (handles.ContainsKey(_method))
                return handles[_method];
            return null;
        }

        #region Editor

        private static void JSEditorNewImage(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string guid = root["guid"].Value;
                EditorMgr.NewImage(guid);
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorDeleteImage(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string guid = root["guid"].Value;
                EditorMgr.DeleteImage(guid);
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorReset(string _activity, string _params)
        {
            try
            {
                EditorMgr.Reset();
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorClean(string _activity, string _params)
        {
            try
            {
                EditorMgr.Clean();
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorImport(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string source = root["source"].Value;
                if(source.Equals(".vcb"))
                {
                    EditorMgr.ImportVCB(()=>
                    {
                        startLoading();
                    },()=>
                    {
                        finishLoading();
                        MessageToJS(JSAPI.MSG.JSEditorOnImportFinish, "", "");
                    }, (_error)=>
                    {
                        finishLoading();
                        onNotify(_error);
                    });
                }
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorSwitchImage(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string guid = root["guid"].Value;
                EditorMgr.SwithImage(guid);
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorUseSkybox(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string source = root["source"].Value;
                if(source.Equals("asset"))
                {
                    string package = root["package"].Value;
                    string file = root["file"].Value;

                    ResourceMgr.PreloadAsset(package, file
                    , () =>
                    {
                        startLoading();
                    }
                    , (_obj) =>
                    {
                        EditorMgr.UseSkybox(_obj as Material);
                        finishLoading();
                    }
                    , (_err) =>
                    {
                        onNotify(_err);
                    });
                }
                else if (source.Equals("anyres"))
                {
                    string package = root["package"].Value+".pkg";
                    string file = root["file"].Value;

                    startLoading();

                    AnyRes.Unpack.LoadPackage(package, (_entries) =>
                    {
                        Log.LogInfo("JSAPI", "AnyRes Memory size: {0}", AnyRes.Unpack.MemorySize);

                        AnyRes.Block b1 = AnyRes.Unpack.LoadFile(package, file + "@1.png");
                        AnyRes.Block b2 = AnyRes.Unpack.LoadFile(package, file + "@2.png");
                        AnyRes.Block b3 = AnyRes.Unpack.LoadFile(package, file + "@3.png");
                        AnyRes.Block b4 = AnyRes.Unpack.LoadFile(package, file + "@4.png");
                        AnyRes.Block b5 = AnyRes.Unpack.LoadFile(package, file + "@5.png");
                        AnyRes.Block b6 = AnyRes.Unpack.LoadFile(package, file + "@6.png");
                        Material mat = AnyRes.SkyboxUtil.FromCubemap(b1.Merge(), b2.Merge(), b3.Merge(), b4.Merge(), b5.Merge(), b6.Merge());
                        EditorMgr.UseSkybox(mat);
                        finishLoading();
                    }, (_err) =>
                    {
                        onNotify(_err);
                    });
                }
                else if(source.Equals("ugc"))
                {
                    string file = root["file"].Value;
                    Log.Info("JSAPI", "read web file: {0}", file);
                    VRXX.Manager.UGCMgr.ReadWebFile(file, () =>
                    {
                        startLoading();
                    }, (_data)=>
                    {
                        Material mat = UGCMgr.BuildSkybox(_data);
                        EditorMgr.UseSkybox(mat);
                        finishLoading();
                    }, (_error)=>
                    {
                        onNotify(_error);
                    });
                }
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorNewClone(string _activity, string _params)
        {
            if (isLoading)
                return;

            try
            {
                JSONNode root = JSON.Parse(_params);
                string package = root["package"].Value;
                string file = root["file"].Value;
                string uuid = root["uuid"].Value;


                ResourceMgr.PreloadAsset(package, file
                , () =>
                {
                    startLoading();
                }
                , (_obj) =>
                {
                    GameObject go = ResourceMgr.CloneGameObject(package, file, uuid);
                    go.SetActive(false);
                    //open collider, support mouse pick
                    Collider collider = go.GetComponent<Collider>();
                    if (null != collider)
                        collider.enabled = true;
                    Battlehub.RTEditor.ExposeToEditor script = go.GetComponent<Battlehub.RTEditor.ExposeToEditor>();
                    if (null == script)
                    {
                        script = go.AddComponent<Battlehub.RTEditor.ExposeToEditor>();
                        //Debug.Log(script);
                    }
                    EditorMgr.AppendGameObject(uuid);
                    go.SetActive(true);
                    finishLoading();
                }
                , (_err) =>
                {
                    onNotify(_err);
                }
            );
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorDeleteClone(string _activity, string _params)
        {
            if (isLoading)
                return;

            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                ResourceMgr.UnloadGameObject(uuid);
                Battlehub.RTEditor.RuntimeSelection.activeObject = null;
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorNewTrigger(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string type = root["type"].Value;
                string uuid = root["uuid"].Value;
                
                if(type.Equals("sight"))
                {
                    TriggerMgr.NewEditorSightTrigger(uuid);
                    EditorMgr.AppendTrigger(uuid);
                    //QT need position and rotation of trigger to save data.
                    Vector3 position = TriggerMgr.QueryTriggerPosition(uuid);
                    Vector3 rotation = TriggerMgr.QueryTriggerRotation(uuid);
                    MessageToJS("JSOnEditorAdjustTrigger", _activity, string.Format("{{\"uuid\":\"{0}\", \"px\":{1}, \"py\":{2}, \"pz\":{3}, \"rx\":{4}, \"ry\":{5}, \"rz\":{6}}}", uuid, position.x, position.y, position.z, rotation.x, rotation.y, rotation.z));
                }
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorModifyTrigger(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                string type = root["type"].Value;
                string alias = root["alias"].Value;


                if (type.Equals("sight"))
                {
                    TriggerMgr.ModifyGazeAlias(uuid, alias);
                }
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorModifyGazeIcon(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                int icon = root["icon"].AsInt;

                TriggerMgr.ModifyGazeIcon(uuid, icon);
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorModifyGazeColor(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                int r = root["r"].AsInt;
                int g = root["g"].AsInt;
                int b = root["b"].AsInt;
                int a = root["a"].AsInt;

                TriggerMgr.ModifyGazeColor(uuid, r, g, b, a);
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }


        private static void JSEditorDeleteTrigger(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                TriggerMgr.DeleteEditorSightTrigger(uuid);
                EditorMgr.RemoveTrigger(uuid);
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }


        private static void JSEditorFocusTrigger(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                string type = root["type"].Value;

                if (type.Equals("sight"))
                {
                    TriggerMgr.FocusSightTrigger(uuid);
                }
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorAdjustTrigger(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                string type = root["type"].Value;

                if (type.Equals("sight"))
                {
                    TriggerMgr.AdjustSightTrigger(uuid);
                    Vector3 position = TriggerMgr.QueryTriggerPosition(uuid);
                    Vector3 rotation = TriggerMgr.QueryTriggerRotation(uuid);
                    MessageToJS("JSOnEditorAdjustTrigger", _activity, string.Format("{{\"px\":{0}, \"py\":{1}, \"pz\":{2}, \"rx\":{3}, \"ry\":{4}, \"rz\":{5}}}", position.x, position.y, position.z, rotation.x, rotation.y, rotation.z));
                }
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSEditorExposeDirectionalLight(string _activity, string _params)
        {
            if (isLoading)
                return;

            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;

                LightMgr.NewDirectionalLight(uuid);
                startLoading();

                Light light = LightMgr.FindDirectionalLight(uuid);
                if (light == null)
                {
                    onNotify(string.Format("{0} not instance", uuid));
                    return;
                }

                Battlehub.RTEditor.ExposeToEditor script = light.gameObject.GetComponent<Battlehub.RTEditor.ExposeToEditor>();
                if (null == script)
                {
                    script = light.gameObject.AddComponent<Battlehub.RTEditor.ExposeToEditor>();
                    //Debug.Log(script);
                }
                //SlateMgr.AttachGameObject(uuid);
                finishLoading();
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }

        }

        private static void JSEditorSelect(string _activity, string _params)
        {
            string uuid = "";
            try
            {
                JSONNode root = JSON.Parse(_params);
                uuid = root["uuid"].Value;
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }

            GameObject go = ResourceMgr.FindGameObject(uuid);
            if (null == go)
            {
                onNotify(string.Format("{0} is not exists", uuid));
                return;
            }

            Battlehub.RTEditor.RuntimeSelection.activeObject = go;
            Battlehub.RTEditor.RuntimeEditor.Instance.SceneView.Focus();
            Battlehub.RTHandles.RuntimeTools.CameraEuler = Battlehub.RTHandles.RuntimeCameraEuler.Target;
            Battlehub.RTHandles.RuntimeTools.Current = Battlehub.RTHandles.RuntimeTool.Move;
        }

        private static void JSEditorDirectionalLightSelect(string _activity, string _params)
        {
            string uuid = "";
            try
            {
                JSONNode root = JSON.Parse(_params);
                uuid = root["uuid"].Value;
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }

            Light light = LightMgr.FindDirectionalLight(uuid);
            if (null == light)
            {
                onNotify(string.Format("{0} is not exists", uuid));
                return;
            }

            Battlehub.RTEditor.RuntimeSelection.activeObject = light.gameObject;
            Battlehub.RTEditor.RuntimeEditor.Instance.SceneView.Focus();
        }

        private static void JSEditorFocus(string _activity, string _params)
        {
            Battlehub.RTEditor.RuntimeEditor.Instance.SceneView.Focus();
           
        }
        #endregion

        #region Resource API

        private static void JSUnloadGameObject(string _activity, string _params)
        {
            if (isLoading)
                return;

            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                ResourceMgr.UnloadGameObject(uuid);
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSLoadScene(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string package = root["package"].Value;
                string file = root["file"].Value;
                ResourceMgr.LoadScene(package, file
                , () =>
                {
                    startLoading();
                }
                , (_scene) =>
                {
                    finishLoading();
                }
                , (_err) =>
                {
                    onNotify(_err);
                }
                );
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSLoadSkybox(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string package = root["package"].Value;
                string file = root["file"].Value;
                ResourceMgr.LoadSkybox(package, file
                , () =>
                {
                    startLoading();
                }
                , (_scene) =>
                {
                    finishLoading();
                }
                , (_err) =>
                {
                    onNotify(_err);
                }
                );
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSCleanSkybox(string _activity, string _params)
        {
            try
            {
                CameraMgr.CleanSkybox();
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        #endregion

        #region Agent API

        private static void JSPlaceAgent(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                float x = root["x"].AsFloat;
                float y = root["y"].AsFloat;
                float z = root["z"].AsFloat;
                AgentMgr.Place(uuid, x, y, z, (_err) =>
                {
                    onNotify(_err);
                });
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSTranslateAgent(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                float x = root["x"].AsFloat;
                float y = root["y"].AsFloat;
                float z = root["z"].AsFloat;
                AgentMgr.Translate(uuid, x, y, z, (_err) =>
                {
                    onNotify(_err);
                });
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSRotateAgent(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                float x = root["x"].AsFloat;
                float y = root["y"].AsFloat;
                float z = root["z"].AsFloat;
                AgentMgr.Rotate(uuid, x, y, z, (_err) =>
                {
                    onNotify(_err);
                });
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        private static void JSScaleAgent(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                float x = root["x"].AsFloat;
                float y = root["y"].AsFloat;
                float z = root["z"].AsFloat;
                AgentMgr.Scale(uuid, x, y, z, (_err) =>
                {
                    onNotify(_err);
                });
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }
        #endregion

        #region Camera API

        private static void JSChangeViewport(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string which = root["which"].Value;
                if (which.Equals("front"))
                    CameraMgr.ChangeViewport(CameraMgr.Viewport.FRONT);
                else if (which.Equals("back"))
                    CameraMgr.ChangeViewport(CameraMgr.Viewport.BACK);
                else if (which.Equals("left"))
                    CameraMgr.ChangeViewport(CameraMgr.Viewport.LEFT);
                else if (which.Equals("right"))
                    CameraMgr.ChangeViewport(CameraMgr.Viewport.RIGHT);
                else if (which.Equals("top"))
                    CameraMgr.ChangeViewport(CameraMgr.Viewport.TOP);
                else if (which.Equals("bottom"))
                    CameraMgr.ChangeViewport(CameraMgr.Viewport.BOTTOM);
                else if (which.Equals("free"))
                    CameraMgr.ChangeViewport(CameraMgr.Viewport.FREE);
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

      

        private static void JSQueryCameraPosition(string _activity, string _param)
        {
            Vector3 position = CameraMgr.position;
            MessageToJS("JSOnQueryCameraPosition", _activity, string.Format("{{\"x\":{0}, \"y\":{1}, \"z\":{2}}}", position.x, position.y, position.z));
        }

        private static void JSQueryCameraRotation(string _activity, string _param)
        {
            Vector3 rotation = CameraMgr.rotation;
            MessageToJS("JSOnQueryCameraRotation", _activity, string.Format("{{\"x\":{0}, \"y\":{1}, \"z\":{2}}}", rotation.x, rotation.y, rotation.z));
        }

        private static void JSAlignGameObject(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                GameObject go = ResourceMgr.FindGameObject(uuid);
                if(null == go)
                {
                    onNotify("GameObject " + uuid + " is not exists!");
                    return;
                }
                CameraMgr.Align(go.transform);
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }
        #endregion

        #region Light API
        private static void JSNewNewDirectionalLight(string _activity, string _params)
        {
            string uuid = LightMgr.NewDirectionalLight();
            MessageToJS("JSOnNewNewDirectionalLight", _activity, string.Format("|{{\"uuid\":{0}}}", uuid));
        }

        private static void JSUnloadDirectionalLight(string _activity, string _params)
        {
            if (isLoading)
                return;

            try
            {
                JSONNode root = JSON.Parse(_params);
                string uuid = root["uuid"].Value;
                LightMgr.UnloadDirectionalLight(uuid);
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }
        #endregion

        #region VR API
        private static void JSVRToggleReticle(string _activity, string _params)
        {
            try
            {
                JSONNode root = JSON.Parse(_params);
                string paramVisible = root["visible"].Value;
                bool visible = bool.Parse(paramVisible);
                VRMgr.ToogleReticle(visible);
            }
            catch (System.Exception e)
            {
                onNotify("Parse json has error: " + e.Message);
            }
        }

        #endregion

        private static void startLoading()
        {
            isLoading = true;
            onLoadStart();
        }

        private static void finishLoading()
        {
            isLoading = false;
            onLoadFinish();
        }

    }
}