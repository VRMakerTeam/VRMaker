//-----------------------------------------------------------------------
// <copyright file="PostRender.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

using System;
using UnityEngine;

/// <summary>
/// Post render class
/// </summary>
public class PostRender : MonoBehaviour
{
    /// <summary>
    /// Called when [post render].
    /// </summary>
    private void OnPostRender()
    {
#if UNITY_ANDROID && !UNITY_EDITOR
        Eyes eye = ((PluginEvents)Camera.current.depth == PluginEvents.RightEyeEndFrame) ?
            Eyes.Right : Eyes.Left;
        VrManager.Instance.EndEyeFramne(eye);
#endif
    }
}