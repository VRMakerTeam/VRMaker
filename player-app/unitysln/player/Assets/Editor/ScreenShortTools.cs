using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class ScreenShortTools : MonoBehaviour {

    void Start()
    {
        GameObject objAgents = GameObject.Find("Agents");

        List<Transform> objs = new List<Transform>();

        foreach (Transform obj in objAgents.transform)
        {
            Debug.Log(obj);
            obj.gameObject.SetActive(false);
            objs.Add(obj);
        }
        dosnap(objs);
    }

    void dosnap(List<Transform> _objs)
    {
        this.StartCoroutine(snap(_objs));
    }

    IEnumerator snap(List<Transform> _objs)
    {
        if (_objs.Count == 0)
        {
            Debug.Log("FINISH");
            yield break;
        }

        yield return 1;
        Transform obj = _objs[0];
        _objs.RemoveAt(0);
        obj.gameObject.SetActive(true);
        Debug.Log("snap :" + obj.name);
        yield return 1;

        Application.CaptureScreenshot(obj.name + ".png");
        yield return 1;

        string sName = null;
        obj.transform.Rotate(0, 0, 0, Space.Self);
        for (int i = 1; i < 8; i++ )
        {
            obj.transform.Rotate(0, 45, 0, Space.Self);
            switch(i)
            {
                //case 0 : sName = null;break;
		        case 1 : sName = "@45";break;
		        case 2 : sName = "@90";break;
		        case 3 : sName = "@135";break;
		        case 4 : sName = "@180";break;
		        case 5 : sName = "@225";break;
		        case 6 : sName = "@270";break;
		        case 7 : sName = "@315";break;
            }
            Application.CaptureScreenshot(obj.name + sName + ".png");
            yield return 1;
        }
        obj.gameObject.SetActive(false);
        dosnap(_objs);
    }
}
