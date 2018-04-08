#pragma once

#include <QString>
#include <QHash>

class MemDB;

/// \brief
/// default path of dump is QStandardPaths::AppDataLocation
class MemDBUtil
{
public:

	/// \return error code
	/// 0 : OK
	/// 1 : bucket is not exists
	/// 2 : file open failed
	static int SaveBucket(const QString& _bucket);

	/// \return error code
	/// 0 : OK
	static int DeleteBucket(const QString& _bucket);

	/// \return error code
	/// 0 : OK
	/// 2 : file open failed
	static int LoadBucket(const QString& _bucket);

	static QString GetDataPath();
	static QByteArray TakeUUID(const QString& _bucket);
protected:
private:
};
