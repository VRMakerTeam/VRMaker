/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using VRXX.Manager;

namespace VRXX.Action.Sound
{
    public static class Play2DUGC
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string cache_asset = "";
            if (!_params.TryGetValue("cache_asset", out cache_asset))
            {
                Log.Error("Play2D", "need params cache_asset");
                return;
            }

            string track = "";
            if (!_params.TryGetValue("track", out track))
            {
                Log.Error("Play2D", "need params track");
                return;
            }

            int track_ = int.Parse(track);

            try
            {
                byte[] bytes = UGCMgr.Take(cache_asset);
                AudioClip clip = UGC.Audio.WAVUtil.BuildAudioClip(bytes);
                CoroutineMgr.Start(play2DSound(clip, track_, _onFinish));
            }
            catch (System.Exception e)
            {
                Log.Error("Play2dSound", "Parse json hsa error:" + e.Message);
            }
        }

        private static IEnumerator play2DSound(AudioClip _ac, int _track, ActionDelegate _onFinish)
        {
            MixerMgr.Play2dSound(_track, _ac);
            while (true)
            {
                yield return new WaitForEndOfFrame();
                float time = MixerMgr.Get2DTrackElapsed(_track);
                if (time >= _ac.length - 0.1)
                {
                    _onFinish();
                    break;
                }
            }
        }


        //SVMAPI
        public static void Play(string _cache_asset, int _track, SVMAPI.FinishDelegate _onFinish)
        {
            try
            {
                byte[] bytes = UGCMgr.Take(_cache_asset);
                AudioClip clip = UGC.Audio.WAVUtil.BuildAudioClip(bytes);
                CoroutineMgr.Start(play2DSound_SVMAPI(clip, _track, _onFinish));
            }
            catch (System.Exception e)
            {
                Log.Error("Play2dSound", "Parse json hsa error:" + e.Message);
            }
        }

        private static IEnumerator play2DSound_SVMAPI(AudioClip _ac, int _track, SVMAPI.FinishDelegate _onFinish)
        {
            MixerMgr.Play2dSound(_track, _ac);
            while (true)
            {
                yield return new WaitForEndOfFrame();
                float time = MixerMgr.Get2DTrackElapsed(_track);
                if (time >= _ac.length - 0.1)
                {
                    _onFinish();
                    break;
                }
            }
        }

    }//class
}//namespace

