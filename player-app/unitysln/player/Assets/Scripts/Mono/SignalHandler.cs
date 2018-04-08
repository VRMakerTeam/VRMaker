using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using UnityEngine;

public class SignalHandler : MonoBehaviour {

    void OnInitialize()
    {
        this.LogInfo("initialize finish");
    }

    void HandleSignal(string _signal)
    {
        VRXX.Manager.SignalMgr.Handle(_signal);
    }

}
