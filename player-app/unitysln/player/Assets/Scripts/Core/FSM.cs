/********************************************************************
     Copyright (c) zyncho.
     All rights reserved.

     author: Steven Sun
*********************************************************************/
using System.Collections.Generic;
using VRXX.Manager;
using Fangs.Lib.Logger;

namespace VRXX
{
   
    public class State
    {
        public class Param
        {
            public string uid { get; private set; }
            public Param(string _uid)
            {
                uid =_uid;
            }
        }

        public Param param { get; set; }

        public virtual void Initialize() {
            this.LogTrace("Initialize : {0}", param.uid);
        }
        public virtual void Release() {
            this.LogTrace("Release : {0}", param.uid);
        }

        public virtual void OnEnter() {
            this.LogTrace("OnEnter : {0}", param.uid);
        }
        public virtual void OnUpdate() {
            this.LogTrace("OnUpdate : {0}", param.uid);
        }
        public virtual void OnExit() {
            this.LogTrace("OnExit : {0}", param.uid);
        }
    }

    public class FSM<T> where T : State, new()
    {
        public State current {get; private set;}

        protected Dictionary<string, State> status = new Dictionary<string, State>(0);

        public FSM()
        {   
            current = null;
        }

        public void Reset()
        {
            current = null;
        }

        public State NewState(State.Param _param)
        {
            State state = new T();
            state.param = _param;
            state.Initialize();
            status.Add(_param.uid, state);
            return state;
        }

        public void DeleteState(State _state)
        {
            _state.OnExit();
            _state.Release();
            if (current == _state)
                current = null;
        }

        public State FindState(string _uid)
        {
            State state = null;
            status.TryGetValue(_uid, out state);
            return state;
        }

        public void SwitchState(string _src, string _dist)
        {
            State srcState = FindState(_src);
            State distState = FindState(_dist);
            SwitchState(srcState, distState);
        }

        public void SwitchState(State _src, State _dist)
        {
            if (null == _src)
                throw new System.Exception("src state is null");
            if (null == _dist)
                throw new System.Exception("dist state is null");

            _src.OnExit();
            _dist.OnEnter();
            current = _dist;
        }

        public void SwitchState(string _dist)
        {
            State distState = FindState(_dist);
            SwitchState(distState);
        }

        public void SwitchState(State _dist)
        {
            if (null == _dist)
                throw new System.Exception("dist state is null");

            if (null != current)
                current.OnExit();
            _dist.OnEnter();
            current = _dist;
        }

        public void Clean()
        {
            foreach(var state in status.Values)
            {
                state.Release();
            }
            status.Clear();
            current = null;
        }

    }//class
}//namespace
