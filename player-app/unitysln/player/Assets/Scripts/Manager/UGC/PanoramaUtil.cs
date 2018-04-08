using UnityEngine;
using System.Collections;
using System.IO;

namespace UGC.Image
{
    public class PanoramaUtil
    {
        private enum Face
        {
            FRONT,
            BACK,
            LEFT,
            RIGHT,
            UP,
            DOWN,
        }

        public static Material GenerateSkybox(Texture2D _srcTexture, int _texSize)
        {
            try
            {
                Material skybox = new Material(Shader.Find("Mobile/Skybox"));

                Texture2D frontText = convertCubemap(_srcTexture, _texSize, Face.FRONT);
                skybox.SetTexture("_FrontTex", frontText);

                Texture2D backText = convertCubemap(_srcTexture, _texSize, Face.BACK);
                skybox.SetTexture("_BackTex", backText);

                Texture2D leftText = convertCubemap(_srcTexture, _texSize, Face.LEFT);
                skybox.SetTexture("_LeftTex", leftText);

                Texture2D rightText = convertCubemap(_srcTexture, _texSize, Face.RIGHT);
                skybox.SetTexture("_RightTex", rightText);

                Texture2D upText = convertCubemap(_srcTexture, _texSize, Face.UP);
                skybox.SetTexture("_UpTex", upText);

                Texture2D downText = convertCubemap(_srcTexture, _texSize, Face.DOWN);
                skybox.SetTexture("_DownTex", downText);
                return skybox;
            }
            catch (System.Exception)
            {
                return null;
            }
        }

        public static Cubemap GenerateCubemap(Texture2D _srcTexture, int _texSize)
        {
            Cubemap cubemap = new Cubemap(_texSize, TextureFormat.RGB24, false);
            //front
            {
                Texture2D frontText = convertCubemap(_srcTexture, _texSize, Face.FRONT);
                Color[] dstCols = new Color[_texSize * _texSize];
                int iPos = 0;
                for (int y = 0; y < _texSize; y++)
                {
                    Color[] srcLines = frontText.GetPixels(0, _texSize - y - 1, _texSize, 1);
                    for (int x = 0; x < _texSize; x++)
                    {
                        dstCols[iPos + x] = srcLines[x];
                    }
                    iPos += _texSize;
                }
                cubemap.SetPixels(dstCols, CubemapFace.PositiveZ);
            }
            //back
            {
                Texture2D backText = convertCubemap(_srcTexture, _texSize, Face.BACK);

                Color[] dstCols = new Color[_texSize * _texSize];

                int iPos = 0;
                for (int y = 0; y < _texSize; y++)
                {
                    Color[] srcLines = backText.GetPixels(0, _texSize - y - 1, _texSize, 1);
                    for (int x = 0; x < _texSize; x++)
                    {
                        dstCols[iPos + x] = srcLines[x];
                    }
                    iPos += _texSize;
                }
                cubemap.SetPixels(dstCols, CubemapFace.NegativeZ);
            }
            //left
            {
                Texture2D leftText = convertCubemap(_srcTexture, _texSize, Face.LEFT);

                Color[] dstCols = new Color[_texSize * _texSize];

                int iPos = 0;
                for (int y = 0; y < _texSize; y++)
                {
                    Color[] srcLines = leftText.GetPixels(0, _texSize - y - 1, _texSize, 1);
                    for (int x = 0; x < _texSize; x++)
                    {
                        dstCols[iPos + x] = srcLines[x];
                    }
                    iPos += _texSize;
                }
                cubemap.SetPixels(dstCols, CubemapFace.PositiveX);
            }
            //right
            {
                Texture2D rightText = convertCubemap(_srcTexture, _texSize, Face.RIGHT);

                Color[] dstCols = new Color[_texSize * _texSize];

                int iPos = 0;
                for (int y = 0; y < _texSize; y++)
                {
                    Color[] srcLines = rightText.GetPixels(0, _texSize - y - 1, _texSize, 1);
                    for (int x = 0; x < _texSize; x++)
                    {
                        dstCols[iPos + x] = srcLines[x];
                    }
                    iPos += _texSize;
                }
                cubemap.SetPixels(dstCols, CubemapFace.NegativeX);
            }
            //up
            {
                Texture2D upText = convertCubemap(_srcTexture, _texSize, Face.UP);

                Color[] dstCols = new Color[_texSize * _texSize];

                int iPos = 0;
                for (int y = 0; y < _texSize; y++)
                {
                    Color[] srcLines = upText.GetPixels(0, _texSize - y - 1, _texSize, 1);
                    for (int x = 0; x < _texSize; x++)
                    {
                        dstCols[iPos + x] = srcLines[x];
                    }
                    iPos += _texSize;
                }
                cubemap.SetPixels(dstCols, CubemapFace.PositiveY);
            }
            //down
            {
                Texture2D downText = convertCubemap(_srcTexture, _texSize, Face.DOWN);

                Color[] dstCols = new Color[_texSize * _texSize];

                int iPos = 0;
                for (int y = 0; y < _texSize; y++)
                {
                    Color[] srcLines = downText.GetPixels(0, _texSize - y - 1, _texSize, 1);
                    for (int x = 0; x < _texSize; x++)
                    {
                        dstCols[iPos + x] = srcLines[x];
                    }
                    iPos += _texSize;
                }
                cubemap.SetPixels(dstCols, CubemapFace.NegativeY);
            }
            cubemap.Apply();
            return cubemap;
        }

