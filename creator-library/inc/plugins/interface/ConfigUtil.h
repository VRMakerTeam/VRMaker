#ifndef _CONFIGUTIL_H_
#define _CONFIGUTIL_H_

#include <Qstring>
#include <QVariant>
#include <QHash>

class ConfigUtil
{
public : 

	static void BuildConfig();
	static int Initialization();
	static QVariant Find(const QString& _key);
	static bool FindString(const QString& _key, QString& _value);
	static bool FindInt(const QString& _key, int& _value);
	static bool FindBool(const QString& _key, bool& _value);
	static bool FindDouble(const QString& _key, double& _value);
	static void ChangeLanguage(const QString& _language);
	static int GetLanguage();
	static void WriteConfig();
private :
	static const QByteArray secret;
	static QHash<QString, QVariant> hash;
	static void parse(const QByteArray& _json);
};

#endif