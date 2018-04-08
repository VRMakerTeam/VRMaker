#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWidget>
#include <QLayoutItem>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSpacerItem>
#include <QLineEdit>
#include <QTextEdit>
#include <QColorDialog>
#include "AppCore.h"
#include "Utils.h"
#include "MemDB.h"
#include "Composite.h"
#include "BlockParams.h"
#include "pluginDiagram/DiagramModel.h"
#include "PluginBean/BeanModel.h"
#include "pluginBucket/BucketActions.h"
#include "pluginBucket/BucketUtils.h"
#include "PluginUnity/UnityActions.h"
#include "pluginDiagram/DiagramActions.h"
#include "PropertyUiTemplateBuilder.h"

class OnButtonClickCallback
{
public:
	OnButtonClickCallback()
	{

	}
	std::function < void() > onClick;
};
Q_DECLARE_METATYPE(OnButtonClickCallback)

QHash<QString, QList<PropertyUiBuilder::Element>*> PropertyUiBuilder::elementsMap;

QString PropertyUiBuilder::alive_;

QHash<QString, QHash<QString, PropertyUiBuilder::Callback>* > PropertyUiBuilder::callbacks;


int PropertyUiBuilder::BindCallback(const QString& _group, const QString& _template, const QString& _method, Callback _callback)
{
	QString key = QString("%1%2").arg(_group).arg(_template);
	if (!callbacks.contains(key))
		callbacks[key] = new QHash<QString, Callback>();

	if (callbacks.value(key)->contains(_method))
		return 1;

	callbacks.value(key)->insert(_method, _callback);
	return 0;
}

void PropertyUiBuilder::BindPicker(const QString& _group, const QString& _template, const QString& _property, const QString& _butText, const QStringList& _filter)
{
	QList<Element>* es = findElements(_group, _template);
	Element e;
	e.type = "picker";
	e.property = _property;
	e.datas["butText"] = _butText;
	e.datas["filter"] = _filter;
	es->append(e);
}

void PropertyUiBuilder::BindRecorder(const QString& _group, const QString& _template, const QString& _property)
{
	QList<Element>* es = findElements(_group, _template);
	Element e;
	e.type = "recorder";
	e.property = _property;
	es->append(e);
}

void PropertyUiBuilder::BindLineEdit(const QString& _group, const QString& _template, const QString& _property, const QString& _label)
{
	QList<Element>* es = findElements(_group, _template);
	Element e;
	e.type = "lineedit";
	e.property = _property;
	e.lable = _label;
	es->append(e);
}

void PropertyUiBuilder::BindNumberSpin(const QString& _group, const QString& _template, const QString& _property, const QString& _label, double _min, double _max)
{
	QList<Element>* es = findElements(_group, _template);
	Element e;
	e.type = "numberspin";
	e.property = _property;
	e.lable = _label;
	e.datas["min"] = _min;
	e.datas["max"] = _max;

	es->append(e);
}

void PropertyUiBuilder::BindIntegerSpin(const QString& _group, const QString& _template, const QString& _property, const QString& _label, int _min, int _max)
{
	QList<Element>* es = findElements(_group, _template);
	Element e;
	e.type = "integerspin";
	e.property = _property;
	e.lable = _label;
	e.datas["min"] = _min;
	e.datas["max"] = _max;

	es->append(e);
}


void PropertyUiBuilder::BindButton(const QString& _group, const QString& _template, const QString& _label, std::function< void() > _onClick)
{
	QList<Element>* es = findElements(_group, _template);
	Element e;
	e.type = "button";
	e.lable = _label;
	OnButtonClickCallback callback;
	callback.onClick = _onClick;
	e.datas["onClick"] = QVariant::fromValue(callback);

	es->append(e);
}


void PropertyUiBuilder::BindTextEdit(const QString& _group, const QString& _template, const QString& _property, const QString& _label)
{
	QList<Element>* es = findElements(_group, _template);
	Element e;
	e.type = "textedit";
	e.property = _property;
	e.lable = _label;
	es->append(e);
}

void PropertyUiBuilder::BindCombobox(const QString& _group, const QString& _template, const QString& _property, const QString& _label, const QStringList _lis)
{
	QList<Element>* es = findElements(_group, _template);
	Element e;
	e.type = "combobox";
	e.property = _property;
	e.lable = _label;

	e.datas["items"] = _lis;
	es->append(e);
}

