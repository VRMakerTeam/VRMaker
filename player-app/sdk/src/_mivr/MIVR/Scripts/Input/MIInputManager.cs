//-----------------------------------------------------------------------
// <copyright file="InputManager.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

using System.Collections;
using System.Runtime.InteropServices;
using UnityEngine;

/// <summary>
/// the input manager for get HMD and controller input
/// </summary>
public class MIInputManager : MonoBehaviour
{
    /// <summary>
    /// The controller button count
    /// </summary>
    private const int ControllerButtonCount = 2;

    /// <summary>
    /// The instance
    /// </summary>
    private static MIInputManager instance;

    /// <summary>
    /// The HMD state
    /// </summary>
    private HmdState hmdState;

    /// <summary>
    /// The controller state
    /// </summary>
    private NativeControllerState controllerState;

    /// <summary>
    /// Gets a value indicating whether [HMD button up].
    /// </summary>
    /// <value>
    ///   <c>true</c> if [HMD button down]; otherwise, <c>false</c>.
    /// </value>
    public static bool HmdButtonUp
    {
        get { return instance != null && (instance.hmdState.ButtonUp == 0x01); }
    }

    /// <summary>
    /// Gets the state of the controller.
    /// </summary>
    /// <value>
    /// The state of the controller.
    /// </value>
    public static MIControllerState ControllerState
    {
        get
        {
            return ConvertState(instance.controllerState);
        }
    }

    /// <summary>
    /// Reads the state of the HMD.
    /// </summary>
    /// <param name="state">The state.</param>
    [DllImport(MiConfig.LibName)]
    private static extern void ReadHMDState(ref HmdState state);

    /// <summary>
    /// Reads the state of the controller.
    /// </summary>
    /// <param name="state">The state.</param>
    [DllImport(MiConfig.LibName)]
    private static extern void ReadControllerState(ref NativeControllerState state);

    /// <summary>
    /// Converts the state.
    /// </summary>
    /// <param name="nativeState">State of the native.</param>
    /// <returns>the controller state</returns>
    private static MIControllerState ConvertState(NativeControllerState nativeState)
    {
        MIControllerState state = new MIControllerState();
#if UNITY_ANDROID && !UNITY_EDITOR
        switch (nativeState.ConnectionState)
        {
            case 0:
                state.ConnectionState = MIConnectionState.Disconnected;
                break;
            case 1:
                state.ConnectionState = MIConnectionState.Connected;
                break;
            case 2:
                state.ConnectionState = MIConnectionState.Connecting;
                break;
            default:
                state.ConnectionState = MIConnectionState.Disconnected;
                break;
        }

        state.IsTouching = nativeState.IsTouching == 0x01;
        state.TouchDown = nativeState.TouchDown == 0x01;
        state.TouchUp = nativeState.TouchUp == 0x01;
        state.TouchPosition = new Vector2(nativeState.TouchPos.X, nativeState.TouchPos.Y);
        state.Orientation = new Quaternion(nativeState.Orientation.X, nativeState.Orientation.Y, nativeState.Orientation.Z, nativeState.Orientation.W);

        const int ClickButtonIndex = 0;
        state.ClickButtonState = nativeState.ButtonState[ClickButtonIndex] == 0x01;
        state.ClickButtonDown = nativeState.ButtonDown[ClickButtonIndex] == 0x01;
        state.ClickButtonUp = nativeState.ButtonUp[ClickButtonIndex] == 0x01;

        const int AppButtonIndex = 1;
        state.AppButtonState = nativeState.ButtonState[AppButtonIndex] == 0x01;
        state.AppButtonDown = nativeState.ButtonDown[AppButtonIndex] == 0x01;
        state.AppButtonUp = nativeState.ButtonUp[AppButtonIndex] == 0x01;
        state.Recentered = nativeState.Recentered == 0x01;
#endif
        return state;
    }

    /// <summary>
    /// Starts this instance.
    /// </summary>
    private void Start()
    {
    }

    /// <summary>
    /// Awakes this instance.
    /// </summary>
    private void Awake()
    {
        if (instance != null)
        {
            Debug.LogError("More than one InputMnager instance was found in your scene. ");
            this.enabled = false;
            return;
        }

        instance = this;
        this.hmdState.ButtonUp = 0x0;
    }

    /// <summary>
    /// Called when [destroy].
    /// </summary>
    private void OnDestroy()
    {
        instance = null;
    }

    /// <summary>
    /// Updates the controller.
    /// </summary>
    private void UpdateStates()
    {
#if UNITY_ANDROID && !UNITY_EDITOR
        ReadHMDState(ref this.hmdState);
        ReadControllerState(ref this.controllerState);
#endif
    }

    /// <summary>
    /// Called when [enable].
    /// </summary>
    private void OnEnable()
    {
        this.StartCoroutine("UpdateStatesEndOfFrame");
    }

    /// <summary>
    /// Called when [disable].
    /// </summary>
    private void OnDisable()
    {
        this.StopCoroutine("UpdateStatesEndOfFrame");
    }

    /// <summary>
    /// Update states
    /// </summary>
    /// <returns>the enumerator</returns>
    private IEnumerator UpdateStatesEndOfFrame()
    {
        while (true)
        {
            this.UpdateStates();
            yield return new WaitForEndOfFrame();
        }
    }

    /// <summary>
    /// The HMD state
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    private struct HmdState
    {
        /// <summary>
        /// The button up
        /// </summary>
        public byte ButtonUp;
    }

    /// <summary>
    /// the QUAT
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    private struct Quatf
    {
        /// <summary>
        /// The X value
        /// </summary>
        internal float X;

        /// <summary>
        /// The Y Value
        /// </summary>
        internal float Y;

        /// <summary>
        /// The Z value
        /// </summary>
        internal float Z;

        /// <summary>
        /// The W value
        /// </summary>
        internal float W;
    }

    /// <summary>
    /// the vector 2f
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    private struct Vector2f
    {
        /// <summary>
        /// the x
        /// </summary>
        internal float X;

        /// <summary>
        /// The y
        /// </summary>
        internal float Y;
    }

    /// <summary>
    /// The controller state
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    private struct NativeControllerState
    {
        /// <summary>
        /// The button state
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = ControllerButtonCount)]
        public byte[] ButtonState;

        /// <summary>
        /// The button down
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = ControllerButtonCount)]
        public byte[] ButtonDown;

        /// <summary>
        /// The button up
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = ControllerButtonCount)]
        public byte[] ButtonUp;

        /// <summary>
        /// whether is touching
        /// </summary>
        public byte IsTouching;

        /// <summary>
        /// The touch down
        /// </summary>
        public byte TouchDown;

        /// <summary>
        /// The touch up
        /// </summary>
        public byte TouchUp;

        /// <summary>
        /// The touch position
        /// </summary>
        public Vector2f TouchPos;

        /// <summary>
        /// The orientation
        /// </summary>
        public Quatf Orientation;

        /// <summary>
        /// The connection state
        /// </summary>
        public int ConnectionState;

        /// <summary>
        /// The recenteredd
        /// </summary>
        public byte Recentered;
    }
}
