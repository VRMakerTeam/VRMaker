/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

using UnityEngine;
using System.Collections;

namespace Fangs.Lib.Logger
{
	public interface ILogger
	{
		void LogTrace (string _category, string _message);
		void LogDebug (string _category, string _message);
		void LogInfo (string _category, string _message);
		void LogWarning (string _category, string _message);
		void LogError (string _category, string _message);
		void LogException (string _category, System.Exception _exception);

		void eventloop ();
	}//interface
}//namespace