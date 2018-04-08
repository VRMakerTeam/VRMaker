//-----------------------------------------------------------------------
// <copyright file="VrManager.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

using System;
using System.Collections;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.InteropServices;
using UnityEngine;

/// <summary>
/// Configuration data 
/// </summary>
public class VrManager : MonoBehaviour
{
    /// <summary>
    /// The format of each eye texture.
    /// </summary>
    [SuppressMessage("StyleCop.CSharp.MaintainabilityRules", "SA1401:FieldsMustBePrivate", Justification = "Unity need this.")]
    public RenderTextureFormat EyeTextureFormat = RenderTextureFormat.Default;

    /// <summary>
    /// The antialiasing level of each eye texture.
    /// </summary>
    [SuppressMessage("StyleCop.CSharp.MaintainabilityRules", "SA1401:FieldsMustBePrivate", Justification = "Unity need this.")]
    public RenderTextureAntiAliasing EyeTextureAntiAliasing = RenderTextureAntiAliasing.Level2;

    /// <summary>
    /// The depth of each eye texture in bits. Valid Unity render texture depths are 0, 16, and 24.
    /// </summary>
    [SuppressMessage("StyleCop.CSharp.MaintainabilityRules", "SA1401:FieldsMustBePrivate", Justification = "Unity need this.")]
    public RenderTextureDepth EyeTextureDepth = RenderTextureDepth.Depth24;

    /// <summary>
    /// If true, TimeWarpDuration will be used to correct the output of each Camera for rotational Latency.
    /// </summary>
    [SuppressMessage("StyleCop.CSharp.MaintainabilityRules", "SA1401:FieldsMustBePrivate", Justification = "Unity need this.")]
    public bool TimeWarp = true;

    /// <summary>
    /// If this is true and TimeWarp is true, each Camera will stop tracking and only TimeWarp will respond to head motion.
    /// </summary>
    [SuppressMessage("StyleCop.CSharp.MaintainabilityRules", "SA1401:FieldsMustBePrivate", Justification = "Unity need this.")]
    public bool FreezeTimeWarp = false;

    /// <summary>
    /// If true, each scene load will cause the head pose to reset.
    /// </summary>
    [SuppressMessage("StyleCop.CSharp.MaintainabilityRules", "SA1401:FieldsMustBePrivate", Justification = "Unity need this.")]
    public bool ResetTrackerOnLoad = true;

    /// <summary>
    /// If true, enable high dynamic range support.
    /// </summary>
    [SuppressMessage("StyleCop.CSharp.MaintainabilityRules", "SA1401:FieldsMustBePrivate", Justification = "Unity need this.")]
    public bool IsHdrEnabled = false;

    /// <summary>
    /// The is HMD present
    /// </summary>
    private bool isHmdPresent;

    /// <summary>
    /// Whether is going home, when go home first we make two eye black, then go home
    /// </summary>
    private bool isGoingHome = false;

    /// <summary>
    /// We wait 1 frame to make screen black, then go home.
    /// </summary>
    private int goingHomeFrameCount = 0;

    /// <summary>
    /// The previous eye texture anti aliasing
    /// </summary>
    private RenderTextureAntiAliasing prevEyeTextureAntiAliasing;

    /// <summary>
    /// The previous eye texture depth
    /// </summary>
    private RenderTextureDepth prevEyeTextureDepth;

    /// <summary>
    /// The previous HDR
    /// </summary>
    private bool prevHdr;

    /// <summary>
    /// The previous eye texture format
    /// </summary>
    private RenderTextureFormat prevEyeTextureFormat;

    /// <summary>
    /// The current activity
    /// </summary>
    private AndroidJavaObject currentActivity;

    /// <summary>
    /// The java VR activity class
    /// </summary>
    private AndroidJavaClass javaVrActivityClass;

    /// <summary>
    /// The volume control transform
    /// </summary>
    private Transform volumeControlTransform;

    /// <summary>
    /// The profile is cached
    /// </summary>
    private bool profileIsCached;

    /// <summary>
    /// The profile
    /// </summary>
    private EyeProfile profile;

    /// <summary>
    /// Occurs when [HMD acquired].
    /// </summary>
    public event Action HmdGained;

    /// <summary>
    /// Occurs when an HMD detached.
    /// </summary>
    public event Action HmdLost;

