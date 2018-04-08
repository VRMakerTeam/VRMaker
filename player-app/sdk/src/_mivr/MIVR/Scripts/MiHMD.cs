//-----------------------------------------------------------------------
// <copyright file="MiHMD.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

using System.Runtime.InteropServices;
using UnityEngine;

/// <summary>
/// Selects a human eye.
/// </summary>
public enum Eyes
{
  /// <summary>
  /// The center
  /// </summary>
  Center = -1,

  /// <summary>
  /// The left
  /// </summary>
  Left = 0,

  /// <summary>
  /// The right
  /// </summary>
  Right = 1,

  /// <summary>
  /// The count
  /// </summary>
  Count = 2
}

/// <summary>
/// the head-mounted HMD
/// </summary>
public class MiHMD
{
  /// <summary>
  /// The eye texture count. Each eye has two textures. One for scene, one for gaze cursor.
  /// </summary>
  private const int EyeTextureCount = 3 * (int)Eyes.Count * 2;

  /// <summary>
  /// The eye parameters
  /// </summary>
  private readonly EyeParameter[] eyeParameters = new EyeParameter[(int)Eyes.Count];

  /// <summary>
  /// The eye textures
  /// </summary>
  private readonly RenderTexture[] eyeTextures = new RenderTexture[EyeTextureCount];

  /// <summary>
  /// The eye texture ids
  /// </summary>
  private readonly int[] eyeTextureIds = new int[EyeTextureCount];

  /// <summary>
  /// The current eye texture index
  /// </summary>
  private int currentEyeTextureIdx = 0;

  /// <summary>
  /// The next eye texture index
  /// </summary>
  private int nextEyeTextureIdx = 0;

  /// <summary>
  /// The w axis
  /// </summary>
  private float w = 0.0f, x = 0.0f, y = 0.0f, z = 0.0f, fov = 90.0f;

  /// <summary>
  /// Initializes a new instance of the <see cref="MiHMD"/> class.
  /// </summary>
  public MiHMD()
  {
    this.InitEyeParameters(Eyes.Left);
    this.InitEyeParameters(Eyes.Right);

    for (int i = 0; i < EyeTextureCount; i += 4)
    {
      this.InitEyeTexture(i, Eyes.Left);
      this.InitEyeTexture(i, Eyes.Right);
    }
  }

  /// <summary>
  /// Occurs when the head pose is reset.
  /// </summary>
  public event System.Action RecenteredHeadPosition;

  /// <summary>
  /// Gets a value indicating whether this instance is present.
  /// </summary>
  /// <value>
  /// <c>true</c> if this instance is present; otherwise, <c>false</c>.
  /// </value>
  public bool IsPresent
  {
    get
    {
      return IsHMDPresent();
    }
  }

  /// <summary>
  /// Gets the current measured Latency values.
  /// </summary>
  public LatencyData Latency
  {
    get
    {
      return new LatencyData
      {
        RenderDuration = 0.0f,
        TimeWarpDuration = 0.0f,
        PostDuration = 0.0f
      };
    }
  }

  /// <summary>
  /// Updates this instance.
  /// </summary>
  public void Update()
  {
    this.UpdateEyeTextureIndex();
  }

  /// <summary>
  /// Gets the head pose at the current time or predicted at the given time.
  /// </summary>
  /// <param name="predictionTime">The prediction time.</param>
  /// <returns>the head transform</returns>
  public MiTransform GetHeadTransform(double predictionTime = 0d)
  {
    float px = 0.0f, py = 0.0f, pz = 0.0f, ow = 0.0f, ox = 0.0f, oy = 0.0f, oz = 0.0f;

    double currentTime = Time.time + predictionTime;
    GetCameraPositionOrientation(ref px, ref py, ref pz, ref ox, ref oy, ref oz, ref ow, currentTime);

    return new MiTransform
    {
      Position = new Vector3(px, py, -pz),
      Rotation = new Quaternion(-ox, -oy, oz, ow),
    };
  }

