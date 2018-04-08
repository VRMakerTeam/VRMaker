#include <assert.h>
#include "MemDB.h"
#include "MemDBUtil.h"
#include "PluginBean/BeanModel.h"
#include "PluginDiagram/DiagramModel.h"

NS(DiagramModel)
{
	NS(Runtime)
	{
		const QString& bucket = "diagram:runtime";
		void InitializeModel()
		{
			MemDB::CreateBucket(bucket);
		}

		MemDB* GetMemDB()
		{
			return MemDB::FindBucket(bucket);
		}

		SETS_STR(diagram_groups, false)

		KV_STR_1(diagram_group_1_alias, false)

		SETS_STR_1(diagram_group_1_blocks, false)
		SETS_STR_1(diagram_extend_1_blocks, false)
		SETS_STR(diagram_extends_filter, false)
	}

	NS(Persistent)
	{
		MemDB* GetMemDB()
		{
			MemDB* runtimeDB = BeanModel::Runtime::GetMemDB();
			assert(runtimeDB);
			QString beanGUID = BeanModel::Runtime::KV::Query::bean_alive_guid(runtimeDB);
			MemDB* beanDB = BeanModel::Persistent::GetMemDB(beanGUID);
			assert(beanDB);
			return beanDB;
		}

		MemDB* GetMemDB(const QString& _beanUUID)
		{
			MemDB* beanDB = BeanModel::Persistent::GetMemDB(_beanUUID);
			assert(beanDB);
			return beanDB;
		}


		SETS_STR(diagram_triggers_guids, true)
		KV_STR_1(diagram_trigger_1_alias, true)
		KV_STR_1(diagram_trigger_1_type, true)
		KV_V(diagram_triggers_index, true)
		KV_DOUBLE_1(diagram_trigger_1_px, true)
		KV_DOUBLE_1(diagram_trigger_1_py, true)
		KV_DOUBLE_1(diagram_trigger_1_pz, true)
		KV_DOUBLE_1(diagram_trigger_1_rx, true)
		KV_DOUBLE_1(diagram_trigger_1_ry, true)
		KV_DOUBLE_1(diagram_trigger_1_rz, true)
		KV_1(diagram_gaze_1_icon, true)
		KV_1(diagram_gaze_1_color_r, true)
		KV_1(diagram_gaze_1_color_g, true)
		KV_1(diagram_gaze_1_color_b, true)
		KV_1(diagram_gaze_1_color_a, true)


		SETS_STR(diagram_clones_guids, true)
		//position
		KV_DOUBLE_1(diagram_clone_1_px, true)
		KV_DOUBLE_1(diagram_clone_1_py, true)
		KV_DOUBLE_1(diagram_clone_1_pz, true)
		//rotation
		KV_DOUBLE_1(diagram_clone_1_rx, true)
		KV_DOUBLE_1(diagram_clone_1_ry, true)
		KV_DOUBLE_1(diagram_clone_1_rz, true)
		//scale
		KV_DOUBLE_1(diagram_clone_1_sx, true)
		KV_DOUBLE_1(diagram_clone_1_sy, true)
		KV_DOUBLE_1(diagram_clone_1_sz, true)
			
		KV_STR_1(diagram_block_1_group, true)
		KV_STR_1(diagram_block_1_template, true)
		KV_1(diagram_block_1_index, true)

		SETS_STR(diagram_pipes_guids, true)
		SETS_STR_1(diagram_pipe_guid_1_blocks_guids, true)

		HASH_STR_1(diagram_block_guid_1_fields, true)
	}
}
