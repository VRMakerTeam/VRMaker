#ifndef __MEMKV_H__
#define __MEMKV_H__

#include <QVariant>
#include <QHash>
#include <QString>

class MemDB
{
public:
	static void CreateBucket(const QString& _bucket);
	static void DestroyBucket(const QString& _bucket);
	static MemDB* FindBucket(const QString& _bucket);

	static QByteArray EncodeBucket(const QString& _bucket);
	static void DecodeBucket(const QString& _data);

	MemDB(const QString& _bucket);

	QString GetBucket();

	void RemoveKV(const QString& _key);
	void KVSet(const QString& _key, const QVariant& _value);
	QVariant KVGet(const QString& _key);
	QList<QString> KVKeys();

	void RemoveSets(const QString& _key);
	void SetsAdd(const QString& _key, const QVariant& _member);
	void SetsRemove(const QString& _key, const QVariant& _member);
	QList<QVariant> SetsMembers(const QString _key);

	void RemoveHash(const QString& _key);
	void HashSet(const QString& _key, const QString& _field, const QVariant& _value);
	QVariant HashGet(const QString& _key, const QString& _field);
	QVariant HashGet(const QString& _key, const QString& _field, QVariant _default);
	QList<QString> HashKeys();
	QList<QString> HashFields(const QString& _key);
private:
	static QHash<QString, MemDB*> buckets;
	QString bucket;
	QHash<QString, QHash<QString, QVariant> > hash;
	QHash<QString, QList<QVariant> > sets;
	QHash<QString, QVariant > kv;
};

#endif // !__KVDB_H__
