using System.IO;
using UnityEngine;
using UnityEditor;

public static class ReleaseTools
{
    private const string companyname = "vrmaker";

    private const string productname_reviewer = "vplayer";
    private const string productname_classroom = "classroom";

    private const string identifier_reviewer = "com." + companyname + "." + productname_reviewer;
    private const string identifier_classroom = "com." + companyname + "." + productname_classroom;

    private const string bundlename_editor_web = "editor_web";
    private const string bundlename_reviewer_pc = "reviewer_pc";
    private const string bundlename_reviewer_cardboard = "reviewer_cardboard";

    static string outpath_player = System.IO.Path.Combine(Path.GetFullPath("."), "../../../runpath/player/");

    [MenuItem("ReleaseTools/Player/Editor - WEB")]
    public static void BuildEditorWebPlayer()
    {
        cleanStreamingAssets();
        buildPlayer(BuildTarget.WebGL, bundlename_editor_web);
    }

    [MenuItem("ReleaseTools/Player/Reviewer - PC")]
    public static void BuildReviewerPCPlayer()
    {
        PlayerSettings.applicationIdentifier = identifier_reviewer;
        PlayerSettings.companyName = companyname;
        PlayerSettings.productName = productname_reviewer;
        EditorUserBuildSettings.selectedStandaloneTarget = BuildTarget.StandaloneWindows;

        cleanStreamingAssets();
        SDKTools.ImportXiaoMiVRSDK();
        buildPlayer(BuildTarget.StandaloneWindows, bundlename_reviewer_pc);
        SDKTools.CleanXiaoMiVRSDK();
    }

    [MenuItem("ReleaseTools/Player/Reviewer - Cardboard")]
    public static void BuildReviewerCardboardPlayer()
    {
        //app info
        PlayerSettings.applicationIdentifier = identifier_reviewer;
        PlayerSettings.companyName = companyname;
        PlayerSettings.productName = productname_reviewer;
        //key
        //icon
        updateAndroidIcons("Assets/AppData/logo-vplayer-cardboard.png");
        //android sdk
        PlayerSettings.Android.minSdkVersion = AndroidSdkVersions.AndroidApiLevel19;
        PlayerSettings.Android.targetSdkVersion = AndroidSdkVersions.AndroidApiLevel24;
        PlayerSettings.SetUseDefaultGraphicsAPIs(BuildTarget.Android, true);

        cleanStreamingAssets();
        SDKTools.ImportGoogleVRSDK();
        buildPlayer(BuildTarget.Android, bundlename_reviewer_cardboard);
        SDKTools.CleanGoogleVRSDK();
    }

    private static void buildPlayer(BuildTarget _target, string _name)
    {
        string path = System.IO.Path.Combine(outpath_player, _name);
        if (_target == BuildTarget.StandaloneWindows || _target == BuildTarget.StandaloneWindows64)
        {
            path = System.IO.Path.Combine(path, _name);
            path = path + ".exe";
        }
        if (_target == BuildTarget.Android)
        {
            path = path + ".apk";
        }
        Debug.Log(string.Format("build player at {0}", path));
        string[] scenes = new string[1];
        scenes[0] = string.Format("Assets/Scenes/{0}.unity", _name);
        BuildPipeline.BuildPlayer(scenes, path, _target, BuildOptions.None);
    }

    private static void cleanStreamingAssets()
    {
        Directory.Delete(Application.streamingAssetsPath, true);
        Directory.CreateDirectory(Application.streamingAssetsPath);
        AssetDatabase.Refresh();
    }

    private static void updateAndroidIcons(string _iconPath)
    {
        Texture2D icon = (Texture2D)AssetDatabase.LoadAssetAtPath(_iconPath, typeof(Texture2D));
        if (null == icon)
            return;

        int iconCount = PlayerSettings.GetIconSizesForTargetGroup(BuildTargetGroup.Android).Length;
        Texture2D[] icons = new Texture2D[iconCount];
        for (int i = 0; i < iconCount; i++)
            icons[i] = icon;

        PlayerSettings.SetIconsForTargetGroup(BuildTargetGroup.Android, icons);
    }
}
