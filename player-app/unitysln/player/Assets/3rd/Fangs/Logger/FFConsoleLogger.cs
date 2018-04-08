/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

using UnityEngine;

namespace Fangs.Lib.Logger
{
	public class ConsoleLogger : ILogger
	{
		public void LogTrace (string _category, string _message)
		{
			Debug.Log (string.Format ("{2:F3} [{0}] TRACE - {1}", _category, _message, Time.time));
		}
		public void LogDebug (string _category, string _message)
		{
			Debug.Log (string.Format ("{2:F3} [{0}] DEBUG - {1}", _category, _message, Time.time));
		}
		public void LogInfo (string _category, string _message)
		{
			Debug.Log (string.Format ("{2:F3} [{0}] INFO - {1}", _category, _message, Time.time));
		}
		public void LogWarning (string _category, string _message)
		{
			Debug.LogWarning (string.Format ("{2:F3} [{0}] WARNING - {1}", _category, _message, Time.time));
		}
		public void LogError (string _category, string _message)
		{
			Debug.LogError (string.Format ("{2:F3} [{0}] ERROR - {1}", _category, _message, Time.time));
		}
		public void LogException (string _category, System.Exception _exception)
		{
			Debug.LogException (new System.Exception(string.Format ("{2:F3} [{0}] EXCEPTION - {1}!!!", _category, _exception.Message,Time.time)));
		}

		public void eventloop ()
		{
		}
	}//class
}//namespace
