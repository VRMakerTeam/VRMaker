#ifndef __SCREENPLAYMODEL_H__
#define __SCREENPLAYMODEL_H__

#include <QString>
#include "MemDBMacro.h"
#include "StoreMacro.h"

/// \brief
/// *Runtime*
/// +astronomy:runtime
///    {kv}
///      |- astronomy:slate:alive:uuid
///           |- (string |uuid of alive slate)
///      | -astronomy:slate:uuid:[uuid]:guid
///           |- (string |guid of slate)
///      | -astronomy:slate:guid:[guid]:uuid
///           |- (string |uuid of slate)
///      | -astronomy:preload:guid:[guid]:uuid
///           |- (string |uuid of preload)
///      | -astronomy:preload:uuid:[uuid]:guid
///           |- (string |guid of preload)
/// *Persistent*
///	+bucket:bean:guid:[guid]
///    {kv}
///      |- astronomy:slate:guid:[guid]:preload:skybox
///           |- (string | guid of skybox)
///    {sets}
///      |- astronomy:slates:guids
///           |- (string[] | guid of slates)
///      |- astronomy:slate:guid:[guid]:preloads:guids
///           |- (string[] | guid of preload)
///      |- astronomy:slate:guid:[guid]:pipes:guids
///           |- (string[] | guid of pipes)
///    {hash}
///      |- astronomy:slate:guid:[guid]:preload:guid:[guid]
///           |- asset:guid
///                |- (string | the guid of asset)

class MemDB;


NS(ScreenplayModel)
{
	NS(Runtime)
	{
		void InitializeModel();
		MemDB* GetMemDB();
		DECLARE_KV_STR(screenplay_slate_alive_uuid)
	}

	NS(Persistent)
	{
		MemDB* GetMemDB();
		MemDB* GetMemDB(const QString& _guid);

		STORE_TAG(slates_update)
		DECLARE_SETS_STR(screenplay_slates_guids)
		DECLARE_KV_STR_1(screenplay_slate_guid_1_alias)
		DECLARE_KV_V(screenplay_slate_index)

		STORE_TAG(slate_update_skybox)
		DECLARE_KV_STR_1(screenplay_slate_guid_1_preload_skybox_file)
		DECLARE_KV_STR_1(screenplay_slate_guid_1_preload_skybox_package)

		STORE_TAG(slate_update_agent)
		DECLARE_SETS_STR_1(screenplay_slate_guid_1_preloads_guids)
		DECLARE_KV_STR_2(screenplay_slate_guid_1_preload_guid_2_asset_file)
		DECLARE_KV_STR_2(screenplay_slate_guid_1_preload_guid_2_asset_package)
		DECLARE_KV_DOUBLE_1(screenplay_preload_guid_1_px)
		DECLARE_KV_DOUBLE_1(screenplay_preload_guid_1_py)
		DECLARE_KV_DOUBLE_1(screenplay_preload_guid_1_pz)
		DECLARE_KV_DOUBLE_1(screenplay_preload_guid_1_rx)
		DECLARE_KV_DOUBLE_1(screenplay_preload_guid_1_ry)
		DECLARE_KV_DOUBLE_1(screenplay_preload_guid_1_rz)
		DECLARE_KV_DOUBLE_1(screenplay_preload_guid_1_sx)
		DECLARE_KV_DOUBLE_1(screenplay_preload_guid_1_sy)
		DECLARE_KV_DOUBLE_1(screenplay_preload_guid_1_sz)

		STORE_TAG(agent_inspector_update)
		DECLARE_KV_1(screenplay_preload_guid_1_gaze)
		DECLARE_KV_1(screenplay_preload_guid_1_gaze_alias)

		DECLARE_SETS_STR_1(screenplay_slate_guid_1_pipes_guids)
		DECLARE_SETS_STR_1(screenplay_slate_guid_1_triggers_guids)
	}
}

#endif //__SCREENPLAYMODEL_H__
