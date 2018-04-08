/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using System.Collections.Generic;
using Fangs.Lib.Core;
using Fangs.Lib.Logger;

namespace VRXX.Manager
{
    public static class MixerMgr
    {
        public const int TRACK_BGM = 0;
        private static Transform root_ { get; set; }

        private static Dictionary<int, AudioSource> pool2D = new Dictionary<int, AudioSource>(0);
        private static Dictionary<int, AudioSource> pool3D = new Dictionary<int, AudioSource>(0);

        public static void Preload()
        {
            root_ = new GameObject("__Mixer__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            root_.gameObject.AddComponent<AudioListener>();
            GameObject tracks2D = new GameObject("2D");
            tracks2D.transform.SetParent(root_);
            for (int i = 0; i < 10; ++i)
            {
                GameObject go = new GameObject(i.ToString());
                go.transform.SetParent(tracks2D.transform);
                AudioSource audioSource = go.AddComponent<AudioSource>();
                pool2D.Add(i, audioSource);
            }
            GameObject tracks3D = new GameObject("3D");
            tracks3D.transform.SetParent(root_);
            for (int i = 0; i < 10; ++i)
            {
                GameObject go = new GameObject(i.ToString());
                go.transform.SetParent(tracks3D.transform);
                AudioSource audioSource = go.AddComponent<AudioSource>();
                pool3D.Add(i, audioSource);
            }
            Log.Info("MixerMgr", "Preload MixerMgr finish ... ");
        }

        public static int FindSpare2dSound()
        {
            foreach (int track in pool2D.Keys)
            {
                if (!pool2D[track].gameObject.activeInHierarchy)
                    return track;
            }
            return -1;
        }

        public static Error Play2dSound(int _track, AudioClip _clip)
        {
            AudioSource source = null;
            if (!pool2D.TryGetValue(_track, out source))
            {
                return AppError.NewAccessErr("2D Audio Source {0} is not exists", _track);
            }
            source.gameObject.SetActive(true);
            source.clip = _clip;
            source.Play();
            return AppError.OK;
        }

        public static float Get2DTrackElapsed(int _track)
        {
            if (pool2D.ContainsKey(_track))
                return pool2D[_track].time;
            return 0f;
        }

        public static Error Loop2dSound(int _track)
        {
            AudioSource source = null;
            if (!pool2D.TryGetValue(_track, out source))
            {
                return AppError.NewAccessErr("2D Audio Source {0} is not exists", _track);
            }
            source.loop = true;
            return AppError.OK;
        }

        public static Error Stop2dSound(int _track)
        {
            AudioSource source = null;
            if (!pool2D.TryGetValue(_track, out source))
            {
                return AppError.NewAccessErr("2D Audio Source {0} is not exists", _track);
            }
            source.Stop();
            source.gameObject.SetActive(false);
            return AppError.OK;
        }

        public static void StopAll2dSound()
        {
           foreach(AudioSource source in pool2D.Values)
            {
                source.Stop();
            }
        }

        public static int FindSpare3dSound()
        {
            foreach (int track in pool3D.Keys)
            {
                if (!pool3D[track].gameObject.activeInHierarchy)
                    return track;
            }
            return -1;
        }

        public static Error Play3dSound(Vector3 _position, int _track, AudioClip _clip)
        {
            AudioSource source = null;
            if (!pool3D.TryGetValue(_track, out source))
            {
                return AppError.NewAccessErr("3D Audio Source {0} is not exists", _track);
            }
            source.gameObject.SetActive(true);
            source.transform.position = _position;
            source.clip = _clip;
            source.Play();
            return AppError.OK;
        }

        public static Error Loop3dSound(int _track)
        {
            AudioSource source = null;
            if (!pool3D.TryGetValue(_track, out source))
            {
                return Error.NewAccessErr("3D Audio Source {0} is not exists", _track);
            }
            source.loop = true;
            return AppError.OK;
        }

        public static Error Stop3dSound(int _track)
        {
            AudioSource source = null;
            if (!pool3D.TryGetValue(_track, out source))
            {
                return Error.NewAccessErr("3D Audio Source {0} is not exists", _track);
            }
            source.Stop();
            source.gameObject.SetActive(false);
            return AppError.OK;
        }

        public static void StopAll3dSound()
        {
            foreach (AudioSource source in pool3D.Values)
            {
                source.Stop();
            }
        }


    }//class
}//namespace

