/********************************************************************
     Copyright (c) zyncho.
     All rights reserved.

     author: Steven Sun
*********************************************************************/

using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Logger;


namespace VRXX.Manager
{
    public static class UIMgr
    {
        private static Transform root_ { get; set; }

        /// <summary>
        /// uuid, UI
        /// </summary>
        private static Dictionary<string, GameObject> Uis = new Dictionary<string, GameObject>(0);

        public static void ClearAllUi()
        {
            foreach(GameObject obj in Uis.Values)
            {
                GameObject.Destroy(obj);
            }
        }

        public static void Preload()
        {
            root_ = new GameObject("__UIMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("TriggerMgr", "Preload UIMgr finish");
        }

        #region 3DText

        private const float buttonHeight = 40f;

        /// <summary>
        /// uuid, currentTextPage
        /// </summary>
        private static Dictionary<string, int> Text_3Ds = new Dictionary<string, int>(0);

        //-------------------------------------------

        public static GameObject New3DText(string _uuid)
        {
            if (null != CanvasMgr.FindElement3d(_uuid))
            {
                return CanvasMgr.FindElement3d(_uuid).gameObject;
            }

            GameObject root_ = new GameObject(_uuid);
            root_.AddComponent<Image>();
            CanvasMgr.Attach3DChild(root_.transform);

            RectTransform panel = root_.GetComponent<RectTransform>();
            panel.sizeDelta = new Vector2(100, 100);
            panel.localPosition = new Vector3(0, 0, 6);
            panel.localScale = new Vector3(0.01f, 0.01f, 0.01f);

            GameObject textbox = new GameObject("Page_1");
            textbox.AddComponent<Text>();
            textbox.transform.SetParent(panel);

            RectTransform textTransform = textbox.GetComponent<RectTransform>();
            textTransform.localPosition = new Vector3(0, 0, 0);
            textTransform.localScale = new Vector3(1f, 1f, 1f);
            textTransform.sizeDelta = new Vector2(0, -40);
            textTransform.anchoredPosition = new Vector2(0, 20);
            textTransform.anchorMin = new Vector2(0, 0);
            textTransform.anchorMax = new Vector2(1, 1);
            textTransform.pivot = new Vector2(0.5f, 0.5f);

            Text text = textbox.GetComponent<Text>();
            //text.color = new Color(0, 0, 0);
            text.alignment = TextAnchor.UpperLeft;
            //text.fontSize = 30;

            Font mFont = Resources.Load<Font>("BuxtonSketch");
            text.font = mFont;

            GameObject close = new GameObject("close");
            close.AddComponent<Image>();
            close.AddComponent<Button>();
            close.transform.SetParent(panel);

            RectTransform closeTransform = close.GetComponent<RectTransform>();
            closeTransform.localPosition = new Vector3(0, 0, 0);
            closeTransform.localScale = new Vector3(1f, 1f, 1f);
            closeTransform.sizeDelta = new Vector3(40, 40);
            closeTransform.anchoredPosition = new Vector2(0, 20);
            closeTransform.anchorMin = new Vector2(0.5f, 0);
            closeTransform.anchorMax = new Vector2(0.5f, 0);
            closeTransform.pivot = new Vector2(0.5f, 0.5f);

            Button but_close = close.GetComponent<Button>();
            but_close.GetComponent<Image>().sprite = (Sprite)Resources.LoadAll("Close", typeof(Sprite))[0];

            but_close.onClick.AddListener(delegate ()
            {
                Delete3DText(root_);
            });

            //up page
            GameObject left = new GameObject("up");
            left.AddComponent<Image>();
            left.AddComponent<Button>();
            left.transform.SetParent(panel);

            RectTransform leftTransform = left.GetComponent<RectTransform>();
            leftTransform.localPosition = new Vector3(0, 0, 0);
            leftTransform.localScale = new Vector3(1f, 1f, 1f);
            leftTransform.sizeDelta = new Vector3(40, 40);
            leftTransform.anchoredPosition = new Vector2(20, 20);
            leftTransform.anchorMin = new Vector2(0, 0);
            leftTransform.anchorMax = new Vector2(0, 0);
            leftTransform.pivot = new Vector2(0.5f, 0.5f);

            Button butt_left = left.GetComponent<Button>();
            butt_left.GetComponent<Image>().sprite = (Sprite)Resources.LoadAll("up", typeof(Sprite))[0];

            butt_left.onClick.AddListener(delegate ()
            {
                Transform root = butt_left.transform.parent;
                string uuid = root.name;
                int curPage = Text_3Ds[uuid];

                if (curPage <= 1)
                {
                    Debug.Log("Is the first page...");
                    return;
                }

                root.FindChild(string.Format("Page_{0}", curPage)).gameObject.SetActive(false);
                curPage--;
                root.FindChild(string.Format("Page_{0}", curPage)).gameObject.SetActive(true);

                Text_3Ds[uuid] = curPage;
            });

            //down page
            GameObject right = new GameObject("down");
            right.AddComponent<Image>();
            right.AddComponent<Button>();
            right.transform.SetParent(panel);

            RectTransform rightTransform = right.GetComponent<RectTransform>();
            rightTransform.localPosition = new Vector3(0, 0, 0);
            rightTransform.localScale = new Vector3(1f, 1f, 1f);
            rightTransform.sizeDelta = new Vector3(40, 40);
            rightTransform.anchoredPosition = new Vector2(-20, 20);
            rightTransform.anchorMin = new Vector2(1, 0);
            rightTransform.anchorMax = new Vector2(1, 0);
            rightTransform.pivot = new Vector2(0.5f, 0.5f);

            Button butt_right = right.GetComponent<Button>();
            butt_right.GetComponent<Image>().sprite = (Sprite)Resources.LoadAll("down", typeof(Sprite))[0];

            butt_right.onClick.AddListener(delegate ()
            {
                Transform root = butt_left.transform.parent;

                string uuid = root.name;
                int curPage = Text_3Ds[uuid];

                Transform temp = null;
                temp = root.FindChild(string.Format("Page_{0}", curPage + 1));
                if (temp == null)
                {
                    Debug.Log("It's already the last page...");
                    return;
                }

                root.FindChild(string.Format("Page_{0}", curPage)).gameObject.SetActive(false);
                curPage++;
                root.FindChild(string.Format("Page_{0}", curPage)).gameObject.SetActive(true);
                Text_3Ds[uuid] = curPage;
            });

            TogglePaging(root_, false);

            if (!Text_3Ds.ContainsKey(_uuid))
                Text_3Ds.Add(_uuid, 1);

            Uis.Add(_uuid, root_);
            return root_;
        }

        // Modify panel
        public static void Modify3DText_Position(GameObject _3dText, Vector3 _position)
        {
            _3dText.GetComponent<RectTransform>().localPosition = _position;
        }

        public static void Modify3DText_Rotation(GameObject _3dText, Vector3 _rotation)
        {
            _3dText.GetComponent<RectTransform>().localEulerAngles = _rotation;
        }

        public static void Modify3DText_Scale(GameObject _3dText, Vector3 _scale)
        {
            _3dText.GetComponent<RectTransform>().localScale = _scale;
        }

        public static void Modify3DText_Color(GameObject _3dText, Color _color)
        {
            _3dText.GetComponent<Image>().color = _color;
        }

        public static void Modify3DText_Size(GameObject _3dText, Vector2 _size)
        {
            _3dText.GetComponent<RectTransform>().sizeDelta = _size;
        }

        public static void Show3DText_Horizontal(GameObject _3dText, string _text, int _fontsize, Vector2 _size, int _Max_y = 500)
        {
            Modify3DText_Size(_3dText, _size);
            _3dText.GetComponentInChildren<Text>().fontSize = _fontsize;
            Text text = _3dText.GetComponentInChildren<Text>();
            text.text = _text;

            if (text.preferredHeight <= _Max_y)
            {
                if (text.preferredHeight <= _size.y)
                    return;

                Modify3DText_Size(_3dText, new Vector2(_size.x, text.preferredHeight + buttonHeight));
            }
            else
            {
                Modify3DText_Size(_3dText, new Vector2(_size.x, _Max_y));
                TogglePaging(_3dText, true);
                ShowPage_Max(_3dText);
            }
        }

        public static void Show3DText_Vertical(GameObject _3dText, string _text, int _fontsize, Vector2 _size, int _Max_x = 600)
        {
            Modify3DText_Size(_3dText, _size);
            _3dText.GetComponentInChildren<Text>().fontSize = _fontsize;
            Text text = _3dText.GetComponentInChildren<Text>();
            text.text = _text;

            bool finish = true;
            while (finish)
            {
                if (text.preferredHeight + buttonHeight >= _size.y)
                {
                    _size.x += _fontsize;
                    Modify3DText_Size(_3dText, new Vector2(_size.x, _size.y));

                    if (_size.x > _Max_x)
                    {
                        Modify3DText_Size(_3dText, new Vector2(_Max_x, _size.y));

                        if (text.preferredHeight + buttonHeight > _size.y)
                        {
                            TogglePaging(_3dText, true);
                            ShowPage_Max(_3dText);
                        }

                        finish = false;
                    }
                }
                else
                {
                    finish = false;
                }
            }
        }

        // Modify text
        public static void Modify3DText_Text(GameObject _3dText, string _text)
        {
            Text text = _3dText.GetComponentInChildren<Text>();
            text.text = _text;
        }

        public static void Modify3DText_FontSize(GameObject _3dText, int _size)
        {
            _3dText.GetComponentInChildren<Text>().fontSize = _size;
        }

        public static void Modify3DText_FontColor(GameObject _3dText, Color _color)
        {
            _3dText.GetComponentInChildren<Text>().color = _color;
        }


        private static void TogglePaging(GameObject _3dText, bool _value)
        {
            _3dText.transform.FindChild("up").gameObject.SetActive(_value);
            _3dText.transform.FindChild("down").gameObject.SetActive(_value);
        }

        private static void ShowPage_Max(GameObject _3dText)
        {
            Transform page_1 = _3dText.transform.FindChild("Page_1");
            Text page = page_1.GetComponent<Text>();

            int PageLineCount = GetPageLineCount(page, _3dText.GetComponent<RectTransform>().sizeDelta.y - buttonHeight);
            List<string> mlis = StripText(page);

            int PageCount = mlis.Count / PageLineCount;
            if (mlis.Count % PageLineCount != 0)
                PageCount += 1;

            string page_1_text = "";
            for (int i = 0; i < PageLineCount; i++)
            {
                if (mlis.Count.Equals(0))
                    break;

                page_1_text += mlis[0];
                mlis.RemoveAt(0);
            }
            page.text = page_1_text;


            for (int i = 0; i < PageCount - 1; i++)
            {
                string name = string.Format("Page_{0}", i + 2);
                CopyPage(page, name);

                Text text = _3dText.transform.FindChild(name).GetComponent<Text>();
                string temp = "";
                for (int j = 0; j < PageLineCount; j++)
                {
                    if (mlis.Count.Equals(0))
                        break;

                    temp += mlis[0];
                    mlis.RemoveAt(0);
                }
                text.text = temp;
            }

        }

        private static int GetPageLineCount(Text _text, float _height)
        {
            string temp = _text.text;
            _text.text = "";
            float lineHeight = _text.preferredHeight;
            _text.text = temp;

            return (int)(_height / (lineHeight + _text.lineSpacing));
        }

        private static List<string> StripText(Text _text)
        {
            string str = _text.text;
            List<string> result = new List<string>();

            Font myFont = _text.font;
            myFont.RequestCharactersInTexture(str, _text.fontSize, _text.fontStyle);
            CharacterInfo characterInfo = new CharacterInfo();

            float lineWidth = _text.transform.parent.GetComponent<RectTransform>().sizeDelta.x;
            char[] arr = str.ToCharArray();
            int tempWidth = 0;
            int startIndex = 0;
            int endIndex = 0;

            for (int i = 0; i < arr.Length; i++)
            {
                myFont.GetCharacterInfo(arr[i], out characterInfo, _text.fontSize);
                tempWidth += characterInfo.advance;
                endIndex++;

                if (tempWidth > lineWidth)
                {
                    result.Add(str.Substring(startIndex, endIndex - 1));
                    startIndex += endIndex - 1;
                    endIndex = 0;
                    tempWidth = 0;
                    i--;
                }
                if (tempWidth == lineWidth)
                {
                    result.Add(str.Substring(startIndex, endIndex));
                    startIndex += endIndex;
                    endIndex = 0;
                    tempWidth = 0;
                }
            }

            if (!endIndex.Equals(0))
            {
                result.Add(str.Substring(startIndex, endIndex));
            }

            return result;
        }

        private static Text CopyPage(Text _text, string _name)
        {
            Text clone = GameObject.Instantiate<Text>(_text, _text.transform.parent);
            clone.name = _name;
            clone.gameObject.SetActive(false);

            return clone;
        }

        private static void Delete3DText(GameObject _3dText)
        {
            GameObject.Destroy(_3dText);
            Text_3Ds.Remove(_3dText.name);
        }



        #endregion

    } //class

} //namespace

