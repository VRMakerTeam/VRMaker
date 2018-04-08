/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using Fangs.Lib.Logger;

namespace VRXX.Manager
{
    public static class TriggerMgr
    {
        private delegate void CallBack();

        private static Transform root_ { get; set; }
        private static Transform sightRoot_ { get; set; }
        private static Dictionary<string, Transform> sightTriggers = new Dictionary<string, Transform>(0);

        public static void Preload()
        {
            root_ = new GameObject("__TriggerMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("TriggerMgr", "Preload TriggerMgr finish");

            sightRoot_ = new GameObject("sights").transform;
            sightRoot_.SetParent(root_);
            sightRoot_.transform.localPosition = Vector3.zero;
            sightRoot_.transform.localScale = Vector3.one;
            sightRoot_.transform.localRotation = Quaternion.identity;
        }

        public static void UpdateFrame()
        {
        }

        public static void Clean()
        {
            foreach (Transform trigger in sightTriggers.Values)
            {
                Object.Destroy(trigger.gameObject);
            }
            sightTriggers.Clear();
        }

        public static Transform NewEditorSightTrigger(string _uuid)
        {
            SightTrigger trigger = NewSightTrigger(_uuid);
            trigger.ToggleAlias(true);
            return trigger.transform;
        }

        public static void DeleteEditorSightTrigger(string _uuid)
        {
            DeleteSightTrigger(_uuid);
        }

        public static SightTrigger NewSightTrigger(string _uuid)
        {
            if (sightTriggers.ContainsKey(_uuid))
                return sightTriggers[_uuid].GetComponent<SightTrigger>();

            GameObject objTrigger = Resources.Load<GameObject>("trigger/sight_trigger");
            GameObject clone = Object.Instantiate(objTrigger);
            clone.name = _uuid;
            Transform trigger = clone.transform;
            sightTriggers.Add(_uuid, trigger);

            CanvasMgr.Attach3DChild(trigger);

            AdjustSightTrigger(_uuid);
            return trigger.GetComponent<SightTrigger>();
        }

        public static void DeleteSightTrigger(string _uuid)
        {
            if (!sightTriggers.ContainsKey(_uuid))
                return;
            Transform trigger = sightTriggers[_uuid];
            CanvasMgr.Detach3DChild(trigger);
            Object.Destroy(trigger.gameObject);
            sightTriggers.Remove(_uuid);
        }

        public static void ModifyGazeAlias(string _uuid, string _alias)
        {
            if (!sightTriggers.ContainsKey(_uuid))
                return;
            SightTrigger trigger = sightTriggers[_uuid].GetComponent<SightTrigger>();
            trigger.UpdateAlias(_alias);
        }

         
        public static void ModifyGazeIcon(string _uuid, int _icon)
        {
            if (!sightTriggers.ContainsKey(_uuid))
                return;
            SightTrigger trigger = sightTriggers[_uuid].GetComponent<SightTrigger>();
            trigger.UpdateIcon(_icon);
        }

        public static void ModifyGazeColor(string _uuid, int _r, int _g, int _b, int _a)
        {
            if (!sightTriggers.ContainsKey(_uuid))
                return;
            SightTrigger trigger = sightTriggers[_uuid].GetComponent<SightTrigger>();
            trigger.UpdateColor(_r, _g, _b, _a);
        }

        public static void ToggleTrigger(string _uuid, bool _visible)
        {
            if (!sightTriggers.ContainsKey(_uuid))
                return;

            sightTriggers[_uuid].gameObject.SetActive(_visible);
        }

        public static void ToggleAllTrigger(bool _visible)
        {
            foreach(Transform trigger in sightTriggers.Values)
            {
                trigger.gameObject.SetActive(_visible);
            }
        }

        public static void AdjustSightTrigger(string _uuid)
        {
            if (!sightTriggers.ContainsKey(_uuid))
                return;

            Transform trigger = sightTriggers[_uuid];
            trigger.localPosition = CameraMgr.forward * 6;
            trigger.LookAt(sightRoot_);
            trigger.Rotate(new Vector3(0, 180, 0));
        }

        public static void AdjustSightTrigger(string _uuid, Vector3 _position, Vector3 _rotation)
        {
            if (!sightTriggers.ContainsKey(_uuid))
                return;

            Transform trigger = sightTriggers[_uuid];
            trigger.localPosition = _position;
            trigger.LookAt(sightRoot_);
            trigger.Rotate(new Vector3(0, 180, 0));
        }

        public static void ModifyIcon(string _uuid, int _icon)
        {
            if (!sightTriggers.ContainsKey(_uuid))
                return;

            Transform trigger = sightTriggers[_uuid];
            SightTrigger cpt = trigger.GetComponent<SightTrigger>();
            if (null == cpt)
                return;

            cpt.UpdateIcon(_icon);
        }

        public static void ModifyColor(string _uuid, int _r, int _g, int _b, int _a)
        {
            if (!sightTriggers.ContainsKey(_uuid))
                return;

            Transform trigger = sightTriggers[_uuid];
            SightTrigger cpt = trigger.GetComponent<SightTrigger>();
            if (null == cpt)
                return;

            cpt.UpdateColor(_r, _g, _b, _a);
        }

        public static void FocusSightTrigger(string _uuid)
        {
            if (!sightTriggers.ContainsKey(_uuid))
                return;

            CameraMgr.LookAt(sightTriggers[_uuid]);
        }

        public static Vector3 QueryTriggerPosition(string _uuid)
        {
            if (!sightTriggers.ContainsKey(_uuid))
                return Vector3.zero;

            return sightTriggers[_uuid].localPosition;
        }

        public static Vector3 QueryTriggerRotation(string _uuid)
        {
            if (!sightTriggers.ContainsKey(_uuid))
                return Vector3.zero;

            return sightTriggers[_uuid].localRotation.eulerAngles;
        }

    }
}