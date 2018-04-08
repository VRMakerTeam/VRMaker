#include <assert.h>
#include "MemDB.h"
#include "MemDBUtil.h"
#include "PluginBean/BeanModel.h"
#include "PluginScreenplay/ScreenplayModel.h"

NS(ScreenplayModel)
{
	NS(Runtime)
	{
		static const QString& bucket = "screenplay:runtime";

		void InitializeModel()
		{
			MemDB::CreateBucket(Runtime::bucket);
		}

		MemDB* GetMemDB()
		{
			return MemDB::FindBucket(ScreenplayModel::Runtime::bucket);
		}

		KV_STR(screenplay_slate_alive_uuid, false)
	}

	NS(Persistent)
	{
		MemDB* GetMemDB()
		{
			MemDB* memDB = BeanModel::Runtime::GetMemDB();
			const QString& guid = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);
			return BeanModel::Persistent::GetMemDB(guid);
		}


		MemDB* GetMemDB(const QString& _guid)
		{
			return BeanModel::Persistent::GetMemDB(_guid);
		}


		SETS_STR(screenplay_slates_guids, true)
		KV_STR_1(screenplay_slate_guid_1_alias, true)
		KV_V(screenplay_slate_index, true)

		KV_STR_1(screenplay_slate_guid_1_preload_skybox_package, true)
		KV_STR_1(screenplay_slate_guid_1_preload_skybox_file, true)
		SETS_STR_1(screenplay_slate_guid_1_preloads_guids, true)

		KV_STR_2(screenplay_slate_guid_1_preload_guid_2_asset_file, true)
		KV_STR_2(screenplay_slate_guid_1_preload_guid_2_asset_package, true)
		KV_DOUBLE_1(screenplay_preload_guid_1_px, true)
		KV_DOUBLE_1(screenplay_preload_guid_1_py, true)
		KV_DOUBLE_1(screenplay_preload_guid_1_pz, true)
		KV_DOUBLE_1(screenplay_preload_guid_1_rx, true)
		KV_DOUBLE_1(screenplay_preload_guid_1_ry, true)
		KV_DOUBLE_1(screenplay_preload_guid_1_rz, true)
		KV_DOUBLE_1(screenplay_preload_guid_1_sx, true)
		KV_DOUBLE_1(screenplay_preload_guid_1_sy, true)
		KV_DOUBLE_1(screenplay_preload_guid_1_sz, true)
		KV_1(screenplay_preload_guid_1_gaze, true)
		KV_1(screenplay_preload_guid_1_gaze_alias, true)

		SETS_STR_1(screenplay_slate_guid_1_pipes_guids, true)
		SETS_STR_1(screenplay_slate_guid_1_triggers_guids, true)
	}
}
