/********************************************************************
     Copyright (c) zyncho.
     All rights reserved.

     author: Steven Sun
*********************************************************************/

using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections;
using Fangs.Lib.Logger;


namespace VRXX.Manager
{

    public class CanvasSubtitle
    {
        public class Animation
        {

        }

        public class InsertAnim : Animation
        {
            public string text;
            public float duration;
            public int mode;
            /// <summary>
            /// 
            /// </summary>
            /// <param name="_duration"></param>
            /// <param name="_mode">
            /// 0 : insert left to right 
            /// 1 : insert bottom to top
            /// 2 : insert fadein
            /// </param>
            /// <param name="_text"></param>
            public InsertAnim(float _duration, int _mode, string _text)
            {
                duration = _duration;
                mode = _mode;
                text = _text;
            }
        }

        public class ExitAnim : Animation
        {
            public float duration;
            public int mode;
            /// <summary>
            /// 
            /// </summary>
            /// <param name="_duration"></param>
            /// <param name="_mode">
            /// 0 : exit left to right
            /// 1 : eixt fadeout
            /// </param>
            public ExitAnim(float _duration, int _mode)
            {
                duration = _duration;
                mode = _mode;
            }
        }

        private RectTransform canvas_ { get; set; }
        private GameObject root_ { get; set; }
        private Image subtitle_ { get; set; }
        private Text text_ { get; set; }

