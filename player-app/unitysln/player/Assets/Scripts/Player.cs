using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using VRXX;
using VRXX.Manager;

public class Player
{
    private Engine engine;

    public void Initialize()
    {
        engine = new Engine();
        engine.Preload();
    }

    public void RunBean(string _beanJson)
    {
        BeanMgr.Run(_beanJson);
    }

    public void RunBean(byte[] _beanData)
    {
        BeanMgr.Run(_beanData);
    }

    public void StopBean()
    {
        BeanMgr.Stop();
    }

    public void Update()
    {
        engine.Update();
    }

    public void Release()
    {
        engine.Dispose();
    }
}
