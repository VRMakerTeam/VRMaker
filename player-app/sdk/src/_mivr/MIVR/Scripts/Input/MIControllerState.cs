//-----------------------------------------------------------------------
// <copyright file="ControllerState.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

using UnityEngine;
using UnityEngineInternal;

/// <summary>
/// the controller connection state
/// </summary>
public enum MIConnectionState
{
    /// <summary>
    /// controller is disconnected.
    /// </summary>
    Disconnected,

    /// <summary>
    /// device is scanning the controller.
    /// </summary>
    Scanning,

    /// <summary>
    /// The device is connecting the controller.
    /// </summary>
    Connecting,

    /// <summary>
    /// The controller is connected.
    /// </summary>
    Connected,

    /// <summary>
    /// The error
    /// </summary>
    Error
}

/// <summary>
/// the controller states
/// </summary>
public class MIControllerState
{
    /// <summary>
    /// Gets or sets the state of the connection.
    /// </summary>
    /// <value>
    /// The state of the connection.
    /// </value>
    public MIConnectionState ConnectionState { get; set; }

    /// <summary>
    /// Gets or sets a value indicating whether this instance is touching.
    /// </summary>
    /// <value>
    /// <c>true</c> if this instance is touching; otherwise, <c>false</c>.
    /// </value>
    public bool IsTouching { get; set; }

    /// <summary>
    /// Gets or sets a value indicating whether [touch down].
    /// </summary>
    /// <value>
    ///   <c>true</c> if [touch down]; otherwise, <c>false</c>.
    /// </value>
    public bool TouchDown { get; set; }

    /// <summary>
    /// Gets or sets a value indicating whether [touch up].
    /// </summary>
    /// <value>
    ///   <c>true</c> if [touch up]; otherwise, <c>false</c>.
    /// </value>
    public bool TouchUp { get; set; }

    /// <summary>
    /// Gets or sets the touch position.
    /// </summary>
    /// <value>
    /// The touch position.
    /// </value>
    public Vector2 TouchPosition { get; set; }

    /// <summary>
    /// Gets or sets the orientation.
    /// </summary>
    /// <value>
    /// The orientation.
    /// </value>
    public Quaternion Orientation { get; set; }

    /// <summary>
    /// Gets or sets a value indicating whether [click button state].
    /// </summary>
    /// <value>
    ///   <c>true</c> if [click button state]; otherwise, <c>false</c>.
    /// </value>
    public bool ClickButtonState { get; set; }

    /// <summary>
    /// Gets or sets a value indicating whether [click button down].
    /// </summary>
    /// <value>
    ///   <c>true</c> if [click button down]; otherwise, <c>false</c>.
    /// </value>
    public bool ClickButtonDown { get; set; }

    /// <summary>
    /// Gets or sets a value indicating whether [click button up].
    /// </summary>
    /// <value>
    ///   <c>true</c> if [click button up]; otherwise, <c>false</c>.
    /// </value>
    public bool ClickButtonUp { get; set; }

    /// <summary>
    /// Gets or sets a value indicating whether [application button state].
    /// </summary>
    /// <value>
    /// <c>true</c> if [application button state]; otherwise, <c>false</c>.
    /// </value>
    public bool AppButtonState { get; set; }

    /// <summary>
    /// Gets or sets a value indicating whether [application button down].
    /// </summary>
    /// <value>
    /// <c>true</c> if [application button down]; otherwise, <c>false</c>.
    /// </value>
    public bool AppButtonDown { get; set; }

    /// <summary>
    /// Gets or sets a value indicating whether [application button up].
    /// </summary>
    /// <value>
    ///   <c>true</c> if [application button up]; otherwise, <c>false</c>.
    /// </value>
    public bool AppButtonUp { get; set; }

    /// <summary>
    /// Gets or sets a value indicating whether this <see cref="ControllerState"/> is recentered.
    /// </summary>
    /// <value>
    ///   <c>true</c> if recentered; otherwise, <c>false</c>.
    /// </value>
    public bool Recentered { get; set; }
}