void PropertyUiBuilder::BindColorPicker(const QString& _group, const QString& _template, const QString& _property, const QString& _label, int _r, int _g, int _b, int _a)
{
	QList<Element>* es = findElements(_group, _template);
	Element e;
	e.type = "colorpicker";
	e.property = _property;
	e.lable = _label;
	e.datas["r"] = _r;
	e.datas["g"] = _g;
	e.datas["b"] = _b;
	e.datas["a"] = _a;

	es->append(e);
}


QString PropertyUiBuilder::Alive()
{
	return alive_;
}


void PropertyUiBuilder::UpdateProperty(const QString& _invoker, const QHash<QString, QVariant>& _properties)
{
	if (0 != invoker_.compare(_invoker))
		return;

	foreach(QString prop, _properties.keys())
	{
		if (!propWidgets.contains(prop))
			continue;

		QVariant value = _properties.value(prop);

		QWidget* w = propWidgets.value(prop);

		DYN_CAST(QLineEdit, w, edit);
		if (NULL != edit)
		{
			if(value.canConvert<QString>())
				edit->setText(value.toString());
		}

		DYN_CAST(QDoubleSpinBox, w, spin);
		if (NULL != spin)
		{
			if (value.canConvert<double>())
				spin->setValue(value.toDouble());
		}
			
	}
}

QList<PropertyUiBuilder::Element>* PropertyUiBuilder::findElements(const QString& _group, const QString& _template)
{
	QString key = QString("%1%2").arg(_group).arg(_template);

	if (!elementsMap.contains(key))
		elementsMap.insert(key, new QList<Element>());

	return elementsMap.value(key);
}

PropertyUiBuilder::PropertyUiBuilder()
	: root_(NULL)
{
	
}

void PropertyUiBuilder::appendPicker(const QString& _property,const QString& _butText, QStringList& _groups)
{
	QPushButton* button = new QPushButton(root_);
	button->setAccessibleName("button:blue");
	button->setText(_butText);
	QLabel* label = new QLabel(root_);
	label->setAccessibleName("label:picker");
	label->setScaledContents(true);
	QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(root_->layout());
	root_->setLayout(layout);
	layout->addWidget(label);
	layout->addWidget(button);

	//refresh
	{
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		QString fileguid = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property);
		QString filepath = BucketUtils::QueryFileThumbPath(fileguid);
		label->setPixmap(QPixmap(filepath));
		QString alias = BucketUtils::QueryFileAlias(fileguid);
		if (!alias.isEmpty())
			button->setText(alias);
	}	

	QObject::connect(button, &QPushButton::clicked, [=] {
		NEW_ACTION(BucketActions::PopupPickerAction, action);
		action->slot = "";
		action->filter = _groups;
		action->onOK = [=](const QString& _guid) {
			MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
			DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property, _guid);
			Composite::Store::Update(DiagramModel::Persistent::Tags::digram_block_update);

			//refresh
			{
				QString filepath = BucketUtils::QueryFileThumbPath(_guid);
				label->setPixmap(QPixmap(filepath));
				QString alias = BucketUtils::QueryFileAlias(_guid);
				if (!alias.isEmpty())
					button->setText(alias);
			}
		};
		INVOKE_ACTION(BucketActions::PopupPickerAction, action);
	});
}



