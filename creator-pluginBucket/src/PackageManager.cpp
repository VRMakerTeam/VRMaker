#include <assert.h>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QByteArray>
#include "MemDB.h"
#include "Composite.h"
#include "PathUtil.h"
#include "AppCore.h"
#include "pluginBucket/BucketModel.h"
#include "pluginBucket/BucketUtils.h"
#include "PackageManager.h"

/// *Runtime*
/// +package
///    {sets}
///      |- groups
///           |- (string[] | guid of group)
///    {sets}
///      |- group:[guid]:files
///           |- (string[] | files of a group
///    {hash}
///      |- group:[group]:alias:language
///           |- [language]
///                |- (string | the value by translator)
///    {hash}
///      |- files:alias
///           |- [fileguid]
///                |- (string | alias of file)
///    {hash}
///      |- files:package
///           |- [fileguid]
///                |- (string | package)
///    {hash}
///      |- files:group
///           |- [fileguid]
///                |- (string | group)
///    {hash}
///      |- file:[fileguid]:alias:language
///           |- [language]
///                |- (string | the value by translator)

#define MEMDB_PACKAGE_BUCKET "package"
#define MANIFEST_FILE_FORMAT "*.mf"
#define LANGUAGE_FILE_FORMAT "*.rlang"
#define KV_SET_GROUPS_KEY "groups"
#define KV_SET_GROUPFIELS_KEY "group:%1:files"
#define KV_SET_GROUPTAGFIELS_KEY "group:%1:tag:%2:files"
#define KV_SET_GROUPTAGS_KEY "group:%1:tags"
#define KV_HASH_KEY_group_1_alias_language "group:[%1]:alias:language"
#define KV_HASH_KEY_tag_1_alias_language "tag:[%1]:alias:language"
#define KV_HASH_KEY_FILESALIAS "files:alias"
#define KV_HASH_KEY_FILEPACKAGE "files:package"
#define KV_HASH_KEY_FILEGROUP "files:group"
#define KV_HASH_KEY_resource_1_alias_language "resource:[%1]:alias:language"


class PackageManagerInternal
{
public:
	static void ParseGroupLangFile();
	static void ParseTagLangFile();
	static void ParseResourceLangFile();
	static void parseLanguageFile(const QString& _file, const QString& _keyFmt);
};

//-----------------------------------------------------------------------
void PackageManagerInternal::ParseGroupLangFile()
{
	QDir dir = QDir::current();
	dir.cd("languages");
	QString filepath = dir.filePath("groups.glang");

	parseLanguageFile(filepath, KV_HASH_KEY_group_1_alias_language);
}


//-----------------------------------------------------------------------
void PackageManagerInternal::ParseTagLangFile()
{
	QDir dir(PathUtil::PackageLocation());
	dir.cd("languages");
	QString filepath = dir.filePath("tags.tlang");

	parseLanguageFile(filepath, KV_HASH_KEY_tag_1_alias_language);
}

//-----------------------------------------------------------------------
void PackageManagerInternal::ParseResourceLangFile()
{
	QDir dir(PathUtil::PackageLocation());
	dir.cd("languages");
	QString filepath = dir.filePath("files.flang");

	parseLanguageFile(filepath, KV_HASH_KEY_resource_1_alias_language);
}

void PackageManagerInternal::parseLanguageFile(const QString& _file, const QString& _keyFmt)
{
	QFile file(_file);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}
	QByteArray content = file.readAll();
	file.close();

	MemDB::CreateBucket(MEMDB_PACKAGE_BUCKET);
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);

	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(content, &json_error);
	if (json_error.error == QJsonParseError::NoError)
	{
		QJsonArray arySourceTexts = parse_doucment.array();
		for each (QJsonValue vSourceText  in arySourceTexts)
		{
			QJsonObject objSourceText = vSourceText.toObject();
			QString sourcetext = objSourceText.take("sourcetext").toString();
			QJsonArray aryTranslator = objSourceText.take("translators").toArray();
			QString hashKey = QString(_keyFmt).arg(sourcetext);
			for each (QJsonValue vTranslator in aryTranslator)
			{
				QJsonObject objTranslator = vTranslator.toObject();
				QString lang = objTranslator.take("language").toString();
				QString targettext = objTranslator.take("targettext").toString();
				memDB->HashSet(hashKey, lang, targettext);
			}
		}
	}
}