        public void Build(RectTransform _parent)
        {
            canvas_ = _parent;
            root_ = new GameObject("subtitle");
            root_.transform.SetParent(canvas_);
            subtitle_ = root_.AddComponent<Image>();

            RectTransform rt = subtitle_.GetComponent<RectTransform>();
            rt.anchorMin = new Vector2(0, 0);
            rt.anchorMax = new Vector2(1, 0);
            rt.anchoredPosition = new Vector2(0, 150);
            rt.sizeDelta = new Vector2(0, 300);

            Image im = subtitle_.GetComponent<Image>();
            im.color = new Color(0, 0, 0, 0.8f);

            GameObject text = new GameObject("text");
            text.transform.SetParent(subtitle_.rectTransform);
            text_ = text.AddComponent<Text>();

            RectTransform text_rt = text.GetComponent<RectTransform>();
            text_rt.anchorMin = new Vector2(0, 0);
            text_rt.anchorMax = new Vector2(1, 1);
            text_rt.anchoredPosition = new Vector2(0, 0);
            text_rt.sizeDelta = new Vector2(0, 0);

            Text t = text_.GetComponent<Text>();
            Font font = Resources.Load<Font>("BuxtonSketch");
            t.font = font;
            t.fontSize = 89;
            t.color = new Color(255, 255, 255, 1);
            t.alignment = TextAnchor.MiddleCenter;

            root_.SetActive(false); 
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="_duration"></param>
        /// <param name="_mode">
        /// 0 :  fadein 
        /// 1 :  fadeout 
        /// </param>
        public void FadeBackGroundSubtitle(float _duration, int _mode)
        {
            CoroutineMgr.Start(FadeBGSubtitle(_duration, _mode));
        }

        public void UpdateSubtitle(string _text, float _duration)
        {
            Debug.Log(string.Format("[TEST: ] {0}_{1}_{2}", "UpdateSubtitle", _text, _duration));
            CoroutineMgr.Start(Update_Subtitle(_duration, _text));
        }

        public void UpdateAnimationSubtitle(Animation _anim = null)
        {
            if (_anim is InsertAnim)
            {
                InsertAnim anim = _anim as InsertAnim;
                CoroutineMgr.Start(InsertSubtitle(anim.duration, anim.mode, anim.text));
            }
            else if (_anim is ExitAnim)
            {
                ExitAnim anim = _anim as ExitAnim;
                CoroutineMgr.Start(ExitSubtitle(anim.duration, anim.mode));
            }
        }

        public void ApplyFontStyle(Color _clor, int _size)
        {
            Text t = text_.GetComponent<Text>();
            t.color = _clor;
            t.fontSize = _size;
        }


        private IEnumerator FadeBGSubtitle(float _duration, int _mode)
        {
            subtitle_.type = Image.Type.Simple;
            yield return new WaitForEndOfFrame();

            float time = 0;
            while (true)
            {
                yield return new WaitForEndOfFrame();
                time += Time.deltaTime;
                float amount = time / _duration;
                float alpha = Mathf.Abs(_mode - amount);

                Color color = subtitle_.color;
                color.a = alpha;
                subtitle_.color = color;

                if (time > _duration)
                    break;
            }
        }

        private IEnumerator Update_Subtitle(float _duration, string _text)
        {
            Text t = text_.GetComponent<Text>();
            t.text = _text;
            yield return new WaitForEndOfFrame();
            float time = 0;
            while (true)
            {
                if (t.text.Equals(""))
                    break;
                yield return new WaitForEndOfFrame();
                time += Time.deltaTime;
                //Debug.Log(time);
                if (time > _duration)
                {
                    text_.text = "";
                    break;
                }
            }
        }

        private IEnumerator InsertSubtitle(float _duration, int _mode, string _text)
        {
            float speed = 3000f;
            float left_ = canvas_.sizeDelta.x;
            float top_ = subtitle_.rectTransform.sizeDelta.y;
            Text t = text_.GetComponent<Text>();
            yield return new WaitForEndOfFrame();

            if (_mode.Equals(0))
            {
                t.rectTransform.Translate(new Vector3(-left_, 0, 0));
                t.rectTransform.anchoredPosition = new Vector2(-left_, 0);
                t.text = _text;
            }
            else if (_mode.Equals(1))
            {
                t.rectTransform.Translate(new Vector3(0, -top_, 0));
                t.rectTransform.anchoredPosition = new Vector2(0, -top_);
                t.text = _text;
            }
            else if (_mode.Equals(2) && t.text.Equals(""))
            {
                Color color = t.color;
                color.a = 0;
                t.color = color;
                t.text = _text;
                Debug.Log("2...");
                CoroutineMgr.Start(Fade_Subtitle(2));
            }
            float time = 0;
            while (true)
            {
                yield return new WaitForEndOfFrame();
                time += Time.deltaTime;

                if (t.rectTransform.anchoredPosition.x < 0 && _mode.Equals(0))
                    t.rectTransform.Translate(new Vector3(speed * Time.deltaTime, 0, 0));
                else if (t.rectTransform.anchoredPosition.y < 0 && _mode.Equals(1))
                    t.rectTransform.Translate(new Vector3(0, (speed / 3f) * Time.deltaTime, 0));
                else
                {
                    t.rectTransform.anchoredPosition = new Vector2(0, 0);
                }

                if (time > _duration)
                {
                    t.text = "";
                    break;
                }
            }
        }

        private IEnumerator ExitSubtitle(float _duration, int _mode)
        {
            float speed = 3000f;
            float left_ = canvas_.sizeDelta.x;
            //float top_ = subtitle_.rectTransform.sizeDelta.y;
            Text t = text_.GetComponent<Text>();
            yield return new WaitForEndOfFrame();

            while(!t.text.Equals(""))
            {
                yield return new WaitForEndOfFrame();
                if (t.rectTransform.anchoredPosition.x < left_ && _mode.Equals(0))
                {
                    t.rectTransform.Translate(new Vector3(speed * Time.deltaTime, 0, 0));
                }

                else if (_mode.Equals(1))
                {
                    CoroutineMgr.Start(Fade_Subtitle(_duration, 1));
                    break;
                }
                else
                {
                    t.rectTransform.anchoredPosition = new Vector2(0, 0);
                    text_.text = "";
                    break;
                }
            }

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_duration"></param>
        /// <param name="_mode">
        /// 0 : insert
        /// 1 : exit
        /// </param>
        /// <returns></returns>
        private IEnumerator Fade_Subtitle(float _duration, int _mode = 0)
        {
            Text t = text_.GetComponent<Text>();
            yield return new WaitForEndOfFrame();
            float time = 0;
            while (true)
            {
                yield return new WaitForEndOfFrame();
                time += Time.deltaTime;
                float amount = time / _duration;
                float alpha = Mathf.Abs(_mode - amount);
                Debug.Log(time);
                Color color = t.color;
                color.a = alpha;
                t.color = color;
                if (time > _duration && _mode.Equals(1))
                {
                    color.a = 1;
                    t.color = color;
                    t.text = "";
                    break;
                }
                else if (time > _duration && _mode.Equals(0))
                    break;
            }
        }
    }
}