  /// <summary>
  /// Gets the pose of the given eye, predicted for the time when the current frame will scan out.
  /// </summary>
  /// <param name="eye">The eye.</param>
  /// <returns>the eye transform</returns>
  public MiTransform GetEyeTransform(Eyes eye)
  {
    if (eye == Eyes.Left)
    {
      var index = 0;
      GetSensorData(
        false,
        ref this.w,
        ref this.x,
        ref this.y,
        ref this.z,
        ref this.fov,
        ref index);
      VrManager.Instance.TimeWarpViewIndex = index;
    }

    Quaternion rotation = new Quaternion(-this.x, -this.y, this.z, this.w);

    float eyeOffsetX = 0.5f * VrManager.Instance.Profile.Ipd;
    eyeOffsetX = (eye == Eyes.Left) ? -eyeOffsetX : eyeOffsetX;

    float neckToEyeHeight = VrManager.Instance.Profile.EyeHeight - VrManager.Instance.Profile.NeckHeight;
    Vector3 headNeckModel = new Vector3(0.0f, neckToEyeHeight, VrManager.Instance.Profile.EyeDepth);
    //// Vector3 position = rotation * (new Vector3(eyeOffsetX, 0.0f, 0.0f) + headNeckModel);
    //// 在外层调用已经把rotation算进父级节点里面了，所以这里返回的position不需要再做一次旋转操作.
    Vector3 position = new Vector3(eyeOffsetX, 0.0f, 0.0f) + headNeckModel;
    //// Subtract the HNM pivot to avoid translating the camera when level.
    //// position -= headNeckModel;

    return new MiTransform
    {
      Position = position,
      Rotation = rotation,
    };
  }

  /// <summary>
  /// reset the header position to center.
  /// </summary>
  public void RecenterHeaderPosition()
  {
    ResetSensorOrientation();

    var handler = this.RecenteredHeadPosition;

    if (handler != null)
    {
      handler.Invoke();
    }
  }

  /// <summary>
  /// Gets the resolution and field of view for the given eye.
  /// </summary>
  /// <param name="eye">The eye.</param>
  /// <returns>the eye parameter</returns>
  public EyeParameter GetEyeParameter(Eyes eye)
  {
    return this.eyeParameters[(int)eye];
  }

  /// <summary>
  /// Gets the currently active render texture for the given eye.
  /// </summary>
  /// <param name="eye">The eye.</param>
  /// <returns>the eye's scene texture</returns>
  public RenderTexture GetEyeSceneTexture(Eyes eye)
  {
    return this.eyeTextures[this.currentEyeTextureIdx + ((int)eye * 2)];
  }

  /// <summary>
  /// Gets the currently active render texture for the given eye.
  /// </summary>
  /// <param name="eye">The eye.</param>
  /// <returns>the eye's gaze texture</returns>
  public RenderTexture GetEyeGazeTexture(Eyes eye)
  {
    return this.eyeTextures[this.currentEyeTextureIdx + (((int)eye * 2) + 1)];
  }

    /// <summary>
    /// Gets the currently active render texture's native ID for the given eye.
    /// </summary>
    /// <param name="eye">The eye.</param>
    /// <returns>the eye's scene texture id</returns>
    public int GetEyeSceneTextureId(Eyes eye)
  {
    return this.eyeTextureIds[this.currentEyeTextureIdx + ((int)eye * 2)];
  }

  /// <summary>
  /// Gets the currently active render texture's native ID for the given eye.
  /// </summary>
  /// <param name="eye">The eye.</param>
  /// <returns>the eye's gaze texture id</returns>
  public int GetEyeGazeTextureId(Eyes eye)
  {
    return this.eyeTextureIds[this.currentEyeTextureIdx + (((int)eye * 2) + 1)];
  }

    /// <summary>
    /// Resets the sensor orientation.
    /// </summary>
    /// <returns>true if success</returns>
    [DllImport(MiConfig.LibName)]
  private static extern bool ResetSensorOrientation();

  /// <summary>
  /// Determines whether [is HMD present].
  /// </summary>
  /// <returns>true if HMD is active</returns>
  [DllImport(MiConfig.LibName)]
  private static extern bool IsHMDPresent();

  /// <summary>
  /// Gets the camera position orientation.
  /// </summary>
  /// <param name="posX">The position X.</param>
  /// <param name="posY">The position Y.</param>
  /// <param name="posZ">The position Z.</param>
  /// <param name="rotationX">The rotationX.</param>
  /// <param name="rotationY">The rotationY.</param>
  /// <param name="rotationZ">The rotationZ.</param>
  /// <param name="rotationW">The rotationW.</param>
  /// <param name="curTime">At time.</param>
  /// <returns>true if success</returns>
  [DllImport(MiConfig.LibName)]
  private static extern bool GetCameraPositionOrientation(
      ref float posX,
      ref float posY,
      ref float posZ,
      ref float rotationX,
      ref float rotationY,
      ref float rotationZ,
      ref float rotationW,
      double curTime);

