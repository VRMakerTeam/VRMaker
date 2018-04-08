#include <QDir>
#include <QImageReader>
#include <QImageWriter>
#include "PackageManager.h"
#include "UGCManager.h"
#include "PathUtil.h"
#include "pluginBucket/BucketUtils.h"

//-----------------------------------------------------------------------
QString BucketUtils::QueryFileGroup(const QString& _file)
{
	return PackageManager::QueryFileGroup(_file);
}

//-----------------------------------------------------------------------
QString BucketUtils::QueryFileAlias(const QString& _file)
{
	return PackageManager::QueryFileAlias(_file);
}

//-----------------------------------------------------------------------
QString BucketUtils::QueryFilePackage(const QString& _file)
{
	return PackageManager::QueryFilePackage(_file);
}

//-----------------------------------------------------------------------
QString BucketUtils::QueryFileThumbPath(const QString& _file)
{
	return PackageManager::QueryFileThumbPath(_file);
}

QByteArray BucketUtils::ReadUGCData(const QString& _file)
{
	return UGCManager::ReadData(_file);
}

//-----------------------------------------------------------------------
void BucketUtils::PlaySound(const QString& _file)
{
	const QString& package = QueryFileThumbPath(_file);

}

//-----------------------------------------------------------------------
void BucketUtils::StopSound()
{

}

void BucketUtils::GenerateThumb(const QString& _directory)
{
	QDir srcDir(_directory);
	QStringList filters;
	filters << QString("*.jpg") << QString("*.png");
	QStringList files = srcDir.entryList(filters);

	QDir destDir = QDir(PathUtil::ThumbLocation());
	
	foreach(QString file, files)
	{
		QString srcFile = QString("%1/%2").arg(srcDir.absolutePath()).arg(file);
		QImageReader reader;
		reader.setFileName(srcFile);

		if (!reader.canRead())
			continue;

		reader.setScaledSize(QSize(240, 135));

		QString destFile = QString("%1/%2").arg(destDir.absolutePath()).arg(file);

		//TODO ÁÙÊ±·½°¸
		if (QFile::exists(destFile))
			continue;

		QImageWriter writer;
		writer.setFileName(destFile);
		if (!writer.canWrite())
			continue;
		writer.write(reader.read());
	}
}
