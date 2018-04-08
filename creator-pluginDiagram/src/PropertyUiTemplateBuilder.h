#pragma once


#include "gdut/Factory.hxx"
#include <functional>
#include <QString>
#include <QVariant>

class QWidget;
class QPushButton;
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QDoubleSpinBox;

#define CALLBACK_ONBUILDFINISH "onBuildFinish"
#define CALLBACK_ONBUILDFINISH_PARAM_INVOKER "invoker"

class PropertyUiBuilder
{
public:
	typedef std::function< void(QHash<QString, QVariant>) > Callback;
	/// \return
	/// 0 : success
	/// 1 : _method is exists
	static int BindCallback(const QString& _group, const QString& _template, const QString& _method, Callback _callback);

	static void BindPicker(const QString& _group, const QString& _template, const QString& _property, const QString& _butText, const QStringList& _filter);
	static void BindRecorder(const QString& _group, const QString& _template, const QString& _property);
	static void BindLineEdit(const QString& _group, const QString& _template, const QString& _property, const QString& _label);
	static void BindNumberSpin(const QString& _group, const QString& _template, const QString& _property, const QString& _label, double _min, double _max);
	static void BindIntegerSpin(const QString& _group, const QString& _template, const QString& _property, const QString& _label, int _min, int _max);
	static void BindButton(const QString& _group, const QString& _template, const QString& _label,  std::function< void() > _onClick);

	static void BindTextEdit(const QString& _group, const QString& _template, const QString& _property, const QString& _label);
	static void BindCombobox(const QString& _group, const QString& _template, const QString& _property, const QString& _label, const QStringList _lis);
	static void BindColorPicker(const QString& _group, const QString& _template, const QString& _property, const QString& _label, int _r, int _g, int _b, int _a);

	static QString Alive();

	void UpdateProperty(const QString& _invoker, const QHash<QString, QVariant>& _properties);

	void Build(QWidget* _widget, const QString& _group, const QString& _template, const QString& _invoker);
	PropertyUiBuilder();
private:
	struct Element
	{
		QString type;
		QString property;
		QString lable;

		//key is name of property, value is value of property
		QHash<QString, QVariant> datas;
	};
	static QList<Element>* findElements(const QString& _group, const QString& _template);
	static QHash<QString, QHash<QString, Callback>* > callbacks;

	void appendPicker(const QString& _property, const QString& _butText, QStringList& _groups);
	void appendRecorder(const QString& _property);
	void appendLineEdit(const QString& _property, const QString& _label);
	void appendIntegerSpin(const QString& _property, const QString& _label, int _min, int _max);
	void appendNumberSpin(const QString& _property, const QString& _label, double _min, double _max);
	void appendButton(const QString& _label, std::function< void() > _onClick);

	void appendTextEdit(const QString& _property, const QString& _label);
	void appendCombobox(const QString& _property, const QString& _label, const QStringList _lis);
	void appendColorPicker(const QString& _property, const QString& _label, int _r, int _g, int _b, int _a);

	// key is group+template
	static QHash<QString, QList<Element>*> elementsMap;
	QHash<QString, QWidget*> propWidgets;
	QString invoker_;
	static QString alive_;
	QWidget* root_;
};