  /// <summary>
  /// Gets the sensor data.
  /// </summary>
  /// <param name="monoscopic">whether use single eye</param>
  /// <param name="w">The w.</param>
  /// <param name="x">The x.</param>
  /// <param name="y">The y.</param>
  /// <param name="z">The z.</param>
  /// <param name="fov">The FOV.</param>
  /// <param name="viewNumber">The view number.</param>
  [DllImport(MiConfig.LibName)]
  private static extern void GetSensorData(
      bool monoscopic,
      ref float w,
      ref float x,
      ref float y,
      ref float z,
      ref float fov,
      ref int viewNumber);

  /// <summary>
  /// Updates the index of the eye texture.
  /// </summary>
  private void UpdateEyeTextureIndex()
  {
    this.currentEyeTextureIdx = this.nextEyeTextureIdx;
    this.nextEyeTextureIdx = (this.nextEyeTextureIdx + 4) % EyeTextureCount;
  }

  /// <summary>
  /// Initializes the eye parameters.
  /// </summary>
  /// <param name="eye">The eye.</param>
  private void InitEyeParameters(Eyes eye)
  {
    Vector2 textureSize = new Vector2(1024, 1024);
    Vector2 fovSize = new Vector2(90, 90);

    this.eyeParameters[(int)eye] = new EyeParameter()
    {
      Resolution = textureSize,
      Fov = fovSize
    };
  }

  /// <summary>
  /// Initializes the eye texture.
  /// </summary>
  /// <param name="index">The index.</param>
  /// <param name="eye">The eye.</param>
  private void InitEyeTexture(int index, Eyes eye)
    {
        EyeParameter eyeParameter = this.eyeParameters[(int)eye];

        // SceneTexture
        int eyeIndex = index + ((int)eye * 2);
        this.eyeTextures[eyeIndex] = new RenderTexture(
            (int)eyeParameter.Resolution.x,
            (int)eyeParameter.Resolution.y,
            (int)VrManager.Instance.EyeTextureDepth,
            VrManager.Instance.EyeTextureFormat)
        {
            antiAliasing = (int)VrManager.Instance.EyeTextureAntiAliasing
        };
        this.eyeTextures[eyeIndex].Create();
        this.eyeTextureIds[eyeIndex] = this.eyeTextures[eyeIndex].GetNativeTexturePtr().ToInt32();

        // GazeTexture
        eyeIndex++;
        this.eyeTextures[eyeIndex] = new RenderTexture(
            (int)eyeParameter.Resolution.x,
            (int)eyeParameter.Resolution.y,
            (int)VrManager.Instance.EyeTextureDepth,
            VrManager.Instance.EyeTextureFormat)
        {
            antiAliasing = (int)VrManager.Instance.EyeTextureAntiAliasing
        };
        this.eyeTextures[eyeIndex].Create();
        this.eyeTextureIds[eyeIndex] = this.eyeTextures[eyeIndex].GetNativeTexturePtr().ToInt32();
    }

  /// <summary>
  /// Specifies the size and field-of-view for one eye texture.
  /// </summary>
  public struct EyeParameter
  {
    /// <summary>
    /// The horizontal and vertical size of the texture.
    /// </summary>
    public Vector2 Resolution;

    /// <summary>
    /// The angle of the horizontal and vertical field of view in degrees.
    /// </summary>
    public Vector2 Fov;
  }

  /// <summary>
  /// Contains Latency measurements for a single frame of rendering.
  /// </summary>
  public struct LatencyData
  {
    /// <summary>
    /// Gets the time it took to render both eyes in seconds.
    /// </summary>
    public float RenderDuration;

    /// <summary>
    /// Gets the time it took to perform TimeWarpDuration in seconds.
    /// </summary>
    public float TimeWarpDuration;

    /// <summary>
    /// Gets the time between the end of TimeWarp and scan-out in seconds.
    /// </summary>
    public float PostDuration;
  }
}
