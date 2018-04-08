using UnityEngine;
using VRXX.Manager;
using Fangs.Lib.Logger;

public class CardboardReviewer : MonoBehaviour
{
    private Player player = new Player();
    void Awake()
    {
        GameObject.DontDestroyOnLoad(this);
    }

    void Start()
    {
        player.Initialize();
        CanvasMgr.UseLandscape();
        VRMgr.SupportCardboard();
        reload();
    }

    void Update()
    {
        player.Update();
    }

    void reload()
    {
        Log.Info("CardboardReviewer:reload", "reload");
        ResourceMgr.CleanAllGameObjects();
        CameraMgr.UseDefaultSkybox();

        player.StopBean();

        ResourceMgr.ReadPersistentData("latest.vx",  (_data) =>
        {
            player.RunBean(_data);
        }, (_err) =>
        {
            Log.Debug("CardboardReviewer::reload", _err);
        });
    }
}
