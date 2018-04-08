/********************************************************************
     Copyright (c) zyncho.
     All rights reserved.

     author: Easlee Liu
*********************************************************************/
using UnityEngine;

namespace VRXX
{
    public static class Convert
    {
        public static int ByteToInt(byte[] _bytes, int _start)
        {
            if (_bytes.Length < _start + 4)
                throw new System.IndexOutOfRangeException();

            int value  = 0;
            value |= _bytes[_start];
            value |= _bytes[_start + 1] << 8;
            value |= _bytes[_start + 2] << 16;
            value |= _bytes[_start + 3] << 24;
            return value;
        }

        public static string ByteToString(byte[] _bytes, int _start, int _length)
        {
            if (_bytes.Length < _start + _length)
                throw new System.IndexOutOfRangeException();

            return System.Text.UTF8Encoding.UTF8.GetString(_bytes, _start, _length);
        }
    }
}