void PropertyUiBuilder::appendRecorder(const QString& _property)
{
	QWidget* widget = AppCore::CreateUI("diagramRecorderBox");
	
	QWidget* toolbar = widget->findChild<QWidget*>("__toolbar");
	QPushButton* trash = widget->findChild<QPushButton*>("__trash");
	QCheckBox* del = widget->findChild<QCheckBox*>("__delete");
	QCheckBox* record = widget->findChild<QCheckBox*>("__record");
	QPushButton* play = widget->findChild<QPushButton*>("__play");
	QPushButton* stop= widget->findChild<QPushButton*>("__stop");

	toolbar->setVisible(false);
	trash->setVisible(false);
	del->setChecked(false);

	QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(root_->layout());
	layout->addWidget( widget);

	//refresh
	{
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		const QString& fileGUID = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property);
		toolbar->setVisible(!(fileGUID.isNull() || fileGUID.isEmpty()));
	}

	QObject::connect(record, &QCheckBox::clicked, [=](bool _clicked) {
		if (_clicked)
		{
			NEW_ACTION(BucketActions::StartRecordUGCAudioAction, action);
			action->uuid = invoker_;
			INVOKE_ACTION(BucketActions::StartRecordUGCAudioAction, action);
		}
		else
		{
			NEW_ACTION(BucketActions::StopRecordUGCAudioAction, action);
			INVOKE_ACTION(BucketActions::StopRecordUGCAudioAction, action);
			toolbar->setVisible(true);
			MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
			DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property, invoker_ + ".wav");
			Composite::Store::Update(DiagramModel::Persistent::Tags::digram_block_update);
		}
	});

	QObject::connect(play, &QPushButton::clicked, [=] {
		NEW_ACTION(BucketActions::StartPlayUGCAudioAction, action);
		action->uuid = invoker_;
		INVOKE_ACTION(BucketActions::StartPlayUGCAudioAction, action);
	});

	QObject::connect(stop, &QPushButton::clicked, [=] {
		NEW_ACTION(BucketActions::StopPlayUGCAudioAction, action);
		INVOKE_ACTION(BucketActions::StopPlayUGCAudioAction, action);
	});

	QObject::connect(del, &QCheckBox::clicked, [=](bool _checked) {
		trash->setVisible(_checked);
	});

	QObject::connect(trash, &QPushButton::clicked, [=] {
		toolbar->setVisible(false);
		trash->setVisible(false);
		del->setChecked(false);
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property, "");
		Composite::Store::Update(DiagramModel::Persistent::Tags::digram_block_update);
	});
}

void PropertyUiBuilder::appendLineEdit(const QString& _property, const QString& _label)
{
	QHBoxLayout* hLayout = new QHBoxLayout(root_);
	QLabel* label = new QLabel(root_);
	label->setText(_label);
	label->setAccessibleName("label:field");
	hLayout->addWidget(label);
	QLineEdit* edit = new QLineEdit(root_);
	edit->setAccessibleName("lineedit:field");
	hLayout->addWidget(edit);

	QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(root_->layout());
	layout->insertItem(layout->count() - 1, hLayout);

	propWidgets[_property] = edit;

	//refresh 
	{
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		QVariant value = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property);
		assert(!value.isNull());

		edit->setText(value.toString());
	}

	QObject::connect(edit, &QLineEdit::textChanged, [=](const QString& _text) {
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property, _text);
		Composite::Store::Update(DiagramModel::Persistent::Tags::digram_block_update);
	});
}


void PropertyUiBuilder::appendIntegerSpin(const QString& _property, const QString& _label, int _min, int _max)
{
	QWidget* widget = new QWidget(root_);
	QHBoxLayout* hLayout = new QHBoxLayout(widget);
	hLayout->setMargin(9);
	hLayout->setSpacing(6);
	widget->setLayout(hLayout);

	QLabel* label = new QLabel(widget);
	label->setText(_label);
	label->setAccessibleName("label:field");
	hLayout->addWidget(label);
	QSpinBox* spinbox = new QSpinBox(widget);
	spinbox->setMinimum(_min);
	spinbox->setMaximum(_max);
	spinbox->setAccessibleName("spinbox:field");
	hLayout->addWidget(spinbox);

	QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(root_->layout());
	layout->addWidget(widget);

	propWidgets[_property] = spinbox;

	//refresh 
	{
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		QVariant value = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property);
		assert(!value.isNull());

		spinbox->setValue(value.toDouble());
	}

	QObject::connect(spinbox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int _value) {
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property, _value);
		Composite::Store::Update(DiagramModel::Persistent::Tags::digram_block_update);
	});
}

