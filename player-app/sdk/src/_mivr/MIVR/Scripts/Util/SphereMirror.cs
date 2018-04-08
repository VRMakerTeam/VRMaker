//-----------------------------------------------------------------------
// <copyright file="SphereMirror.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

using UnityEngine;

/// <summary>
/// sphere mirror
/// </summary>
public class SphereMirror : MonoBehaviour
{
    /// <summary>
    /// Starts this instance.
    /// </summary>
    private void Start()
    {
        Vector2[] vec2UVs = this.transform.GetComponent<MeshFilter>().mesh.uv;

        for (int i = 0; i < vec2UVs.Length; i++)
        {
            vec2UVs[i] = new Vector2(1.0f - vec2UVs[i].x, vec2UVs[i].y);
        }

        this.transform.GetComponent<MeshFilter>().mesh.uv = vec2UVs;
    }

    /// <summary>
    /// Updates this instance.
    /// </summary>
    private void Update()
    {
    }
}
