/********************************************************************
     Copyright (c) zyncho.
     All rights reserved.

     author: Steven Sun
*********************************************************************/
using System.Collections.Generic;
using VRXX.Manager;

namespace VRXX
{
    /// <summary>
    /// <code>
    /// class Product
    /// {
    /// }
    /// class ProductFactory : Factory<Product>
    /// {
    /// }
    /// </code>
    /// </summary>
    public class Factory<T> where T : class, new()
    {
        /// <summary>
        /// </summary>
        /// <param name="_uuid"></param>
        /// <returns>
        /// if _uuid is exists, return null
        /// </returns>
        public static T New(string _uuid)
        {
            if (products.ContainsKey(_uuid))
                return null;
            
            T product = new T();
            products.Add(_uuid, product);
            return product;
        }

        public static void Delete(T _slate)
        {
            Delete(_slate);
        }

        /// <summary>
        /// </summary>
        /// <param name="_uuid"></param>
        /// <returns>
        /// if _uuid isn't exists, return null
        /// </returns>
        public static T Find(string _uuid)
        {
            if (!products.ContainsKey(_uuid))
                return null;

            T product = null;
            products.TryGetValue(_uuid, out product);
            return product;
        }

        private static Dictionary<string, T> products = new Dictionary<string, T>();
    }//class
}//namespace
