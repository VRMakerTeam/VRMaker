using UnityEngine;

public class ReticleHandler : MonoBehaviour
{
    public string path { get; set; }
    public string trigger { get; set; }

    public void onPointerEnter()
    {
        if (null == XGaze.instance)
            return;
        XGaze.instance.path = path;
        XGaze.instance.trigger = trigger;
        XGaze.instance.target = this.transform;
        XGaze.instance.Enter();
    }

    public void onPointerExit()
    {
        if (null == XGaze.instance)
            return;
        XGaze.instance.Exit();
    }

    public void onPointerClick()
    {
        if (null == XGaze.instance)
            return;
        XGaze.instance.Click();
    }
}