void PackageManager::Parse()
{
	MemDB::CreateBucket(MEMDB_PACKAGE_BUCKET);
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);

	QDir dir(PathUtil::PackageLocation());
	dir.cd("manifests");

	QStringList filters;
	filters << QString(MANIFEST_FILE_FORMAT);
	QStringList minifestList = dir.entryList(filters);

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
			QString groupStr = obj.take("group").toString();
			QString packageStr = obj.take("guid").toString();
			QJsonArray arrayFiles = obj.take("files").toArray();

			memDB->SetsAdd(KV_SET_GROUPS_KEY, groupStr);

			for each(QJsonValue v in arrayFiles) {
				QJsonObject file = v.toObject();
				QString fileStr = file.take("file").toString();
				QString fileAlias = file.take("alias").toString();
				for each(QJsonValue tag in file.take("tags").toArray())
				{
					memDB->SetsAdd(QString(KV_SET_GROUPTAGS_KEY).arg(groupStr), tag.toString());
					memDB->SetsAdd(QString(KV_SET_GROUPTAGFIELS_KEY).arg(groupStr).arg(tag.toString()), fileStr);
				}
				memDB->SetsAdd(QString(KV_SET_GROUPFIELS_KEY).arg(groupStr), fileStr);
				memDB->HashSet(KV_HASH_KEY_FILEPACKAGE, fileStr, packageStr);
				memDB->HashSet(KV_HASH_KEY_FILESALIAS, fileStr, fileAlias);
				memDB->HashSet(KV_HASH_KEY_FILEGROUP, fileStr, groupStr);
			}
		}
	}

	PackageManagerInternal::ParseGroupLangFile();
	PackageManagerInternal::ParseTagLangFile();
	PackageManagerInternal::ParseResourceLangFile();
	Composite::Store::Update(BucketModel::Runtime::Tags::package);

	//…˙≥…Àı¬‘Õº
	BucketUtils::GenerateThumb(dirpath);
}

//-----------------------------------------------------------------------
QList<QString> PackageManager::AllGroups()
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);
	QList<QVariant>& members = memDB->SetsMembers(KV_SET_GROUPS_KEY);
	QList<QString> groups;
	for each(QVariant val in members)
	{
		groups.append(val.toString());
	}
	return groups;
}

//-----------------------------------------------------------------------
QStringList PackageManager::ListFilesInGroup(const QString& _group)
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);
	QList<QVariant>& members = memDB->SetsMembers(QString(KV_SET_GROUPFIELS_KEY).arg(_group));

	QStringList packages;
	for each(QVariant val in members)
	{
		packages.append(val.toString());
	}
	return packages;
}

//-----------------------------------------------------------------------
QStringList PackageManager::ListTagsInGroup(const QString& _group)
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);
	QList<QVariant>& members = memDB->SetsMembers(QString(KV_SET_GROUPTAGS_KEY).arg(_group));

	QStringList tags;
	for each(QVariant val in members)
	{
		tags.append(val.toString());
	}
	return tags;
}

//-----------------------------------------------------------------------
QString PackageManager::TranslateGroup(const QString& _group)
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);
	QString hashKey = QString(KV_HASH_KEY_group_1_alias_language).arg(_group);
	QString language = AppCore::Language();
	QString targettext = memDB->HashGet(hashKey, language, _group).toString();
	return targettext.isEmpty() || targettext.isNull() ? _group : targettext;
}

QString PackageManager::TranslateTag(const QString& _tag)
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);
	QString hashKey = QString(KV_HASH_KEY_tag_1_alias_language).arg(_tag);
	QString language = AppCore::Language();
	QString targettext = memDB->HashGet(hashKey, language, _tag).toString();
	return targettext.isEmpty() || targettext.isNull() ? _tag : targettext;
}

//-----------------------------------------------------------------------
QString PackageManager::QueryFileGroup(const QString& _file)
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);
	QString group = memDB->HashGet(KV_HASH_KEY_FILEGROUP, _file).toString();
	return group.split('/').at(0);
}

//-----------------------------------------------------------------------
QString PackageManager::QueryFileAlias(const QString& _file)
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);
	return memDB->HashGet(KV_HASH_KEY_FILESALIAS, _file).toString();
}

//-----------------------------------------------------------------------
QString PackageManager::QueryFilePackage(const QString& _file)
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);
	return memDB->HashGet(KV_HASH_KEY_FILEPACKAGE, _file).toString();
}

//-----------------------------------------------------------------------
QString PackageManager::TranslateResource(const QString& _alias)
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);
	QString hashKey = QString(KV_HASH_KEY_resource_1_alias_language).arg(_alias);
	QString language = AppCore::Language();
	QString targettext = memDB->HashGet(hashKey, language, _alias).toString();
	return targettext.isEmpty() || targettext.isNull() ? _alias : targettext;
}

//-----------------------------------------------------------------------
QString PackageManager::QueryFileThumbPath(const QString& _file)
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);
	QString package = memDB->HashGet(KV_HASH_KEY_FILEPACKAGE, _file).toString();
	QString path = QString("%1/%2@%2.png").arg(PathUtil::ThumbLocation()).arg(package).arg(_file);
	if(QFile::exists(path))
		return path;

	path = QString("%1/%2.png").arg(PathUtil::ThumbLocation()).arg(package);
	if (QFile::exists(path))
		return path;

	return "";
}

QStringList PackageManager::ListFilesInGroupTags(const QString& _group, const QStringList& _tags)
{
	MemDB* memDB = MemDB::FindBucket(MEMDB_PACKAGE_BUCKET);
	assert(memDB);
	QStringList packages;
	for each (QString tag in _tags)
	{
		QVariantList files = memDB->SetsMembers(QString(KV_SET_GROUPTAGFIELS_KEY).arg(_group).arg(tag));
		for each (QVariant file in files)
		{
			if (!packages.contains(file.toString()))
				packages.append(file.toString());
		}
	}
	return packages;
}
