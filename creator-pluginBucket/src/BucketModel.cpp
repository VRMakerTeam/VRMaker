#include "MemDB.h"
#include "MemDBUtil.h"
#include "PluginBucket/BucketModel.h"

NS(BucketModel)
{
	NS(Runtime)
	{
		static const QString& bucket = "bucket:runtime";

		void InitializeModel()
		{
			MemDB::CreateBucket(Runtime::bucket);
		}
		MemDB* GetMemDB()
		{
			return MemDB::FindBucket(BucketModel::Runtime::bucket);
		}

		KV_STR(assets_group_filter, false)
		SETS_STR(assets_tags_filter, false)
	}

	NS(Persistent)
	{

	}
}
