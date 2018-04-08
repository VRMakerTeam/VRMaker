//-----------------------------------------------------------------------
// <copyright file="MiStereoPostRender.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------
#if UNITY_EDITOR || UNITY_STANDALONE_WIN

using System;
using UnityEngine;

/// <summary>
/// Used in editor mode to realize the stereo rendering - post render
/// </summary>
[RequireComponent(typeof(Camera))]
public class MiStereoPostRender : MonoBehaviour 
{
    /// <summary>
    /// Distortion Mesh Width
    /// </summary>
    private const int distortionMeshWidth = 45;

    /// <summary>
    /// Distortion Mesh Height
    /// </summary>
    private const int distortionMeshHeight = 45;

    /// <summary>
    /// Distortion Mesh
    /// </summary>
    private Mesh distortionMesh;

    /// <summary>
    /// Mesh Material
    /// </summary>
    private Material meshMaterial;

    /// <summary>
    /// Gets the camera which is the component to the current GameObject.
    /// </summary>
    /// <value>
    /// The camera which is the component of the current GameObject.
    /// </value>
    public Camera cam { get; private set; }

    /// <summary>
    /// Awakes this instance.
    /// </summary>
    private void Awake()
    {
        this.cam = GetComponent<Camera>();
        this.meshMaterial = new Material(Shader.Find("MIVR/UnlitTexture"));
    }

    /// <summary>
    /// OnPreCull is called before a camera culls the scene.
    /// </summary>
    private void OnPreCull() 
    {
        MiEmulation emulation = MiEmulation.Instance;
        float realAspect = (float)Screen.width / Screen.height;
        float deviceAspect = emulation.screenSize.x / emulation.screenSize.y;
        this.cam.orthographicSize = 0.5f * Mathf.Max(1.0f, deviceAspect / realAspect);
    }

    /// <summary>
    /// This function is called when the MonoBehaviour will be destroyed.
    /// </summary>
    private void OnDestroy()
    {
        UnityEngine.Object.Destroy(meshMaterial);
        meshMaterial = null;
    }

    /// <summary>
    /// Reset the camera's status
    /// </summary>
    private void Reset()
    {
        //    The member variable 'cam' could not always be initialized when this method is called in edtior mode, so check it here. 
        if (this.cam == null) 
        {
            this.cam = GetComponent<Camera>();
        }
		
        this.cam.clearFlags = CameraClearFlags.Depth;
        this.cam.backgroundColor = Color.yellow;
        this.cam.orthographic = true;
        this.cam.orthographicSize = 0.5f;
        this.cam.cullingMask = 0;
        this.cam.useOcclusionCulling = false;
		
        //    100 is a very big integer to make use this cam render after other cameras
        this.cam.depth = 100;

        if (distortionMesh == null)
        {
            ConstructDistortionMesh();
        }
    }

    /// <summary>
    /// Do the barrel distortion
    /// </summary>
    private void OnRenderObject()
    {
        if (Camera.current != this.cam)
            return;

        if (MiEmulation.Instance.StereoRT == null || meshMaterial == null) 
        {
            return;
        }

        if (this.distortionMesh == null) 
        {
            ConstructDistortionMesh();
        }

        this.meshMaterial.mainTexture = MiEmulation.Instance.StereoRT;
        this.meshMaterial.SetPass(0);
        Graphics.DrawMeshNow(distortionMesh, transform.position, transform.rotation);
        MiEmulation.Instance.StereoRT.DiscardContents();

        MiEmulation.Instance.UpdateState();
    }

    /// <summary>
    /// Construct the DistortionMesh
    /// </summary>
    private void ConstructDistortionMesh()
    {
        this.distortionMesh = new Mesh();

        Vector3[] vertices;
        Vector2[] texcoords;
        CalcMeshVertices(out vertices, out texcoords);
        int[] indices = CalcMeshIndices();

        // Set the edge color black to avoid alias
        Color[] colors = CalcMeshColors();

        distortionMesh.vertices = vertices;
        distortionMesh.uv = texcoords;
        distortionMesh.colors = colors;
        distortionMesh.triangles = indices;
        ;
        distortionMesh.UploadMeshData(true);
    }

