#include <assert.h>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include "PathUtil.h"
#include "MemDB.h"
#include "Settings.h"
#include "pluginBucket/BucketUtils.h"
#include "UGCManager.h"


#define MEMDB_UGC_BUCKET "ugc"
#define KV_SET_UGCAUDIO_FILES_KEY "ugc:audio:files"
#define KV_SET_UGCIMAGE_FILES_KEY "ugc:image:files"


void UGCManager::Parse()
{
	//创建数据库，如果存在则忽略
	MemDB::CreateBucket(MEMDB_UGC_BUCKET);
	MemDB* memDB = MemDB::FindBucket(MEMDB_UGC_BUCKET);
	assert(memDB);

	//切换到ugc目录
	QDir dir(PathUtil::PackageLocation());
	if(!dir.cd("ugc"))
		return;

	//获取所有的图片文件
	QString dirpath = dir.absolutePath();
	QStringList filters;
	filters << QString("*.jpg") << QString("*.png");
	QStringList files = dir.entryList(filters);
	//将所有的文件加入到数据库中
	for each(QString file in  files)
	{
		//会忽略重复的
		memDB->SetsAdd(KV_SET_UGCIMAGE_FILES_KEY, file);
	}

	//获取所有的音频文件
	filters.clear();
	filters << QString("*.wav");
	files = dir.entryList(filters);
	//将所有的文件加入到数据库中
	for each(QString file in  files)
	{
		//会忽略重复的
		memDB->SetsAdd(KV_SET_UGCAUDIO_FILES_KEY, file);
	}

	//生成缩略图
	BucketUtils::GenerateThumb(dirpath);
}

QStringList UGCManager::ListAudios()
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_UGC_BUCKET);
	assert(memDB);

	QStringList files;
	for each (QVariant var in memDB->SetsMembers(KV_SET_UGCAUDIO_FILES_KEY))
	{
		files.append(var.toString());
	}
	return files;
}

QStringList UGCManager::ListImages()
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_UGC_BUCKET);
	assert(memDB);

	QStringList files;
	for each (QVariant var in memDB->SetsMembers(KV_SET_UGCIMAGE_FILES_KEY))
	{
		files.append(var.toString());
	}
	return files;
}

QByteArray UGCManager::ReadData(const QString& _file)
{
	QDir dir(PathUtil::PackageLocation());
	QString path = QString("%1/ugc/%2").arg(dir.absolutePath()).arg(_file);
	QFile file(path);
	QByteArray data;
	if (file.open(QIODevice::ReadOnly))
	{
		data = file.readAll();
		file.close();
	}
	return data;
}

