#pragma once

#include <QString>
#include "MemDBMacro.h"
#include "StoreMacro.h"

/// \brief

/// *Runtime*
/// +bean:runtime
///    {kv}
///      |- bean:alive:guid
///           |- (string | the guid of bean)
///      |- bean:archetype:uuid:[uuid]:guid
///           |- (string | the guid of archetype)
///      |- bean:source:selected:guid
///           |- (string | the guid of selected source)
/// *Persistent*
///	+bucket:bean:guid:[guid]
///    {kv}
///      |- bean:source:guid
///           |- (string | the guid of source)
///		

class MemDB;

NS(BeanModel)
{
	NS(Runtime)
	{
		void InitializeModel();
		MemDB* GetMemDB();

		STORE_TAG(bean_source)
		DECLARE_SETS_STR(bean_sources_guids)
		DECLARE_KV_STR_1(bean_source_guid_1_alias)
		DECLARE_KV_STR_1(bean_source_guid_1_category)
		DECLARE_KV_1(bean_source_guid_1_extends)

		DECLARE_KV_STR(bean_alive_guid)
		DECLARE_SETS_STR(bean_source_selected_guid)

		DECLARE_SETS_STR(bean_alias)
	}

	NS(Persistent)
	{
		void InitializeModel(const QString& _bucketGUID);
		void ReleaseModel(const QString& _bucketGUID);
		MemDB* GetMemDB(const QString& _bucketGUID);

		STORE_TAG(beans_update);
		STORE_TAG(bean_update);
		DECLARE_KV_STR(bean_alias);
		DECLARE_SETS_STR(bean_extends);
	}
}
