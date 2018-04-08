using UnityEditor;
using UnityEngine;
using System.Collections.Generic;
using System.IO;
using SimpleJSON;
using System.Text.RegularExpressions;

public static class BuildTools
{
    //--------------------------------
    // Settings
    //--------------------------------
  
    static string outpath_pkg = System.IO.Path.Combine(Application.persistentDataPath, "pkgs/");
    static string outpath_manifest = System.IO.Path.Combine(Application.persistentDataPath, "manifests/");

    [MenuItem("BuildTools/AssetBundle/WebGL")]
    public static void BuildAssetBundleForWebGL()
    {
        buildAssetBundle(BuildTarget.WebGL);
    }

    [MenuItem("BuildTools/AssetBundle/Win32")]
    public static void BuildAssetBundleForWin32()
    {
        buildAssetBundle(BuildTarget.StandaloneWindows);
    }

    [MenuItem("BuildTools/AssetBundle/Android")]
    public static void BuildAssetBundleForAndroid()
    {
        buildAssetBundle(BuildTarget.Android);
    }


    private static void buildAssetBundle(BuildTarget _target)
    {
        Directory.CreateDirectory(outpath_pkg);
        string path = System.IO.Path.Combine(outpath_pkg, convertToPlatform(_target));
        Directory.CreateDirectory(path);
        Debug.Log(string.Format("build assetbundle at {0}", path));
        BuildPipeline.BuildAssetBundles(path, BuildAssetBundleOptions.ForceRebuildAssetBundle, _target);
    }

    private static string convertToPlatform(BuildTarget _target)
    {
        if (BuildTarget.StandaloneWindows == _target)
            return "win32";
        if (BuildTarget.StandaloneWindows64 == _target)
            return "win64";
        if (BuildTarget.WebGL == _target)
            return "webgl";
        if (BuildTarget.Android == _target)
            return "android";
        return "unknow";
    }



    [MenuItem("BuildTools/Manifests")]
    public static void Buildmanifests()
    {
        Directory.CreateDirectory(outpath_manifest);
        buildManifests();
    }

    private static void buildManifests()
    {
        string path = System.IO.Path.Combine(Application.dataPath, "Packages");

        DirectoryInfo test = new DirectoryInfo(path);
        if (test.Exists)
        {

            foreach(string group in Directory.GetDirectories(path))
            {
                foreach(string section in Directory.GetDirectories(group))
                {
                    foreach(string package in Directory.GetDirectories(section))
                    {
                        foreach(string file in Directory.GetFiles(Path.Combine(package, "manifest")))
                        {
                            if(Path.GetExtension(file).Equals(".meta"))
                            {
                                continue;
                            }
                            string dist = Path.Combine(outpath_manifest, Path.GetFileName(file));
                            Debug.Log(string.Format("Copy {0} to {1}", file, dist));
                            File.Copy(file, dist, true);
                        }
                    }
                }
            }
        }
        else
        {
            Debug.Log("Without this directory...");
        }
    }

}
