/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using UnityEngine.SceneManagement;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using SimpleJSON;
using Fangs.Lib.Logger;
using Fangs.Lib.ResIO;

namespace VRXX.Manager
{
	public static class ResourceMgr
	{
        public delegate void OnLoadReadyDelegate();
        public delegate void OnLoadObjectSuccessDelegate(Object _resource);
        public delegate void OnLoadSceneSuccessDelegate(string _scene);
        public delegate void OnLoadImageSuccessDelegate(Texture2D _image);


        public delegate void OnReadTextSuccessDelegate(string _resource);
        public delegate void OnReadDataSuccessDelegate(byte[] _resource);

        public delegate void OnErrorDelegate(string _error);

        private static Transform root_ { get; set; }

        // key is uuid of gameobject
        private static Dictionary<string, Object> preloadAssets = new Dictionary<string, Object>(0);

        private static Dictionary<string, GameObject> gameobjects = new Dictionary<string, GameObject>(0);
        private static Dictionary<string, AssetBundle> bundles = new Dictionary<string, AssetBundle>(0);
        private static UniResLoader resLoader = null;

        public static void Preload ()
		{
            root_ = new GameObject("__ResourceMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            resLoader = new UniResLoader(CoroutineMgr.mono);
            ResBundle.UseLoader(resLoader);
            Log.Info("ResourceMgr", "Preload ResourceMgr finish");
		}

        public static void UseExternalBundle(string _bundle)
        {
            resLoader.AppendExternalBundle(_bundle);
        }

        public static void ToggleAllGameObjects(bool _flag)
        {
            foreach (GameObject go in gameobjects.Values)
            {
                go.SetActive(_flag);
            }
        }

        public static void ToggleGameObjects(string[] _uuids, bool _flag)
        {
            foreach (string uuid in _uuids)
            {
                GameObject go = null;
                if (gameobjects.TryGetValue(uuid, out go))
                    go.SetActive(_flag);
            }
        }

        public static void PreloadAsset(string _package, string _file
            , OnLoadReadyDelegate _onReady
            , OnLoadObjectSuccessDelegate _onSuccess
            , OnErrorDelegate _onError)
        {
            if (string.IsNullOrEmpty(_package))
            {
                _onError("package is null");
                return;
            }

            if (string.IsNullOrEmpty(_file))
            {
                _onError("file is null");
                return;
            }

            _onReady();
            string assetID = _package + "@" + _file;

            if (!preloadAssets.ContainsKey(assetID))
            {
                preloadAssets.Add(assetID, null);
                CoroutineMgr.Start(asyncLoadBundle(_package, _file, (_res) =>
                {
                    Log.Debug("ResourceMgr", "load res [{0}] finish...", assetID);
                    preloadAssets[assetID] = _res;
                    _onSuccess(_res);
                },
                _onError));
            }
            else
            {
                Log.Debug("ResourceMgr", "res [{0}] is exists...", assetID);
                _onSuccess(preloadAssets[assetID]);
            }
        }

        public static void PreloadAnyRes(string _package, string _file
            , OnLoadReadyDelegate _onReady
            , OnLoadObjectSuccessDelegate _onSuccess
            , OnErrorDelegate _onError)
        {
            if (string.IsNullOrEmpty(_package))
            {
                _onError("package is null");
                return;
            }

            if (string.IsNullOrEmpty(_file))
            {
                _onError("file is null");
                return;
            }

            _onReady();
            
        }

        public static Object FindFromPreload(string _package, string _file)
        {
            string assetID = _package + "@" + _file;
            if (!preloadAssets.ContainsKey(assetID))
                return null;
            return preloadAssets[assetID];
        }

        public static GameObject CloneGameObject(string _package, string _file, string _uuid)
        {
            string assetID = _package + "@" + _file;

            if (!preloadAssets.ContainsKey(assetID))
                return null;

            GameObject clone = GameObject.Instantiate(preloadAssets[assetID]) as GameObject;
            GameObject.DontDestroyOnLoad(clone);
            gameobjects[_uuid] = clone;
            clone.name = _uuid;
            return clone;
        }

        public static void CleanAllGameObjects()
        {
            foreach (GameObject go in gameobjects.Values)
            {
                GameObject.Destroy(go);
            }
            gameobjects.Clear();
        }

        public static void UnloadGameObject(string _uuid)
        {
            if (gameobjects.ContainsKey(_uuid))
            {
                GameObject obj = gameobjects[_uuid];
                gameobjects.Remove(_uuid);
                GameObject.Destroy(obj);
                Resources.UnloadUnusedAssets();
            }
        }

        public static GameObject FindGameObject(string _uuid)
        {
            GameObject go = null;
            gameobjects.TryGetValue(_uuid, out go);
            return go;
        }


        public static void LoadScene(string _package, string _file
            , OnLoadReadyDelegate _onReady
            , OnLoadSceneSuccessDelegate _onSuccess
            , OnErrorDelegate _onError)
        {
            if (string.IsNullOrEmpty(_package))
            {
                _onError("package is null");
                return;
            }

            if (string.IsNullOrEmpty(_file))
            {
                _onError("file is null");
                return;
            }

            if (SceneManager.GetActiveScene().name.Equals(_file))
                return;

            _onReady();
            resLoader.AppendExternalBundle(_package);
            ResBundle.AsyncLoadBundle(_package, (_bundle) =>
            {
                _onSuccess(_file);
            });
        }

        public static void LoadSkybox(string _package, string _file
            , OnLoadReadyDelegate _onReady
            , OnLoadObjectSuccessDelegate _onSuccess
            , OnErrorDelegate _onError)
        {
            if (string.IsNullOrEmpty(_package))
            {
                _onError("package is null");
                return;
            }

            if (string.IsNullOrEmpty(_file))
            {
                _onError("file is null");
                return;
            }

            _onReady();

            CoroutineMgr.Start(asyncLoadBundle(_package, _file, (_res) =>
            {
                Log.Debug("ResourceMgr", "load skybox [{0}] finish...", _file);
                CameraMgr.ApplySkybox(_res as Material);
                _onSuccess(_res);
            },
            _onError));
        }


        public static void LoadAudioClip(string _package, string _file, int _track
            , OnLoadReadyDelegate _onReady
            , OnLoadObjectSuccessDelegate _onSuccess
            , OnErrorDelegate _onError) 
        {
            if (string.IsNullOrEmpty(_package))
            {
                _onError("package is null");
                return;
            }
            if (string.IsNullOrEmpty(_file))
            {
                _onError("file is null");
                return;
            }

            _onReady();
           
            CoroutineMgr.Start(asyncLoadBundle(_package, _file, (_res) =>
            {
                Log.Debug("ResourceMgr", "load audioclip [{0}] finish...", _file);
                _onSuccess(_res);
            },
           _onError));
        }

        public static void ReadStreamText(string _file
           , OnReadTextSuccessDelegate _onSuccess
           , OnErrorDelegate _onError)
        {
            if (string.IsNullOrEmpty(_file))
            {
                _onError("file is null");
                return;
            }
            CoroutineMgr.Start(readStreamText(_file, _onSuccess, _onError));
        }

        public static void ReadPersistentText(string _file
            , OnReadTextSuccessDelegate _onSuccess
            , OnErrorDelegate _onError)
        {
            if (string.IsNullOrEmpty(_file))
            {
                _onError("file is null");
                return;
            }
            CoroutineMgr.Start(readPersistentText(_file, _onSuccess, _onError));
        }

        public static void ReadPersistentData(string _file
           , OnReadDataSuccessDelegate _onSuccess
           , OnErrorDelegate _onError)
        {
            if (string.IsNullOrEmpty(_file))
            {
                _onError("file is null");
                return;
            }
            CoroutineMgr.Start(readPersistentData(_file, _onSuccess, _onError));
        }

        public static void ReadPersistentImage(string _uuid
            , OnLoadImageSuccessDelegate _onSuccess
            , OnErrorDelegate _onError)
        {
            if (string.IsNullOrEmpty(_uuid))
            {
                _onError("file is null");
                return;
            }
            CoroutineMgr.Start(readPersistentImage(_uuid, _onSuccess, _onError));
        }



        private static IEnumerator readPersistentText(string _file, OnReadTextSuccessDelegate _onSuccess, OnErrorDelegate _onError)
        {
            string path = Path.Combine(VRXX.Platform.GetPersistentDataPath(), _file);
            Log.Debug("ResourceMgr", "read text [{0}] from [{1}]", _file, path);
            WWW www = new WWW(path);
            yield return www;
            if (www.error != null)
            {
                _onError(www.error);
                yield break;
            }
            _onSuccess(www.text);
        }

        private static IEnumerator readPersistentData(string _file, OnReadDataSuccessDelegate _onSuccess, OnErrorDelegate _onError)
        {
            string path = Path.Combine(VRXX.Platform.GetPersistentDataPath(), _file);
            Log.Debug("ResourceMgr", "read data [{0}] from [{1}]", _file, path);
            WWW www = new WWW(path);
            yield return www;
            if (www.error != null)
            {
                _onError(www.error);
                yield break;
            }
            _onSuccess(www.bytes);
        }

        private static IEnumerator readPersistentImage(string _file, OnLoadImageSuccessDelegate _onSuccess, OnErrorDelegate _onError)
        {
            string path = Path.Combine(VRXX.Platform.GetPersistentDataPath(), _file);
            Log.Debug("ResourceMgr", "read image [{0}] from [{1}]", _file, path);
            WWW www = new WWW(path);
            yield return www;
            if (www.error != null)
            {
                _onError(www.error);
                yield break;
            }
            _onSuccess(www.texture);
        }

        private static IEnumerator readStreamText(string _file, OnReadTextSuccessDelegate _onSuccess, OnErrorDelegate _onError)
        {
            string path = Path.Combine(VRXX.Platform.GetStreamingAssetsPath(), _file);
            Log.Debug("ResourceMgr", "read text [{0}] from [{1}]", _file, path);
            WWW www = new WWW(path);
            yield return www;
            if(www.error != null)
            {
                _onError(www.error);
                yield break;
            }
            _onSuccess(www.text);
        }

        private static IEnumerator asyncLoadBundle(string _bundle, string _res, OnLoadObjectSuccessDelegate _onSuccess, OnErrorDelegate _onError)
        {
            AssetBundle bundle = null;
            if (bundles.ContainsKey(_bundle))
            {
                bundle = bundles[_bundle];
            }
            else
            {
                string path = Path.Combine(VRXX.Platform.GetPersistentDataPath() + "/pkgs/" + VRXX.Platform.Alias, _bundle);
                Log.Debug("ResourceMgr", "load bundle from {0}", path);
                WWW www = new WWW(path);
                yield return www;
                if (null != www.error)
                {
                    _onError(www.error);
                    yield break;
                }
                bundle = www.assetBundle;
                if (null == bundle)
                {
                    _onError("bundle is null");
                    yield break;
                }
                bundles.Add(_bundle, bundle);
            }

            AssetBundleRequest req = bundle.LoadAssetAsync(_res);
            yield return req;
            _onSuccess(req.asset);
        }
    }//class
}//namespace
