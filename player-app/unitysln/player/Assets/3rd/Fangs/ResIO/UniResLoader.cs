/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

using UnityEngine;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Logger;

namespace Fangs.Lib.ResIO
{
	public class UniResOperation : IResOpeartion
	{
		private enum Owner
		{
			NONE,
			WW,
			BR
		}
		private WWW www_ = null;
		private AssetBundleRequest req_ = null;
		private Owner owner_ = Owner.NONE;
		private float progress_ = 0f;

		public void Reset ()
		{
			Use (0.0f);
		}
		
		public float progress {
			get {
				if (Owner.BR == owner_)
					progress_ = req_.progress;
				else if (Owner.WW == owner_)
					progress_ = www_.progress;
				return progress_;
			}
		}

		internal void Use (WWW _www)
		{
			www_ = _www;
			owner_ = Owner.WW;
			progress_ = 0f;
		}

		internal void Use (AssetBundleRequest _req)
		{
			req_ = _req;
			owner_ = Owner.BR;
			progress_ = 0f;
		}

		internal void Use (float _progress)
		{
			owner_ = Owner.NONE;
			progress_ = _progress;
		}
	}

	public class UniResLoader : IResLoader
	{
		private MonoBehaviour mono = null;
		private Dictionary<string, AssetBundle> bundles = new Dictionary<string, AssetBundle> (0);
		// default path is streamingAssetsPath, 
		// external path is persistentDataPath
		private List<string> externalBundles = new List< string> (0); 
		public UniResLoader (MonoBehaviour _mono)
		{
			mono = _mono;
		}

		public T Load<T> (string _bundle, string _res) where T : Object
		{
			throw new System.NotImplementedException ("NotImplementedException");
		}

		public void AppendExternalBundle (string _bundle)
		{
			if (externalBundles.Contains (_bundle))
				return;
			externalBundles.Add (_bundle);
		}

		public void RemoveExternalBundle (string _bundle)
		{
			if (!externalBundles.Contains (_bundle))
				return;
			externalBundles.Remove (_bundle);
		}

		public IResOpeartion AsyncLoadBundle (string _bundle, OnLoadBundleFinishDelegate _finish)
		{
			UniResOperation op = new UniResOperation ();
			if (!bundles.ContainsKey (_bundle)) {
				this.LogDebug ("Load Bundle {0} ... ", _bundle);
				mono.StartCoroutine (asyncLoadBundle (op, _bundle, (_ab) =>
				{
					this.LogDebug ("Load Bundle {0} finish", _bundle);
					_finish (_ab);
				}));
			} else {
				op.Use (1.0f);
				this.LogDebug ("Load exits Bundle {0}", _bundle);
				_finish (bundles [_bundle]);
			}
			return op;
		}

		public IResOpeartion AsyncLoadRes<T> (string _bundle, string _res, OnLoadResFinishDelegate<T> _finish) where T : Object
		{
			this.LogDebug ("Load res {0} from {1}... ", _res, _bundle);
			
			AssetBundle bundle = null;
			UniResOperation op = new UniResOperation ();
			if (bundles.TryGetValue (_bundle, out bundle)) {
				mono.StartCoroutine (asyncLoadRes<T> (op, bundle, _res, _finish));
			} else {
				this.LogError ("bundle {0} is not exists!", _bundle);
				op.Use (0.0f);
			}
			return op;
		}

		public void Closure ()
		{
			foreach (AssetBundle bundle in bundles.Values)
				bundle.Unload (false);
			bundles.Clear ();
			Resources.UnloadUnusedAssets ();
			System.GC.Collect ();
		}

		private IEnumerator asyncLoadBundle (UniResOperation _op, string _bundle, OnLoadBundleFinishDelegate _finish)
		{
			string dir = Application.streamingAssetsPath;
			if (externalBundles.Contains (_bundle))
				dir = Application.persistentDataPath;
            if(!dir.Contains("file://"))
                dir = dir.Insert(0, "file://");
			string path = System.IO.Path.Combine (dir, "res/" + _bundle);
			WWW www = new WWW (path);
			_op.Use (www);
			yield return www;
			if (null != www.error) {
				this.LogError (www.error);
				yield break;
			}
			AssetBundle bundle = www.assetBundle;
			if (null == bundle) {
				this.LogError ("bundle is null");
				yield break;
			}
			bundles.Add (_bundle, bundle);
			www.Dispose ();
			_op.Use (1.0f);
			_finish (bundle);
		}

		private IEnumerator asyncLoadRes<T> (UniResOperation _op, AssetBundle _bundle, string _res, OnLoadResFinishDelegate<T> _finish) where T : Object
		{
			AssetBundleRequest req = _bundle.LoadAssetAsync<T> (_res);
			_op.Use (req);
			yield return req;
			this.LogDebug ("Load res {0} finish", _res);
			_op.Use (1.0f);
			_finish (req.asset as T);
		}

	}//class 
}//namespace