    /// <summary>
    /// Calculate the distortion mesh position and uv
    /// </summary>
    /// <param name="vertices">mesh vertex positions.</param>
    /// <param name="texcoords">mesh vertex texcoords.</param>
    private void CalcMeshVertices(out Vector3[] vertices, out Vector2[] texcoords) 
    {
        MiEmulation emulation = MiEmulation.Instance;
        float[] frustumFromLens = new float[4];
        float[] bareFrustum = new float[4];
        emulation.GetFrustum(frustumFromLens, true);
        emulation.GetFrustum(bareFrustum, false);
        Rect viewport;
        viewport = emulation.GetViewport(bareFrustum);
        vertices = new Vector3[2 * distortionMeshWidth * distortionMeshHeight];
        texcoords = new Vector2[2 * distortionMeshWidth * distortionMeshHeight];
        int vertexIndex = 0;

        // Caculate left, then right
        for (int side = 0; side < 2; side++) 
        {
            for (int row = 0; row < distortionMeshHeight; row++) 
            {
                for (int col = 0; col < distortionMeshWidth; col++) 
                {
                    float u = (float)col / (distortionMeshWidth - 1.0f);
                    float v = (float)row / (distortionMeshHeight - 1.0f);

                    // do barrel distortion - calc the vertex position in normalized way
                    float tanAngleX = Mathf.Lerp(frustumFromLens[0], frustumFromLens[2], u);
                    float tanAngleY = Mathf.Lerp(frustumFromLens[3], frustumFromLens[1], v);
                    float distorted = Mathf.Sqrt(tanAngleX * tanAngleX + tanAngleY * tanAngleY);
                    float undistorted = emulation.DistortInverse(distorted);
                    float x = tanAngleX * undistorted / distorted;
                    float y = tanAngleY * undistorted / distorted;
                    x = (x - bareFrustum[0]) / (bareFrustum[2] - bareFrustum[0]);
                    y = (y - bareFrustum[3]) / (bareFrustum[1] - bareFrustum[3]);

                    // Convert to screen space
                    x = (viewport.x + x * viewport.width - 0.5f) * emulation.screenSize.x / emulation.screenSize.y;
                    y = viewport.y + y * viewport.height - 0.5f;
                    vertices[vertexIndex] = new Vector3(x, y, 1.0f);

                    // Modify 's' according the the left or the right side of the mesh. 
                    u = (u + side) * 0.5f;
                    texcoords[vertexIndex] = new Vector2(u, v);
                    vertexIndex++;
                }
            }

            // switch to the right side
            float w = frustumFromLens[2] - frustumFromLens[0];
            frustumFromLens[0] = -(w + frustumFromLens[0]);
            frustumFromLens[2] = w - frustumFromLens[2];
            w = bareFrustum[2] - bareFrustum[0];
            bareFrustum[0] = -(w + bareFrustum[0]);
            bareFrustum[2] = w - bareFrustum[2];
            viewport.x = 1.0f - (viewport.x + viewport.width);
        }
    }

    /// <summary>
    /// Calculate the distortion mesh vertex color
    /// </summary>
    /// <returns>the mesh vertex colors</returns>
    private Color[] CalcMeshColors() 
    {
        Color[] colors = new Color[distortionMeshWidth * distortionMeshHeight * 2];
        int vertexIndex = 0;
        int offset = distortionMeshWidth * distortionMeshHeight;
        for (int row = 0; row < distortionMeshHeight; row++) 
        {
            for (int col = 0; col < distortionMeshWidth; col++, vertexIndex++) 
            {
                colors[vertexIndex] = Color.white;
                colors[vertexIndex + offset] = Color.white;
                if (col == 0 || row == 0 || col == (distortionMeshWidth - 1) || row == (distortionMeshHeight - 1)) 
                {
                    // Make the edge color same as background color
                    colors[vertexIndex] = Color.black;
                    colors[vertexIndex + offset] = Color.black;
                }
            }
        }

        return colors;
    }

    /// <summary>
    /// Calculate the distortion mesh indices
    /// </summary>
	/// <returns>the mesh indices</returns>
    private int[] CalcMeshIndices() 
    {
        int[] indices = new int[2 * (distortionMeshWidth - 1) * (distortionMeshHeight - 1) * 6];
        int halfwidth = distortionMeshWidth / 2;
        int halfheight = distortionMeshHeight / 2;
        int index = 0;
        int vertexIndex = 0;

        // Caculate left, then right
        for (int side = 0; side < 2; side++) 
        {
            for (int row = 0; row < distortionMeshHeight; row++)
            {
                for (int col = 0; col < distortionMeshWidth; col++, vertexIndex++)
                {
                    if (col == 0 || row == 0)
                        continue;

                    // Generate Quad indices
                    if ((col <= halfwidth) == (row <= halfheight))
                    {
                        // Lower left, upper right
                        indices[index++] = vertexIndex;
                        indices[index++] = vertexIndex - distortionMeshWidth;
                        indices[index++] = vertexIndex - distortionMeshWidth - 1;
                        indices[index++] = vertexIndex - distortionMeshWidth - 1;
                        indices[index++] = vertexIndex - 1;
                        indices[index++] = vertexIndex;
                    } 
                    else 
                    {
                        // Upper left, lower right.
                        indices[index++] = vertexIndex - 1;
                        indices[index++] = vertexIndex;
                        indices[index++] = vertexIndex - distortionMeshWidth;
                        indices[index++] = vertexIndex - distortionMeshWidth;
                        indices[index++] = vertexIndex - distortionMeshWidth - 1;
                        indices[index++] = vertexIndex - 1;
                    }
                }
            }
        }
		
        return indices;
    }
}
#endif
