/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using Fangs.Lib.Core;

namespace Fangs.Lib.Net
{
    /// <summary>
    /// <code>
    /// Http.Operation operation =  http.AsyncGET (url, 
    ///     (_data) => {
    ///         // success
	///		}, (_err) => {
    ///		    // fail
	///	    }
    ///	);
    /// </code>
    /// </summary>
    public class Http
	{
        public class Agent
        {
            public WWW www { get; set; }
            public enum Status { IDLE, RUNNING, DONE}
            public Status state = Status.IDLE;
        }

        public class Operation
        {
            public Operation(Agent _agent)
            {
                agent_ = _agent;
            }

            public float progress
            {
                get
                {
                    if (Agent.Status.IDLE == agent_.state)
                        return 0f;
                    if (Agent.Status.DONE == agent_.state)
                        return 1f;
                    return agent_.www.progress;
                }
            }

            public void Abort()
            {
                if (null != agent_.www)
                {
                    agent_.www.Dispose();
                    agent_.www = null;
                    agent_.state = Agent.Status.IDLE;
                }
            }

            private Agent agent_{ get; set; }
        }


        public delegate void SuccessCallback (byte[] _data);
		public delegate void FailCallback (Error _err);
		public MonoBehaviour mono;

        public Operation AsyncGET (string _url, SuccessCallback _onSuccess, FailCallback _onFail)
		{
			return asyncGET (_url, _onSuccess, _onFail);
		}

        public Operation AsyncPOST(string _url, Dictionary<string, object> _params, SuccessCallback _onSuccess, FailCallback _onFail)
        {
            return asyncPOST(_url, _params, _onSuccess, _onFail);
        }

        private Operation asyncGET (string _url, SuccessCallback _onSuccess, FailCallback _onFail)
		{
			this.LogTrace (_url);
            Agent agent = new Agent();
            Operation operation = new Operation(agent);
            mono.StartCoroutine (get (_url, operation, agent, _onSuccess, _onFail));
            return operation;
		}

        private Operation asyncPOST(string _url, Dictionary<string, object> _params, SuccessCallback _onSuccess, FailCallback _onFail)
        {
            this.LogTrace(_url);
            Agent agent = new Agent();
            Operation operation = new Operation(agent);
            mono.StartCoroutine(post(_url, _params, operation, agent, _onSuccess, _onFail));
            return operation;
        }

        private IEnumerator get (string _url, Operation _operation, Agent _agent, SuccessCallback _onSuccess, FailCallback _onFail)
		{
            _agent.state = Agent.Status.RUNNING;
            _agent.www = new WWW(_url);
            yield return _agent.www;
			if (null != _agent.www.error) {
				_onFail (Error.NewAccessErr (_agent.www.error));
				yield break;
			}
			_onSuccess (_agent.www.bytes);
            _agent.www.Dispose();
            _agent.www = null;
            _agent.state = Agent.Status.DONE;
        }

        private IEnumerator post(string _url, Dictionary<string, object> _params, Operation _operation, Agent _agent, SuccessCallback _onSuccess, FailCallback _onFail)
        {
            _agent.state = Agent.Status.RUNNING;
            WWWForm form = new WWWForm();
            foreach (string key in _params.Keys)
            {
                object value = _params[key];
                if(value is string)
                {
                    form.AddField(key, (string)_params[key]);
                }
                else if(value is int)
                {
                    form.AddField(key, (int)_params[key]);
                }
                else
                {
                    _onFail(Error.NewAccessErr("params only support string and int."));
                    yield break;
                }
            }

            _agent.www = new WWW(_url, form);
            yield return _agent.www;
            if (null != _agent.www.error)
            {
                _onFail(Error.NewAccessErr(_agent.www.error));
                yield break;
            }
            _onSuccess(_agent.www.bytes);
            _agent.www.Dispose();
            _agent.www = null;
            _agent.state = Agent.Status.DONE;
        }
    }
}//namespace