        private static Texture2D convertCubemap(Texture2D _srcTexture, int _textSize, Face _face)
        {
            Texture2D tex = new Texture2D(_textSize, _textSize, TextureFormat.RGB24, false);

            Vector3[] vDirA = new Vector3[4];
            if (_face == Face.FRONT)
            {
                vDirA[0] = new Vector3(-1.0f, -1.0f, -1.0f);
                vDirA[1] = new Vector3(1.0f, -1.0f, -1.0f);
                vDirA[2] = new Vector3(-1.0f, 1.0f, -1.0f);
                vDirA[3] = new Vector3(1.0f, 1.0f, -1.0f);
            }
            if (_face == Face.BACK)
            {
                vDirA[0] = new Vector3(1.0f, -1.0f, 1.0f);
                vDirA[1] = new Vector3(-1.0f, -1.0f, 1.0f);
                vDirA[2] = new Vector3(1.0f, 1.0f, 1.0f);
                vDirA[3] = new Vector3(-1.0f, 1.0f, 1.0f);
            }
            if (_face == Face.LEFT)
            {
                vDirA[0] = new Vector3(1.0f, -1.0f, -1.0f);
                vDirA[1] = new Vector3(1.0f, -1.0f, 1.0f);
                vDirA[2] = new Vector3(1.0f, 1.0f, -1.0f);
                vDirA[3] = new Vector3(1.0f, 1.0f, 1.0f);
            }
            if (_face == Face.RIGHT)
            {
                vDirA[0] = new Vector3(-1.0f, -1.0f, 1.0f);
                vDirA[1] = new Vector3(-1.0f, -1.0f, -1.0f);
                vDirA[2] = new Vector3(-1.0f, 1.0f, 1.0f);
                vDirA[3] = new Vector3(-1.0f, 1.0f, -1.0f);
            }
            if (_face == Face.UP)
            {
                vDirA[0] = new Vector3(-1.0f, 1.0f, -1.0f);
                vDirA[1] = new Vector3(1.0f, 1.0f, -1.0f);
                vDirA[2] = new Vector3(-1.0f, 1.0f, 1.0f);
                vDirA[3] = new Vector3(1.0f, 1.0f, 1.0f);
            }
            if (_face == Face.DOWN)
            {
                vDirA[0] = new Vector3(-1.0f, -1.0f, 1.0f);
                vDirA[1] = new Vector3(1.0f, -1.0f, 1.0f);
                vDirA[2] = new Vector3(-1.0f, -1.0f, -1.0f);
                vDirA[3] = new Vector3(1.0f, -1.0f, -1.0f);
            }

            Vector3 rotDX1 = (vDirA[1] - vDirA[0]) / (float)_textSize;
            Vector3 rotDX2 = (vDirA[3] - vDirA[2]) / (float)_textSize;

            float dy = 1.0f / (float)_textSize;
            float fy = 0.0f;

            Color[] cols = new Color[_textSize];
            for (int y = 0; y < _textSize; y++)
            {
                Vector3 xv1 = vDirA[0];
                Vector3 xv2 = vDirA[2];
                for (int x = 0; x < _textSize; x++)
                {
                    Vector3 v = ((xv2 - xv1) * fy) + xv1;
                    v.Normalize();
                    cols[x] = calcProjectionSpherical(_srcTexture, v);
                    xv1 += rotDX1;
                    xv2 += rotDX2;
                }
                tex.SetPixels(0, y, _textSize, 1, cols);
                fy += dy;
            }
            tex.wrapMode = TextureWrapMode.Clamp;
            tex.Apply();
            return tex;
        }

        private static Color calcProjectionSpherical(Texture2D _srcTexture ,Vector3 vDir)
        {
            float direction = 0.0f;
            float theta = Mathf.Atan2(vDir.z, vDir.x);
            float phi = Mathf.Acos(vDir.y);

            theta += direction * Mathf.PI / 180.0f;
            while (theta < -Mathf.PI) theta += Mathf.PI + Mathf.PI;
            while (theta > Mathf.PI) theta -= Mathf.PI + Mathf.PI;

            float dx = theta / Mathf.PI;
            float dy = phi / Mathf.PI;

            dx = dx * 0.5f + 0.5f;
            int px = (int)(dx * (float)_srcTexture.width);
            if (px < 0) px = 0;
            if (px >= _srcTexture.width) px = _srcTexture.width - 1;
            int py = (int)(dy * (float)_srcTexture.height);
            if (py < 0) py = 0;
            if (py >= _srcTexture.height) py = _srcTexture.height - 1;

            Color col = _srcTexture.GetPixel(px, _srcTexture.height - py - 1);
            return col;
        }
    }
}