/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using Fangs.Lib.Net;
using System.IO;
using SimpleJSON;

namespace VRXX.Manager
{
    public static class HttpMgr
    {
        public delegate void OnSuccessFinishDelegate();
        public delegate void OnErrorDelegate(string _error);

        public static float batchProgress
        {
            get
            {
                if (operations.Count == 0)
                    return 0f;

                float total = (float)operations.Count;
                float current = 0f; 
                foreach(Http.Operation operation in operations.Values)
                {
                    current += operation.progress;
                }
                return current / total;
            }
        }

        private static Transform root_ { get; set; }

        private static Http http = new Http();

        private static string resPath = Application.persistentDataPath + "/res/";
        private static string beansPath = Application.persistentDataPath + "/beans/";

        private static Dictionary<string, Http.Operation> operations = new Dictionary<string, Http.Operation>(0);

        public static void Preload()
        {
            root_ = new GameObject("__HttpMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("CoroutineMgr", "Preload CoroutineMgr  finish ");
            http.mono = CoroutineMgr.mono;

            if (!Directory.Exists(resPath))
                Directory.CreateDirectory(resPath);
            if (!Directory.Exists(beansPath))
                Directory.CreateDirectory(beansPath);
        }

        public static void BatchDownloadRes(List<string> _list, OnSuccessFinishDelegate _onFinish)
        {
            operations.Clear();
            List<string> tasks = new List<string>();
            foreach(string task in _list)
            {
                string file = Path.GetFileName(task);
                // All files will store in the persistentDataPath
                ResourceMgr.UseExternalBundle(file);
                if (!File.Exists(resPath + file))
                {
                    tasks.Add(task);
                }
            }

            Log.Info("HttpMgr", "BatchDownload [{0}] tasks", tasks.Count);
            if(0 == tasks.Count)
            {
                _onFinish();
                return;
            }

            int count = 0;
            foreach (string task in tasks)
            {
                string file = Path.GetFileName(task);
                Log.Debug("HttpMgr", "Async Get: {0}", task);
                Http.Operation operation = http.AsyncGET(task, (_data) =>
                {
                    Log.Debug("HttpMgr", "Get Finish: {0}", task);
                    System.IO.File.WriteAllBytes(resPath + file, _data);
                    onFinish(ref count, tasks.Count, _onFinish);
                }, (_err) =>
                {
                    Log.Error("HttpMgr", _err);
                });
                operations.Add(task, operation);
            }
        }

        public static void BatchDownloadImages(List<string> _list, OnSuccessFinishDelegate _onFinish)
        {
            List<string> tasks = new List<string>();
            foreach(string task in _list)
            {
                string file = Path.GetFileName(task);
                ResourceMgr.UseExternalBundle(file);
                if (!File.Exists(beansPath + file))
                {
                    tasks.Add(task);
                }
            }
            Log.Info("HttpMgr", "BatchDownload [{0}] tasks", tasks.Count);
            if(0 == tasks.Count)
            {
                _onFinish();
                return;
            }
            int count = 0;
            foreach (string task in tasks)
            {
                string file = Path.GetFileName(task);
                Log.Debug("HttpMgr", "Async Get: {0}", task);
                Http.Operation operation = http.AsyncGET(task, (_data) =>
                {
                    Log.Debug("HttpMgr", "Get Finish: {0}", task);
                    System.IO.File.WriteAllBytes(beansPath + file, _data);
                    onFinish(ref count, tasks.Count, _onFinish);
                }, (_err) =>
                {
                    onFinish(ref count, tasks.Count, _onFinish);
                });
            }
        }

        public static Http.Operation DownLoadBean(string _uuid, OnSuccessFinishDelegate _onFinish, OnErrorDelegate _onError)
        {
            string uri = Settings.fangs_domain + "/storage/pullstring";
            Log.Debug("HttpMgr", "DownloadBean: {0}", uri);
            Dictionary<string, object> paramAry = new Dictionary<string, object>(0);
            paramAry.Add("bucket", HttpMgr.Base64Encode(Settings.beans_bucket));
            paramAry.Add("key", HttpMgr.Base64Encode(_uuid));
            Http.Operation operation = http.AsyncPOST(uri, paramAry, (_data) =>
            {
                Log.Debug("HttpMgr", "DownloadBean Finish: {0}", uri);
                JSONNode json = JSON.Parse(System.Text.Encoding.UTF8.GetString(_data));

                string bean = HttpMgr.Base64Decode(json["data"]["value"].Value);
                File.WriteAllText(Path.Combine(beansPath, _uuid + ".json"), bean);
                _onFinish();

            }, (_err) =>
            {
                Log.Error("HttpMgr", _err);
                _onError(_err.errmessage);
            });

            return operation;
        }

        public static void DownloadManifest(OnSuccessFinishDelegate _onFinish, OnErrorDelegate _onError)
        {
            string uri = Settings.fangs_domain + "/storage/list";
            Log.Debug("HttpMgr", "DownloadMinifest: {0}", uri);
            Dictionary<string, object> paramAry = new Dictionary<string, object>(0);
            paramAry.Add("bucket", HttpMgr.Base64Encode(Settings.beans_bucket));
            paramAry.Add("sort", HttpMgr.Base64Encode("time"));
            paramAry.Add("from", 0);
            paramAry.Add("to", 32);
            Http.Operation operation = http.AsyncPOST(uri, paramAry, (_data) =>
            {
                Log.Debug("HttpMgr", "DownloadMinifest Finish: {0}", uri);
                // parse the response
                JSONNode json = JSON.Parse(System.Text.Encoding.UTF8.GetString(_data));
                int errcode = json["errcode"].AsInt;
                // 0 is OK
                if (0 != errcode)
                {
                    _onError(string.Format("Download manifest has error, the errorcode is {0}", errcode));
                    return;
                }

                // take all the guid of bean and put them to a array.
                int count = json["data"]["keys:length"].AsInt;
                string[] beans = new string[count];
                for(int i = 0; i < count; ++i)
                {
                    string field = string.Format("keys:{0}", i);
                    string value = json["data"][field].Value;
                    beans[i] = Base64Decode(value);
                }
                // save the minifest file with beans
                JSONArray beanAry = new JSONArray();
                foreach (string bean in beans)
                    beanAry.Add(bean);
                JSONClass root = new JSONClass();
                root.Add("beans", beanAry);
                string manifestJson = root.ToJSON(0);

                File.WriteAllText(Path.Combine(beansPath, "manifest.txt"), manifestJson);
                _onFinish();
            }, (_err) =>
            {
                Log.Error("HttpMgr", _err);
                _onError(_err.errmessage);
            });
        }



        public static string Base64Encode(string _value)
        {
            var plainTextBytes = System.Text.Encoding.UTF8.GetBytes(_value);
            return System.Convert.ToBase64String(plainTextBytes);
        }

        public static string Base64Decode(string _value)
        {
            var base64EncodedBytes = System.Convert.FromBase64String(_value);
            return System.Text.Encoding.UTF8.GetString(base64EncodedBytes); ;
        }

        public static void StopDownloadRes()
        {
            foreach (Http.Operation operation in operations.Values)
            {
                operation.Abort();
                Log.Debug("HttpMgr", "stop download");
            }
        }

        private static void onFinish(ref int _current, int _total, OnSuccessFinishDelegate _onFinish)
        {
            _current++;
            Log.Debug("HttpMgr", "download status: {0}/{1}", _current, _total);
            if (_current == _total)
                _onFinish();
        }

    }//class
}//namespace

