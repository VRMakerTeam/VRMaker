/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

using UnityEngine;

namespace Fangs.Lib.Logger
{
	public class UniSocketLogger : ILogger
	{
		public void eventloop ()
		{
		}
		public void LogTrace (string _category, string _message)
		{
			Debug.Log (string.Format ("[{0}] TRACE - {1}", _category, _message));
		}
		public void LogDebug (string _category, string _message)
		{
			Debug.Log (string.Format ("[{0}] DEBUG - {1}", _category, _message));
		}
		public void LogInfo (string _category, string _message)
		{
			Debug.Log (string.Format ("[{0}] INFO - {1}", _category, _message));
		}
		public void LogWarning (string _category, string _message)
		{
			Debug.LogWarning (string.Format ("[{0}] WARNING - {1}", _category, _message));
		}
		public void LogError (string _category, string _message)
		{
			Debug.LogError (string.Format ("[{0}] ERROR - {1}", _category, _message));
		}
		public void LogException (string _category, System.Exception _exception)
		{
			Debug.LogError (string.Format ("[{0}] EXCEPTION - !!!", _category));
			Debug.LogException (_exception);
		}
	}//class
}//namespace