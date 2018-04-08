/********************************************************************
Copyright (c) 2010-2020 by Easlee
@created:	10-10-2011 
@filename: 	Factory.hxx
@author:	Easlee Liu (liu@easlee.me)
@desc:
*********************************************************************/
#ifndef EL_GDUT_DESIGNPATTERN_FACTORY_H_
#define EL_GDUT_DESIGNPATTERN_FACTORY_H_

#include <map>
#include <string>
#include <assert.h>

namespace el
{
	namespace gdut
	{

		template<class T> class ProductManagerPrivate;
		class ProductClassInfoPrivate;
		template<class T> class ProductClassCreatorPrivate;

		template< class T >
		class Factory
		{
		public:
			template <class K>
			static T* create()
			{
				return new K();
			}

			static T* create(std::string _productName)
			{
				return createProduct(_productName);
			}

			static T* create()
			{
				return createProduct(productName());
			}

			static void destory(T* _object)
			{
				if (_object)
				{
					delete _object;
					_object = 0;
				}
			}

			static std::string productsToString()
			{
				return ProductManagerPrivate<T>::productsToString();
			}

			static void setProductName(std::string _productClassName)
			{
				productName() = _productClassName;
			}

			static std::string getProductName()
			{
				return productName();
			}

			static std::string lastErrorString()
			{
				return errorString();
			}
		protected:
			Factory() {};
			~Factory() {};
		private:
			static std::string& productName()
			{
				static std::string sProductName;
				return sProductName;
			}
			static std::string& errorString()
			{
				static std::string sErrorString;
				return sErrorString;
			}
			static T* createProduct(std::string _productName)
			{
				std::map<std::string, std::string>& productsAlias =
					ProductManagerPrivate<T>::getProductAliasTable();
				std::map<std::string, std::string>::iterator itrAlias = productsAlias.find(_productName);
				if (itrAlias != productsAlias.end())
				{
					std::map<std::string, ProductClassInfoPrivate>& productsClass =
						ProductManagerPrivate<T>::getProductClassTable();
					std::map<std::string, ProductClassInfoPrivate>::iterator itrClass = productsClass.find(itrAlias->second);
					if (itrClass != productsClass.end())
						return  static_cast<T*>((itrClass->second).createObject());
				}

				assert(false);
				errorString() = "Invalid product's name : " + _productName;
				return 0;
			}
		};

		class ProductClassInfoPrivate
		{
			template< typename T > friend class Factory;
			template< typename T > friend class ProductManagerPrivate;
		private:
			void* (*createObject)();
		};

		template<class T>
		class ProductClassCreatorPrivate
		{
			template< typename T > friend class ProductManagerPrivate;
		private:
			static void* create()
			{
				return static_cast<void*>(new T());
			}
		};

		template <class T>
		class ProductManagerPrivate
		{
			template< typename T > friend class Factory;
		public:
			template<class K>
			static void registerProduct(K* _class, std::string _className, std::string _alias)
			{
				ProductClassInfoPrivate classInfo;
				classInfo.createObject = ProductClassCreatorPrivate<K>::create;
				getProductClassTable().insert(make_pair(_className, classInfo));
				getProductAliasTable().insert(make_pair(_alias, _className));
			}
		private:
			static std::map<std::string, ProductClassInfoPrivate>& getProductClassTable()
			{
				static std::map<std::string, ProductClassInfoPrivate> classMap_;
				return classMap_;
			}
			static std::map<std::string, std::string>& getProductAliasTable()
			{
				static std::map<std::string, std::string> aliasMap_;
				return aliasMap_;
			}
			static std::string productsToString()
			{
				std::string names;
				std::map<std::string, ProductClassInfoPrivate>& productClassMap =
					getProductClassTable();
				std::map<std::string, ProductClassInfoPrivate>::const_iterator citr =
					productClassMap.begin();
				for (; citr != productClassMap.end(); ++citr)
				{
					names.append(citr->first);
					names.append(" ");
				}
				return names;
			}
		};


	}//namespace gdut
}//namespace el

#define DECLARE_PRODUCT_CLASS(pBaseClass, pClass)    \
    class product_##pClass \
    { \
    public: \
        product_##pClass() \
        { \
        el::gdut::ProductManagerPrivate<pBaseClass>::registerProduct((pClass*)0, #pClass, #pClass); \
        } \
    }; 

#define DECLARE_ALIAS_PRODUCT_CLASS(pBaseClass, pClass, alias)    \
class product_##pClass \
    { \
    public: \
         product_##pClass() \
        { \
            el::gdut::ProductManagerPrivate<pBaseClass>::registerProduct((pClass*)0, #pClass, alias); \
        } \
    }; 

#define REGISTER_PRODUCT_CLASS(pBaseClass, pClass)    \
    static product_##pClass sProduct_##pClass;

#endif //EL_GDUT_DESIGNPATTERN_FACTORY_H_