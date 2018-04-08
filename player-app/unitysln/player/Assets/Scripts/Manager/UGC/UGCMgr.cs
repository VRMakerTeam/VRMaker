/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Steven Sun
*********************************************************************/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Core;
using Fangs.Lib.Logger;

using System;
using System.IO;
using System.Security.Cryptography;
using ICSharpCode.SharpZipLib.GZip;
using UGC.Audio;

namespace VRXX.Manager
{
    public static class UGCMgr
    {
        public class Res
        {
            public const int TYPE_WAV = 10;
            public const int TYPE_JPG = 20;

            public string uuid;
            public int type;
            public object data;
        }
        private static Transform root_ { get; set; }

        private const string APPSecret = "d9a0254492c7429127fd4e15eb1d68ec";

        /// <summary>
        /// key: uuid 
        /// </summary>
        private static Dictionary<string, Res> ugcs_ = new Dictionary<string, Res>();
        private static Dictionary<string, byte[]> caches_ = new Dictionary<string, byte[]>();

        public static void Preload()
        {
            root_ = new GameObject("__UGCMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            Log.Info("UGCMgr", "Preload UGCMgr finish");
        }

        public static void ReadWebFile(string _file, System.Action _onReady, System.Action<byte[]> _onSuccess, System.Action<string> _onError)
        {
            CoroutineMgr.Start(readWebFile(_file, _onReady, _onSuccess, _onError));
        }

        public static void ReadRes(string _respath)
        {
            //read res
            byte[] resBytes = File.ReadAllBytes(_respath);

            //decrypt resBytes
            byte[] decrptBytes = decryptAES(resBytes, APPSecret);

            //decompress decrptBytes
            byte[] decompressBytes = decompress(decrptBytes);

            //decode decompressBytes
            decodeBytes(decompressBytes);
        }

        public static Res Find(string _uuid)
        {
            Res res = new Res();
            if (!ugcs_.TryGetValue(_uuid, out res))
                return null;
            return res;
        }

        public static void Cache(string _filename, Res _res)
        {
            ugcs_[_filename] = _res;
        }

        public static void Cache(string _filename, byte[] _data)
        {
            caches_[_filename] = _data;
        }

        public static byte[] Take(string _filename)
        {
            return caches_[_filename];
        }

        public static Material BuildSkybox(byte[] _data)
        {
            Texture2D texture = new Texture2D(2, 2);
            texture.LoadImage(_data);
            //Material mat = UGC.Image.PanoramaUtil.GenerateSkybox(texture, 1024);
            Material mat = Resources.Load<Material>("ugc_skybox");
            Material matClone = GameObject.Instantiate<Material>(mat);
            Cubemap cubemap = UGC.Image.PanoramaUtil.GenerateCubemap(texture, 1024);
            matClone.SetTexture("_Tex", cubemap);
            return matClone;
        }

        private static void decodeBytes(byte[] _bytes)
        {
            int pos = 0;
            //decode count
            int count = 0;
            byteToInt(_bytes[0], _bytes[1], _bytes[2], _bytes[3], ref count);
            pos += 4;
            //decode file
            while (count != 0)
            {
                Res res = new Res();
                //decode type
                int type = 0;
                byteToInt(_bytes[pos], _bytes[pos + 1], _bytes[pos + 2], _bytes[pos + 3], ref type);
                res.type = type;
                pos += 4;

                //decode uuid
                byte[] uuidBytes = new byte[36];
                Array.Copy(_bytes, pos, uuidBytes, 0, 36);
                string uuid = System.Text.UTF8Encoding.UTF8.GetString(uuidBytes);
                res.uuid = uuid;
                pos += 36;

                //decode size
                int size = 0;
                byteToInt(_bytes[pos], _bytes[pos + 1], _bytes[pos + 2], _bytes[pos + 3], ref size);
                pos += 4;

                //decode res
                byte[] resBytes = new byte[size];
                Array.Copy(_bytes, pos, resBytes, 0, size);
                pos += size;

                switch (type)
                {
                    case Res.TYPE_JPG:
                        Texture2D texture = new Texture2D(400, 400);
                        texture.LoadImage(resBytes);
                        res.data = texture;
                        ugcs_.Add(uuid, res);
                        break;
                    case Res.TYPE_WAV:
                        AudioClip audioClip = UGC.Audio.WAVUtil.BuildAudioClip(resBytes);
                        res.data = audioClip;
                        ugcs_.Add(uuid, res);
                        break;
                }
                count--;
            }
        }

        private static void byteToInt(byte _b0, byte _b1, byte _b2, byte _b3, ref int _i)
        {
            _i |= _b0;
            _i |= _b1 << 8;
            _i |= _b2 << 16;
            _i |= _b3 << 24;
        }

        private static byte[] decryptAES(byte[] _bytes, string _key)
        {
            byte[] keyArray = System.Text.UTF8Encoding.UTF8.GetBytes(_key);
            byte[] toEncryptArray = _bytes;

            RijndaelManaged rDel = new RijndaelManaged();
            rDel.Key = keyArray;
            rDel.Mode = CipherMode.ECB;
            rDel.Padding = PaddingMode.PKCS7;

            ICryptoTransform cTransform = rDel.CreateDecryptor();
            byte[] resultArray = cTransform.TransformFinalBlock(toEncryptArray, 0, toEncryptArray.Length);

            return resultArray;
        }

        private static byte[] decompress(byte[] _bytes)
        {
            MemoryStream ms = new MemoryStream();
            GZipInputStream gzi = new GZipInputStream(new MemoryStream(_bytes));
            int count = 0;
            byte[] data = new byte[4096];
            while ((count = gzi.Read(data, 0, data.Length)) != 0)
            {
                ms.Write(data, 0, count);
            }
            return ms.ToArray();
        }

        public static IEnumerator readWebFile(string _file, System.Action _onReady, System.Action<byte[]> _onSuccess, System.Action<string> _onError)
        {
            _onReady();
            yield return 1;
            WWW www = new WWW(VRXX.Platform.GetStreamingAssetsPath() + "/ugc/" + _file);
            yield return www;
            if(null != www.error)
            {
                _onError(www.error);
                yield break;
            }
            _onSuccess(www.bytes);
        }

    }
}
