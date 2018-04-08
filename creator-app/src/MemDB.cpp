#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "MemDB.h"
QHash<QString, MemDB*> MemDB::buckets;

//-----------------------------------------------------------------------
void MemDB::CreateBucket(const QString& _bucket)
{
	if (buckets.contains(_bucket))
		return;
	buckets.insert(_bucket, new MemDB(_bucket));
}

//-----------------------------------------------------------------------
void MemDB::DestroyBucket(const QString& _bucket)
{
	if (!buckets.contains(_bucket))
		return;
	delete buckets.value(_bucket);
	buckets.remove(_bucket);
}

//-----------------------------------------------------------------------
MemDB* MemDB::FindBucket(const QString& _bucket)
{
	if (buckets.contains(_bucket))
		return buckets.value(_bucket);
	return NULL;
}

//-----------------------------------------------------------------------
QByteArray MemDB::EncodeBucket(const QString& _bucket)
{
	MemDB* memDB = FindBucket(_bucket);
	if (NULL == memDB)
		return QByteArray();
	QJsonObject objKV;
	for (auto itr = memDB->kv.cbegin(); itr != memDB->kv.cend(); ++itr)
	{
		objKV.insert(itr.key(), QJsonValue::fromVariant(itr.value()));
	}

	QJsonObject objSets;
	for (auto itr = memDB->sets.cbegin(); itr != memDB->sets.cend(); ++itr)
	{
		QJsonArray arySets;
		for each (QVariant value in itr.value())
		{
			arySets.append(QJsonValue::fromVariant(value));
		}
		objSets.insert(itr.key(), arySets);
	}

	QJsonObject objHash;
	for (auto itr = memDB->hash.cbegin(); itr != memDB->hash.cend(); ++itr)
	{
		QJsonObject objSubHash;
		for (auto itrSub = itr.value().cbegin(); itrSub != itr.value().cend(); ++itrSub)
		{
			objSubHash.insert(itrSub.key(), QJsonValue::fromVariant(itrSub.value()));
		}
		objHash.insert(itr.key(), objSubHash);
	}

	QJsonObject root;
	root.insert("bucket", _bucket);
	root.insert("kv", objKV);
	root.insert("sets", objSets);
	root.insert("hash", objHash);

	QJsonDocument doc;
	doc.setObject(root);
	return doc.toJson();
}

//-----------------------------------------------------------------------
void MemDB::DecodeBucket(const QString& _data)
{
	QJsonDocument doc = QJsonDocument::fromJson(_data.toUtf8());
	QJsonObject root = doc.object();
	QString bucket = root.take("bucket").toString();
	MemDB* memDB = FindBucket(bucket);
	//override a exists bucket is not allowed
	if (NULL != memDB)
		return;

	CreateBucket(bucket);
	memDB = FindBucket(bucket);

	QJsonObject objKV = root.take("kv").toObject();
	for each (QString key in objKV.keys())
	{
		memDB->KVSet(key, objKV[key].toVariant());
	}

	QJsonObject objHash = root.take("hash").toObject();
	for each (QString key in objHash.keys())
	{
		QJsonObject objField = objHash.take(key).toObject();
		for each (QString field in objField.keys())
		{
			memDB->HashSet(key, field, objField[field].toVariant());
		}
	}

	QJsonObject objSets = root.take("sets").toObject();
	for each (QString key in objSets.keys())
	{
		QJsonArray aryMember = objSets.take(key).toArray();
		for each (QJsonValue member in aryMember)
		{
			memDB->SetsAdd(key, member.toVariant());
		}
	}

}

//-----------------------------------------------------------------------
MemDB::MemDB(const QString& _bucket)
{
	bucket = _bucket;
}

//-----------------------------------------------------------------------
QString MemDB::GetBucket()
{
	return bucket;
}

void MemDB::RemoveKV(const QString& _key)
{
	if (kv.contains(_key))
		kv.remove(_key);
}

//-----------------------------------------------------------------------
void MemDB::KVSet(const QString& _key, const QVariant& _value)
{
	kv[_key] = _value;
}

//-----------------------------------------------------------------------
QVariant MemDB::KVGet(const QString& _key)
{
	if (kv.contains(_key))
		return kv.value(_key);
	return QVariant();
}

//-----------------------------------------------------------------------
QList<QString> MemDB::KVKeys()
{
	return kv.keys();
}

void MemDB::RemoveSets(const QString& _key)
{
	if (sets.contains(_key))
		sets.remove(_key);
}

//-----------------------------------------------------------------------
void MemDB::SetsAdd(const QString& _key, const QVariant& _member)
{
	if (sets[_key].contains(_member))
		return;
	sets[_key].append(_member);
}

void MemDB::SetsRemove(const QString& _key, const QVariant& _member)
{
	if (!sets.contains(_key))
		return;

	if (!sets.value(_key).contains(_member))
		return;

	sets[_key].removeAll(_member);
}

//-----------------------------------------------------------------------
QList<QVariant> MemDB::SetsMembers(const QString _key)
{
	return sets.value(_key);
}

void MemDB::RemoveHash(const QString& _key)
{
	if (hash.contains(_key))
		hash.remove(_key);
}

//-----------------------------------------------------------------------
void MemDB::HashSet(const QString& _key, const QString& _field, const QVariant& _value)
{
	hash[_key][_field] = _value;
}
//-----------------------------------------------------------------------
QVariant MemDB::HashGet(const QString& _key, const QString& _field)
{
	if (!hash.contains(_key))
		return QVariant();
	if (!hash[_key].contains(_field))
		return QVariant();
	return hash.value(_key).value(_field);
}

//-----------------------------------------------------------------------
QVariant MemDB::HashGet(const QString& _key, const QString& _field, QVariant _default)
{
	if (!hash.contains(_key))
		return _default;
	if (!hash[_key].contains(_field))
		return _default;
	return hash.value(_key).value(_field);
}

//-----------------------------------------------------------------------
QList<QString> MemDB::HashKeys()
{
	return hash.keys();
}

//-----------------------------------------------------------------------
QList<QString> MemDB::HashFields(const QString& _key)
{
	QList<QString> fields;
	if (hash.contains(_key))
	{
		fields = hash.value(_key).keys();
	}
	return fields;
}

