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
using Fangs.Lib.Net;

using SimpleJSON;
using VRXX.Manager;

namespace VRXX.Manager
{
    public static class BeanMgr
    {
        public delegate void CallBack();

        private static Transform root_ { get; set; }

        public static void Preload()
        {
            root_ = new GameObject("__BeanMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("BeanMgr", "Preload BeanMgr finish");
        }

        public static void Run(string _beanJson)
        {
            Log.Info("BeanMgr", "Run Bean {0}", _beanJson);
            parseBeanJson(_beanJson); 
            SlateMgr.Run();
        }

        public static void Run(byte[] _beanData)
        {
            Log.Info("BeanMgr", "Run Bean data, size is  [{0}]", _beanData.Length);
            int beanSize;
            string beanJson = takeBeanJsonFromData(_beanData, out beanSize);
            Log.Info("BeanMgr", "Run Bean {0}", beanJson);
            parseBeanJson(beanJson);
            int offset = 4 + beanSize;

            int ugcfiles_count = bytesToInt(_beanData, offset);
            offset += 4;
            Log.Info("BeanMgr", "has {0} ugc files", ugcfiles_count);
            //skip ugcfiles_count;
            
            for(int i = 0; i < ugcfiles_count; ++i)
            {
                int size_ugcfile_filename = bytesToInt(_beanData, offset);
                offset += 4;
                string filename = bytesToString(_beanData, offset, size_ugcfile_filename);
                Log.Info("BeanMgr", "take ugcfile : [{0}]", filename);
                offset += size_ugcfile_filename;
                int size_ugcfile_data = bytesToInt(_beanData, offset);
                offset += 4;
                byte[] filedata = new byte[size_ugcfile_data];
                System.Array.Copy(_beanData, offset, filedata, 0, size_ugcfile_data);
                UGCMgr.Cache(filename, filedata);
                offset += size_ugcfile_data;
            }
            
            int cachefiles_count = bytesToInt(_beanData, offset);
            offset += 4;
            Log.Info("BeanMgr", "has {0} cache files", cachefiles_count);
            //skip ugcfiles_count;
            for (int i = 0; i < cachefiles_count; ++i)
            {
                int size_cachefile_filename = bytesToInt(_beanData, offset);
                offset += 4;
                string filename = bytesToString(_beanData, offset, size_cachefile_filename);
                Log.Info("BeanMgr", "take cachefile : [{0}]", filename);
                offset += size_cachefile_filename;
                int size_cachefile_data = bytesToInt(_beanData, offset);
                offset += 4;
                byte[] filedata = new byte[size_cachefile_data];
                System.Array.Copy(_beanData, offset, filedata, 0, size_cachefile_data);
                offset += size_cachefile_data;

                UGCMgr.Cache(filename, filedata);
            }

            SlateMgr.Run();
        }

        public static void Stop()
        {
            BlockMgr.Clean();
            SlateMgr.Stop();
        }

        private static void parseBeanJson(string _beanJson)
        {
            JSONNode root = JSON.Parse(_beanJson);
            JSONArray slateAry = root["slates"].AsArray;

            foreach (JSONNode slateNode in slateAry)
            {
                string slateUUID = slateNode["guid"].Value;
                Slate slate = SlateMgr.NewSlate(slateUUID);
                slate.alias = slateNode["alias"].Value;

                Log.Info("BeanMgr", "parse blocks");
                JSONArray pipeAry = slateNode["pipes"].AsArray;
                foreach (JSONNode pipeNode in pipeAry)
                {
                    string pipeUUID = pipeNode["guid"].Value;

                    JSONArray actionAry = pipeNode["actions"].AsArray;
                    foreach (JSONNode blockNode in actionAry)
                    {
                        string blockUUID = blockNode["guid"].Value;
                        Block block = BlockMgr.NewBlock(blockUUID);
                        block.action = blockNode["action"].Value;
                        block.path = slateUUID;
                        slate.AppendBlock(blockUUID);
                        JSONArray paramsAry = blockNode["params"].AsArray;
                        foreach (JSONNode node in paramsAry)
                        {
                            string key = node["key"].Value;
                            string value = node["value"].Value;
                            block.param[key] = value;
                        }
                    }
                }

                Log.Info("BeanMgr", "link blocks");
                slate.LinkBlocks();

                Log.Info("BeanMgr", "parse preloads");
                JSONArray preloadAry = slateNode["preloads"].AsArray;
                foreach (JSONNode node in preloadAry)
                {
                    string guid = node["guid"].Value;
                    Slate.Asset asset = slate.RegisterAsset(guid);
                    asset.slate = slateUUID;
                    asset.group = node["group"].Value;
                    asset.package = node["package"].Value;
                    asset.file = node["file"].Value;
                    asset.guid = node["guid"].Value;
                    asset.px = node["px"].AsFloat;
                    asset.py = node["py"].AsFloat;
                    asset.pz = node["pz"].AsFloat;
                    asset.rx = node["rx"].AsFloat;
                    asset.ry = node["ry"].AsFloat;
                    asset.rz = node["rz"].AsFloat;
                    asset.sx = node["sx"].AsFloat;
                    asset.sy = node["sy"].AsFloat;
                    asset.sz = node["sz"].AsFloat;
                    asset.gaze = node["gaze"].AsBool;
                    asset.gazeAlias = node["gaze.alias"].Value;
                }

                Log.Info("BeanMgr", "parse triggers");
                JSONArray triggerAry = slateNode["triggers"].AsArray;
                foreach (JSONNode node in triggerAry)
                {
                    string uuid = node["uuid"].Value;
                    string alias = node["alias"].Value;
                    float px = node["px"].AsFloat;
                    float py = node["py"].AsFloat;
                    float pz = node["pz"].AsFloat;
                    float rx = node["rx"].AsFloat;
                    float ry = node["ry"].AsFloat;
                    float rz = node["rz"].AsFloat;
                    int icon = node["icon"].AsInt;
                    int r = node["color.r"].AsInt;
                    int g = node["color.g"].AsInt;
                    int b = node["color.b"].AsInt;
                    int a = node["color.a"].AsInt;
                    SightTrigger trigger = TriggerMgr.NewSightTrigger(uuid);
                    trigger.trigger = alias;
                    trigger.path = slateUUID;
                    TriggerMgr.AdjustSightTrigger(uuid, new Vector3(px, py, pz), new Vector3(rx, ry, rz));
                    TriggerMgr.ModifyIcon(uuid, icon);
                    TriggerMgr.ModifyColor(uuid, r, g, b, a);
                    TriggerMgr.AdjustSightTrigger(uuid, new Vector3(px, py, pz), new Vector3(rx, ry, rz));
                    slate.RegisterTrigger(uuid);
                }
            }
        }

        private static string takeBeanJsonFromData(byte[] _data, out int _size)
        {
            _size = bytesToInt(_data, 0);
            Log.Info("BeanMgr", "size of Bean.json is {0}", _size);
            return bytesToString(_data, 4, _size);
        }

        private static int bytesToInt(byte[] _bytes, int _startIndex)
        {
            int addr = _bytes[_startIndex + 0] ;
            addr |= (_bytes[_startIndex + 1] << 8);
            addr |= (_bytes[_startIndex + 2] << 16);
            addr |= (_bytes[_startIndex + 3] << 24);
            return addr;
        }

        private static string bytesToString(byte[] _bytes, int _startIndex, int _size)
        {
            string text = System.Text.UTF8Encoding.UTF8.GetString(_bytes, _startIndex, _size);
            return text;
        }
    }
}