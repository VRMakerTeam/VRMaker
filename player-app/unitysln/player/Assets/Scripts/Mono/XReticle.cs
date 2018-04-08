using UnityEngine;

/// <summary>
/// the reticle object which used for gaze input
/// </summary>
public class XReticle : MonoBehaviour
{
    /// <summary>
    /// Minimum inner angle of the reticle (in degrees).
    /// </summary>
    private const float KReticleMinInnerAngle = 0.0f;

    /// <summary>
    /// Minimum outer angle of the reticle (in degrees).
    /// </summary>
    private const float KReticleMinOuterAngle = 0.5f;

    /// <summary>
    /// Angle at which to expand the reticle when intersecting with an object(in degrees).
    /// </summary>
    private const float KReticleGrowthAngle = 1.5f;

    /// <summary>
    /// Minimum distance of the reticle (in meters).
    /// </summary>
    private const float KReticleDistanceMin = 0.75f;

    /// <summary>
    /// Maximum distance of the reticle (in meters).
    /// </summary>
    private const float KReticleDistanceMax = 10.0f;

    /// <summary>
    /// Reticle scale.
    /// </summary>
    private const float KReticleScale = 0.4f;

    /// <summary>
    /// The reticle segments
    /// </summary>
    private int reticleSegments = 20;

    /// <summary>
    /// Growth speed multiplier for the reticle
    /// </summary>
    private float reticleGrowthSpeed = 8.0f;

    /// <summary>
    /// Current inner angle of the reticle (in degrees).
    /// </summary>
    private float reticleInnerAngle = 0.0f;

    /// <summary>
    /// Current outer angle of the reticle (in degrees).
    /// </summary>
    private float reticleOuterAngle = 0.5f;

    /// <summary>
    /// Current distance of the reticle (in meters).
    /// </summary>
    private float reticleDistanceInMeters = 10.0f;

    /// <summary>
    /// Current inner diameter of the reticle, before distance multiplication.
    /// </summary>
    private float reticleInnerDiameter = 0.0f;

    /// <summary>
    /// Current outer diameter of the reticle, before distance multiplication.
    /// </summary>
    private float reticleOuterDiameter = 0.0f;

    /// <summary>
    /// Reticle Material.
    /// </summary>
    private Material materialComp;

    /// <summary>
    /// Reticle Color.
    /// </summary>
    private Color reticleColor = Color.yellow;

    /// <summary>
    /// Called when [gaze start].
    /// </summary>
    /// <param name="camera">The camera.</param>
    /// <param name="targetObject">The target object.</param>
    /// <param name="intersectionPosition">The intersection position.</param>
    public void OnGazeStart(Camera camera, GameObject targetObject, Vector3 intersectionPosition)
    {
        this.SetGazeTarget(intersectionPosition);
    }

    /// <summary>
    /// Called when [gaze stay].
    /// </summary>
    /// <param name="camera">The camera.</param>
    /// <param name="targetObject">The target object.</param>
    /// <param name="intersectionPosition">The intersection position.</param>
    public void OnGazeStay(Camera camera, GameObject targetObject, Vector3 intersectionPosition)
    {
        this.SetGazeTarget(intersectionPosition);
    }

    /// <summary>
    /// Called when [gaze exit].
    /// </summary>
    /// <param name="camera">The camera.</param>
    /// <param name="targetObject">The target object.</param>
    public void OnGazeExit(Camera camera, GameObject targetObject)
    {
        this.reticleDistanceInMeters = KReticleDistanceMax;
        this.reticleInnerAngle = KReticleMinInnerAngle;
        this.reticleOuterAngle = KReticleMinOuterAngle;
    }

    /// <summary>
    /// Called when [gaze trigger start].
    /// </summary>
    /// <param name="camera">The camera.</param>
    public void OnGazeTriggerStart(Camera camera)
    {
    }

    /// <summary>
    /// Called when [gaze trigger end].
    /// </summary>
    /// <param name="camera">The camera.</param>
    public void OnGazeTriggerEnd(Camera camera)
    {
    }

    /// <summary>
    /// Unity Awake.
    /// </summary>
    private void Awake()
    {
        this.materialComp = gameObject.GetComponent<Renderer>().material;
        this.materialComp.color = this.reticleColor;
    }

    /// <summary>
    /// Starts this instance.
    /// </summary>
    private void Start()
    {
        this.CreateReticleVertices();
    }

    /// <summary>
    /// Unity Update.
    /// </summary>
    private void Update()
    {
        this.UpdateDiameters();
    }

