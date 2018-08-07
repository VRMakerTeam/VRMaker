/********************************************************************
     Copyright (c) zyncho.
     All rights reserved.

     author: Easlee Liu
*********************************************************************/
using UnityEngine;
using System.Collections.Generic;

namespace VRXX
{
    public static class Platform
    {
        /// <summary>
        /// 
        /// </summary>
        public static Dictionary<string, string> redirectPersistentDataPaths = new Dictionary<string, string>();

        public static string Alias
        {
            get
            {
                RuntimePlatform platform = Application.platform;
                if (RuntimePlatform.WindowsPlayer == platform)
                    return "Win32";
                if (RuntimePlatform.WindowsEditor == platform)
                    return "Win32";
                if (RuntimePlatform.WebGLPlayer == platform)
                    return "WebGL";
                if (RuntimePlatform.Android == platform)
                    return "Android";
                if (RuntimePlatform.IPhonePlayer == platform)
                    return "iOS";
                return "unknow";
            }
        }
      

        public static string GetStreamingAssetsPath()
        {
            string path = Application.streamingAssetsPath;
            RuntimePlatform platform = Application.platform;
            if (RuntimePlatform.WindowsPlayer == platform ||
                RuntimePlatform.WindowsEditor == platform)
            {
                path = "file://" + path;
            }
            return path;
        }

        public static string GetPersistentDataPath(bool isWrite = false)
        {
            string path = Application.persistentDataPath;
            if (!isWrite)
                path = "file:///" + path;

            if (RuntimePlatform.WebGLPlayer == Application.platform)
                path = Application.streamingAssetsPath;

            if (redirectPersistentDataPaths.ContainsKey(Alias))
                path = redirectPersistentDataPaths[Alias];

            return path;
        }

        public static void RedirectPersistentDataPath(string _platform, string _path)
        {
            redirectPersistentDataPaths[_platform] = _path;
        }
    }
}

