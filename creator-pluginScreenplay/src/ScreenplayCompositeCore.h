#ifndef __SCREENPLAYCOMPOSITECORE_H__
#define __SCREENPLAYCOMPOSITECORE_H__

#include "Composite.h"


class AppCore;

namespace Screenplay
{
	class CompositeCore : public Composite::Core
	{
	public:
		CompositeCore();
		~CompositeCore();
		void SetAppCore(AppCore* _appCore);
		AppCore* GetAppCore();
	private:
		AppCore* ptrAppCore_;
	};
}


#endif // __SCREENPLAYCOMPOSITECORE_H__