void PropertyUiBuilder::appendNumberSpin(const QString& _property, const QString& _label, double _min, double _max)
{
	QWidget* widget = new QWidget(root_);
	QHBoxLayout* hLayout = new QHBoxLayout(widget);
	hLayout->setMargin(9);
	hLayout->setSpacing(6);
	widget->setLayout(hLayout);

	QLabel* label = new QLabel(widget);
	label->setText(_label);
	label->setAccessibleName("label:field");
	hLayout->addWidget(label);
	QDoubleSpinBox* spinbox = new QDoubleSpinBox(widget);
	spinbox->setMinimum(_min);
	spinbox->setMaximum(_max);
	spinbox->setAccessibleName("spinbox:field");
	hLayout->addWidget(spinbox);

	QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(root_->layout());
	layout->addWidget(widget);

	propWidgets[_property] = spinbox;

	//refresh 
	{
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		QVariant value = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property);
		assert(!value.isNull());

		spinbox->setValue(value.toDouble());
	}

	QObject::connect(spinbox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double _value) {
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property, _value);
		Composite::Store::Update(DiagramModel::Persistent::Tags::digram_block_update);
	});
}


void PropertyUiBuilder::appendButton(const QString& _label, std::function< void() > _onClick)
{
	QPushButton* button = new QPushButton(root_);
	button->setAccessibleName("button:blue");
	button->setText(_label);

	root_->layout()->addWidget(button);

	QObject::connect(button, &QPushButton::clicked, [=]() {
		_onClick();
	});


}


void PropertyUiBuilder::appendTextEdit(const QString& _property, const QString& _label)
{
	QHBoxLayout* hLayout = new QHBoxLayout();
	QLabel* label = new QLabel(root_);
	label->setText(_label);
	label->setAccessibleName("label:field");
	hLayout->addWidget(label);

	QTextEdit* edit = new QTextEdit(root_);
	edit->setAccessibleName("textedit:field");
	hLayout->addWidget(edit);

	QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(root_->layout());
	layout->insertItem(layout->count() - 1, hLayout);

	propWidgets[_property] = edit;

	//refresh
	{
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		QVariant value = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property);
		assert(!value.isNull());

		edit->setText(value.toString());
	}
	QObject::connect(edit, &QTextEdit::textChanged, [=]() {
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property, edit->toPlainText());
		Composite::Store::Update(DiagramModel::Persistent::Tags::digram_block_update);
	});
}

void PropertyUiBuilder::appendCombobox(const QString& _property, const QString& _label, const QStringList _lis)
{
	QHBoxLayout* hLayout = new QHBoxLayout();
	QLabel*  label = new QLabel(root_);
	label->setText(_label);
	label->setAccessibleName("label:field");
	hLayout->addWidget(label);
	QComboBox* combobox = new QComboBox(root_);
	combobox->setAccessibleName("combobox:field");
	combobox->setMaxVisibleItems(5);
	combobox->addItems(_lis);
	combobox->setMaximumWidth(130);
	combobox->setMinimumHeight(30);
	hLayout->addWidget(combobox);

	QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(root_->layout());
	layout->insertItem(layout->count() - 1, hLayout);

	propWidgets[_property] = combobox;

	//refresh
	{
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		QVariant value = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property);
		assert(!value.isNull());

		combobox->setCurrentText(value.toString());
	}

	QObject::connect(combobox, &QComboBox::currentTextChanged, [=](const QString& _text) {

		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, _property, _text);
		Composite::Store::Update(DiagramModel::Persistent::Tags::digram_block_update);
	});

}

