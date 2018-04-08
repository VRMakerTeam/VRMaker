//-----------------------------------------------------------------------
// <copyright file="GazeInputModule.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

using UnityEngine;
using UnityEngine.EventSystems;

/// <summary>
/// the gaze input module
/// </summary>
public class MIGazeInputModule : BaseInputModule
{
    /// <summary>
    /// The gaze point which to cast rays, in viewport coordinates.
    /// </summary>
    private Vector2 gazePoint = new Vector2(0.5f, 0.5f);

    /// <summary>
    /// The pointer data
    /// </summary>
    private PointerEventData pointerData;

    /// <summary>
    /// The is active
    /// </summary>
    private bool isActive = false;

    /// <summary>
    /// Gets or sets the gaze pointer.
    /// </summary>
    /// <value>
    /// The gaze pointer.
    /// </value>
    public static Reticle GazePointer { get; set; }

    /// <summary>
    /// Should be activated.
    /// </summary>
    /// <returns>
    /// Should the module be activated.
    /// </returns>
    public override bool ShouldActivateModule()
    {
        bool activeState = base.ShouldActivateModule();

        if (activeState != this.isActive)
        {
            this.isActive = activeState;
        }

        return activeState;
    }

    /// <summary>
    /// Called when the module is deactivated. Override this if you want custom code to execute when you deactivate your module.
    /// </summary>
    public override void DeactivateModule()
    {
        this.DisableGazePointer();
        base.DeactivateModule();
        if (this.pointerData != null)
        {
            this.HandlePendingClick();
            this.HandlePointerExitAndEnter(this.pointerData, null);
            this.pointerData = null;
        }

        this.eventSystem.SetSelectedGameObject(null, this.GetBaseEventData());
    }

    /// <summary>
    /// Is the pointer with the given ID over an EventSystem object?
    /// </summary>
    /// <param name="pointerId">Pointer ID.</param>
    /// <returns>true if the point over game object</returns>
    public override bool IsPointerOverGameObject(int pointerId)
    {
        return this.pointerData != null && this.pointerData.pointerEnter != null;
    }

    /// <summary>
    /// Process the current tick for the module.
    /// </summary>
    public override void Process()
    {
        GameObject gazeObjectPrevious = this.GetCurrentGameObject();

        this.CastRayFromGaze();
        this.UpdateCurrentObject();
        this.UpdateReticle(gazeObjectPrevious);

        Camera camera = this.pointerData.enterEventCamera;

        if (this.pointerData.pointerCurrentRaycast.gameObject != null && Input.GetMouseButtonDown(0))
        {
           ExecuteEvents.Execute(this.pointerData.pointerCurrentRaycast.gameObject, this.pointerData, ExecuteEvents.pointerDownHandler);
        }

        if (!this.pointerData.eligibleForClick &&
                         (Input.GetMouseButtonUp(0) || MIInputManager.HmdButtonUp ||
                         (null != MIInputManager.ControllerState && MIInputManager.ControllerState.ClickButtonUp)))
        {
            this.HandleTrigger();
            if (GazePointer != null)
            {
                GazePointer.OnGazeTriggerStart(camera);
            }
        }
        else if (!Input.GetMouseButton(0))
        {
            this.HandlePendingClick();
        }
    }

    /// <summary>
    /// Casts the ray from gaze.
    /// </summary>
    private void CastRayFromGaze()
    {
        if (this.pointerData == null)
        {
            this.pointerData = new PointerEventData(this.eventSystem);
        }

        this.pointerData.Reset();
        this.pointerData.position = new Vector2(this.gazePoint.x * Screen.width, this.gazePoint.y * Screen.height);
        this.eventSystem.RaycastAll(this.pointerData, this.m_RaycastResultCache);

        this.pointerData.pointerCurrentRaycast = BaseInputModule.FindFirstRaycast(this.m_RaycastResultCache);
        this.m_RaycastResultCache.Clear();
    }

    /// <summary>
    /// Updates the current object.
    /// </summary>
    private void UpdateCurrentObject()
    {
        var enterTarget = this.pointerData.pointerCurrentRaycast.gameObject;
        this.HandlePointerExitAndEnter(this.pointerData, enterTarget);

        var selected = ExecuteEvents.GetEventHandler<ISelectHandler>(enterTarget);
        if (selected == this.eventSystem.currentSelectedGameObject)
        {
            ExecuteEvents.Execute(
                this.eventSystem.currentSelectedGameObject, 
                this.GetBaseEventData(),
                ExecuteEvents.updateSelectedHandler);
        }
        else
        {
            this.eventSystem.SetSelectedGameObject(null, this.pointerData);
        }
    }