    /// <summary>
    /// Called when [enable].
    /// </summary>
    private void OnEnable()
    {
        XGazeInputModule.GazePointer = this;
    }

    /// <summary>
    /// Called when [disable].
    /// </summary>
    private void OnDisable()
    {
        if (XGazeInputModule.GazePointer == this)
        {
            XGazeInputModule.GazePointer = null;
        }
    }

    /// <summary>
    /// Creates the reticle vertices.
    /// </summary>
    private void CreateReticleVertices()
    {
        Mesh mesh = new Mesh();
        this.gameObject.AddComponent<MeshFilter>();
        this.GetComponent<MeshFilter>().mesh = mesh;

        int segments_count = this.reticleSegments;
        int vertex_count = (segments_count + 1) * 2;

        Vector3[] vertices = new Vector3[vertex_count];

        const float TwoPI = Mathf.PI * 2.0f;
        int vi = 0;
        for (int si = 0; si <= segments_count; ++si)
        {
            float angle = (float)si / (float)segments_count * TwoPI;

            float x = Mathf.Sin(angle);
            float y = Mathf.Cos(angle);

            vertices[vi++] = new Vector3(x, y, 0.0f); // Outer vertex.
            vertices[vi++] = new Vector3(x, y, 1.0f); // Inner vertex.
        }

        int indices_count = (segments_count + 1) * 3 * 2;
        int[] indices = new int[indices_count];

        int vert = 0;
        int idx = 0;
        for (int si = 0; si < segments_count; ++si)
        {
            indices[idx++] = vert + 1;
            indices[idx++] = vert;
            indices[idx++] = vert + 2;

            indices[idx++] = vert + 1;
            indices[idx++] = vert + 2;
            indices[idx++] = vert + 3;

            vert += 2;
        }

        mesh.vertices = vertices;
        mesh.triangles = indices;
        mesh.RecalculateBounds();
        ;
    }

    /// <summary>
    /// Update Reticle Diameter when Update
    /// </summary>
    private void UpdateDiameters()
    {
        this.reticleDistanceInMeters =
          Mathf.Clamp(this.reticleDistanceInMeters, KReticleDistanceMin, KReticleDistanceMax);

        if (this.reticleInnerAngle < XReticle.KReticleMinInnerAngle)
        {
            this.reticleInnerAngle = XReticle.KReticleMinInnerAngle;
        }

        if (this.reticleOuterAngle < XReticle.KReticleMinOuterAngle)
        {
            this.reticleOuterAngle = XReticle.KReticleMinOuterAngle;
        }

        float inner_half_angle_radians = Mathf.Deg2Rad * this.reticleInnerAngle * XReticle.KReticleScale;
        float outer_half_angle_radians = Mathf.Deg2Rad * this.reticleOuterAngle * XReticle.KReticleScale;

        float inner_diameter = 2.0f * Mathf.Tan(inner_half_angle_radians);
        float outer_diameter = 2.0f * Mathf.Tan(outer_half_angle_radians);

        this.reticleInnerDiameter =
            Mathf.Lerp(this.reticleInnerDiameter, inner_diameter, Time.deltaTime * this.reticleGrowthSpeed);
        this.reticleOuterDiameter =
            Mathf.Lerp(this.reticleOuterDiameter, outer_diameter, Time.deltaTime * this.reticleGrowthSpeed);

        this.materialComp.SetFloat("_InnerDiameter", this.reticleInnerDiameter * this.reticleDistanceInMeters * XReticle.KReticleScale);
        this.materialComp.SetFloat("_OuterDiameter", this.reticleOuterDiameter * this.reticleDistanceInMeters * XReticle.KReticleScale);
        this.materialComp.SetFloat("_DistanceInMeters", this.reticleDistanceInMeters);
    }

    /// <summary>
    /// Set GazeTarget When target Changed.
    /// </summary>
    /// <param name="target">The position of target</param>
    private void SetGazeTarget(Vector3 target)
    {
        Vector3 targetLocalPosition = Camera.main.transform.InverseTransformPoint(target);

        this.reticleDistanceInMeters =
            Mathf.Clamp(targetLocalPosition.z, KReticleDistanceMin, KReticleDistanceMax);
        this.reticleInnerAngle = XReticle.KReticleMinInnerAngle + XReticle.KReticleGrowthAngle;
        this.reticleOuterAngle = XReticle.KReticleMinOuterAngle + XReticle.KReticleGrowthAngle;
    }
}