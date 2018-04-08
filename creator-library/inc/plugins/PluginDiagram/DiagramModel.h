#pragma once
#include "MemDBMacro.h"
#include "StoreMacro.h"
#include <QString>
#include <QVariant>
/// \brief

/// *Runtime*
/// +diagram:runtime
///    {sets}
///      |- diagram:groups
///          |- (string[] group)
/// *Persistent*
///	+bucket:bean:guid:[guid]
///    {kv}
///      |- diagram:block:[block]:action
///           |- (string | the template of block)
///    {sets}
///      |- diagram:pipes:guids
///           |- (string[] | the guid of pipes)
///      |- diagram:pipe:guid:[guid]:blocks:guids
///           |- (string[] | the guid of blocks)
///    {hash}
///      |- diagram:block:guid:[guid]:fields
///           |- [filed]
///                |- (variant | the value of filed in block) 


class MemDB;

NS(DiagramModel)
{
	NS(Runtime)
	{
		void InitializeModel();
		MemDB* GetMemDB();

		STORE_TAG(blocks_groups);
		DECLARE_SETS_STR(diagram_groups)
		DECLARE_KV_STR_1(diagram_group_1_alias)

		STORE_TAG(blocks_register);

		DECLARE_SETS_STR_1(diagram_group_1_blocks)
		DECLARE_SETS_STR_1(diagram_extend_1_blocks)
		DECLARE_SETS_STR(diagram_extends_filter)
	}

	NS(Persistent)
	{
		MemDB* GetMemDB();
		MemDB* GetMemDB(const QString& _beanUUID);

		DECLARE_KV_STR_1(diagram_block_1_group)
		DECLARE_KV_STR_1(diagram_block_1_template)
		DECLARE_KV_1(diagram_block_1_index)

		STORE_TAG(digram_triggers_update)
		DECLARE_SETS_STR(diagram_triggers_guids)
		DECLARE_KV_V(diagram_triggers_index)
		DECLARE_KV_STR_1(diagram_trigger_1_alias)
		DECLARE_KV_STR_1(diagram_trigger_1_type)
		DECLARE_KV_DOUBLE_1(diagram_trigger_1_px)
		DECLARE_KV_DOUBLE_1(diagram_trigger_1_py)
		DECLARE_KV_DOUBLE_1(diagram_trigger_1_pz)
		DECLARE_KV_DOUBLE_1(diagram_trigger_1_rx)
		DECLARE_KV_DOUBLE_1(diagram_trigger_1_ry)
		DECLARE_KV_DOUBLE_1(diagram_trigger_1_rz)
		DECLARE_KV_1(diagram_gaze_1_icon)
		DECLARE_KV_1(diagram_gaze_1_color_r)
		DECLARE_KV_1(diagram_gaze_1_color_g)
		DECLARE_KV_1(diagram_gaze_1_color_b)
		DECLARE_KV_1(diagram_gaze_1_color_a)

		DECLARE_SETS_STR(diagram_clones_guids)
		//position
		DECLARE_KV_DOUBLE_1(diagram_clone_1_px)
		DECLARE_KV_DOUBLE_1(diagram_clone_1_py)
		DECLARE_KV_DOUBLE_1(diagram_clone_1_pz)
		//rotation
		DECLARE_KV_DOUBLE_1(diagram_clone_1_rx)
		DECLARE_KV_DOUBLE_1(diagram_clone_1_ry)
		DECLARE_KV_DOUBLE_1(diagram_clone_1_rz)
		//scale
		DECLARE_KV_DOUBLE_1(diagram_clone_1_sx)
		DECLARE_KV_DOUBLE_1(diagram_clone_1_sy)
		DECLARE_KV_DOUBLE_1(diagram_clone_1_sz)


		STORE_TAG(digram_pipes_update)
		DECLARE_SETS_STR(diagram_pipes_guids)

		STORE_TAG(digram_blocks_update)
		DECLARE_SETS_STR_1(diagram_pipe_guid_1_blocks_guids)
		DECLARE_SETS_STR(diagram_blocks_guids)

		STORE_TAG(digram_block_update)
		DECLARE_HASH_STR_1(diagram_block_guid_1_fields)


	}
}
