#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <functional>

class QWidget;

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class DragDropWidget;

namespace DragDropWidgeDelegate{
	struct AppendCloneParams
	{
		DragDropWidget* source;
		QWidget* item;
		QPoint point;
	};
}


class DragDropWidget : public QWidget
{
public:
	enum Mode {
		ONLY_DRAG,
		ONLY_DROP,
		DRAGDROP,
	};
	
	DragDropWidget(QWidget *parent = 0);
	void SetMode(Mode _mode);
	std::function<void(const DragDropWidgeDelegate::AppendCloneParams* _params)> AppendCloneDelegate;
	std::function<void(QWidget* _widget)> OnSelectDelegate;
	std::function<void(QWidget* _widget)> OnDropSelfDelegate;
	void AddTag(const QString& _tag);
	bool HasTag(const QString& _tag);
protected:
	void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
	void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
	void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	
private:
	Mode mode;
	QStringList tags_;
};

#endif // DRAGWIDGET_H
