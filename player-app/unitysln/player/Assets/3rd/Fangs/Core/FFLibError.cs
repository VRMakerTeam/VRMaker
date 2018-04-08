/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

namespace Fangs.Lib.Core
{
    /// <summary>
    /// NOTE: errcode < 0 used for function
    /// </summary>
    public class Error
    {
        public int errcode;
        public string errmessage;
        private static Error ok_ = new Error(0, null);

        public Error(int _errcode, string _errmessage)
        {
            errcode = _errcode;
            errmessage = _errmessage;
        }

        public override string ToString()
        {
        	return string.Format("LibError {0}:{1}", errcode, errmessage);
        }

        public static Error OK
        {
            get
            {
                return ok_;
            }
        }

        public static Error NewNullErr(string _message, params object[] _args)
        {
            string message = string.Format(_message, _args);
            return new Error(-1, message);
        }

        public static Error NewParamErr(string _message, params object[] _args)
        {
            string message = string.Format(_message, _args);
            return new Error(-2, message);
        }

        public static Error NewAccessErr(string _message, params object[] _args)
        {
            string message = string.Format(_message, _args);
            return new Error(-3, message);
        }
    }//class
}//namespace

