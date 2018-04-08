#include "Utils.h"
#include "BucketCompositeCore.h"


//-----------------------------------------------------------------------
void Bucket::CompositeCore::SetAppCore(AppCore* _appCore)
{
	ptrAppCore_ = _appCore;
}

//-----------------------------------------------------------------------
AppCore* Bucket::CompositeCore::GetAppCore()
{
	return ptrAppCore_;
}
//-----------------------------------------------------------------------
Bucket::CompositeCore::~CompositeCore()
{
}
