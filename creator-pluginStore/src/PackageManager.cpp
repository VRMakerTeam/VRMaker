#include <QDir>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QByteArray>
#include <algorithm>
#include <iostream>
#include "PackageManager.h"
#include <qglobal.h>
#include <qvector.h>
#include <qlist.h>
using namespace std;

#define MANIFEST_FILE_FORMAT "*.mf"
#define CACHE_KEY_GROUP "group"
#define CACHE_KEY_PIC "pic"
#define CACHE_KEY_DATE "date"
#define CACHE_KEY_VERSION "version"
#define CACHE_KEY_GUID "guid"
#define CACHE_KEY_AUTHOR "author"
#define CACHE_KEY_COUNT "count"

QStringList CacheManager::minifestList;

QHash<QString, QStringList> CacheManager::groups;

QHash<QString, QStringList> CacheManager::stages;

QHash<QString, QStringList> CacheManager::files;

QHash<QString, QString> CacheManager::voices;

QHash<QString, QString> CacheManager::images;

QHash<QString, QString> CacheManager::musics;

QHash<QString, QString> CacheManager::scrolls;

QHash<QString, QString> CacheManager::animations;

QHash<QString, QList<FPoint>> CacheManager::points;

QHash <QString, QString> CacheManager::agentsMap;

// id - alias
QHash<int, QString> CacheManager::cacheAlias;

//id - extra
QHash<int, QString> CacheManager::pic;
QHash<int, QString> CacheManager::author;
QHash<int, QString> CacheManager::date;
QHash<int, QString> CacheManager::version;
QHash<int, QString> CacheManager::guid;
QHash<int, QString> CacheManager::group;

//id - tags
QHash<int, QStringList> CacheManager::tags;
QVector<QString> CacheManager::tagNames;

//id - count
QHash<int, int> CacheManager::storyboard_count;

//group - ids,tips: int to QString
QHash<QString, QStringList> CacheManager::cacheGroups;

//tag - ids tips: int to QString
QHash<QString, QStringList> CacheManager::cacheTags;


QStringList CacheManager::GetTagsById(const int& _id)
{
	return tags[_id];
}

QString CacheManager::GetCacheAliasById(const int& _id)
{
	return cacheAlias[_id];
}

QString CacheManager::GetPicById(const int& _id)
{
	return pic[_id];
}

QString CacheManager::GetAuthorById(const int& _id)
{
	return author[_id];
}

QString CacheManager::GetDateById(const int& _id)
{
	return date[_id];
}

QString CacheManager::GetGuidById(const int& _id)
{
	return guid[_id];
}

QString CacheManager::GetVersionById(const int& _id)
{
	return version[_id];
}

QString CacheManager::GetGroupById(const int& _id)
{
	return group[_id];
}


int CacheManager::GetCountById(const int& _id)
{
	return storyboard_count[_id];
}

QStringList CacheManager::ListCacheGroup(const QString& _group)
{
	return cacheGroups[_group];
}

void CacheManager::ParseCache()
{
	
	QDir dir = QDir::current();
	dir.cd("cache");
	QString filepath = dir.filePath("cache.json");
	QFile file(filepath);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{ 
		QByteArray content = file.readAll();
		QJsonParseError json_error;
		QJsonDocument parse_doucment = QJsonDocument::fromJson(content, &json_error);
		qDebug() << json_error.error;
		if (json_error.error == QJsonParseError::NoError)
		{
			QJsonObject obj = parse_doucment.object();
			QJsonArray productsArray = obj.take("products").toArray();
			
			for each(QJsonValue v in  productsArray)
			{
				QString alias =  v.toObject().take("alias").toString();
				int id = v.toObject().take("id").toInt();
				QJsonArray extraArray =  v.toObject().take("extra").toArray();
				cacheAlias[id] = alias;
				QJsonArray tagArray = v.toObject().take("tags").toArray();
				for each (QJsonValue v in tagArray)
				{ 
					tags[id].append(v.toString());
					tagNames.append(v.toString());
					qDebug() << "tag:v.toString()" << v.toString();
				}

				for each (QJsonValue v in extraArray)
				{
					QString key = v.toObject().take("key").toString();
					QJsonValue value = v.toObject().take("value");
					if (key.compare(CACHE_KEY_GROUP) == 0)
					{					
						cacheGroups[value.toString()].append(QString::number(id));
						group[id] = value.toString();
					}
					if (key.compare(CACHE_KEY_COUNT) == 0)
					{
						//qDebug() << "value.toInt()" << value.toInt();
						storyboard_count[id] = value.toInt();
					}
					else if (key.compare(CACHE_KEY_PIC) == 0)
					{
						pic[id] = value.toString();
					}
					else if (key.compare(CACHE_KEY_AUTHOR) == 0)
					{
						author[id] = value.toString();
					}
					else if (key.compare(CACHE_KEY_DATE) == 0)
					{
						date[id] = value.toString();
					}
					else if (key.compare(CACHE_KEY_GUID) == 0)
					{
						guid[id] = value.toString();
					}
					else if (key.compare(CACHE_KEY_VERSION) == 0)
					{
						version[id] = value.toString();
					}
				}
		
			}

		}
		
	}
	
}

QVector<QString> CacheManager::getTagsName()
{
	return tagNames;
}