    /// <summary>
    /// Occurs when the application is resumed.
    /// </summary>
    public event Action OnApplicationResumed;

    /// <summary>
    /// Occurs before plugin initialized
    /// </summary>
    public event Action OnConfigureVrModeParms;

    /// <summary>
    /// Occurs when the Eye Texture AntiAliasing level is modified.
    /// </summary>
    internal event Action<RenderTextureAntiAliasing, RenderTextureAntiAliasing> EyeTextureAntiAliasingModified;

    /// <summary>
    /// Occurs when the Eye Texture Depth is modified.
    /// </summary>
    internal event Action<RenderTextureDepth, RenderTextureDepth> EyeTextureDepthModified;

    /// <summary>
    /// Occurs when the Eye Texture Format is modified.
    /// </summary>
    internal event Action<RenderTextureFormat, RenderTextureFormat> EyeTextureFormatModified;

    /// <summary>
    /// Occurs when HDR mode is modified.
    /// </summary>
    internal event Action<bool, bool> HdrModified;

    /// <summary>
    /// the render texture antialiasing level
    /// </summary>
    public enum RenderTextureAntiAliasing
    {
        /// <summary>
        /// The level1
        /// </summary>
        Level1 = 1,

        /// <summary>
        /// The level2
        /// </summary>
        Level2 = 2,

        /// <summary>
        /// The level4
        /// </summary>
        Level4 = 4,

        /// <summary>
        /// The level8
        /// </summary>
        Level8 = 8,
    }

    /// <summary>
    /// the render texture depth value
    /// </summary>
    public enum RenderTextureDepth
    {
        /// <summary>
        /// The depth0
        /// </summary>
        Depth0 = 0,

        /// <summary>
        /// The depth16
        /// </summary>
        Depth16 = 16,

        /// <summary>
        /// The depth24
        /// </summary>
        Depth24 = 24,
    }

    /// <summary>
    /// Gets the singleton instance.
    /// </summary>
    public static VrManager Instance { get; private set; }

    /// <summary>
    /// Gets the SDK version.
    /// </summary>
    /// <value>
    /// The SDK version.
    /// </value>
    public static string SDKVersion
    {
        get { return "0.1"; }
    }

    /// <summary>
    /// Gets a reference to the active HMD
    /// </summary>
    public MiHMD Hmd { get; private set; }

    /// <summary>
    /// Gets or sets the index of the time warp view.
    /// </summary>
    /// <value>
    /// The index of the time warp view.
    /// </value>
    public int TimeWarpViewIndex { get; set; }

    /// <summary>
    /// Gets the current profile, which contains information about the user's settings and body dimensions.
    /// </summary>
    public EyeProfile Profile
    {
        get
        {
            if (!this.profileIsCached)
            {
                float ipd = 0.0f;
                GetInterpupillaryDistance(ref ipd);

                float eyeHeight = 0.0f;
                GetPlayerEyeHeight(ref eyeHeight);

                this.profile = new EyeProfile
                {
                    Ipd = ipd,
                    EyeHeight = eyeHeight,
                    EyeDepth = 0.0805f,
                    NeckHeight = eyeHeight - 0.075f
                };

                this.profileIsCached = true;
            }

            return this.profile;
        }
    }

    /// <summary>
    /// Gets the battery level.
    /// </summary>
    /// <value>
    /// The battery level.
    /// </value>
    public float BatteryLevel
    {
        get
        {
            return GetBatteryLevel();
        }
    }

    /// <summary>
    /// Gets the battery temperature.
    /// </summary>
    /// <value>
    /// The battery temperature.
    /// </value>
    public float BatteryTemperature
    {
        get
        {
            return GetBatteryTemperature();
        }
    }

    /// <summary>
    /// Gets the battery status.
    /// </summary>
    /// <value>
    /// The battery status.
    /// </value>
    public int BatteryStatus
    {
        get
        {
            return GetBatteryStatus();
        }
    }

    /// <summary>
    /// Gets the volume level.
    /// </summary>
    /// <value>
    /// The volume level.
    /// </value>
    public int VolumeLevel
    {
        get
        {
            return GetVolume();
        }
    }

    /// <summary>
    /// Gets the time since last volume change.
    /// </summary>
    /// <value>
    /// The time since last volume change.
    /// </value>
    public double TimeSinceLastVolumeChange
    {
        get
        {
            return GetTimeSinceLastVolumeChange();
        }
    }

