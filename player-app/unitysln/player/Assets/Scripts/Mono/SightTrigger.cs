using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections.Generic;
using Fangs.Lib.Logger;

using VRXX.Manager;

public class SightTrigger : MonoBehaviour {

    public string trigger { get; set; }
    public string path { get; set; }
    public Sprite[] icons;

    public static SightTrigger sender = null;

    private float timer = 0;
    private float duration = 2;
    private bool tick = false;

    private static List<SightTrigger> triggers = new List<SightTrigger>(0);
    private Image circle = null;
    private Image circleBG = null;
    private Text alias = null;
    private Image icon = null;
    private Image outline = null;

    void Awake()
    {
        triggers.Add(this);
        circle = this.transform.FindChild("circle").GetComponent<Image>();
        circleBG = this.transform.FindChild("circle_bg").GetComponent<Image>();
        alias = this.transform.FindChild("alias").GetComponent<Text>();
        icon = this.transform.FindChild("icon").GetComponent<Image>();
        outline = this.transform.FindChild("outline").GetComponent<Image>();
    }

    public void ToggleAlias(bool _visible)
    {
        alias.gameObject.SetActive(_visible);
    }

    public void UpdateAlias(string _alias)
    {
        alias.text = _alias;
    }

    public void UpdateIcon(int _iconIndex)
    {
        icon.sprite = icons[_iconIndex];
    }

    public void UpdateColor(int _r, int _g, int _b, int _a)
    {
        icon.color = new Color(_r/255.0f, _g/255.0f, _b/255.0f, _a/255.0f);
        outline.color = new Color(_r / 255.0f, _g / 255.0f, _b / 255.0f, _a / 255.0f);
    }

    public void onPointerEnter()
    {
        this.LogDebug("sight trigger enter");
        startTick();
    }

    public void onPointerExit()
    {
        this.LogDebug("sight trigger exit");
        finishTick();
    }

    public void onPointerClick()
    {
        this.LogDebug("sight trigger click");
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
        this.LogDebug("sighttrigger execute path:{0} trigger:{1}", path, trigger);
        sender = this;
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
        circleBG.gameObject.SetActive(false);
        circle.gameObject.SetActive(false);
        tick = false;
        timer = 0;
    }

    private void startTick()
    {
        timer = 0;
        tick = true;
        circleBG.gameObject.SetActive(true);
        circle.gameObject.SetActive(true);

    }
}
