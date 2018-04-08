#pragma once

#include "gdut/Factory.hxx"
#include <QHash>
#include <QVariant>
#include <QString>

class QWidget;
class QLabel;

class BlockUiArchetype;
class BlockUiClone;

class BlockUiBuilder
{
	friend BlockUiArchetype;
	friend BlockUiClone;
public:
	virtual ~BlockUiBuilder();
	QHash<QString, QVariant> Properties();
	void Reset();
protected:
	BlockUiBuilder(QWidget* _widget, const QString& _uuid);
	QString newUUID();
	void addText(const QString& _text);
	void bindProperty(const QString& _property);

	struct Element
	{
		QString type;
		QVariant param;
	};

	QString uuid_;
	QHash<QString, QVariant> properties;
	QHash<QString, QLabel*> binds;
	QList<Element> elements;
	QWidget* owner_;
};


class BlockUiClone : public BlockUiBuilder
{
public:
	static BlockUiClone* New(QWidget* _widget, const QString& _uuid, const QString& _group, const QString& _template);
	void Refresh(const QHash<QString, QVariant>& _params);
	~BlockUiClone();
protected:
	BlockUiClone(QWidget* _widget, const QString& _uuid);
private:

};

class BlockUiArchetype : public BlockUiBuilder
{
	friend BlockUiClone;
public:
	static BlockUiArchetype* New(QWidget* _widget, const QString& _uuid, const QString& _group, const QString& _template);
	static BlockUiArchetype* Find(const QString& _group, const QString& _template);

	~BlockUiArchetype();
	void RegisterProperty(const QString& _property, QVariant _defaultValue);
	void UseImage(const QString& _image);

	void AddText(const QString& _text);
	void BindProperty(const QString& _property);
protected:
	BlockUiArchetype(QWidget* _widget, const QString& _uuid);
private:
	static QHash<QString, BlockUiArchetype*> archetypes;
};

