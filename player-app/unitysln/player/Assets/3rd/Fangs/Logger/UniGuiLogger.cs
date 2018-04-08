/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

using UnityEngine;
using System.Collections.Generic;
using System;

namespace Fangs.Lib.Logger
{
	public class UniGuiLogger : ILogger
	{
		public int fontSize = 14;
		private List<string> msgs = new List<string> ();
		private Dictionary<int, string> stacktraces = new Dictionary<int, string> ();
		private Vector2 scrollPosition = Vector2.zero;
		private bool toggleLog = false;
		private bool togglePause = false;
		private bool toggleDetail = false;

		private Rect rectToggleLog = new Rect (0, 0, Screen.width * 0.05f, Screen.height * 0.05f);
		private Rect rectTogglePause = new Rect (Screen.width * 0.05f, 0, Screen.width * 0.05f, Screen.height * 0.05f);
		private Rect rectArea = new Rect (0, Screen.height * 0.05f, Screen.width, Screen.height * 0.5f);
		private Rect rectDetail = new Rect (0, Screen.height * 0.6f, Screen.width, Screen.height * 0.4f);
		private GUIStyle msgStyle;
		private GUIStyle warningStyle;
		private GUIStyle errorStyle;
		private string detail = "";

		public void eventloop ()
		{
			drawOverlay ();
		}

		public void LogTrace (string _category, string _message)
		{
			string msg = string.Format ("[{0}] {1:g}|TRACE - {2}", _category, DateTime.Now.TimeOfDay, _message);
			msgs.Add (msg);
		}
		public void LogDebug (string _category, string _message)
		{
			string msg = string.Format ("[{0}] {1:g}|DEBUG - {2}", _category, DateTime.Now.TimeOfDay, _message);
			msgs.Add (msg);
		}
		public void LogInfo (string _category, string _message)
		{
			string msg = string.Format ("[{0}] {1:g}|INFO - {2}", _category, DateTime.Now.TimeOfDay, _message);
			msgs.Add (msg);
		}
		public void LogWarning (string _category, string _message)
		{
			string msg = string.Format ("[{0}] {1:g}|WARNING - {2}", _category, DateTime.Now.TimeOfDay, _message);
			msgs.Add (msg);
		}
		public void LogError (string _category, string _message)
		{
			string msg = string.Format ("[{0}] {1:g}|ERROR - {2}", _category, DateTime.Now.TimeOfDay, _message);
			msgs.Add (msg);
		}
		public void LogException (string _category, System.Exception _exception)
		{
			string msg = string.Format ("[{0}] {1:g}|EXCEPTION - {2}", _category, DateTime.Now.TimeOfDay, _exception.Message);
			if (null != _exception.StackTrace)
				stacktraces.Add (msg.GetHashCode (), _exception.StackTrace);
			msgs.Add (msg);
		}


		public void drawOverlay ()
		{
			if (null == msgStyle) {
				msgStyle = new GUIStyle ();
				msgStyle.fontSize = fontSize;
				msgStyle.normal.textColor = Color.green;
			}
			if (null == warningStyle) {
				warningStyle = new GUIStyle (msgStyle);
				warningStyle.normal.textColor = Color.yellow;
			}

			if (null == errorStyle) {
				errorStyle = new GUIStyle (msgStyle);
				errorStyle.normal.textColor = Color.red;
			}


			if (toggleLog = GUI.Toggle (rectToggleLog, toggleLog, "log")) {
				togglePause = GUI.Toggle (rectTogglePause, togglePause, "pause");

				GUI.Box (rectArea, "");
				float scrollHeight = msgs.Count * fontSize;
				scrollPosition = GUI.BeginScrollView (rectArea, scrollPosition,
				                                     new Rect (0, 0, rectArea.width * 3f, scrollHeight));
				GUILayout.BeginVertical ();
				for (int i = 0; i < msgs.Count; ++i) {
					GUIStyle style = msgStyle;
					if (msgs [i].Contains ("ERROR") || msgs [i].Contains ("EXCEPTION"))
						style = errorStyle;
					else if (msgs [i].Contains ("WARNING"))
						style = warningStyle;
					if (GUILayout.Button (msgs [i], style)) {
						if (stacktraces.ContainsKey (msgs [i].GetHashCode ())) {
							toggleDetail = true;
							detail = stacktraces [msgs [i].GetHashCode ()];
						}
					}
				}
				GUILayout.EndVertical ();


				if (scrollHeight > rectArea.height && !togglePause)
					GUI.ScrollTo (new Rect (0, scrollHeight - rectArea.height, rectArea.width, rectArea.height));

				GUI.EndScrollView ();
			}

			if (toggleDetail) {
				GUI.Box (rectDetail, "");
				GUI.TextArea (rectDetail, detail);
				if (GUI.Button (new Rect (rectDetail.width - 32, rectDetail.y - 32, 32, 32), "X")) {
					toggleDetail = false;
				}
			}
		}
	}//class
}//namespace
