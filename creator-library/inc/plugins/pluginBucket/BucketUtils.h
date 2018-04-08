#pragma once

#include <QString>

class BucketUtils
{
public:
	static QString QueryFileGroup(const QString& _file);
	static QString QueryFileAlias(const QString& _file);
	static QString QueryFilePackage(const QString& _file);
	/// \return package@file, like as dd3b49bd-aa06-4279-86bd-c4ed2ab9a723@adfb5f3d-2bcb-494e-8364-0c32684f15d2
	static QString QueryFileThumbPath(const QString& _file);

	static QByteArray ReadUGCData(const QString& _file);

	static void PlaySound(const QString& _file);
	static void StopSound();

	static void GenerateThumb(const QString& _directory);
};