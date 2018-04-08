#include "DiagramCompositeCore.h"

//-----------------------------------------------------------------------
Diagram::CompositeCore::~CompositeCore()
{
}
//-----------------------------------------------------------------------
void Diagram::CompositeCore::SetAppCore(AppCore* _appCore)
{
	ptrAppCore_ = _appCore;
}

//-----------------------------------------------------------------------
AppCore* Diagram::CompositeCore::GetAppCore()
{
	return ptrAppCore_;
}
