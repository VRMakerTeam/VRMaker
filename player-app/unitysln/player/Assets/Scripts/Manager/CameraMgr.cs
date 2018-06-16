/********************************************************************
     Copyright (c) zyncho
     All rights reserved.

     author: Easlee Liu
*********************************************************************/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Fangs.Lib.Logger;
using Fangs.Lib.Core;

namespace VRXX.Manager
{
    public static class CameraMgr
    {
        public delegate void OnFinishDelegate();
        public enum Viewport
        {
            FREE,
            FRONT,
            BACK,
            LEFT,
            RIGHT,
            TOP,
            BOTTOM
        }
        private static Transform root_ { get; set; }
        private static Skybox skyboxCpt { get; set; }
        private static Transform camera_ { get; set; }
        private static Transform innerCamera_ { get; set; }

        static Vector3 Face_ = new Vector3();
        static Vector3 Left_ = new Vector3();
        static Vector3 Right_ = new Vector3();
        private static float cameraControlSpeed_ = 50f;

        public static Transform camera
        {
            get
            {
                return camera_;
            }
        }

        public static Vector3 position
        {
            get
            {
                return camera_.position;
            }
        }

        public static Vector3 rotation
        {
            get
            {
                return camera_.rotation.eulerAngles;
            }
        }

        public static Vector3 forward
        {
            get
            {
                return camera_.forward;
            }
        }

        public static Material skybox
        {
            get
            {
                return RenderSettings.skybox;
            }
        }

        public static void Preload()
        {
            root_ = new GameObject("__CameraMgr__").transform;
            GameObject.DontDestroyOnLoad(root_.gameObject);
            GameObject go = new GameObject("Camera");
            go.tag = "MainCamera";
            go.transform.SetParent(root_);
            go.AddComponent<Camera>();
            go.AddComponent<GUILayer>();
            go.AddComponent<FlareLayer>();
            skyboxCpt = go.AddComponent<Skybox>();
            skyboxCpt.enabled = false;
            camera_ = go.transform;
            Log.Info("CameraMgr", "Preload CameraMgr finish");
            innerCamera_ = camera_;
        }

        public static GameObject GetMainCamera()
        {
            return camera_.gameObject;
        }

        public static void InjectOuterCamera(Transform _camera)
        {
            camera_ = _camera;
            innerCamera_.gameObject.SetActive(false);
            _camera.position = innerCamera_.position;
            _camera.rotation = innerCamera_.rotation;
        }

        public static void ReuseInnerCamera()
        {
            innerCamera_.position = camera_.position;
            innerCamera_.rotation = camera_.rotation;
            camera_ = innerCamera_;
            camera_.gameObject.SetActive(true);
        }

        public static void ToggleCamera(bool _visible)
        {
            camera_.gameObject.SetActive(_visible);
        }

        public static void ApplySkybox(Material _skybox)
        {
            RenderSettings.skybox = _skybox;
            //skyboxCpt.material = _skybox;
            //skyboxCpt.enabled = true;
        }

        public static void CleanSkybox()
        {
            RenderSettings.skybox = null;
        }

        public static void UseDefaultSkybox()
        {
            Material skybox = Resources.Load<Material>("default-skybox");
            RenderSettings.skybox = skybox;
        }

        public static void ChangeViewport(Viewport _viewport)
        {
            Vector3 rotation = Vector3.zero;
            if(Viewport.FREE == _viewport)
            {
                rotation.x = 45;
                rotation.y = -45;
            }
            else if (Viewport.FRONT == _viewport)
            {
                rotation.y = 180;
            }
            else if (Viewport.BACK == _viewport)
            {
                //nothing to do
            }
            else if (Viewport.LEFT == _viewport)
            {
                rotation.y = 90;
            }
            else if (Viewport.RIGHT == _viewport)
            {
                rotation.y = -90;
            }
            else if (Viewport.TOP == _viewport)
            {
                rotation.x = 90;
            }
            else if (Viewport.BOTTOM == _viewport)
            {
                rotation.x = -90;
            }
            camera_.localRotation = Quaternion.Euler(rotation);
        }

        public static void Place(Vector3 _position)
        {
            camera_.position = _position;
        }

        public static void Shake(float _duration, OnFinishDelegate _onFinish)
        {
            CoroutineMgr.Start(ShakeCamera(_duration, _onFinish));
        }

        private static IEnumerator ShakeCamera(float _duration, OnFinishDelegate _onFinish)
        {
            Quaternion rot = camera_.GetComponent<Transform>().rotation;
            float shakelevel = 2f;
            yield return new WaitForEndOfFrame();

            float time = 0;
            while (time < _duration)
            {
                yield return new WaitForEndOfFrame();
                time += Time.deltaTime;
                Vector3 vet = rot.eulerAngles + Random.insideUnitSphere * shakelevel;
                camera_.GetComponent<Transform>().rotation = Quaternion.Euler(vet);
            }

            camera_.GetComponent<Transform>().rotation = rot;

            //Transform c = camera_.GetComponent<Transform>();
            //Vector3 originPosition = c.position;
            //Quaternion originRotation = c.rotation;
            //float x = c.position.x;
            //float y = c.position.y;
            //float z = c.position.z;
            //float _x = c.rotation.x;
            //float _y = c.rotation.y;
            //float _z = c.rotation.z;
            //float _w = c.rotation.w;

            //float shake_intensity = 0.2f;
            //float shake_decay = 0.0006f;
            //yield return new WaitForEndOfFrame();

            //float time = 0;
            //while (time < _duration)
            //{
            //    yield return new WaitForEndOfFrame();
            //    time += Time.deltaTime;
            //    c.position = originPosition + Random.insideUnitSphere * shake_intensity;
            //    c.rotation = new Quaternion(
            //    originRotation.x + Random.Range(-shake_intensity, shake_intensity) * 0.2f,
            //    originRotation.y + Random.Range(-shake_intensity, shake_intensity) * 0.2f,
            //    originRotation.z + Random.Range(-shake_intensity, shake_intensity) * 0.2f,
            //    originRotation.w + Random.Range(-shake_intensity, shake_intensity) * 0.2f);
            //    shake_intensity -= shake_decay;
            //}
            //camera_.position = new Vector3(x, y, z);
            //camera_.rotation = new Quaternion(_x, _y, _z, _w);
            //_onFinish();
        }


