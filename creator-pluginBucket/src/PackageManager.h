#ifndef __PACKAGEMANAGER_H__
#define __PACKAGEMANAGER_H__

#include <QStringList>
#include <QList>

class PackageManager
{
public:
	//重新读取磁盘上的官方资源进行解析
	static void Parse();

	static QList<QString> AllGroups();

	/// \return the list of files
	static QStringList ListFilesInGroup(const QString& _group);
	static QStringList ListTagsInGroup(const QString& _group);
	static QString TranslateGroup(const QString& _group);
	static QString TranslateTag(const QString& _tag);

	static QString QueryFileGroup(const QString& _file);
	static QString QueryFileAlias(const QString& _file);
	static QString QueryFilePackage(const QString& _file);
	static QString TranslateResource(const QString& _alias);

	/// \return package@file, like as dd3b49bd-aa06-4279-86bd-c4ed2ab9a723@adfb5f3d-2bcb-494e-8364-0c32684f15d2
	static QString QueryFileThumbPath(const QString& _file);

	/// \return the list of files
	static QStringList ListFilesInGroupTags(const QString& _group, const QStringList& _tags);

private:
	

};

#endif
