using UnityEngine;

/// <summary>
/// the center camera
/// </summary>
/// <seealso cref="UnityEngine.MonoBehaviour" />
public class XCenterCamera : MonoBehaviour
{
    /// <summary>
    /// The cam
    /// </summary>
    private Camera cam;

    /// <summary>
    /// Awakes this instance.
    /// </summary>
    private void Awake()
    {
        this.cam = this.GetComponent<Camera>();
    }

    /// <summary>
    /// OnPreCull is called before a camera culls the scene.
    /// </summary>
    private void OnPreCull()
    {
        if (this.cam != null)
        {
            // Turn off the center camera to save rendering time.
            // center camera is only used for raycasting 
            this.cam.enabled = false;
        }
    }
}
