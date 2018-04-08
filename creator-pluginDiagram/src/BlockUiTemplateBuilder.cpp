#include <QDir>
#include <QUuid>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "BlockParams.h"
#include "MemDB.h"
#include "PluginDiagramLogger.h"
#include "PluginBean/BeanModel.h"
#include "pluginBucket/BucketUtils.h"
#include "BlockUiTemplateBuilder.h"


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
QHash<QString, BlockUiArchetype*> BlockUiArchetype::archetypes;

BlockUiBuilder::BlockUiBuilder(QWidget* _widget, const QString& _uuid)
{
	owner_ = _widget;
	uuid_ = _uuid;

	QString stylesheet = "";
	// font style
	stylesheet.append("QLabel{");
	stylesheet.append("color: rgb(255,255,255);");
	stylesheet.append("font: 11pt;");
	stylesheet.append("}");
	// value style
	stylesheet.append("QLabel[accessibleName=\"block:label:param\"]{");
	stylesheet.append("background-color: rgb(65,65,65);");
	stylesheet.append("padding: 5;");
	stylesheet.append("}");
	// apple style
	_widget->setStyleSheet(stylesheet);
}

BlockUiBuilder::~BlockUiBuilder()
{

}

QHash<QString, QVariant> BlockUiBuilder::Properties()
{
	return properties;
}

void BlockUiBuilder::Reset()
{

}

QString BlockUiBuilder::newUUID()
{
	return QUuid::createUuid().toString().remove("{").remove("}");
}

void BlockUiBuilder::addText(const QString& _text)
{
	QLabel* label = new QLabel(owner_);
	label->setText(_text);
	label->setAlignment(Qt::AlignCenter);
	label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	owner_->layout()->addWidget(label);

	Element element;
	element.type = "text";
	element.param = _text;
	elements.append(element);
}

void BlockUiBuilder::bindProperty(const QString& _property)
{
	QLabel* label = new QLabel(owner_);
	label->setMinimumSize(36, 36);
	label->setMaximumSize(256, 36);
	label->setText("???");
	label->setAlignment(Qt::AlignCenter);
	label->setAccessibleName("block:label:param");
	label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	owner_->layout()->addWidget(label);

	binds[_property] = label;

	Element element;
	element.type = "property";
	element.param = _property;
	elements.append(element);
}

BlockUiArchetype* BlockUiArchetype::New(QWidget* _widget, const QString& _uuid, const QString& _group, const QString& _template)
{
	QString key = QString("%1%2").arg(_group).arg(_template);
	if (archetypes.contains(key))
		return NULL;

	BlockUiArchetype* builder = new BlockUiArchetype(_widget, _uuid);
	archetypes.insert(key, builder);
	return builder;
}

BlockUiArchetype* BlockUiArchetype::Find(const QString& _group, const QString& _template)
{
	QString key = QString("%1%2").arg(_group).arg(_template);
	if (archetypes.contains(key))
		return archetypes.value(key);
	return NULL;
}

BlockUiArchetype::~BlockUiArchetype()
{

}

void BlockUiArchetype::RegisterProperty(const QString& _property, QVariant _defaultValue)
{
	properties[_property] = _defaultValue;
}

void BlockUiArchetype::UseImage(const QString& _image)
{
	QString path = QDir::current().absoluteFilePath(QString("svm/actions/%1").arg(_image));
	QString stylesheet = owner_->styleSheet();
	// background style
	stylesheet.append("QWidget[accessibleName=\"block:bg\"]{");
	stylesheet.append("border-width:40 24 6 24;");
	stylesheet.append(QString("border-image:url(%1);").arg(path));
	stylesheet.append("}");
	owner_->setStyleSheet(stylesheet);
}

void BlockUiArchetype::AddText(const QString& _text)
{
	addText(_text);
}

void BlockUiArchetype::BindProperty(const QString& _property)
{
	bindProperty(_property);
}

BlockUiArchetype::BlockUiArchetype(QWidget* _widget, const QString& _uuid)
	: BlockUiBuilder(_widget, _uuid)
{

}

BlockUiClone* BlockUiClone::New(QWidget* _widget, const QString& _uuid, const QString& _group, const QString& _template)
{
	BlockUiBuilder* archetype = BlockUiArchetype::Find(_group, _template);
	assert(archetype);

	BlockUiClone* builder = new BlockUiClone(_widget, _uuid);
	_widget->setStyleSheet(archetype->owner_->styleSheet());
	builder->properties = archetype->properties;

	foreach(Element element, archetype->elements)
	{
		if (0 == element.type.compare("text"))
			builder->addText(element.param.toString());
		else if (0 == element.type.compare("property"))
			builder->bindProperty(element.param.toString());
	}

	return builder;
}

BlockUiClone::BlockUiClone(QWidget* _widget, const QString& _uuid)
	: BlockUiBuilder(_widget, _uuid)
{

}

BlockUiClone::~BlockUiClone()
{

}

void BlockUiClone::Refresh(const QHash<QString, QVariant>& _params)
{
	foreach(QString key, _params.keys())
	{
		if (!binds.contains(key))
			continue;

		QLabel* w = binds.value(key);
		if (NULL == w)
			continue;

		w->setText(_params.value(key).toString());
	}
}
