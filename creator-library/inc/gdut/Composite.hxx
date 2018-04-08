/********************************************************************
Copyright (c) 2010-2020 by Easlee
@created:	10-10-2011 
@filename: 	Factory.hxx
@author:	Easlee Liu (liu@easlee.me)
@desc:
*********************************************************************/
#ifndef EL_GDUT_DESIGNPATTERN_COMPOSITE_H_
#define EL_GDUT_DESIGNPATTERN_COMPOSITE_H_

#include <list>
#include <string>
#include <assert.h>

namespace el
{
	namespace gdut
	{
		template<class T>
		class Component
		{
		public:
			virtual void add(T* _component) = 0;
			virtual void remove(T* _component) = 0;
		protected:
			template <class T> friend class Composite;
			void addParent(T* _parent)
			{
				parents_.push_back(_parent);
			}
			std::list<T*> parents_;
		};

		template<class T>
		class Composite : public T
		{
		public:
			void add(T* _component)
			{
				children_.push_back(_component);
				_component->addParent(this);
			}
			void remove(T* _component)
			{

			}
		protected:
			std::list<T*> children_;
		};

		template<class T>
		class Leaf : public T
		{
		public:
			void add(T* _component)
			{
				assert(false);
			}
			void remove(T* _component)
			{
				assert(false);
			}
		protected:
		private:
		};

	}//namespace gdut
}//namespace el

#endif //EL_GDUT_DESIGNPATTERN_COMPOSITE_H_