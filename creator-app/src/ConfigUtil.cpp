#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "ConfigUtil.h"

QHash<QString, QVariant> ConfigUtil::hash;


int ConfigUtil::Initialization()
{
	int result = 1;

	QFile file;

	// use config.json override configuration
	file.setFileName("conf/config.json");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		parse(file.readAll());
		file.close();
		result = 0;
	}

	return result;
}

QVariant ConfigUtil::Find(const QString& _key)
{
	if (hash.contains(_key))
	{
		return hash[_key];
	}
	else
	{
		qDebug() << _key + " the key doesn't exist.";
		return QVariant();
	}
}

bool ConfigUtil::FindString(const QString& _key, QString& _value)
{
	if (hash.contains(_key))
	{
		if(hash[_key].type() == QVariant::String)
		{
			_value =  hash[_key].value<QString>();
			return true;
		}
		else
		{
			qDebug() << QString("the key value Qstring doesn't exist.");
			return false;
		}
	}
	else
	{
		qDebug() << _key + " the key doesn't exist.";
		return false;
	}
}

bool ConfigUtil::FindInt(const QString& _key, int& _value)
{
	if (hash.contains(_key))
	{
		if (hash[_key].type() == QVariant::Double)
		{
			_value = hash[_key].value<int>();
			return true;
		}
		else
		{
			qDebug() << QString("the key value int doesn't exist.");
			return false;
		}
	}
	else
	{
		qDebug() << _key + " the key doesn't exist.";
		return false;
	}
}

bool ConfigUtil::FindBool(const QString& _key, bool& _value)
{
	if (hash.contains(_key))
	{

		if (hash[_key].type() == QVariant::Bool)
		{
			_value = hash[_key].value<bool>();
			return true;
		}
		else
		{
			qDebug() << QString("the key value bool doesn't exist.");
			return false;
		}
	}
	else
	{
		qDebug() << _key + " the key doesn't exist.";
		return false;
	}
}

bool ConfigUtil::FindDouble(const QString& _key, double& _value)
{
	if (hash.contains(_key))
	{
		if (hash[_key].type() == QVariant::Double)
		{
			_value = hash[_key].value<double>();
			return true;
		}
		else
		{
			qDebug() << QString("the key value double doesn't exist.");
			return false;
		}
	}
	else
	{
		qDebug() << _key + " the key doesn't exist.";
		return false;
	}
}

void ConfigUtil::parse(const QByteArray& _json)
{
	QJsonDocument jd = QJsonDocument::fromJson(_json);
	QJsonObject jo = jd.object();
	QStringList jl = jo.keys();

	for each (QString var in jl)
	{
		hash[var] = jo.value(var).toVariant();
	}
}

