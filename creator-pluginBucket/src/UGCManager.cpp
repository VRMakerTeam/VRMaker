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
	//�������ݿ⣬������������
	MemDB::CreateBucket(MEMDB_UGC_BUCKET);
	MemDB* memDB = MemDB::FindBucket(MEMDB_UGC_BUCKET);
	assert(memDB);

	//�л���ugcĿ¼
	QDir dir(PathUtil::PackageLocation());
	if(!dir.cd("ugc"))
		return;

	//��ȡ���е�ͼƬ�ļ�
	QString dirpath = dir.absolutePath();
	QStringList filters;
	filters << QString("*.jpg") << QString("*.png");
	QStringList files = dir.entryList(filters);
	//�����е��ļ����뵽���ݿ���
	for each(QString file in  files)
	{
		//������ظ���
		memDB->SetsAdd(KV_SET_UGCIMAGE_FILES_KEY, file);
	}

	//��ȡ���е���Ƶ�ļ�
	filters.clear();
	filters << QString("*.wav");
	files = dir.entryList(filters);
	//�����е��ļ����뵽���ݿ���
	for each(QString file in  files)
	{
		//������ظ���
		memDB->SetsAdd(KV_SET_UGCAUDIO_FILES_KEY, file);
	}

	//��������ͼ
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

