//-----------------------------------------------------------------------
// <copyright file="NativePluginEvents.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

using System;
using System.Runtime.InteropServices;
using UnityEngine;

/// <summary>
/// the plugin events enum
/// </summary>
public enum PluginEvents
{
    /// <summary>
    /// The initialize render thread
    /// </summary>
    InitRenderThread = 0,

    /// <summary>
    /// The pause
    /// </summary>
    Pause = 1,

    /// <summary>
    /// The resume
    /// </summary>
    Resume = 2,

    /// <summary>
    /// The left eye end frame
    /// </summary>
    LeftEyeEndFrame = 3,

    /// <summary>
    /// The right eye end frame
    /// </summary>
    RightEyeEndFrame = 4,

    /// <summary>
    /// The time warp
    /// </summary>
    TimeWarp = 5,

    /// <summary>
    /// The platform UI
    /// </summary>
    PlatformUI = 6,

    /// <summary>
    /// The platform UI confirm quit
    /// </summary>
    PlatformUIConfirmQuit = 7,

    /// <summary>
    /// The reset VR mode parameter
    /// </summary>
    ResetVrModeParams = 8,

    /// <summary>
    /// The platform UI tutorial
    /// </summary>
    PlatformUITutorial = 9,

    /// <summary>
    /// The shutdown render thread
    /// </summary>
    QuitRenderThread = 10,
}

/// <summary>
/// Send user-defined events to a native code plugin
/// </summary>
public static class NativePluginEvents
{
    /// <summary>
    /// Issues the plugin event.
    /// </summary>
    /// <param name="pluginEventType">Type of the plugin event.</param>
    public static void IssuePluginEvent(PluginEvents pluginEventType)
    {
#if UNITY_ANDROID && !UNITY_EDITOR
        GL.IssuePluginEvent(GetRenderEventFunc(), (int)pluginEventType);
#endif
    }

    /// <summary>
    /// Issues the plugin event with parameter.
    /// </summary>
    /// <param name="pluginEventType">Type of the plugin event.</param>
    /// <param name="param">The parameter.</param>
    public static void IssuePluginEventWithParam(PluginEvents pluginEventType, long param)
    {
#if UNITY_ANDROID && !UNITY_EDITOR
        //SetEventData((int)pluginEventType, param);

        uint data = 0x80000000 | ((uint) pluginEventType << 25) | 
            (uint)(param & 0x00000000FFFFFFFF) ;
        GL.IssuePluginEvent(GetRenderEventFunc(), (int)data);

        data = 0x80000000 | 0x40000000 | ((uint)pluginEventType << 25) |
            (uint)(((uint)(param >> 32) & 0x00000000FFFFFFFF) );
        GL.IssuePluginEvent(GetRenderEventFunc(), (int)data);

        GL.IssuePluginEvent(GetRenderEventFunc(), (int)pluginEventType);
#endif
    }

    /// <summary>
    /// Enable the split line.
    /// </summary>
    /// <param name="enabled">Whether enable split line.</param>
    [DllImport(MiConfig.LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void EnableSplitLine(int enabled);

    /// <summary>
    /// Gets the render event function.
    /// </summary>
    /// <returns>the function point</returns>
    [DllImport(MiConfig.LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr GetRenderEventFunc();

    /// <summary>
    /// Sets the event data.
    /// </summary>
    /// <param name="eventId">The event identifier.</param>
    /// <param name="eventData">The event data.</param>
    [DllImport(MiConfig.LibName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void SetEventData(int eventId, long eventData);
}
