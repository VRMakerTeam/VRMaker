#pragma once

#include "Composite.h"

class AppCore;

namespace Diagram
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


}//namespace Diagram