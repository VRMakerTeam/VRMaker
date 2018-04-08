#pragma once

#include <QStringList>

class DeviceUtil
{
public:
	/// \return errorcode
	/// -1 : config error
	/// 0 : OK
	/// 1 : none device
	/// 2 : copy file failed
	static int PushFile(const QString& _bundle, const QString& _srcFile, const QString& _destFile);
};