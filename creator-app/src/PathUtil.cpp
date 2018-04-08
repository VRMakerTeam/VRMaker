#include <QStandardPaths>
#include <QDir>
#include "PathUtil.h"

QString PathUtil::PackageLocation()
{
	QDir dir(QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation));
	QString path = dir.path() + "/AppData/LocalLow/vrmaker/vplayer";
	dir.mkpath(path);
	return path;
}

QString PathUtil::ThumbLocation()
{
	QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	QString path = dir.path() + "/thumb";
	dir.mkpath(path);
	return path;
}

QString PathUtil::AppDataLocation()
{
	return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

QString PathUtil::TempLocation()
{
	return QStandardPaths::writableLocation(QStandardPaths::TempLocation);
}

