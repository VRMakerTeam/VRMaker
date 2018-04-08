using UnityEngine;
using UnityEditor;
using System.IO;
using System.Collections.Generic;

public static class SDKTools
{
    private static string srcPluginPath = Path.Combine(Path.GetFullPath("."), "../../sdk/plugin");
    private static string srcCodePath = Path.Combine(Path.GetFullPath("."), "../../sdk/src");
    private static string srcStreamPath = Path.Combine(Path.GetFullPath("."), "../../sdk/stream");
    private static string destPluginPath = Path.Combine(Path.GetFullPath("."), "Assets/Plugins");
    private static string destCodePath = Path.Combine(Path.GetFullPath("."), "Assets/3rd");
    private static string destStreamPath = Path.Combine(Path.GetFullPath("."), "Assets/StreamingAssets");

    [MenuItem("SDK/Import/MIVR")]
    public static void ImportXiaoMiVRSDK()
    {
        setSymbol("__SDK_VR_XIAOMI__");

        importFolder(Path.Combine(srcPluginPath, "_mivr"), destPluginPath);
        importFolder(Path.Combine(srcCodePath, "_mivr"), destCodePath);

        AssetDatabase.Refresh();

        PlayerSettings.virtualRealitySupported = false;
    }

    [MenuItem("SDK/Clean/MIVR")]
    public static void CleanXiaoMiVRSDK()
    {
        cleanSymbol();

        cleanFolder(destPluginPath, Path.Combine(srcPluginPath, "_mivr"));
        cleanFolder(destCodePath, Path.Combine(srcCodePath, "_mivr"));

        AssetDatabase.Refresh();
    }

    [MenuItem("SDK/Import/GoogleVR")]
    public static void ImportGoogleVRSDK()
    {
        setSymbol("__SDK_VR_GOOGLE__");

        importFolder(Path.Combine(srcPluginPath, "_googlevr"), destPluginPath);
        importFolder(Path.Combine(srcCodePath, "_googlevr"), destCodePath);

        AssetDatabase.Refresh();

        PlayerSettings.virtualRealitySupported = true;
        
    }

    [MenuItem("SDK/Clean/GoogleVR")]
    public static void CleanGoogleVRSDK()
    {
        cleanSymbol();

        cleanFolder(destPluginPath, Path.Combine(srcPluginPath, "_googlevr"));
        cleanFolder(destCodePath, Path.Combine(srcCodePath, "_googlevr"));

        AssetDatabase.Refresh();
    }

    private static void cleanSymbol()
    {
        setSymbol("");
    }

    private static void setSymbol(string _symbol)
    {
        PlayerSettings.SetScriptingDefineSymbolsForGroup(BuildTargetGroup.Android, _symbol);
        PlayerSettings.SetScriptingDefineSymbolsForGroup(BuildTargetGroup.Standalone, _symbol);
        PlayerSettings.SetScriptingDefineSymbolsForGroup(BuildTargetGroup.iOS, _symbol);
        PlayerSettings.SetScriptingDefineSymbolsForGroup(BuildTargetGroup.WebGL, _symbol);
    }

    private static void importFolder(string _from, string _to)
    {
        if (!Directory.Exists(_from))
            return;

        if (!Directory.Exists(_to))
            Directory.CreateDirectory(_to);

        copyFiles(_from, _to);

        string[] sourceFolders = Directory.GetDirectories(_from);

        foreach (string sourceFolder in sourceFolders)
        {
            string destFolder = _to + "\\" + sourceFolder.Substring(_from.Length + 1);

            importFolder(sourceFolder, destFolder);
        }
    }

    private static void copyFiles(string _sourceDirectory, string _destDirectory)
    {
        string[] sourceFilePaths = Directory.GetFiles(_sourceDirectory);

        foreach (string sourceFilePath in sourceFilePaths)
        {
            string destFilePath = _destDirectory + "\\" + sourceFilePath.Substring(_sourceDirectory.Length + 1);
            File.Copy(sourceFilePath, destFilePath, true);
        }
    }


    private static void cleanFolder(string _target, string _source)
    {
        DirectoryInfo target = new DirectoryInfo(_target);
        DirectoryInfo source = new DirectoryInfo(_source);

        List<string> files = parseFolder(target.FullName, source.FullName, _source);

        foreach (string file in files)
        {
            if (File.Exists(file))
                File.Delete(file);
        }

        Debug.Log(string.Format("Delete {0} files", files.Count));
    }

    private static List<string> parseFolder(string _target, string _source, string _now)
    {
        List<string> files = new List<string>();

        if (!Directory.Exists(_source))
            return files;

        if (!Directory.Exists(_target))
            return files;

        DirectoryInfo d = new DirectoryInfo(_now);
        FileSystemInfo[] fsinfos = d.GetFileSystemInfos();
        foreach (FileSystemInfo fsinfo in fsinfos)
        {
            if (fsinfo is DirectoryInfo)
            {
                files.AddRange(parseFolder(_target, _source, fsinfo.FullName));
            }
            else
            {
                files.Add(fsinfo.FullName.Replace(_source, _target));
            }
        }
        return files;

    }
}

