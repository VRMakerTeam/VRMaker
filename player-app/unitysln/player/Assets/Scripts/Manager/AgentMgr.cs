/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using Fangs.Lib.Core;

namespace VRXX.Manager
{
    public static class AgentMgr
    {
        public delegate void OnErrorDelegate(string _error);
        private static Transform root_ { get; set; }

        public static void Preload()
        {
            root_ = new GameObject("__AgentMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("AgentMgr", "Preload AgentMgr finish");
        }

        public static void Place(string _uuid, float _x, float _y, float _z, OnErrorDelegate _onError)
        {
            GameObject go = ResourceMgr.FindGameObject(_uuid);
            if(null == go)
            {
                _onError(string.Format("{0} is not exists !!!", _uuid));
                return;
            }
            go.transform.localPosition = (new Vector3(_x, _y, _z));
        }

        public static void Rotation(string _uuid, float _x, float _y, float _z, OnErrorDelegate _onError)
        {
            GameObject go = ResourceMgr.FindGameObject(_uuid);
            if (null == go)
            {
                _onError(string.Format("{0} is not exists !!!", _uuid));
                return;
            }
            go.transform.localEulerAngles = new Vector3(_x, _y, _z);
        }

        public static void Translate(string _uuid, float _x, float _y, float _z, OnErrorDelegate _onError)
        {
            GameObject go = ResourceMgr.FindGameObject(_uuid);
            if (null == go)
            {
                _onError(string.Format("{0} is not exists !!!", _uuid));
                return;
            }
            go.transform.Translate(new Vector3(_x, _y, _z));
        }

        public static void Rotate(string _uuid, float _x, float _y, float _z, OnErrorDelegate _onError)
        {
            GameObject go = ResourceMgr.FindGameObject(_uuid);
            if (null == go)
            {
                _onError(string.Format("{0} is not exists !!!", _uuid));
                return;
            }
            go.transform.Rotate(new Vector3(_x, _y, _z));
        }

        public static void Scale(string _uuid, float _x, float _y, float _z, OnErrorDelegate _onError)
        {
            GameObject go = ResourceMgr.FindGameObject(_uuid);
            if (null == go)
            {
                _onError(string.Format("{0} is not exists !!!", _uuid));
                return;
            }
            go.transform.localScale = new Vector3(_x, _y, _z);
        }

        // TO DO
        public static void SmoothFollow(string _selfUUID, string _targetUUID, float _height, float _heightDamping, float _rotationDamping,  OnErrorDelegate _onError)
        {
            GameObject self = ResourceMgr.FindGameObject(_selfUUID);
            if (null == self)
            {
                _onError(string.Format("{0} is not exists !!!", _selfUUID));
                return;
            }
            GameObject target = ResourceMgr.FindGameObject(_targetUUID);
            if (null == target)
            {
                _onError(string.Format("{0} is not exists !!!", _targetUUID));
                return;
            }


        }

    }//class
}//namespace

