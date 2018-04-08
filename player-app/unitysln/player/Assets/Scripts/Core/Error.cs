/********************************************************************
     Copyright (c) zyncho.
     All rights reserved.

     author: Easlee Liu
*********************************************************************/
using Fangs.Lib.Core;

namespace VRXX
{
	public class AppError : Error
	{
        public AppError(int _errcode, string _message) : base(_errcode, _message)
        {
        }

		public override string ToString ()
		{
			return string.Format ("App Error {0}:{1}", errcode, errmessage);
		}
	}//class
}//namespace

