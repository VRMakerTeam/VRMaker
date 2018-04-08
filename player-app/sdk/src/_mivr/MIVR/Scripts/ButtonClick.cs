//-----------------------------------------------------------------------
// <copyright file="ButtonClick.cs" company="XiaoMi Corporation">
//     All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// the sample button click script
/// </summary>
public class ButtonClick : MonoBehaviour
{
    /// <summary>
    /// Called when [click].
    /// </summary>
    public void OnClick()
    {
        Debug.Log("**** OnClick.");
        this.transform.GetComponentInChildren<Text>().text = (Random.value * 100).ToString();
    }

    /// <summary>
    /// Called when [pointer down].
    /// </summary>
    public void OnPointerDown()
    {
        Debug.Log("**** OnPointerDown.");
    }

    /// <summary>
    /// Starts this instance.
    /// </summary>
    private void Start()
    {
    }
}