    /// <summary>
    /// Quits the application.
    /// </summary>
    public void QuitApp()
    {
        var cameras = this.GetComponentsInChildren<Camera>();
        foreach (var camera in cameras)
        {
            camera.cullingMask = 0;
        }

        this.isGoingHome = true;
    }

    /// <summary>
    /// Starts the VR mode.
    /// </summary>
    public void StartVrMode()
    {
        NativePluginEvents.IssuePluginEvent(PluginEvents.Resume);
    }

    /// <summary>
    /// Stops the VR mode.
    /// </summary>
    public void StopVrMode()
    {
        NativePluginEvents.IssuePluginEvent(PluginEvents.Pause);
    }

    /// <summary>
    /// Returns to launcher.
    /// </summary>
    public void ReturnToLauncher()
    {
        this.PlatformUiConfirmQuit();
    }

    /// <summary>
    /// Sets the initialize variables.
    /// </summary>
    /// <param name="activity">The activity.</param>
    /// <param name="activityClass">The activity class.</param>
    public void SetInitVariables(IntPtr activity, IntPtr activityClass)
    {
        SetInitClasses(activity, activityClass);
    }

    /// <summary>
    /// Platforms the UI confirm quit.
    /// </summary>
    public void PlatformUiConfirmQuit()
    {
        NativePluginEvents.IssuePluginEvent(PluginEvents.PlatformUIConfirmQuit);
    }

    /// <summary>
    /// Platforms the UI global menu.
    /// </summary>
    public void PlatformUiGlobalMenu()
    {
        NativePluginEvents.IssuePluginEvent(PluginEvents.PlatformUI);
    }

    /// <summary>
    /// Does the time warp.
    /// </summary>
    /// <param name="timeWarpViewIndex">Index of the time warp view.</param>
    public void DoTimeWarp(int timeWarpViewIndex)
    {
        NativePluginEvents.IssuePluginEventWithParam(PluginEvents.TimeWarp, timeWarpViewIndex);
    }

    /// <summary>
    /// Ends the eye frame.
    /// </summary>
    /// <param name="eye">The eye.</param>
    public void EndEyeFramne(Eyes eye)
    {
#if UNITY_ANDROID && !UNITY_EDITOR 
    PluginEvents pluginEventType = (eye == Eyes.Left) ?
        PluginEvents.LeftEyeEndFrame :
        PluginEvents.RightEyeEndFrame;
    int eyeSceneTextureId = this.Hmd.GetEyeSceneTextureId(eye);
    int eyeGazeTextureId = this.Hmd.GetEyeGazeTextureId(eye);

    NativePluginEvents.IssuePluginEventWithParam(pluginEventType, ((long)eyeGazeTextureId << 32) + (long)eyeSceneTextureId);
#endif
    }

    /// <summary>
    /// Initializes the render thread.
    /// </summary>
    public void InitRenderThread()
    {
        NativePluginEvents.IssuePluginEvent(PluginEvents.InitRenderThread);
    }

    /// <summary>
    /// Goes to home.
    /// </summary>
    [DllImport(MiConfig.LibName)]
    private static extern void GoHome();

    /// <summary>
    /// Sets the initialize classes.
    /// </summary>
    /// <param name="activity">The activity.</param>
    /// <param name="activityClass">The activity class.</param>
    [DllImport(MiConfig.LibName)]
    private static extern void SetInitClasses(IntPtr activity, IntPtr activityClass);

    /// <summary>
    /// Sets the eye parameters.
    /// </summary>
    /// <param name="width">The width.</param>
    /// <param name="height">The height.</param>
    [DllImport(MiConfig.LibName)]
    private static extern void SetEyeParms(int width, int height);

    /// <summary>
    /// Gets the battery level.
    /// </summary>
    /// <returns>the battery level</returns>
    [DllImport(MiConfig.LibName)]
    private static extern float GetBatteryLevel();

    /// <summary>
    /// Gets the battery status.
    /// </summary>
    /// <returns>the battery status</returns>
    [DllImport(MiConfig.LibName)]
    private static extern int GetBatteryStatus();

    /// <summary>
    /// Gets the battery temperature.
    /// </summary>
    /// <returns>the battery temperature</returns>
    [DllImport(MiConfig.LibName)]
    private static extern float GetBatteryTemperature();

