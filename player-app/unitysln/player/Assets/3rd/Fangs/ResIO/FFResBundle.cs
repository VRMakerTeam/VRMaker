/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

using UnityEngine;
using Fangs.Lib.Logger;
using System.Collections.Generic;

namespace Fangs.Lib.ResIO
{
	public static class ResBundle
	{
		private static IResLoader resLoaderImpl = null;

		public static void UseLoader (IResLoader _implement)
		{
			resLoaderImpl = _implement;
		}

		public static T Load<T> (string _bundle, string _res) where T : Object
		{
			if (null == resLoaderImpl) {
				Log.Error ("ResBundle", "resLoaderImpl is null");
				return null;
			}
			return resLoaderImpl.Load<T> (_bundle, _res);
		}

		public static IResOpeartion AsyncLoadBundle (string _bundle, OnLoadBundleFinishDelegate _finish)
		{
			if (null == resLoaderImpl) {
				Log.Error ("ResBundle", "resLoaderImpl is null");
				return null;
			}
			return resLoaderImpl.AsyncLoadBundle (_bundle, _finish);
		}

		public static IResOpeartion AsyncLoadRes<T> (string _bundle, string _res, OnLoadResFinishDelegate<T> _finish) where T : Object
		{
			if (null == resLoaderImpl) {
				Log.Error ("ResBundle", "resLoaderImpl is null");
				return null;
			}
			return resLoaderImpl.AsyncLoadRes<T> (_bundle, _res, _finish);
		}

		/// <summary>
		/// Unload unused resources and collect garbage
		/// </summary>
		public static void Closure ()
		{
			if (null == resLoaderImpl)
				Log.Error ("ResBundle", "resLoaderImpl is null");
			else
				resLoaderImpl.Closure ();
		}
	}//class 

	public static class ResFlow
	{
		public delegate void TaskInvokeDelegate ();
		private struct Task
		{
			public const int TYPE_BUNDLE = 1;
			public const int TYPE_RES = 2;
			public const int TYPE_INVOKE = 3;
			public string bundle;
			public string res;
			public int type;
			public OnLoadBundleFinishDelegate onBundleFinish;
			public OnLoadResFinishDelegate<Object> onResFinish;
			public TaskInvokeDelegate invoke;
		}
		
		public static IResOpeartion resOp { get; private set; }
		public delegate void OnFinishDelegate ();
		
		private  static Queue<Task> tasks = new Queue<Task> (0);
		
		public static void AddBundleTask (string _bundle, OnLoadBundleFinishDelegate _onFinish)
		{
			addBundleTask (_bundle, _onFinish);
		}

		public static void AddResTask (string _bundle, string _res, OnLoadResFinishDelegate<Object> _onFinish)
		{
			addResTask (_bundle, _res, _onFinish);
		}

		public static void AddInvokeTask (TaskInvokeDelegate _invoke)
		{
			addInvokeTask (_invoke);
		}
		
		public static void Execute (OnFinishDelegate _onFinish)
		{
			execute (_onFinish);
		}

		private static void addBundleTask (string _bundle, OnLoadBundleFinishDelegate _onFinish)
		{
			Task task = new Task ();
			task.bundle = _bundle;
			task.res = "";
			task.type = Task.TYPE_BUNDLE;
			task.onBundleFinish = _onFinish;
			tasks.Enqueue (task);
		}

		private static void addResTask (string _bundle, string _res, OnLoadResFinishDelegate<Object> _onFinish)
		{
			Task task = new Task ();
			task.bundle = _bundle;
			task.res = _res;
			task.type = Task.TYPE_RES;
			task.onResFinish = _onFinish;
			tasks.Enqueue (task);
		}

		private static void addInvokeTask (TaskInvokeDelegate _delegate)
		{
			Task task = new Task ();
			task.type = Task.TYPE_INVOKE;
			task.invoke = _delegate;
			tasks.Enqueue (task);
		}
		
		private static void execute (OnFinishDelegate _onFinish)
		{
			if (tasks.Count == 0) {
				if (null != _onFinish)
					_onFinish ();
				return;
			}
			
			Task task = tasks.Dequeue ();
			if (Task.TYPE_BUNDLE == task.type) {
				resOp = ResBundle.AsyncLoadBundle (task.bundle, (_ab) => {
					task.onBundleFinish (_ab);
					execute (_onFinish);
				});
			} else if (Task.TYPE_RES == task.type) {
				resOp = ResBundle.AsyncLoadRes<Object> (task.bundle, task.res, (_asset) =>
				{
					task.onResFinish (_asset);
					execute (_onFinish);
				});
			} else if (Task.TYPE_INVOKE == task.type) {
				task.invoke ();
				execute (_onFinish);
			}
		}
	}//class 
}//namespace
