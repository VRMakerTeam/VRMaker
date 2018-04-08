#include "Utils.h"
#include "BeanCompositeCore.h"


//-----------------------------------------------------------------------
Bean::CompositeCore::~CompositeCore()
{
}

//-----------------------------------------------------------------------
void Bean::CompositeCore::SetAppCore(AppCore* _appCore)
{
	ptrAppCore_ = _appCore;
}

//-----------------------------------------------------------------------
AppCore* Bean::CompositeCore::GetAppCore()
{
	return ptrAppCore_;
}