void CacheManager::Parse()
{
	QDir dir = QDir::current();
	dir.cd("package");

	QStringList filters;
	filters << QString(MANIFEST_FILE_FORMAT);
	minifestList = dir.entryList(filters);

	QString dirpath = dir.absolutePath();

	for each(QString manifest in  minifestList)
	{
		QString filepath = dir.filePath(manifest);
		QFile file(filepath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			continue;
		}
		QByteArray content = file.readAll();
		QJsonParseError json_error;
		QJsonDocument parse_doucment = QJsonDocument::fromJson(content, &json_error);
		if (json_error.error == QJsonParseError::NoError)
		{
			QJsonObject obj = parse_doucment.object();
			QString guidStr = obj.take("guid").toString();
			QString groupStr = obj.take("group").toString();
			if (groupStr.compare(PACKAGE_GROUP_ZONE) == 0)
			{
				QJsonArray arrayStages = obj.take("stages").toArray();
				QStringList stagelist;
				for each(QJsonValue v in arrayStages){
					QString& code = QString("%1@%2").arg(guidStr).arg(v.toString());	
					stages[guidStr].append(code);
					stagelist << code;
				}
				
				QJsonObject objPoints = obj.take("points").toObject();
				for each(QString stageCode in  stagelist){
					QJsonArray arrayPoints = objPoints.take(stageCode).toArray();
					for each(QJsonValue v in arrayPoints){
						QJsonObject objPoint = v.toObject();
						double x = objPoint.take("x").toDouble();
						double y = objPoint.take("y").toDouble();
						points[stageCode].append(FPoint(x, y));
					}
				}
			}
			else if (groupStr.compare(PACKAGE_GROUP_AGENT) == 0)
			{
				QJsonArray arrayFiles = obj.take("files").toArray();
				for each(QJsonValue v in arrayFiles){
					files[guidStr].append(v.toString());
					agentsMap[v.toString()] = guidStr;
				}
			}
			else if (groupStr.compare(PACKAGE_GROUP_VOICE) == 0)
			{
				//TODO
				voices[guidStr] = obj.take("alias").toString();
			}
			else if (groupStr.compare(PACKAGE_GROUP_MUSIC) == 0)
			{
				//TODO
				musics[guidStr] = obj.take("alias").toString();
			}
			else if (groupStr.compare(PACKAGE_GROUP_ANIMATION) == 0)
			{
				//TODO alias不能保证唯一
				animations[guidStr] = obj.take("alias").toString();
			}
			else if (groupStr.compare(PACKAGE_GROUP_SCROLL) == 0)
			{
				//TODO alias不能保证唯一
				scrolls[guidStr] = obj.take("alias").toString();
			}
			else if (groupStr.compare(PACKAGE_GROUP_IMAGE) == 0)
			{
				//TODO alias不能保证唯一
				images[guidStr] = obj.take("alias").toString();
			}
			groups[groupStr].append(guidStr);
		}
	}
	
}

QVector<QString> CacheManager::getAllTags(QVector<QString> &tagsName)
{
	qSort(tagsName.begin(), tagsName.end());
	tagsName.erase(unique(tagsName.begin(), tagsName.end()), tagsName.end());
	return tagsName;
}
QStringList CacheManager::ListGroup(const QString& _group)
{
	return groups[_group];
}

QStringList CacheManager::ListZoneStage(const QString& _package)
{
	return stages[_package];
}

QStringList CacheManager::ListAgentFile(const QString& _package)
{
	return files[_package];
}

QList<FPoint> CacheManager::ListPoints(const QString& _code)
{
	return points[_code];
}

QString CacheManager::GetVoiceAlias(const QString& _package)
{
	return voices[_package];
}

QString CacheManager::GetVoicePackage(const QString& _alias)
{
	auto itr = qFind(voices.begin(), voices.end(), _alias);
	if (itr != voices.end())
		return itr.key();
	return "";
}

QString CacheManager::GetScrollAlias(const QString& _package)
{
	return scrolls[_package];
}

QString CacheManager::GetScrollPackage(const QString& _alias)
{
	auto itr = qFind(scrolls.begin(), scrolls.end(), _alias);
	if (itr != scrolls.end())
		return itr.key();
	return "";
}

QString CacheManager::GetMusicAlias(const QString& _package)
{
	return musics[_package];
}

QString CacheManager::GetMusicPackage(const QString& _alias)
{
	auto itr = qFind(musics.begin(), musics.end(), _alias);
	if (itr != musics.end())
		return itr.key();
	return "";
}

QString CacheManager::GetAnimationAlias(const QString& _package)
{
	return animations[_package];
}

QString CacheManager::GetAnimationPackage(const QString& _alias)
{
	auto itr = qFind(animations.begin(), animations.end(), _alias);
	if (itr != animations.end())
		return itr.key();
	return "";
}

QString CacheManager::GetImageAlias(const QString& _package)
{
	return images[_package];
}

QString CacheManager::GetImagePackage(const QString& _alias)
{
	auto itr = qFind(images.begin(), images.end(), _alias);
	if (itr != images.end())
		return itr.key();
	return "";
}

QString CacheManager::FindPackageOfAgent(const QString& _file)
{
	return agentsMap[_file];
}

