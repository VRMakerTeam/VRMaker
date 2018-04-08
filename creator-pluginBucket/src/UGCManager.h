#ifndef __UGCMANAGER_H__
#define __UGCMANAGER_H__

#include <QStringList>
#include <QList>

class UGCManager
{
public:
	static void Parse();
	static QStringList ListAudios();
	static QStringList ListImages();

	static QByteArray ReadData(const QString& _file);
private:
};

#endif
