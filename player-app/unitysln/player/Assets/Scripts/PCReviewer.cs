using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using VRXX.Manager;
using Fangs.Lib.Logger;

public class PCReviewer : MonoBehaviour
{
    [System.Serializable]
    public class PanelVR
    {
        public GameObject root;
        public Button cardboard;
        public Button mi;
    }

    [System.Serializable]
    public class HUD
    {
        public GameObject root;
        public PanelVR panelVR;
        public Text loading;
    }

    public HUD hud;

    private Player player = new Player();
    void Awake()
    {
        GameObject.DontDestroyOnLoad(this);
    }

    IEnumerator Start()
    {
        hud.loading.gameObject.SetActive(true);

        player.Initialize();
        CanvasMgr.AttachPanel(hud.panelVR.root);
        CanvasMgr.AttachPanel(hud.loading.gameObject);
        GameObject.Destroy(hud.root);

        hud.panelVR.cardboard.onClick.AddListener(() =>
        {
            VRMgr.SupportCardboard();
            InputMgr.Toggle(false);
            hud.panelVR.root.SetActive(false);
        });

        hud.panelVR.mi.onClick.AddListener(() =>
        {
            VRMgr.SupportMI();
            InputMgr.Toggle(false);
            hud.panelVR.root.SetActive(false);
        });

        yield return 1;

        ResourceMgr.ReadPersistentData("latest.vx", (_data) =>
        {
            player.RunBean(_data);
            hud.loading.gameObject.SetActive(false);
        }, (_err) =>
        {
            Log.Error("ReviewPlayer", _err);
        });
    }

    void Update()
    {
        player.Update();

        if(Input.GetKeyDown(KeyCode.V))
        {
            hud.panelVR.root.SetActive(!hud.panelVR.root.activeSelf);
        }
    }
}
