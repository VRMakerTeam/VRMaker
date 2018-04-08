//-----------------------------------------------------------------------
// <copyright file="ControllerSample.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

using System.Text;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Sample scene
/// </summary>
public class ControllerSample : MonoBehaviour
{
#if UNITY_ANDROID && !UNITY_EDITOR
    /// <summary>
    /// The text UI;
    /// </summary>
    private Text textUI;

    /// <summary>
    /// Tht touch down count.
    /// </summary>
    private int touchDownCount = 0;

    /// <summary>
    /// the touch up count
    /// </summary>
    private int touchUpCount = 0;

    /// <summary>
    /// the click count
    /// </summary>
    private int clickDownCount = 0;

    /// <summary>
    /// the click up count
    /// </summary>
    private int clickUpCount = 0;

    /// <summary>
    /// the app button down count.
    /// </summary>
    private int appDownCount = 0;

    /// <summary>
    /// the app up count
    /// </summary>
    private int appUpCount = 0;

    /// <summary>
    /// the app up count
    /// </summary>
    private int recenteredCount = 0;

    /// <summary>
    /// The mi camera
    /// </summary>
    private GameObject miCamera;

    /// <summary>
    /// The line
    /// </summary>
    private GameObject controller;

    /// <summary>
    /// start the script
    /// </summary>
    private void Start()
    {
        this.textUI = this.GetComponent<Text>();
        this.miCamera = GameObject.Find("/MiCamera");
        this.controller = GameObject.Find("/Controller");
    }

    /// <summary>
    /// update the script
    /// </summary>
    private void Update()
    {
        MIControllerState state = MIInputManager.ControllerState;
        this.touchDownCount += state.TouchDown ? 1 : 0;
        this.touchUpCount += state.TouchUp ? 1 : 0;
        this.clickDownCount += state.ClickButtonDown ? 1 : 0;
        this.clickUpCount += state.ClickButtonUp ? 1 : 0;
        this.appDownCount += state.AppButtonDown ? 1 : 0;
        this.appUpCount += state.AppButtonUp ? 1 : 0;
        this.recenteredCount += state.Recentered ? 1 : 0;

        StringBuilder result = new StringBuilder();
        result.AppendFormat("connection state: {0}", state.ConnectionState);
        result.AppendLine();
        result.AppendFormat("IsTouching {0}", state.IsTouching);
        result.AppendLine();
        result.AppendFormat("TouchDown count: {0}", this.touchDownCount);
        result.AppendLine();
        result.AppendFormat("TouchUp count: {0}", this.touchUpCount);
        result.AppendLine();
        result.AppendFormat("Touch position x: {0}, y: {1}", state.TouchPosition.x, state.TouchPosition.y);
        result.AppendLine();
        result.AppendFormat("Click button state is {0}", state.ClickButtonState);
        result.AppendLine();
        result.AppendFormat("Click button down count is {0}", this.clickDownCount);
        result.AppendLine();
        result.AppendFormat("Click button up count is {0}", this.clickUpCount);
        result.AppendLine();
        result.AppendFormat("App button state is {0}", state.AppButtonState);
        result.AppendLine();
        result.AppendFormat("App button down count is {0}", this.appDownCount);
        result.AppendLine();
        result.AppendFormat("App button up count is {0}", this.appUpCount);
        result.AppendLine();
        result.AppendFormat("Recentered count is {0}", this.recenteredCount);
        result.AppendLine();

        this.textUI.text = result.ToString();

        this.controller.transform.rotation = state.Orientation * new Quaternion(0,1,0,0);
    }
#endif
}
