/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

using UnityEngine;

namespace Fangs.Lib.ResIO
{
	public delegate void OnLoadBundleFinishDelegate (AssetBundle _ab);
	public delegate void OnLoadResFinishDelegate<T> (T _res) where T : Object;

	public interface IResOpeartion
	{
		float progress { get; }
		void Reset ();
	}

	public interface IResLoader
	{
		T Load<T> (string _bundle, string _res) where T : Object;
		IResOpeartion AsyncLoadBundle (string _bundle, OnLoadBundleFinishDelegate _finish) ;
		IResOpeartion AsyncLoadRes<T> (string _bundle, string _res, OnLoadResFinishDelegate<T> _finish) where T : Object;

		/// <summary>
		/// Unload unused resources and collect garbage
		/// </summary>
		void Closure ();
	}//class 
}//namespace
