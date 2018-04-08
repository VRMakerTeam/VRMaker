#ifndef CLICKWIDGET
#define CLICKWIDGET

#include <QWidget>
#include <QMouseEvent>

class ClickWidget : public QWidget
{
	Q_OBJECT
signals:
	///when click widget
	void clicked();
	///if widget in a group, one is checked, other will are set unchecked.
	void toggled(bool _toggled);
public:
	ClickWidget(QWidget* _parent)
        :QWidget(_parent)
    {
		isPressed = false;
		isChecked = false;
		group = "";
    }

	ClickWidget()
        :QWidget(NULL)
    {
		isPressed = false;
		isChecked = false;
		group = "";
    }

	~ClickWidget()
	{
		auto map = groupsMap();
		if (map->contains(group))
		{
			map->value(group)->removeAll(this);
		}
	}

	void setGroup(const QString& _group)
	{
		group = _group;
		auto map = groupsMap();
		if (!map->contains(_group))
		{
			map->insert(_group, new QList<ClickWidget*>());
		}
		map->value(_group)->append(this);
	}

	void setChecked(bool _checked)
	{
		isChecked = _checked;
	}

protected:
    void mousePressEvent(QMouseEvent* _event)
    {
        if (_event->button() == Qt::LeftButton)
        {
            isPressed = true;
            _event->accept();
        }
    }

    void mouseReleaseEvent(QMouseEvent* )
    {
		if (isPressed)
		{
			emit clicked();
			auto map = groupsMap();
			if (map->contains(group))
			{
				QList<ClickWidget*>* widgets = map->value(group);
				for each (ClickWidget* w in *widgets)
				{
					if (w == this)
						this->emitToggle(true);
					else
						w->emitToggle(false);
				}
			}
			else
			{
				isChecked = !isChecked;
				this->emitToggle(isChecked);
			}
		}
        isPressed = false;
    }

private:
    bool isPressed;
	QString group;
	bool isChecked;

	QHash<QString, QList<ClickWidget*>*>* groupsMap()
	{
		static QHash<QString, QList<ClickWidget*>*>* groups = new QHash<QString, QList<ClickWidget *> *>();
		return groups;
	}
	void emitToggle(bool _toggled)
	{
		emit toggled(_toggled);
	}
};
#endif // HEADERWIDGET

