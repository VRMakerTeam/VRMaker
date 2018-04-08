#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__

#include <QStringList>
#include <QHash>
#include <QPoint>


#define PACKAGE_GROUP_ZONE "zone"
#define PACKAGE_GROUP_SCROLL "scroll"
#define PACKAGE_GROUP_AGENT "agent"
#define PACKAGE_GROUP_VOICE "voice"
#define PACKAGE_GROUP_MUSIC "music"
#define PACKAGE_GROUP_IMAGE "image"
#define PACKAGE_GROUP_ANIMATION "animation"

#define CACHE_GROUP_ZONE "zone"
#define CACHE_GROUP_SCROLL "scroll"
#define CACHE_GROUP_AGENT "agent"
#define CACHE_GROUP_VOICE "voice"
#define CACHE_GROUP_MUSIC "music"
#define CACHE_GROUP_IMAGE "image"
#define CACHE_GROUP_ANIMATION "animation"


struct FPoint
{
	FPoint(double _x, double _y)
		: x(_x)
		, y(_y)
	{
	}
	double x;
	double y;
};

class CacheManager
{
public:
	//重新读取磁盘上的文件进行解析
	static void Parse();

	//parse store file config
	static void ParseCache();

	//列出一个组下的所有包文件
	static QStringList ListGroup(const QString& _group);

	//list cache group
	static QStringList ListCacheGroup(const QString& _group);

	//Zone:Stage
	static QStringList ListZoneStage(const QString& _package);
	//Agent:File
	static QStringList ListAgentFile(const QString& _package);

	//code is zoneguid-stageguid,like as 82c7a7b4-9dd8-4e24-ba03-98b477129c63:sky1
	static QList<FPoint> ListPoints(const QString& _code);

	static QString GetVoiceAlias(const QString& _package);
	static QString GetVoicePackage(const QString& _alias);
	static QString GetScrollAlias(const QString& _package);
	static QString GetScrollPackage(const QString& _alias);
	static QString GetMusicAlias(const QString& _package);
	static QString GetMusicPackage(const QString& _alias);
	static QString GetAnimationAlias(const QString& _package);
	static QString GetAnimationPackage(const QString& _alias);
	static QString GetImageAlias(const QString& _package);
	static QString GetImagePackage(const QString& _alias);

	//cache
	static QString GetPicById(const int& _id);
	static QString GetAuthorById(const int& _id);
	static QString GetDateById(const int& _id);
	static QString GetGuidById(const int& _id);
	static QString GetVersionById(const int& _id);
	static QString GetGroupById(const int& _id);
	static int GetCountById(const int& _id);

	static QString GetCacheAliasById(const int& _id);

	static QString FindPackageOfAgent(const QString& _file);
	static QStringList GetTagsById(const int& _id);
	static QVector<QString> getAllTags(QVector<QString> &tagsName);
	static QVector<QString> getTagsName();
private:
	static QStringList minifestList;
	static QHash<QString, QStringList> groups;
	//value is guid-stage 
	static QHash<QString, QStringList> stages;
	static QHash<QString, QStringList> files;
	//key is guid, value is alias
	static QHash<QString, QString> voices;
	static QHash<QString, QString> images;
	static QHash<QString, QString> musics;
	static QHash<QString, QString> scrolls;
	static QHash<QString, QString> animations;

	//key is guid-stage 
	static QHash<QString, QList<FPoint>> points;
	static QHash <QString, QString> agentsMap;

	static QHash<int, QString> cacheAlias;
	
	//id - extra
	static QHash<int, QString> pic;
	static QHash<int, QString> author;
	static QHash<int, QString> date;
	static QHash<int, QString> version;
	static QHash<int, QString> guid;
	static QHash<int, QString> group;
	static QHash<int, int> storyboard_count;
	//id - tags
	static QHash<int, QStringList> tags;

	static QVector<QString> tagNames;

	static QHash<QString, QStringList> cacheGroups;

	//tag - ids
	static QHash<QString, QStringList> cacheTags;

	//group - tags
	static QHash<QString, QStringList> groupTags;

};

#endif
