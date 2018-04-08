#include <QProcess>
#include <QDebug>
#include "Settings.h"
#include "ConfigUtil.h"
#include "DeviceUtil.h"

class DeviceUtilPrivete
{
public:
	static QProcess process;
};

int DeviceUtil::PushFile(const QString& _bundle, const QString& _srcfile, const QString& _destFile)
{
	QString deviceName = "";
	if (!ConfigUtil::FindString("device.name", deviceName))
		return -1;

	QString devicePlatform = "";
	if (!ConfigUtil::FindString("device.platform", devicePlatform))
		return -1;

	// pc will read file from AppData\LocalLow
	if (0 == devicePlatform.compare("pc"))
		return 0;

	QProcess process;
	process.start("adb devices");
	if (!process.waitForFinished(5000))
	{
		return 1;
	}

	QString output = "";

	output = QString::fromLocal8Bit(process.readAllStandardOutput());
	//remove last '/r/n/r/n'
	output = output.remove(output.size() - 4, 4);
	QStringList outputlist = output.split("\r\n");
	//remove the "List ofdevices attached"
	outputlist.removeAll("List of devices attached");
	bool hasDevice = false;
	for each (QString device in outputlist)
	{
		if (device.contains("device"))
			hasDevice = true;
	}
	if (!hasDevice)
		return 1;

	QString devicePersistentPath = "";
	if (!ConfigUtil::FindString(QString("device.%1.persistentpath.%2").arg(devicePlatform).arg(deviceName), devicePersistentPath))
		return 2;

	QString destPath = QString("%1/%2").arg(devicePersistentPath).arg(_destFile);
	process.start(QString("adb push \"%1\" %2").arg(_srcfile).arg(destPath));
	if (!process.waitForFinished(5000))
	{
		return 2;
	}
	output = QString::fromLocal8Bit(process.readAllStandardOutput());
	qDebug() << output;
	return 0;
}
