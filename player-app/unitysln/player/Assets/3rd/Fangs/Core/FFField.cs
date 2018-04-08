/********************************************************************
     Copyright (c) OMO Game Studio.
     All rights reserved.

	 \ Fangs Framework /

     author:	Easlee Liu

*********************************************************************/

namespace Fangs.Lib.Core
{
    public class Field
    {
        private object value;

        public Field(object _value)
        {
            if(_value is System.Int16 ||
               _value is System.UInt16 ||
                _value is System.Int32 ||
                _value is System.UInt32 ||
                _value is System.Int64 ||
                _value is System.UInt64 ||
                _value is System.Double ||
                _value is System.Boolean ||
                _value is System.String)
            {
                value = _value;
            }
            else
            {
                throw new System.ArgumentException("Unable to wrap the given object with Field");
            }
        }

        public static explicit operator System.String(Field _field)
        {
            if (!(_field.value is System.String))
                throw new System.InvalidCastException(
                    "Instance of Field doesn't hold a string");
            return (string)_field.value;
        }
        
    }//class
}//namespace

