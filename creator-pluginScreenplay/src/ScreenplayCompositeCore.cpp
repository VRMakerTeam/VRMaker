#include "Utils.h"
#include "ScreenplayCompositeCore.h"

Screenplay::CompositeCore::CompositeCore()
{
	ptrAppCore_ = NULL;
}

//-----------------------------------------------------------------------
Screenplay::CompositeCore::~CompositeCore()
{

}

//-----------------------------------------------------------------------
void Screenplay::CompositeCore::SetAppCore(AppCore* _appCore)
{
	ptrAppCore_ = _appCore;
}

//-----------------------------------------------------------------------
AppCore* Screenplay::CompositeCore::GetAppCore()
{
	return ptrAppCore_;
}
