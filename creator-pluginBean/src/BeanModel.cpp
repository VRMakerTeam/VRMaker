#include "MemDB.h"
#include "MemDBUtil.h"
#include "PluginBean/BeanModel.h"

NS(BeanModel)
{
	NS(Runtime)
	{
		static const QString& bucket = "bean:runtime";

		void InitializeModel()
		{
			MemDB::CreateBucket(Runtime::bucket);
		}
		MemDB* GetMemDB()
		{
			return MemDB::FindBucket(BeanModel::Runtime::bucket);
		}

		SETS_STR(bean_sources_guids, false)
		KV_STR_1(bean_source_guid_1_alias, false)
		KV_STR_1(bean_source_guid_1_category, false)
		KV_1(bean_source_guid_1_extends, false)

		KV_STR(bean_alive_guid, false)
		SETS_STR(bean_source_selected_guid, false)

		SETS_STR(bean_alias, false)
	}

	NS(Persistent)
	{
		const QString& bucket_bean_guid_1 = "bucket.bean.guid.[%1]";
		void InitializeModel(const QString& _bucketGUID)
		{
			const QString& bucket = QString(BeanModel::Persistent::bucket_bean_guid_1).arg(_bucketGUID);
			MemDB::CreateBucket(bucket);
			MemDBUtil::SaveBucket(bucket);
		}
		void ReleaseModel(const QString& _bucketGUID)
		{
			const QString& bucket = QString(BeanModel::Persistent::bucket_bean_guid_1).arg(_bucketGUID);
			MemDB::DestroyBucket(_bucketGUID);
			MemDBUtil::DeleteBucket(bucket);
		}
		MemDB* GetMemDB(const QString& _bucketGUID)
		{
			return MemDB::FindBucket(QString(bucket_bean_guid_1).arg(_bucketGUID));
		}

		KV_STR(bean_alias, true);
		SETS_STR(bean_extends, true);
	}
}

