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
    public static class Play2D
    {
        public static void Execute(Dictionary<string, string> _params, ActionDelegate _onFinish)
        {
            string package = "";
            if (!_params.TryGetValue("package", out package))
            {
                Log.Error("Play2D", "need params package");
                return;
            }

            string file = "";
            if (!_params.TryGetValue("file", out file))
            {
                Log.Error("Play2D", "need params file");
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
                ResourceMgr.LoadAudioClip(package, file, track_
                               , () =>
                               {
                                   Log.Debug("Play2D", "ready");
                               }
                               , (_audioclip) =>
                               {
                                   AudioClip clip = _audioclip as AudioClip;
                                   CoroutineMgr.Start(play2DSound(clip, track_, _onFinish));
                               }
                               , (_error) =>
                               {
                                   Log.Error("Play2D", _error);
                               });
            }
            catch (System.Exception e)
            {
                Log.Error("Play2dSound", "Parse json hsa error:" + e.Message);
            }
        }

        private static IEnumerator play2DSound(AudioClip _ac, int _track, ActionDelegate _onFinish)
        {
            MixerMgr.Stop2dSound(_track);
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
        public static void Play(string _package, string _file, int _track, SVMAPI.FinishDelegate _onFinish)
        {
            try
            {
                ResourceMgr.LoadAudioClip(_package, _file, _track
                               , () =>
                               {
                                   Log.Debug("Play2D", "ready");
                               }
                               , (_audioclip) =>
                               {
                                   AudioClip clip = _audioclip as AudioClip;
                                   CoroutineMgr.Start(play2DSound_SVMAPI(clip, _track, _onFinish));
                               }
                               , (_error) =>
                               {
                                   Log.Error("Play2D", _error);
                               });
            }
            catch (System.Exception e)
            {
                Log.Error("Play2dSound", "Parse json hsa error:" + e.Message);
            }
        }

        private static IEnumerator play2DSound_SVMAPI(AudioClip _ac, int _track, SVMAPI.FinishDelegate _onFinish)
        {
            MixerMgr.Stop2dSound(_track);
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

