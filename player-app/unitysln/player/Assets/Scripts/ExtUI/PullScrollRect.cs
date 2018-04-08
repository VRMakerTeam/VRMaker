using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using UnityEngine.Events;
using UnityEngine.EventSystems;
using VRXX.Manager;

[System.Serializable]
 public class RefreshTip
 {
    public GameObject root;
    public GameObject pullTip;
    public GameObject releaseTip;
    public GameObject refreshTip;
 }

public class PullScrollRect : ScrollRect {
    public RefreshTip refreshTip;
    public int pullY = -100;
    public int refreshY = -250;
    public delegate void onCallbackDelegate();
    public onCallbackDelegate onTriggerRefresh;
    private delegate void OnTimerSucceesDelegate();

    private enum Status
    {
        IDLE,
        PULL,
        REFRESH,
    }

    private Status state = Status.IDLE;

    public override void OnBeginDrag(PointerEventData eventData)
    {
        if (Status.REFRESH == state)
            return;

        base.OnBeginDrag(eventData);
    }

    public override void OnDrag(PointerEventData eventData)
    {
        if (Status.REFRESH == state)
            return;

        base.OnDrag(eventData);

        if (this.content.localPosition.y < pullY && this.content.localPosition.y > refreshY)
        {
            refreshTip.root.SetActive(true);
            refreshTip.pullTip.SetActive(true);
            refreshTip.releaseTip.SetActive(false);
            refreshTip.refreshTip.SetActive(false);
            if (Status.PULL == state)
                state = Status.IDLE;
        }

        if (this.content.localPosition.y <= refreshY)
        {
            refreshTip.pullTip.SetActive(false);
            refreshTip.releaseTip.SetActive(true);
            refreshTip.refreshTip.SetActive(false);
            state = Status.PULL;
        }

    }
    public override void OnEndDrag(PointerEventData eventData)
    {
        if (Status.REFRESH == state)
            return;

        base.OnEndDrag(eventData);

        if (Status.PULL == state)
        {
            refreshTip.pullTip.SetActive(false);
            refreshTip.releaseTip.SetActive(false);
            refreshTip.refreshTip.SetActive(true);
            state = Status.REFRESH;
            if (null != onTriggerRefresh)
            {
                CoroutineMgr.Start(delayOneSecond(() =>
                    {
                        onTriggerRefresh();
                    }));
            }
        }         
        else
        {
            refreshTip.root.SetActive(false);
        }
    }

    public override void OnScroll(PointerEventData data)
    {
        base.OnScroll(data);
    }

    private IEnumerator delayOneSecond(OnTimerSucceesDelegate _onSuccees)
    {
        float time = 1;
        while (time > 0)
        {
            yield return new WaitForEndOfFrame();
            time -= Time.deltaTime;
        }
        _onSuccees();
    }

    public void FinishRefresh()
    {
        state = Status.IDLE;
        refreshTip.root.SetActive(false);
    }
}