        public static void ZoomPush(float _duration, float _distance)
        {
            CoroutineMgr.Start(ZoomCamera(_duration, _distance));
        }
        public static void ZoomPull(float _duration, float _distance)
        {
            CoroutineMgr.Start(ZoomCamera(_duration, _distance, false));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_mode">
        /// true : zoompush
        /// false : zoompull
        /// </param>
        /// <returns></returns>
        private static IEnumerator ZoomCamera(float _duration, float _distance, bool _mode = true)
        {
            float duration = Mathf.Abs(_duration);
            float speed = _distance/ duration;
            Transform c = camera_.GetComponent<Transform>();
            yield return new WaitForEndOfFrame();
            if (!_mode)
            {
                c.position -= c.forward * _distance;
            }
            
            float timer = 0;
            while (timer < duration)
            {
                yield return new WaitForEndOfFrame();
                timer += Time.deltaTime;
                c.Translate(c.transform.forward * (speed)* Time.deltaTime);
            }
        }

        public static void Zoom(float _speed)
        {
            camera_.Translate(Vector3.forward * _speed, Space.Self);
        }

        public static void Euler(Vector3 _rotation)
        {
            camera_.rotation = Quaternion.Euler(_rotation);
        }

        public static void FOV(float _value)
        {
            Camera _camera = camera_.GetComponent<Camera>();
            _camera.fieldOfView = _value;
        }

        public static void Move(Vector3 _position, Vector3 _rotation, float _duration, OnFinishDelegate _onFinis)
        {
            CoroutineMgr.Start(move(_position, _rotation, _duration, _onFinis));
        }

        public static void Translate(float _x, float _y, float _z)
        {
            camera_.Translate(new Vector3(_x, _y, _z));
        }
        public static void Translate(Transform _transform)
        {
            camera_.Translate(_transform.position);
        }

        public static void Fly(Vector3 _axis, float _speed)
        {
            Vector3 forward = camera_.forward * _axis.z;
            Vector3 up = camera_.up * _axis.y;
            Vector3 right = camera_.right * _axis.x;
            Vector3 direction = forward + up + right;
            direction.Normalize();
            camera_.position += direction * _speed * Time.deltaTime;
        }


        public static void Rotate(Vector3 _axis)
        {
            Vector3 rotation = camera_.rotation.eulerAngles;
            rotation.x += _axis.x;
            rotation.y += _axis.y;
            rotation.z += _axis.z;

            camera_.rotation = Quaternion.Euler(rotation);

        }

        public static void LookAt(Transform _target)
        {
            camera_.LookAt(_target);
        }
        public static Ray ScreenPointToRay(Vector3 _position)
        {
            Camera camera = camera_.GetComponent<Camera>();
            return camera.ScreenPointToRay(_position);
        }

        public static void Align(Transform _target)
        {
            /*
            float dist = Vector3.Distance(camera_.position, _target.position);
            Vector3 direction = _target.position - camera_.position;
            float angleForward = Vector3.Angle(camera_.forward, direction);
            float angleRight = Vector3.Angle(camera_.right, direction);
            float lengthRight = Mathf.Abs(Mathf.Sin(Mathf.Deg2Rad* angleForward) * dist);
            float lengthForward = Mathf.Abs(Mathf.Cos(Mathf.Deg2Rad * angleForward) * dist);
            */
            float maxLength = 0;
            MeshFilter mesh = _target.GetComponent<MeshFilter>();
            if (null != mesh)
            {
                Vector3 size = mesh.mesh.bounds.size;
                if (size.x > maxLength)
                    maxLength = size.x;
                if (size.y > maxLength)
                    maxLength = size.y;
                if (size.z > maxLength)
                    maxLength = size.z;
            }
            foreach (Transform child in _target)
            {
                mesh = child.GetComponent<MeshFilter>();
                if (null == mesh)
                    continue;
                Vector3 size = mesh.mesh.bounds.size;
                if (size.x > maxLength)
                    maxLength = size.x;
                if (size.y > maxLength)
                    maxLength = size.y;
                if (size.z > maxLength)
                    maxLength = size.z;
            }

            // move the camera to the target before the adjust
            camera_.transform.position = _target.position;
            camera_.Translate(-Vector3.forward * maxLength * 2, Space.Self);
        }

        private static IEnumerator move(Vector3 _position, Vector3 _rotation, float _duration, OnFinishDelegate _onFinish)
        {
            Vector3 camerapos = camera_.position;
            Quaternion camerarot = camera_.rotation;
            float timer = 0f;
            while (timer < _duration)
            {
                camera_.position = Vector3.Lerp(camerapos, _position, timer / _duration);
                camera_.rotation = Quaternion.Lerp(camerarot, Quaternion.Euler(_rotation), timer / _duration);
                //TODO Use Lerp
               // camera_.LookAt(_position);
                yield return new WaitForEndOfFrame();
                timer += Time.deltaTime;
            }
            if (null != _onFinish)
                _onFinish();
        }
    }//class
}//namespace