    /// <summary>
    /// Gets the volume.
    /// </summary>
    /// <returns>the volume level</returns>
    [DllImport(MiConfig.LibName)]
    private static extern int GetVolume();

    /// <summary>
    /// Gets the time since last volume change.
    /// </summary>
    /// <returns>the time duration</returns>
    [DllImport(MiConfig.LibName)]
    private static extern double GetTimeSinceLastVolumeChange();

    /// <summary>
    /// Gets the height of the player eye.
    /// </summary>
    /// <param name="eyeHeight">Height of the eye.</param>
    /// <returns>true if success</returns>
    [DllImport(MiConfig.LibName)]
    private static extern bool GetPlayerEyeHeight(ref float eyeHeight);

    /// <summary>
    /// Gets the IPD.
    /// </summary>
    /// <param name="interpupillaryDistance">The distance.</param>
    /// <returns>true if success</returns>
    [DllImport(MiConfig.LibName)]
    private static extern bool GetInterpupillaryDistance(ref float interpupillaryDistance);

    /// <summary>
    /// Awakes this instance.
    /// </summary>
    private void Awake()
    {
        if (Instance != null)
        {
            this.enabled = false;
            MonoBehaviour.DestroyImmediate(this);
            return;
        }

        Instance = this;
#if UNITY_ANDROID && !UNITY_EDITOR
        if (Screen.orientation != ScreenOrientation.LandscapeLeft)
        {
            Debug.LogError("***** Default screen Rotation must be set to landscape left for VR.\n");

            Debug.Break();
            Application.Quit();
        }

        if (Input.gyro.enabled)
        {
            Input.gyro.enabled = false;
        }

        // NOTE: On Adreno Lollipop, it is an error to have antiAliasing set on the
        // main window surface with front buffer rendering enabled. The view will
        // render black.
        // On Adreno KitKat, some tiling control modes will cause the view to render
        // black.
        if (QualitySettings.antiAliasing > 1)
        {
            Debug.LogError("*** Antialiasing must be disabled ***");
        }

        // we sync in the TimeWarpDuration, so we don't want unity
        // syncing elsewhere
        QualitySettings.vSyncCount = 0;

        Application.targetFrameRate = 60;
        Application.runInBackground = false;
        Screen.sleepTimeout = SleepTimeout.NeverSleep;

        if (this.currentActivity == null)
        {
          AndroidJavaClass unityPlayer = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
          this.currentActivity = unityPlayer.GetStatic<AndroidJavaObject>("currentActivity");
          this.javaVrActivityClass = new AndroidJavaClass("com.mi.dlabs.vr.sdk.plugins.unity.MiVRUnityPlayerActivity");

          this.SetInitVariables(this.currentActivity.GetRawObject(), this.javaVrActivityClass.GetRawClass());
        }

        this.prevEyeTextureAntiAliasing = this.EyeTextureAntiAliasing;
        this.prevEyeTextureDepth = this.EyeTextureDepth;
        this.prevEyeTextureFormat = this.EyeTextureFormat;
        this.prevHdr = this.IsHdrEnabled;

        if (this.Hmd == null)
        {
          this.Hmd = new MiHMD();
        }

        if (this.ResetTrackerOnLoad)
        {
          this.Hmd.RecenterHeaderPosition();
        }
#endif
    }

    /// <summary>
    /// Called when [destroy].
    /// </summary>
    private void OnDestroy()
    {
        RenderTexture.active = null;
    }

    /// <summary>
    /// Called when [application quit].
    /// </summary>
    private void OnApplicationQuit()
    {
        Debug.Log("OnApplicationQuit");

        NativePluginEvents.IssuePluginEvent(PluginEvents.QuitRenderThread);
    }

    /// <summary>
    /// Called when [enable].
    /// </summary>
    private void OnEnable()
    {
        this.StartCoroutine(this.TimeWarpCoroutine());
    }

    /// <summary>
    /// Called when [disable].
    /// </summary>
    private void OnDisable()
    {
        this.StopAllCoroutines();
    }

    /// <summary>
    /// Starts this instance.
    /// </summary>
    private void Start()
    {
        if (this.OnConfigureVrModeParms != null)
        {
            this.OnConfigureVrModeParms();
        }
#if UNITY_ANDROID && !UNITY_EDITOR
        MiHMD.EyeParameter leftEyeDesc = this.Hmd.GetEyeParameter(Eyes.Left);
        Vector2 resolution = leftEyeDesc.Resolution;
        SetEyeParms((int)resolution.x, (int)resolution.y);

        this.InitRenderThread();
#endif
    }

