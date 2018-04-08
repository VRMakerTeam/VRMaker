#pragma once

#include <QString>
#include "MemDBMacro.h"
#include "StoreMacro.h"

/// \brief

class MemDB;

NS(BucketModel)
{
	NS(Runtime)
	{
		STORE_TAG(package);
		STORE_TAG(filter);
		STORE_TAG(tag);
		void InitializeModel();
		MemDB* GetMemDB();

		DECLARE_KV_STR(assets_group_filter)
		DECLARE_SETS_STR(assets_tags_filter)
	}

	NS(Persistent)
	{
		
	}
}