void PropertyUiBuilder::appendColorPicker(const QString& _property, const QString& _label, int _r, int _g, int _b, int _a)
{
	QHBoxLayout* hLayout = new QHBoxLayout();
	QLabel* label = new QLabel(root_);
	label->setText(_label);
	label->setAccessibleName("label:field");
	hLayout->addWidget(label);

	QPushButton* button = new QPushButton(root_);
	button->setAccessibleName("colorpicker:field");
	button->setAutoFillBackground(true);
	button->setFlat(true);
	button->setMaximumWidth(60);
	//button->setMaximumHeight(120);
	hLayout->addWidget(button);

	QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(root_->layout());
	layout->insertItem(layout->count() - 1, hLayout);

	//refresh 
	{
		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		QVariant r = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(persistentMemDB, invoker_, QString("%1:r").arg(_property));
		assert(!r.isNull());
		QVariant g = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(persistentMemDB, invoker_, QString("%1:g").arg(_property));
		assert(!g.isNull());
		QVariant b = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(persistentMemDB, invoker_, QString("%1:b").arg(_property));
		assert(!b.isNull());
		QVariant a = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(persistentMemDB, invoker_, QString("%1:a").arg(_property));
		assert(!a.isNull());

		if (r.isNull() && g.isNull() && b.isNull() && a.isNull())
		{
			QPalette palette = button->palette();
			palette.setColor(QPalette::ColorRole::Button, QColor::fromRgb(_r, _g, _b, _a));
			button->setPalette(palette);

			MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
			DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, QString("%1:r").arg(_property), _r);
			DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, QString("%1:g").arg(_property), _g);
			DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, QString("%1:b").arg(_property), _b);
			DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, QString("%1:a").arg(_property), _a);
			Composite::Store::Update(DiagramModel::Persistent::Tags::digram_block_update);
		}
		else
		{
			QPalette palette = button->palette();
			palette.setColor(QPalette::ColorRole::Button, QColor::fromRgb(r.toInt(), g.toInt(), b.toInt(), a.toInt()));
			button->setPalette(palette);
		}
	}


	QObject::connect(button, &QPushButton::clicked, [=] {
		QColor color = QColorDialog::getColor(Qt::white, NULL, "Color",QColorDialog::ColorDialogOption::ShowAlphaChannel);
		if (!color.isValid())
			return;

		MemDB* persistentMemDB = DiagramModel::Persistent::GetMemDB();
		DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, QString("%1:r").arg(_property), color.red());
		DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, QString("%1:g").arg(_property), color.green());
		DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, QString("%1:b").arg(_property), color.blue());
		DiagramModel::Persistent::Hash::Update::diagram_block_guid_1_fields(persistentMemDB, invoker_, QString("%1:a").arg(_property), color.alpha());
		Composite::Store::Update(DiagramModel::Persistent::Tags::digram_block_update);

		QPalette palette = button->palette();
		palette.setColor(QPalette::ColorRole::Button, color);
		button->setPalette(palette);
	});
}



void PropertyUiBuilder::Build(QWidget* _parent, const QString& _group, const QString& _template, const QString& _invoker)
{
	invoker_ = _invoker;
	alive_ = _invoker;

	while (QLayoutItem* item = _parent->layout()->itemAt(0))
	{
		_parent->layout()->removeItem(item);
		item->widget()->setParent(NULL);
		item->widget()->deleteLater();
	}

	root_ = new QWidget(_parent);
	QVBoxLayout* layout = new QVBoxLayout(root_);
	layout->setMargin(0);
	root_->setLayout(layout);
	_parent->layout()->addWidget(root_);

	QList<Element>* es = findElements(_group, _template);

	foreach(Element e, *es)
	{
		if (0 == e.type.compare("picker"))
			appendPicker(e.property, e.datas["butText"].toString(), e.datas["filter"].toStringList());
		else if (0 == e.type.compare("recorder"))
			appendRecorder(e.property);
		else if (0 == e.type.compare("lineedit"))
			appendLineEdit(e.property, e.lable);
		else if (0 == e.type.compare("numberspin"))
			appendNumberSpin(e.property, e.lable, e.datas["min"].toDouble(), e.datas["max"].toDouble());
		else if (0 == e.type.compare("integerspin"))
			appendIntegerSpin(e.property, e.lable, e.datas["min"].toInt(), e.datas["max"].toInt());
		else if (0 == e.type.compare("button"))
			appendButton(e.lable, e.datas["onClick"].value< OnButtonClickCallback >().onClick);
		else if (0 == e.type.compare("textedit"))
			appendTextEdit(e.property, e.lable);
		else if (0 == e.type.compare("combobox"))
			appendCombobox(e.property, e.lable, e.datas["items"].toStringList());
		else if (0 == e.type.compare("colorpicker"))
			appendColorPicker(e.property, e.lable, e.datas["r"].toInt(), e.datas["g"].toInt(), e.datas["b"].toInt(), e.datas["a"].toInt());
	}


	QString key = QString("%1%2").arg(_group).arg(_template);
	if (!callbacks.contains(key))
		return;
	if (!callbacks.value(key)->contains(CALLBACK_ONBUILDFINISH))
		return;

	Callback callback = callbacks.value(key)->value(CALLBACK_ONBUILDFINISH);
	QHash<QString, QVariant> params;
	params[CALLBACK_ONBUILDFINISH_PARAM_INVOKER] = _invoker;
	callback(params);
}