    /// <summary>
    /// Updates this instance.
    /// </summary>
    private void Update()
    {
#if UNITY_ANDROID && !UNITY_EDITOR
        if (this.isGoingHome)
        {
            if (this.goingHomeFrameCount == 1)
            {
                this.goingHomeFrameCount++;
                GoHome();
                return;
            }
            else if (this.goingHomeFrameCount < 1)
            {
                this.goingHomeFrameCount++;
            }
            else
            {
                return;
            }
        }

        if (this.HmdLost != null && this.isHmdPresent && !this.Hmd.IsPresent)
        {
            this.HmdLost.Invoke();
        }

        if (this.HmdGained != null && !this.isHmdPresent && this.Hmd.IsPresent)
        {
            this.HmdGained.Invoke();
        }

        this.isHmdPresent = this.Hmd.IsPresent;

        if (this.EyeTextureAntiAliasingModified != null && this.EyeTextureAntiAliasing != this.prevEyeTextureAntiAliasing)
        {
            this.EyeTextureAntiAliasingModified(this.prevEyeTextureAntiAliasing, this.EyeTextureAntiAliasing);
        }

        this.prevEyeTextureAntiAliasing = this.EyeTextureAntiAliasing;

        if (this.EyeTextureDepthModified != null && this.EyeTextureDepth != this.prevEyeTextureDepth)
        {
            this.EyeTextureDepthModified(this.prevEyeTextureDepth, this.EyeTextureDepth);
        }

        this.prevEyeTextureDepth = this.EyeTextureDepth;

        if (this.EyeTextureFormatModified != null && this.EyeTextureFormat != this.prevEyeTextureFormat)
        {
            this.EyeTextureFormatModified(this.prevEyeTextureFormat, this.EyeTextureFormat);
        }

        this.prevEyeTextureFormat = this.EyeTextureFormat;

        if (this.HdrModified != null && this.IsHdrEnabled != this.prevHdr)
        {
            this.HdrModified(this.prevHdr, this.IsHdrEnabled);
        }

        this.prevHdr = this.IsHdrEnabled;

        this.Hmd.Update();
#endif
    }

    /// <summary>
    /// TimeWarp routine.
    /// </summary>
    /// <returns>the IEnumerator</returns>
    private IEnumerator TimeWarpCoroutine()
    {
        while (true)
        {
            yield return new WaitForEndOfFrame();

            this.DoTimeWarp(this.TimeWarpViewIndex);
        }
    }

    /// <summary>
    /// Called when [pause].
    /// </summary>
    private void OnPause()
    {
        this.StopVrMode();
    }

    /// <summary>
    /// Called when [resume].
    /// </summary>
    /// <returns>the enumerator</returns>
    private IEnumerator OnResume()
    {
        yield return null; // delay 1 frame to allow Unity enough time to create the windowSurface
        yield return null;
        yield return null; // delay 3 frame to fix bug: when first lauch, egl will throw exception: native_window_api_connect failed

        if (this.OnApplicationResumed != null)
        {
            this.OnApplicationResumed();
        }

        this.StartVrMode();
    }

    /// <summary>
    /// Called when [application pause].
    /// </summary>
    /// <param name="pause">if set to <c>true</c> [pause].</param>
    private void OnApplicationPause(bool pause)
    {
        if (pause)
        {
            this.OnPause();
        }
        else
        {
            this.StartCoroutine(this.OnResume());
        }
    }

    /// <summary>
    /// Contains information about the user's preferences and body dimensions.
    /// </summary>
    public struct EyeProfile
    {
        /// <summary>
        /// The interpupillary distance
        /// </summary>
        [SuppressMessage("StyleCop.CSharp.DocumentationRules", "SA1650:ElementDocumentationMustBeSpelledCorrectly", Justification = "Reviewed.")]
        public float Ipd;

        /// <summary>
        /// The eye height
        /// </summary>
        public float EyeHeight;

        /// <summary>
        /// The eye depth
        /// </summary>
        public float EyeDepth;

        /// <summary>
        /// The neck height
        /// </summary>
        public float NeckHeight;
    }
}
