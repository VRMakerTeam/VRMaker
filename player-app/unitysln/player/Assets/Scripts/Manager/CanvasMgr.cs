/********************************************************************
     Copyright (c) zyncho.
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections;
using Fangs.Lib.Logger;

namespace VRXX.Manager
{
	public static class CanvasMgr
	{
        public static CanvasSubtitle Subtitle { get; private set; }

		private static Transform root_ { get; set; }
        private static RectTransform canvas2D_ { get; set; }
        private static RectTransform canvas3D_ { get; set; }
        private static Image mask_ { get; set; }
        private static GameObject input_ { get; set; }

        public delegate void OnFinishDelegate();

        public static Transform Canvas3D
        {
            get
            {
                return canvas3D_;
            }
        }

        public static void Preload ()
		{
            root_ = new GameObject("__CanvasMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);

            GameObject canvas2D = new GameObject("Canvas2D");
            canvas2D.transform.SetParent(root_);
            Canvas canvas2DCpt = canvas2D.AddComponent<Canvas>();
            canvas2DCpt.renderMode = RenderMode.ScreenSpaceOverlay;

            CanvasScaler scaler2D = canvas2D.AddComponent<CanvasScaler>();
            scaler2D.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
            scaler2D.referenceResolution = new Vector2(1920, 1080);
            scaler2D.screenMatchMode = CanvasScaler.ScreenMatchMode.MatchWidthOrHeight;
            scaler2D.matchWidthOrHeight = 1;

            canvas2D.AddComponent<GraphicRaycaster>();
            canvas2D_ = canvas2D.GetComponent<RectTransform>();

            GameObject canvas3D = new GameObject("Canvas3D");
            canvas3D.transform.SetParent(root_);
            Canvas canvas3DCpt = canvas3D.AddComponent<Canvas>();
            canvas3DCpt.renderMode = RenderMode.WorldSpace;

            CanvasScaler scaler3D = canvas3D.AddComponent<CanvasScaler>();
            scaler3D.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
            scaler3D.referenceResolution = new Vector2(1920, 1080);
            scaler3D.screenMatchMode = CanvasScaler.ScreenMatchMode.MatchWidthOrHeight;
            scaler3D.matchWidthOrHeight = 1;

            canvas3D.AddComponent<GraphicRaycaster>();
            canvas3D_ = canvas3D.GetComponent<RectTransform>();


            input_ = new GameObject("EventSystem");
            input_.AddComponent<StandaloneInputModule>();
            input_.transform.SetParent(root_);

            BuildMask();
            BuildMask3D();

            Subtitle = new CanvasSubtitle();
            Subtitle.Build(canvas2D_);
        }

        #region Canvas2D

        private static void BuildMask()
        {
            GameObject mask = new GameObject("Mask");
            mask.transform.SetParent(canvas2D_);
            mask_ = mask.AddComponent<Image>();

            RectTransform rt = mask_.GetComponent<RectTransform>();
            rt.anchorMin = new Vector2(0, 0);
            rt.anchorMax = new Vector2(1, 1);
            rt.anchoredPosition = new Vector2(0, 0);
            rt.sizeDelta = new Vector2(0, 0);
            rt.localScale = Vector3.one;

            Image im = mask_.GetComponent<Image>();
            Sprite pict = Resources.Load<Sprite>("mask");
            im.sprite = pict;
            im.color = new Color(0, 0, 0, 0);
        }

        public static Transform FindElement(string _path)
        {
            return canvas2D_.Find(_path);
        }

        public static void Toggle2D(bool _visible)
        {
            canvas2D_.gameObject.SetActive(_visible);
        }

        public static bool isActiveCanvas2D()
        {
            return canvas2D_.gameObject.activeSelf;
        }

        public static void ToggleMask2D(bool _visible)
        {
            FindElement("Mask").gameObject.SetActive(_visible);
        }

        public static void SetMaskColor(Color _color)
        {
            Image im = mask_.GetComponent<Image>();
            im.color = _color;
        }

        public static void ApplyScaleModeConstantPixelSize()
        {
            CanvasScaler scaler = canvas2D_.GetComponent<CanvasScaler>();
            scaler.uiScaleMode = CanvasScaler.ScaleMode.ConstantPixelSize;
        }

        #endregion


        #region Canvas3D

        private static void BuildMask3D()
        {
            GameObject mask = new GameObject("Mask");
            mask.AddComponent<Image>();
            mask.transform.SetParent(canvas3D_);

            mask.GetComponent<Transform>().localPosition = new Vector3(0, 0, 6);
            mask.GetComponent<Image>().color = new Color(255, 255, 255, 0);

            ToggleMask3D(false);
        }

        public static Transform FindElement3d(string _uuid)
        {
            return canvas3D_.Find(_uuid);
        }

        public static void Toggle3D(bool _visible)
        {
            canvas3D_.gameObject.SetActive(_visible);
        }

        public static bool isActiveCanvas3D()
        {
            return canvas3D_.gameObject.activeSelf;
        }

        public static void ToggleMask3D(bool _visible)
        {
            FindElement3d("Mask").gameObject.SetActive(_visible);
        }

        public static void SetMaskColor3D(Color _color)
        {
            FindElement3d("Mask").GetComponent<Image>().color = _color;
        }

        public static void Attach3DChild(Transform _child)
        {
            _child.SetParent(canvas3D_);
        }

        public static void Detach3DChild(Transform _child)
        {
            _child.SetParent(null);
        }

        #endregion


        public static void Update ()
		{
			//updatePop ();
			//updateFade ();
			
		}

		public static void UseLandscape ()
		{
			Screen.orientation = ScreenOrientation.Landscape;
            CanvasScaler ca = canvas2D_.GetComponent<CanvasScaler>();
			ca.referenceResolution = new Vector2 (1920f, 1080f);
		}

		public static void UsePortrait ()
		{
			Screen.orientation = ScreenOrientation.Portrait;
            CanvasScaler ca = canvas2D_.GetComponent<CanvasScaler>();
			ca.referenceResolution = new Vector2 (1080f, 1920f);
		}

        public static void ToggleEvent(bool _flag)
        {
            input_.SetActive(_flag);
        }

		public static void AttachPanel (GameObject _panel)
		{
            RectTransform rt = _panel.GetComponent<RectTransform>();
            Quaternion rotation = rt.localRotation;
            Vector3 position = rt.localPosition;
            Vector3 scale = rt.localScale;
            Vector2 anchorMin = rt.anchorMin;
            Vector2 anchorMax = rt.anchorMax;
            Vector2 anchoredPosition = rt.anchoredPosition;
            Vector2 sizeDelta = rt.sizeDelta;
            Vector2 pivot = rt.pivot;
            _panel.transform.SetParent(canvas2D_);
            rt.localRotation = rotation;
            rt.localPosition = position;
            rt.localScale = scale;
            rt.anchorMin = anchorMin;
            rt.anchorMax = anchorMax;
            rt.anchoredPosition = anchoredPosition;
            rt.sizeDelta = sizeDelta;
            rt.pivot = pivot;
        }

        public static void TogglePanel(string _path, bool _flag)
        {
            Transform child = canvas2D_.Find(_path);
            if (null != child)
                child.gameObject.SetActive(_flag);
        }


		public static void DetachRoot (RectTransform _panel)
		{
 
		}



        /// <summary>
        /// 
        /// </summary>
        /// <param name="_duration"></param>
        /// <param name="_mode">
        /// 0 : FadeIn
        /// 1 : FadeOut
        /// </param>
        public static void FadeMask(float _duration, int _mode)
        {
            CoroutineMgr.Start(Fade_Mask(_duration, _mode, 
                () =>{ }));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_duration"></param>
        /// <param name="_mode">
        /// 0 : FadeIn
        /// 1 : FadeOut
        /// </param>
        public static void FadeMask(float _duration, int _mode, OnFinishDelegate _onFinish)
        {
            CoroutineMgr.Start(Fade_Mask(_duration, _mode, () =>
                {
                    _onFinish();
                }));
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="_duration"></param>
        /// <param name="_mode">
        /// 0 : fadein
        /// 1 : fadeout
        /// </param>
        /// <param name="_reverse">
        /// true : clockwise rotate
        /// false : anticlockwise rotate
        /// </param>
        public static void RotateFadeMask(float _duration, int _mode, bool _inversion)
        {
            CoroutineMgr.Start(RotateMask(_duration, _mode, _inversion));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_duration"></param>
        /// <param name="_mode">
        /// 0 : fadein
        /// 1 : fadeout
        /// </param>
        /// <param name="_reverse">
        /// true : from left to right
        /// false : from right to left
        /// </param>
        public static void HorizontalFadeMask(float _duration, int _mode, bool _inversion)
        {
            CoroutineMgr.Start(HorizontalMask(_duration, _mode, _inversion));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_duration"></param>
        /// <param name="_mode">
        /// 0 : fadein
        /// 1 : fadeout
        /// </param>
        /// <param name="_reverse">
        /// true : from bottom to top
        /// false : from top to bottom 
        /// </param>
        public static void VerticalFadeMask(float _duration, int _mode, bool _inversion)
        {
            CoroutineMgr.Start(VerticalMask(_duration, _mode, _inversion));
        }

        private static IEnumerator Fade_Mask(float _duration, int _mode, OnFinishDelegate _onFinish)
        {
            mask_.type = Image.Type.Simple;
            yield return new WaitForEndOfFrame();

            float time = 0;
            while (true)
            {
                yield return new WaitForEndOfFrame();
                time += Time.deltaTime;
                float amount = time / _duration;
                float alpha = Mathf.Abs(_mode - amount);

                Color color = mask_.color;
                color.a = alpha;
                mask_.color = color;

                if (time > _duration)
                    break;
            }
            if (time > _duration)
                _onFinish();
        }

        private static IEnumerator RotateMask(float _duration, int _mode, bool _inversion)
        {
            mask_.type = Image.Type.Filled;
            mask_.fillMethod = Image.FillMethod.Radial360;
            mask_.fillClockwise = _inversion;
            Color color = mask_.color;
            color.a = 1;
            mask_.color = color;

            float time = 0;
            while (true)
            {
                yield return new WaitForEndOfFrame();
                time += Time.deltaTime;
                float amount = time / _duration;

                float f = _mode - amount;
                if ((_mode - amount) < 0 && _mode == 1)
                    f = 0;
                mask_.fillAmount = Mathf.Abs(f);

                if (time > _duration)
                    break;
            }
            
        }

        private static IEnumerator HorizontalMask(float _duration, int _mode, bool _inversion)
        {
            mask_.type = Image.Type.Filled;
            mask_.fillMethod = Image.FillMethod.Horizontal;
            Color color = mask_.color;
            color.a = 1;
            mask_.color = color;

            if (_mode == 0)
            {
                if (_inversion)
                    mask_.fillOrigin = 1;
                else
                    mask_.fillOrigin = 0;
            }
            else if (_mode == 1)
            {
                if (_inversion)
                    mask_.fillOrigin = 0;
                else
                    mask_.fillOrigin = 1;
            }

            float time = 0;
            while (true)
            {
                yield return new WaitForEndOfFrame();
                time += Time.deltaTime;
                float amount = time / _duration;

                float f = _mode - amount;
                if ((_mode - amount) < 0 && _mode == 1)
                    f = 0;

                mask_.fillAmount = Mathf.Abs(f);

                if (time > _duration)
                    break;
            }

        }

        private static IEnumerator VerticalMask(float _duration, int _mode, bool _inversion)
        {
            mask_.type = Image.Type.Filled;
            mask_.fillMethod = Image.FillMethod.Vertical;
            Color color = mask_.color;
            color.a = 1;
            mask_.color = color;

            if (_mode == 0)
            {
                if (_inversion)
                    mask_.fillOrigin = 0;
                else
                    mask_.fillOrigin = 1;
            }
            else if (_mode == 1)
            {
                if (_inversion)
                    mask_.fillOrigin = 1;
                else
                    mask_.fillOrigin = 0;
            }

            float time = 0;
            while (true)
            {
                yield return new WaitForEndOfFrame();
                time += Time.deltaTime;

                float amount = time / _duration;
                float f = _mode - amount;
                if (f < 0 && _mode == 1)
                    f = 0;
                mask_.fillAmount = Mathf.Abs(f);

                if (time > _duration)
                    break;
            }

            //if (_mode == 0 && _inversion)
            //    mask_.fillOrigin = 1;
            //else if (_mode == 0 && !_inversion)
            //    mask_.fillOrigin = 0;

            //Color color = mask_.color;
            //color.a = 1;
            //mask_.color = color;

            //float time = 0;
            //while (true)
            //{
            //    yield return new WaitForEndOfFrame();
            //    time += Time.deltaTime;
            //    float amount = time / _duration;

            //    float f = _mode - amount;
            //    if ((_mode - amount) < 0 && _mode == 1)
            //        f = 0;

            //    mask_.fillAmount = Mathf.Abs(f);

            //    if (time > _duration)
            //        break;
            //}



        }

        

	}//class
}//namespace
