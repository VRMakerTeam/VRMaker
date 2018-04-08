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


namespace VRXX.Manager
{
    public static class LightMgr
    {
        private static Transform root_ { get; set; }
        private static Transform light_ { get; set; }

        // key is uuid
        private static Dictionary<string, Light> directionalLights = new Dictionary<string, Light>(0);
        private static Dictionary<string, Light> pointLights = new Dictionary<string, Light>(0);
        private static Dictionary<string, Light> spotLights = new Dictionary<string, Light>(0);


        public static void Preload()
        {
            root_ = new GameObject("__LightMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("BeanMgr", "Preload BeanMgr finish");
        }

        public static string NewDirectionalLight()
        {
            return newLight(LightType.Directional);
        }

        public static void NewDirectionalLight(string _uuid)
        {
            newLight(LightType.Directional, _uuid);
        }

        public static string NewPointLight()
        {
            return newLight(LightType.Point);
        }
        public static string NewSpotLight()
        {
            return newLight(LightType.Spot);
        }

        public static Light FindDirectionalLight(string _uuid)
        {
            Light light = null;
            directionalLights.TryGetValue(_uuid, out light);
            return light;
        }

        public static void UnloadDirectionalLight(string _uuid)
        {
            if (directionalLights.ContainsKey(_uuid))
            {
                GameObject obj = directionalLights[_uuid].gameObject;
                directionalLights.Remove(_uuid);
                GameObject.Destroy(obj);
            }
        }

        public static void ModifyDirectionalLight(string _uuid, Color _color, float _intensity, Vector3 _rotation)
        {
            Light light = null;
            if (!directionalLights.TryGetValue(_uuid, out light))
            {
                Log.Debug("LightMgr", "DirectionalLight [{0}] is not exist...", _uuid);
                return;
            }

            light.color = _color;
            light.intensity = _intensity;
            light.transform.Rotate(Vector3.zero);
            light.transform.Rotate(_rotation);
        }

        public static void ModifyPointLight(string _uuid, Color _color, float _intensity, float _range, Vector3 _position)
        {
            Light light = null;
            if (!pointLights.TryGetValue(_uuid, out light))
            {
                Log.Debug("LightMgr", "PointLight [{0}] is not exist...", _uuid);
                return;
            }
            light.color = _color;
            light.intensity = _intensity;
            light.range = _range;
            light.transform.position = _position;
        }

        public static void ModifySpotLight(string _uuid, Color _color, float _intensity, float _range, float _angle, Vector3 _position, Vector3 _rotation)
        {
            Light light = null;
            if (!spotLights.TryGetValue(_uuid, out light))
            {
                Log.Debug("LightMgr", "SpotLight [{0}] is not exist...", _uuid);
                return;
            }

            light.color = _color;
            light.intensity = _intensity;
            light.range = _range;
            light.spotAngle = _angle;
            light.transform.Rotate(Vector3.zero);
            light.transform.Rotate(_rotation);
            light.transform.position = _position;
        }

        private static string newLight(LightType  _type)
        {
            string uuid = generateUUID();
            GameObject go = new GameObject(uuid);
            Light light = go.AddComponent<Light>();
            go.transform.SetParent(root_);
            light.type = _type;
            if(LightType.Directional == _type)
            {
                useDefaultDirectionalLight(light);
                directionalLights.Add(uuid, light);
            }
            else if (LightType.Point == _type)
            {
                //light.Li
                pointLights.Add(uuid, light);
            }
            else if (LightType.Spot == _type)
            {
                spotLights.Add(uuid, light);
            }
            
            return uuid;
        }

        private static void newLight(LightType _type, string _uuid)
        {
            string uuid = _uuid;
            GameObject go = new GameObject(uuid);
            Light light = go.AddComponent<Light>();
            go.transform.SetParent(root_);
            light.type = _type;
            if (LightType.Directional == _type)
            {
                useDefaultDirectionalLight(light);
                directionalLights.Add(uuid, light);
            }
            else if (LightType.Point == _type)
            {
                //light.Li
                pointLights.Add(uuid, light);
            }
            else if (LightType.Spot == _type)
            {
                spotLights.Add(uuid, light);
            }
        }

        private static void useDefaultDirectionalLight(Light _light)
        {
            _light.color = new Color(255, 255, 255, 255);
            _light.intensity = 1;
            _light.transform.Rotate(Vector3.zero);
            _light.transform.Rotate(new Vector3(50, -30, 0));
        }

        private static string generateUUID()
        {
            string uuid = System.Guid.NewGuid().ToString();
            return uuid;
        }
    }
}
