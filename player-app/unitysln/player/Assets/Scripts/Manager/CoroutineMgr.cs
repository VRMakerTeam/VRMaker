/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using System.Collections;
using Fangs.Lib.Logger;

class CorotineCpt : MonoBehaviour
{

}

namespace VRXX.Manager
{
	public static class CoroutineMgr
	{
        public static MonoBehaviour mono { get; private set; }

        private static Transform root_ { get; set; }

		public static void Preload ()
		{
            root_ = new GameObject("__CoroutineMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            mono = root_.gameObject.AddComponent<CorotineCpt> ();
            Log.Info("CoroutineMgr", "Preload CoroutineMgr  finish ");
        }

        public static void Start (IEnumerator _routine)
		{
            mono.StartCoroutine (_routine);
		}

        public static void Stop(IEnumerator _routine)
        {
            mono.StopCoroutine(_routine);
        }

	}//class
}//namespace

