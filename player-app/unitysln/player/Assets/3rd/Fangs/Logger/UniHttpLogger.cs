/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace Fangs.Lib.Logger
{
	public class UniHttpLogger : ILogger
	{
		public string serviceUrl;
		public MonoBehaviour shell;

		private Queue<string> msgQueue = new Queue<string> ();

		public void eventloop ()
		{
			while (msgQueue.Count > 0) {
				string msg = msgQueue.Dequeue ();
				shell.StartCoroutine (Post (msg));
			}
		}

		public void LogTrace (string _category, string _message)
		{
			string msg = string.Format ("[{0}] TRACE - {1}", _category, _message);
			AddTask (msg);
		}

		public void LogDebug (string _category, string _message)
		{
			string msg = string.Format ("[{0}] DEBUG - {1}", _category, _message);
			AddTask (msg);
		}

		public void LogInfo (string _category, string _message)
		{
			string msg = string.Format ("[{0}] INFO - {1}", _category, _message);
			AddTask (msg);
		}

		public void LogWarning (string _category, string _message)
		{
			string msg = string.Format ("[{0}] WARNING - {1}", _category, _message);
			AddTask (msg);
		}

		public void LogError (string _category, string _message)
		{
			string msg = string.Format ("[{0}] ERROR - {1}", _category, _message);
			AddTask (msg);
		}

		public void LogException (string _category, System.Exception _exception)
		{
			string msg = string.Format ("[{0}] EXCEPTION - {1}", _category, _exception.ToString ());
			AddTask (msg);
		}

		private void AddTask (string _msg)
		{
			msgQueue.Enqueue (_msg);
		}

		private IEnumerator Post (string _msg)
		{
			WWWForm form = new WWWForm ();
			form.AddField ("msg", _msg);
			WWW www = new WWW (serviceUrl, form);
			yield return www;
			if (null != www.error) {
				Debug.LogError ("[UniHttpLogger] ERROR - " + www.error);
			}
		}

	}//class
}//namespace