    /// <summary>
    /// Updates the reticle.
    /// </summary>
    /// <param name="previousGazedObject">The previous gazed object.</param>
    private void UpdateReticle(GameObject previousGazedObject)
    {
        if (GazePointer == null)
        {
            return;
        }

        Camera camera = this.pointerData.enterEventCamera;
        GameObject currentGazeObject = this.GetCurrentGameObject();

        Vector3 intersectionPosition = this.GetIntersectionPosition();

        if (currentGazeObject == previousGazedObject)
        {
            if (currentGazeObject != null)
            {
                GazePointer.OnGazeStay(camera, currentGazeObject, intersectionPosition);
            }
        }
        else
        {
            if (previousGazedObject != null)
            {
                GazePointer.OnGazeExit(camera, previousGazedObject);
            }

            if (currentGazeObject != null)
            {
                GazePointer.OnGazeStart(camera, currentGazeObject, intersectionPosition);
            }
        }
    }

    /// <summary>
    /// Handles the pending click.
    /// </summary>
    private void HandlePendingClick()
    {
        if (!this.pointerData.eligibleForClick)
        {
            return;
        }

        if (GazePointer != null)
        {
            Camera camera = this.pointerData.enterEventCamera;
            GazePointer.OnGazeTriggerEnd(camera);
        }

        var hitObject = this.pointerData.pointerCurrentRaycast.gameObject;

        ExecuteEvents.Execute(this.pointerData.pointerPress, this.pointerData, ExecuteEvents.pointerUpHandler);
        ExecuteEvents.Execute(this.pointerData.pointerPress, this.pointerData, ExecuteEvents.pointerClickHandler);

        this.pointerData.pointerPress = null;
        this.pointerData.rawPointerPress = null;
        this.pointerData.eligibleForClick = false;
        this.pointerData.clickCount = 0;
    }

    /// <summary>
    /// Handles the trigger.
    /// </summary>
    private void HandleTrigger()
    {
        var triggerObject = this.pointerData.pointerCurrentRaycast.gameObject;

        this.pointerData.pressPosition = this.pointerData.position;
        this.pointerData.pointerPressRaycast = this.pointerData.pointerCurrentRaycast;
        this.pointerData.pointerPress = ExecuteEvents.ExecuteHierarchy(triggerObject, this.pointerData, ExecuteEvents.pointerDownHandler)
            ?? ExecuteEvents.GetEventHandler<IPointerClickHandler>(triggerObject);

        this.pointerData.rawPointerPress = triggerObject;
        this.pointerData.eligibleForClick = true;
        this.pointerData.delta = Vector2.zero;
        this.pointerData.dragging = false;
        this.pointerData.useDragThreshold = true;
        this.pointerData.clickCount = 1;
        this.pointerData.clickTime = Time.unscaledTime;
    }

    /// <summary>
    /// Gets the current game object.
    /// </summary>
    /// <returns>the current gazed game object</returns>
    private GameObject GetCurrentGameObject()
    {
        if (this.pointerData != null && this.pointerData.enterEventCamera != null)
        {
            return this.pointerData.pointerCurrentRaycast.gameObject;
        }

        return null;
    }

    /// <summary>
    /// Gets the intersection position.
    /// </summary>
    /// <returns>the intersection position</returns>
    private Vector3 GetIntersectionPosition()
    {
        Camera cam = this.pointerData.enterEventCamera;
        if (cam == null)
        {
            return Vector3.zero;
        }

        float intersectionDistance = this.pointerData.pointerCurrentRaycast.distance + cam.nearClipPlane;
        Vector3 intersectionPosition = cam.transform.position + (cam.transform.forward * intersectionDistance);

        return intersectionPosition;
    }

    /// <summary>
    /// Disables the gaze pointer.
    /// </summary>
    private void DisableGazePointer()
    {
        if (GazePointer == null)
        {
            return;
        }

        GameObject currentGameObject = this.GetCurrentGameObject();
        if (currentGameObject)
        {
            Camera eventCamera = this.pointerData.enterEventCamera;
            GazePointer.OnGazeExit(eventCamera, currentGameObject);
        }
    }
}