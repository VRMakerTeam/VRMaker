#ifndef __BUCKETCOMPOSITECORE_H__
#define __BUCKETCOMPOSITECORE_H__

#include "Composite.h"

class AppCore;

namespace Bucket
{
	class CompositeCore : public Composite::Core
	{
	public:
		~CompositeCore();
		void SetAppCore(AppCore* _appCore);
		AppCore* GetAppCore();
	private:
		AppCore* ptrAppCore_;
	};
}


#endif // __BUCKETCOMPOSITECORE_H__

