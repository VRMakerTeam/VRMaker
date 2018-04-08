using UnityEngine;
using System.Collections;
using System.IO;

namespace UGC.Audio
{
    public class WAVUtil
    {
        // convert two bytes to one float in the range -1 to 1
        static float bytesToFloat(byte firstByte, byte secondByte)
        {
            // convert two bytes to one short (little endian)
            short s = (short)((secondByte << 8) | firstByte);
            // convert to range from -1 to (just below) 1
            return s / 32768.0F;
        }

        static int bytesToInt(byte[] bytes, int offset = 0)
        {
            int value = 0;
            for (int i = 0; i < 4; i++)
            {
                value |= ((int)bytes[offset + i]) << (i * 8);
            }
            return value;
        }

       

        public static AudioClip BuildAudioClip(byte[] _data)
        {
            // properties
           

            // Determine if mono or stereo
            int channel = _data[22];     // Forget byte 23 as 99.999% of WAVs are 1 or 2 channels

            // Get the frequency
            int frequency = bytesToInt(_data, 24);

            // Get past all the other sub chunks to get to the data subchunk:
            int pos = 12;   // First Subchunk ID from 12 to 16

            // Keep iterating until we find the data chunk (i.e. 64 61 74 61 ...... (i.e. 100 97 116 97 in decimal))
            while (!(_data[pos] == 100 && _data[pos + 1] == 97 && _data[pos + 2] == 116 && _data[pos + 3] == 97))
            {
                pos += 4;
                int chunkSize = _data[pos] + _data[pos + 1] * 256 + _data[pos + 2] * 65536 + _data[pos + 3] * 16777216;
                pos += 4 + chunkSize;
            }
            pos += 8;

            // Pos is now positioned to start of actual sound data.
            int sample = (_data.Length - pos) / 2;     // 2 bytes per sample (16 bit sound mono)
            if (channel == 2) sample /= 2;        // 4 bytes per sample (16 bit stereo)

            // Allocate memory (right will be null if only mono sound)
            float[] LeftChannel = new float[sample];
            float[] RightChannel;
            if (channel == 2) RightChannel = new float[sample];
            else RightChannel = null;

            // Write to double array/s:
            int i = 0;
            while (pos < _data.Length)
            {
                LeftChannel[i] = bytesToFloat(_data[pos], _data[pos + 1]);
                pos += 2;
                if (channel == 2)
                {
                    RightChannel[i] = bytesToFloat(_data[pos], _data[pos + 1]);
                    pos += 2;
                }
                i++;
            }
            AudioClip audioClip = AudioClip.Create("UGCSound", sample, 1, frequency, false, false);
            audioClip.SetData(LeftChannel, 0);
            return audioClip;
        }

    }

}