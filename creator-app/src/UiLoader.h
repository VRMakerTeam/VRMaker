#pragma once

#include <QtUiTools/QUiLoader>
#include "HeaderWidget.h"
#include "ClickWidget.h"
#include "DragDropWidget.h"

class UiLoader : public QUiLoader
{
public:
	UiLoader(QObject * parent = 0) 
		: QUiLoader(parent) 
	{
	}

	QWidget * createWidget(const QString & _className, QWidget * _parent = 0, const QString & _name = QString()) 
	{
		if (_className == "HeaderWidget") {
			HeaderWidget * w = new HeaderWidget(_parent);
			w->setObjectName(_name);
			return w;
		}

		if (_className == "ClickWidget") {
			ClickWidget * w = new ClickWidget(_parent);
			w->setObjectName(_name);
			return w;
		}

		if (_className == "DragDropWidget") {
			DragDropWidget * w = new DragDropWidget(_parent);
			w->setObjectName(_name);
			return w;
		}

		return QUiLoader::createWidget(_className, _parent, _name);
	}
};