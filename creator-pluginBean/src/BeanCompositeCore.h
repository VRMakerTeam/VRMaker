#pragma once

#include "Composite.h"

class AppCore;

namespace Bean
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
}//namespace Bean
