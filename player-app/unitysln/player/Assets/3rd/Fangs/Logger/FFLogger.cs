/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

namespace Fangs.Lib.Logger
{
	public static class Log
	{
		public enum LogLevel
		{
			ALL,
			TRACE,
			DEBUG,
			INFO,
			WARNING,
			ERROR,
			EXCEPTION,
			NONE
		}

		private static LogLevel level_ = LogLevel.ALL;
		private static ILogger implement_ = null;
        private static bool useStackTraceGetInfo = false;


        public static string type { get; set; }

        public static void UseStackTraceGetInfo()
        {
            useStackTraceGetInfo = true;
        }


        public static ILogger implement {
			set {
				implement_ = value;
				if (implement_ is ConsoleLogger)
					type = "console";
				else if (implement_ is UniGuiLogger)
					type = "gui";
				else 
					type = "unkonw";
			}
			private get {
				if (null == implement_) {
					implement = new ConsoleLogger ();
					UnityEngine.Debug.Log ("Logger is working with " + implement_.ToString ());
				}
				return implement_;
			}
		}

		public static LogLevel level {
			set {
				level_ = value;
			}

			get {
				return level_;
			}
		}

		public static void EventLoop ()
		{
			implement_.eventloop ();
		}

		#region 

		public static void Trace (string _class, object _obj)
		{
			trace (_class, _obj.ToString ());
		}

		public static void Debug (string _class, object _obj)
		{
			debug (_class, _obj.ToString ());
		}

		public static void Info (string _class, object _obj)
		{
			info (_class, _obj.ToString ());
		}

		public static void Warning (string _class, object _obj)
		{
			warning (_class, _obj.ToString ());
		}

		public static void Error (string _class, object _obj)
		{
			error (_class, _obj.ToString ());
		}

		public static void Trace (string _class, string _message, params object[] _args)
		{
			trace (_class, _message, _args);
		}

		public static void Debug (string _class, string _message, params object[] _args)
		{
			debug (_class, _message, _args);
		}

		public static void Info (string _class, string _message, params object[] _args)
		{
			info (_class, _message, _args);
		}

		public static void Warning (string _class, string _message, params object[] _args)
		{
			warning (_class, _message, _args);
		}

		public static void Error (string _class, string _message, params object[] _args)
		{
			error (_class, _message, _args);
		}
		
		public static void Exception (string _class, System.Exception _exception)
		{
			exception (_class, _exception);
		}
		#endregion

		#region

		public static void LogTrace (this object _self, object _obj)
		{
			trace (_self.ToString (), _obj.ToString ());
		}

		public static void LogDebug (this object _self, object _obj)
		{
			debug (_self.ToString (), _obj.ToString ());
		}

		public static void LogInfo (this object _self, object _obj)
		{
			info (_self.ToString (), _obj.ToString ());
		}

		public static void LogWarning (this object _self, object _obj)
		{
			warning (_self.ToString (), _obj.ToString ());
		}

		public static void LogError (this object _self, object _obj)
		{
			error (_self.ToString (), _obj.ToString ());
		}

		public static void LogTrace (this object _self, string _message, params object[] _args)
		{
			trace (_self.ToString (), _message, _args);
		}

		public static void LogDebug (this object _self, string _message, params object[] _args)
		{
			debug (_self.ToString (), _message, _args);
		}

		public static void LogInfo (this object _self, string _message, params object[] _args)
		{
			info (_self.ToString (), _message, _args);
		}

		public static void LogWarning (this object _self, string _message, params object[] _args)
		{
			warning (_self.ToString (), _message, _args);
		}

		public static void LogError (this object _self, string _message, params object[] _args)
		{
			error (_self.ToString (), _message, _args);
		}

		public static void LogException (this object _self, System.Exception _exception)
		{
			exception (_self.ToString (), _exception);
		}
		#endregion

        #region
		private static void trace (string _class, string _message, params object[] _args)
		{
			if (null == implement || level > LogLevel.TRACE)
				return;
			string message = string.Format (_message, _args);
			string categoray = string.Format ("{0}::{1}", _class, getMethod());
			implement.LogTrace (categoray, message);
		}

		private static void debug (string _class, string _message, params object[] _args)
		{
			if (null == implement || level > LogLevel.DEBUG)
				return;
			string message = string.Format (_message, _args);
			string categoray = string.Format ("{0}::{1}", _class, getMethod());
            implement.LogDebug(categoray, message);
        }

		private static void info (string _class, string _message, params object[] _args)
		{
			if (null == implement || level > LogLevel.INFO)
				return;
			string message = string.Format (_message, _args);
			string categoray = string.Format ("{0}::{1}", _class, getMethod());
            implement.LogInfo(categoray, message);
        }

		private static void warning (string _class, string _message, params object[] _args)
		{
			if (null == implement || level > LogLevel.WARNING)
				return;
			string message = string.Format (_message, _args);
			string categoray = string.Format ("{0}::{1}", _class, getMethod());
            implement.LogWarning(categoray, message);
        }

		private static void error (string _class, string _message, params object[] _args)
		{
			if (null == implement || level > LogLevel.ERROR)
				return;
			string message = string.Format (_message, _args);
			string categoray = string.Format ("{0}::{1}", _class, getMethod());
            implement.LogError(categoray, message);
        }

		private static void exception (string _class, System.Exception _exception)
		{
			if (null == implement || level > LogLevel.EXCEPTION)
				return;
			string categoray = string.Format ("{0}::{1}", _class, getMethod());
            implement.LogException(categoray, _exception);
        }
        #endregion

        private static string getMethod()
        {
            if(useStackTraceGetInfo)
            {
                System.Diagnostics.StackFrame sf = (new System.Diagnostics.StackTrace()).GetFrame(3);
                return sf.GetMethod().Name;
            }
            return "";
        }
    }
}
