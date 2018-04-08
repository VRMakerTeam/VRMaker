/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Core;
using Fangs.Lib.Logger;
using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace Fangs.Lib.Net
{
	public class AsyncUdpClient
	{  
		public delegate void OnReceiveFinishCallback (byte[] _bytes);

		public static bool messageSent = false;  
		// Receive a message and write it to the console.  
 
		public IPEndPoint remoteEP = null;  
		public IPEndPoint localEP = null;  

		private UdpClient  udp = null;  

		public OnReceiveFinishCallback onFinish = null;
        
		public AsyncUdpClient ()
		{  
			udp = new UdpClient ();
		}  

		public void Close ()
		{
			udp.Close ();
		}

		public void Send (byte[] _bytes)
		{     
			udp.Connect (remoteEP);
			udp.BeginSend (_bytes, _bytes.Length, new AsyncCallback (SendCallback), null);  
		}  

		public void Receive (OnReceiveFinishCallback _onFinish)
		{  
			onFinish = _onFinish;
			udp.BeginReceive (new AsyncCallback (ReceiveCallback), null);  
		}  
		
		private void SendCallback (IAsyncResult iar)
		{  
			if (iar.IsCompleted) {  
				udp.EndSend (iar);
			}              
		}  

		private void ReceiveCallback (IAsyncResult iar)
		{  
			if (iar.IsCompleted) {  
				byte[] bytes = udp.EndReceive (iar, ref remoteEP);  
				onFinish (bytes);
			}              
		}  
	}//class  


	public class RPC
	{
		public class Secrecy
		{
			public ulong userID = 0;
			public ulong session = 0;
		}

		public class Request
		{
			public byte[] bytes { get; set; }
			public override string ToString ()
			{
				System.Text.StringBuilder sb = new System.Text.StringBuilder ();
				for (int i = 0; i < bytes.Length; ++i) {
					sb.AppendFormat ("{0} ", bytes [i]);
				}
				return sb.ToString ();
			}
		}//class
		
		public class Response
		{
			public byte[] bytes { get; set; }
			public override string ToString ()
			{
				System.Text.StringBuilder sb = new System.Text.StringBuilder ();
				for (int i = 0; i < bytes.Length; ++i) {
					sb.AppendFormat ("{0} ", bytes [i]);
				}
				return sb.ToString ();
			}
		}//class

		private class Procedure
		{
			public Request req = null;
			public FinishCallback finishCallback = null;
			public ErrorCallback errorCallback = null;
			public Response rep = null;
			public Error err = null;
			public AsyncUdpClient udp = null;
		}
		
		public delegate void FinishCallback (Response _response);
		public delegate void ErrorCallback (Error _err);

		public static RPC instance {
			get {
				if (null == instance_)
					instance_ = new RPC ();
				return instance_;
			}
		}

		public MonoBehaviour mono = null;
		public Secrecy secrecy = new Secrecy ();

		private static RPC instance_ = null;
		private IPEndPoint remoteEP = null;  
		private List<Procedure> procedures = new List<Procedure> (0);
		private List<Procedure> finishedProcedure = new List<Procedure> (0);

		public void InitUDP (string _service)
		{
			string[] host = _service.Split (':');
			remoteEP = new IPEndPoint (Dns.GetHostAddresses (host [0]) [0], int.Parse (host [1])); 
			this.LogDebug ("remote is {0}", remoteEP.ToString ());
			mono.StartCoroutine (loop ());
		}

		public void Invoke (Request _request, FinishCallback _finish, ErrorCallback _err)
		{
			invoke (_request, _finish, _err);
		}

		public byte[] EncodeRequest (uint _msgid, byte[] _bytes)
		{
			return encodeRequest (_msgid, _bytes);
		}

		public static uint PeekMsgID (Response _rep)
		{
			return peekMsgID (_rep);
		}

		public static uint PeekErrcode (Response _rep)
		{
			return peekErrcode (_rep);
		}

		private void invoke (Request _request, FinishCallback _finish, ErrorCallback _err)
		{
			if (null == _request) {
				this.LogError (Error.NewNullErr ("_request is null"));
				return;
			}

			if (null == _request.bytes) {
				this.LogError (Error.NewNullErr ("_request.bytes is null"));
				return;
			}

			if (null == _finish) {
				this.LogError (Error.NewNullErr ("_finish is null"));
				return;
			}

			if (null == _err) {
				this.LogError (Error.NewNullErr ("_err is null"));
				return;
			}

			Procedure procedure = new Procedure ();
			procedure.finishCallback = _finish;
			procedure.errorCallback = _err;
			procedure.req = _request;
			procedure.udp = new AsyncUdpClient ();
			procedure.udp.remoteEP = remoteEP;
			procedures.Add (procedure);
			try {

				procedure.udp.Send (_request.bytes);
				procedure.udp.Receive ((_bytes) => { 
					procedure.rep = new Response ();
					procedure.rep.bytes = _bytes;
				});

			} catch (System.Exception e) {
				this.LogException (e);
				procedure.err = Error.NewAccessErr (e.Message);
			}
		}

		
		private IEnumerator loop ()
		{
			while (true) {
				finishedProcedure = new List<Procedure> ();
				for (int i = 0; i < procedures.Count; ++i) {
					if (null != procedures [i].err) {
						procedures [i].errorCallback (procedures [i].err);
						finishedProcedure.Add (procedures [i]);
					} else if (null != procedures [i].rep) {
						procedures [i].finishCallback (procedures [i].rep);
						finishedProcedure.Add (procedures [i]);
					}
				}
				for (int i = 0; i < finishedProcedure.Count; ++i) {
					finishedProcedure [i].udp.Close ();
					procedures.Remove (finishedProcedure [i]);
				}
				finishedProcedure.Clear ();
				yield return 1;
			}
		}

		private byte[] encodeRequest (uint _msgID, byte[] _bytes)
		{
			//add sizeof(ulong)*2
			int head = 20;

			byte[] buffer = _bytes;
			if (buffer == null)
				buffer = new byte[head];
			else
				buffer = new byte[_bytes.Length + head];

			//add userID
			buffer [0] = (byte)(secrecy.userID);  
			buffer [1] = (byte)(secrecy.userID >> 8);  
			buffer [2] = (byte)(secrecy.userID >> 16);  
			buffer [3] = (byte)(secrecy.userID >> 24);  
			buffer [4] = (byte)(secrecy.userID >> 32);  
			buffer [5] = (byte)(secrecy.userID >> 40);  
			buffer [6] = (byte)(secrecy.userID >> 48);  
			buffer [7] = (byte)(secrecy.userID >> 56);  
 
			//add userID
			buffer [8] = (byte)(secrecy.session);  
			buffer [9] = (byte)(secrecy.session >> 8);  
			buffer [10] = (byte)(secrecy.session >> 16);  
			buffer [11] = (byte)(secrecy.session >> 24);  
			buffer [12] = (byte)(secrecy.session >> 32);  
			buffer [13] = (byte)(secrecy.session >> 40);  
			buffer [14] = (byte)(secrecy.session >> 48);  
			buffer [15] = (byte)(secrecy.session >> 56);  

			//add msgid
			buffer [16] = (byte)(_msgID);  
			buffer [17] = (byte)(_msgID >> 8);  
			buffer [18] = (byte)(_msgID >> 16);  
			buffer [19] = (byte)(_msgID >> 24);  

			if (_bytes != null)
				Array.Copy (_bytes, 0, buffer, head, _bytes.Length);
			return buffer;
		}

		private static uint peekMsgID (Response _response)
		{
			uint msgid = 0;
			msgid |= (uint)_response.bytes [0];
			msgid |= ((uint)_response.bytes [1]) << 8;
			msgid |= ((uint)_response.bytes [2]) << 16;
			msgid |= ((uint)_response.bytes [3]) << 32;
			return msgid;
		}

		private static uint peekErrcode (Response _response)
		{
			uint errcode = 0;
			errcode |= ((uint)_response.bytes [4]);
			errcode |= ((uint)_response.bytes [5]) << 8;
			errcode |= ((uint)_response.bytes [6]) << 16;
			errcode |= ((uint)_response.bytes [7]) << 32;
			return errcode;
		}

		//private 
	}//class
}//namespace
