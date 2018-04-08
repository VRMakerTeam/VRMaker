using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using Fangs.Lib.Logger;
using VRXX.Manager;

[System.Serializable]
public class HUD
{
    [System.Serializable]
    public class PanelLoading
    {
        public GameObject root;
        public Animation animLoading;
    }
    [System.Serializable]
    public class PanelNotify
    {
        public GameObject root;
        public Text message;
    }
    [System.Serializable]
    public class PanelEditor
    {
        public GameObject root;
    }
    [System.Serializable]
    public class PanelSlate
    {
        public GameObject root;
        public Image slatePic;
        public Button toggle;
    }

    public GameObject root;
    public PanelLoading panelLoading;
    public PanelNotify panelNotify;
    public PanelEditor panelEditor;
    public PanelSlate panelSlate;

    public void Reset()
    {
        panelLoading.root.SetActive(false);
        panelNotify.root.SetActive(false);
        panelEditor.root.SetActive(false);
        panelSlate.root.SetActive(false);
    }
}



public class Editor : MonoBehaviour {
    public HUD hud;
    public Battlehub.RTEditor.RuntimeEditor editor;

    private Player player = new Player();

    private float cameraZoomSpeed = 10;

    class Picker
    {
        public Ray ray;
        public Transform picked = null;
        public Bounds bounds;
    }

    private Picker picker_ = new Picker();

    void Awake()
    {
        GameObject.DontDestroyOnLoad(this);
        hud.Reset();
        finishLoading();
    }

   
    void Start()
    {
        player.Initialize();
        CanvasMgr.ApplyScaleModeConstantPixelSize();
        CanvasMgr.AttachPanel(hud.panelLoading.root);
        CanvasMgr.AttachPanel(hud.panelNotify.root);
        CanvasMgr.AttachPanel(hud.panelEditor.root);
        CanvasMgr.AttachPanel(hud.panelSlate.root);
        GameObject.Destroy(hud.root);
        initEditor();

        VRXX.JSAPI.onNotify = this.notify;
        VRXX.JSAPI.onLoadStart = this.startLoading;
        VRXX.JSAPI.onLoadFinish = this.finishLoading;
        VRXX.JSAPI.Initialize();
    }

    void Update()
    {
        //if (SlateMgr.Status.CURRENT == SlateMgr.state)
        //{
        //    hud.panelSlate.root.SetActive(false);
        //}   
        player.Update();
    }

    public void JSMessage(string _message)
    {
        VRXX.AppError err = VRXX.JSAPI.HandleMessage(_message);
        if (0 != err.errcode)
            this.LogError(err.errmessage);
    }

    private void initEditor()
    {
        //editor.gameObject.SetActive(false);
        editor.CloseButton.SetActive(false);
        editor.EditButton.SetActive(false);
        editor.gameObject.SetActive(true);
        editor.gameObject.SetActive(true);
        hud.panelEditor.root.SetActive(true);
        editor.IsOn = true;
        Battlehub.RTHandles.RuntimeTools.CameraEuler = Battlehub.RTHandles.RuntimeCameraEuler.Head;
        Battlehub.RTHandles.BaseHandle.OnPositionHandleDrop += VRXX.Manager.EditorMgr.OnPositionHandleDrop;
        Battlehub.RTHandles.BaseHandle.OnRotationHandleDrop += VRXX.Manager.EditorMgr.OnRotationHandleDrop;
        Battlehub.RTHandles.BaseHandle.OnScaleHandleDrop += VRXX.Manager.EditorMgr.OnScaleHandleDrop;
    }

    private void startLoading()
    {
        hud.panelLoading.root.SetActive(true);
        hud.panelLoading.animLoading.Play();
    }

    private void finishLoading()
    {
        hud.panelLoading.animLoading.Stop();
        hud.panelLoading.root.SetActive(false);
    }


    private void notify(string _message)
    {
        hud.panelNotify.message.text = _message;
        hud.panelNotify.root.SetActive(true);
        StartCoroutine(waitHideNotify()); 
    }

    private IEnumerator waitHideNotify()
    {
        yield return new WaitForSeconds(3);
        hud.panelNotify.root.gameObject.SetActive(false);
    }
}
