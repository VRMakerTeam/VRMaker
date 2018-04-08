//-----------------------------------------------------------------------
// <copyright file="LoadingCursor.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

using UnityEngine;

/// <summary>
/// The loading cursor 
/// </summary>
public class LoadingCursor : MonoBehaviour
{
    /// <summary>
    /// Gets or sets the rotate speed.
    /// </summary>
    /// <value>
    /// The rotate speed.
    /// </value>
    public Vector3 RotateSpeed { get; set; }

    /// <summary>
    /// Awakes this instance.
    /// </summary>
    private void Awake()
    {
        this.RotateSpeed = new Vector3(0.0f, 0.0f, -50.0f);
    }

    /// <summary>
    /// Updates this instance.
    /// </summary>
    private void Update()
    {
        this.transform.Rotate(this.RotateSpeed * Time.smoothDeltaTime);
    }
}
