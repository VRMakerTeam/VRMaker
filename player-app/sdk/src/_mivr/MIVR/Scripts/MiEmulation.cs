//-----------------------------------------------------------------------
// <copyright file="MiEmulation.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------
#if UNITY_EDITOR || UNITY_STANDALONE_WIN
using System;
using UnityEngine;

/// <summary>
/// Emulation to be only used in unity in editor mode
/// </summary>
public class MiEmulation
{
    /// <summary>
    /// Interpupillary Distance
    /// </summary>
    public float ipd = 0.06f;

    /// <summary>
    /// device max fov: outer/upper/inner/lower
    /// </summary>
    public Vector4 lensesMaxFOV = new Vector4(60.0f, 60.0f, 60.0f, 60.0f);

    /// <summary>
    /// screen size: width/height/border
    /// </summary>
    public Vector3 screenSize = new Vector3(0.11f, 0.062f, 0.004f);

    /// <summary>
    /// Distance from lens center to the phone screen.
    /// </summary>
    public float screenDistance = 0.039f;

    /// <summary>
    /// Distortion: k1/k2
    /// </summary>
    public Vector2 distortion = new Vector2(0.34f, 0.55f);

    /// <summary>
    /// Head rotation
    /// </summary>
    public Vector3 headEulerAngles = new Vector3(0.0f, 0.0f, 0.0f);

    /// <summary>
    /// Render Target for Stereo in Editor mode.
    /// </summary>
    private RenderTexture stereoRT;
	
    /// <summary>
    /// Singleton
    /// </summary>
    private static MiEmulation instance;

    /// <summary>
    /// Gets vertical offset of the lens centers from the screen center.
    /// </summary>
    /// <value>
    /// Can not set.
    /// </value>
    public float VerticalLensOffset
    {
        get
        {
            return 0.035f - screenSize.z - screenSize.y * 0.5f;
        }
    }

    /// <summary>
    /// Gets the Stereo RenderTarget.
    /// </summary>
    /// <value>
    /// RenderTexture.
    /// </value>
    public RenderTexture StereoRT
    {
        get
        {
            if (!SystemInfo.supportsRenderTextures)
            {
                Debug.LogError("The system does not support the render textures.");
                return null;
            }
            if (stereoRT == null)
            {
                stereoRT = new RenderTexture(Screen.width, Screen.height, (int)VrManager.Instance.EyeTextureDepth, VrManager.Instance.EyeTextureFormat);
                stereoRT.anisoLevel = 0;
                stereoRT.antiAliasing = Mathf.Max(QualitySettings.antiAliasing, 1);
                stereoRT.Create();
            }

            return stereoRT;
        }
        set
        {
            if (stereoRT == value)
            {
                return;
            }

            if (stereoRT)
            {
                stereoRT.Release();
            }
            stereoRT = value;
        }
    }

    /// <summary>
    /// Singleton
    /// </summary>
    /// <value>
    /// Can not set.
    /// </value>
    public static MiEmulation Instance 
    {
        get 
        {
            if (instance == null) 
            {
                instance = new MiEmulation();
            }

            return instance;
        }
    }

    /// <summary>
    /// Get lens distortd or undistorted frustums.
    /// </summary>
    /// <param name="frustum">lens frustums.</param>
    /// <param name="isSeeFromLens">whether see from lens.</param>
    public void GetFrustum(float[] frustum, bool isSeeFromLens)
    {
        // Use MaxFov to calc angles
        float fovLeft = Mathf.Tan(-lensesMaxFOV.x * Mathf.Deg2Rad);
        float fovTop = Mathf.Tan(lensesMaxFOV.y * Mathf.Deg2Rad);
        float fovRight = Mathf.Tan(lensesMaxFOV.z * Mathf.Deg2Rad);
        float fovBottom = Mathf.Tan(-lensesMaxFOV.w * Mathf.Deg2Rad);

        if (!isSeeFromLens)
        {
            fovLeft = DistortInverse(fovLeft);
            fovTop = DistortInverse(fovTop);
            fovRight = DistortInverse(fovRight);
            fovBottom = DistortInverse(fovBottom);
        }

        // In the horizontal direction, it has two eye views
        float halfWidth = screenSize.x / 4;
        float halfHeight = screenSize.y / 2;

        // Calculate viewport center position
        float viewportCenterX = ipd * 0.5f - halfWidth;
        float viewportCenterY = -VerticalLensOffset;
        float viewportCenterZ = screenDistance;

        // Viewport angles
        float viewportLeft = (viewportCenterX - halfWidth) / viewportCenterZ;
        float viewportTop = (viewportCenterY + halfHeight) / viewportCenterZ;
        float viewportRight = (viewportCenterX + halfWidth) / viewportCenterZ;
        float viewportBottom = (viewportCenterY - halfHeight) / viewportCenterZ;
		
		if (isSeeFromLens)
		{
			viewportLeft = Distort(viewportLeft);
			viewportTop = Distort(viewportTop);
			viewportRight = Distort(viewportRight);
			viewportBottom = Distort(viewportBottom);
		}

        frustum[0] = Math.Max(fovLeft, viewportLeft);
        frustum[1] = Math.Min(fovTop, viewportTop);
        frustum[2] = Math.Min(fovRight, viewportRight);
        frustum[3] = Math.Max(fovBottom, viewportBottom);
    }

