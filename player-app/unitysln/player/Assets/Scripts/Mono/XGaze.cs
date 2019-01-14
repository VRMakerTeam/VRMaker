using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections.Generic;
using Fangs.Lib.Logger;

public class XGaze : MonoBehaviour {
    public static XGaze instance { get; private set; }

    public string path { get; set; }
    public string trigger { get; set; }
    public Transform target { get; set; }

    private float timer = 0;
    private float duration = 2f;
    private bool tick = false;

    private Image circle = null;

    void Awake()
    {
        instance = this;
        circle = this.transform.Find("adjust/circle").GetComponent<Image>();
        XGaze.instance.gameObject.SetActive(false);
    }

    public void Enter()
    {
        startTick();
    }

    public void Exit()
    {
        finishTick();
    }

    public void Click()
    {
        finishTick();
        execute();
    }

    void Update()
    {
        if (!tick)
            return;

        timer += Time.deltaTime;
        circle.fillAmount = timer / duration;
        if (timer >= duration)
        {
            finishTick();
            execute();
        }
    }


    private void execute()
    {
        this.LogDebug("xgaze execute path:{0} trigger:{1}", path, trigger);
        foreach (string uuid in VRXX.Manager.BlockMgr.ListWithPath(path))
        {
            VRXX.Manager.Block block = VRXX.Manager.BlockMgr.FindBlock(uuid);
            if (!block.action.Equals("Entry:OnGazeAccepted"))
                continue;
            if (!block.param["trigger"].Equals(trigger))
                continue;
            block.Execute();
        }
    }

    private void finishTick()
    {
        tick = false;
        timer = 0;
        XGaze.instance.gameObject.SetActive(false);
    }

    private void startTick()
    {
        if (null == target.GetComponent<ReticleHandler>())
            return;

        timer = 0;
        tick = true;
        //adjust once before show
        XGaze.instance.gameObject.SetActive(true);
    }
}
