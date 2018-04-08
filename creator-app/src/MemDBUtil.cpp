#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QStandardPaths>
#include <QCryptographicHash>
#include "MemDB.h"
#include "MemDBUtil.h"

QString getBucketDumpFilePath(const QString& _bucket)
{
	QDir dir(MemDBUtil::GetDataPath());
	QString bucket = _bucket;
	return dir.absoluteFilePath(bucket.replace(":", ".").append(".vcb"));
}

//-----------------------------------------------------------------------
int MemDBUtil::SaveBucket(const QString& _bucket)
{
	MemDB* memDB = MemDB::FindBucket(_bucket);
	if (NULL == memDB)
		return 1;

	const QByteArray content = MemDB::EncodeBucket(_bucket);
	QString filepath = getBucketDumpFilePath(_bucket);
	QFile file(filepath);
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream  out(&file);
		out << content;
		file.close();
	}
	else
	{
		file.close();
		return 2;
	}
	return 0;
}

int MemDBUtil::DeleteBucket(const QString& _bucket)
{
	QString filepath = getBucketDumpFilePath(_bucket);
	QFile::remove(filepath);
	return 0;
}

//-----------------------------------------------------------------------
int MemDBUtil::LoadBucket(const QString& _bucket)
{
	QString filepath = getBucketDumpFilePath(_bucket);
	QFile file(filepath);
	QString content;
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray bytes;
		QDataStream  in(&file);
		in >> bytes;
		file.close();
		content = QString::fromUtf8(bytes);
	}
	else
	{
		file.close();
		return 2;
	}

	MemDB::DecodeBucket(content);
	return 0;
}

QString MemDBUtil::GetDataPath()
{
	QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	if (!dir.exists())
		dir.mkpath(dir.absolutePath());

	if (!dir.exists("memdb_dump"))
		dir.mkdir("memdb_dump");
	dir.cd("memdb_dump");
	return dir.absolutePath();
}

QByteArray MemDBUtil::TakeUUID(const QString& _bucket)
{
	QString bucket = _bucket;
	bucket = bucket.remove(0, bucket.indexOf("[") + 1);
	bucket = bucket.remove(bucket.indexOf("]"), bucket.size() - bucket.indexOf("]"));
	return bucket.toUtf8();
}