    /// <summary>
    /// Distortion
    /// </summary>
    /// <param name="r">value to be distorted.</param>
    /// <returns>distorted result</returns>
    public float Distort(float r)
    {
        float r2 = r * r;
        return ((distortion.x * r2 + distortion.x) * r2 + 1.0f) * r;
    }

    // <summary>
    /// Inverse Distortion
    /// </summary>
    /// <param name="radius">distorted value to be inversed.</param>
    /// <returns>Secant method, inverse distorted result</returns>
    public float DistortInverse(float radius)
    {
        float r0 = 0.0f;
        float r1 = 1.0f;
        float delta0 = radius - Distort(r0);
        while (Mathf.Abs(r1 - r0) > 0.0001f)
        {
            float delta1 = radius - Distort(r1);
            float r2 = r1 - delta1 * ((r1 - r0) / (delta1 - delta0));
            r0 = r1;
            r1 = r2;
            delta0 = delta1;
        }
        return r1;
    }

    /// <summary>
    /// Compute the distortion mesh position and uv
    /// </summary>
    /// <param name="undistortedFrustum">Undistorated frusutm.</param>
    /// <returns>the visible viewport</returns>
    public Rect GetViewport(float[] undistortedFrustum)
    {
        float eyeX = (screenSize.x - ipd) * 0.5f;
        float eyeY = VerticalLensOffset + screenSize.y * 0.5f;
        float left = (undistortedFrustum[0] * screenDistance + eyeX) / screenSize.x;
        float right = (undistortedFrustum[2] * screenDistance + eyeX) / screenSize.x;
        float top = (undistortedFrustum[1] * screenDistance + eyeY) / screenSize.y;
        float bottom = (undistortedFrustum[3] * screenDistance + eyeY) / screenSize.y;
        return new Rect(left, bottom, right - left, top - bottom);
    }

    /// <summary>
    /// Updates the device's state.
    /// </summary>
    public void UpdateState()
    {
        bool smooth = true;
        float verticalSpeed = 6.0f;
        float horizontalSpeed = (float)Screen.width / Screen.height * verticalSpeed;
        if (Input.GetKey(KeyCode.LeftAlt) || Input.GetKey(KeyCode.RightAlt))
        {
            headEulerAngles.x += Input.GetAxis("Mouse X") * horizontalSpeed;
            if (headEulerAngles.x <= -180.0f)
            {
                headEulerAngles.x += 360.0f;
            }
            else if (headEulerAngles.x > 180.0f)
            {
                headEulerAngles.x -= 360.0f;
            }


            headEulerAngles.y -= Input.GetAxis("Mouse Y") * verticalSpeed;
            headEulerAngles.y = Mathf.Clamp(headEulerAngles.y, -88.0f, 88.0f);
        }
        else if (Input.GetKey(KeyCode.LeftControl) || Input.GetKey(KeyCode.RightControl))
        {
            smooth = false;
            headEulerAngles.z += Input.GetAxis("Mouse X") * horizontalSpeed;
            headEulerAngles.z = Mathf.Clamp(headEulerAngles.z, -88.0f, 88.0f);
        }

        if (smooth)
        {
            headEulerAngles.z = Mathf.Lerp(headEulerAngles.z, 0.0f, Time.deltaTime / (Time.deltaTime + 0.1f));
        }
    }

    /// <summary>
    /// Constructor
    /// </summary>
    private MiEmulation() { }
}

#endif