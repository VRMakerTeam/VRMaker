#include <QWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QMimeData>
#include <QPainter>
#include <QDrag>

#include "DragDropWidget.h"

union DDItem
{
	int value;
	QWidget* widget;
};

DragDropWidget::DragDropWidget(QWidget *parent)
	: QWidget(parent)
{
	setAcceptDrops(true);
	AppendCloneDelegate = NULL;
	OnSelectDelegate = NULL;
	OnDropSelfDelegate = NULL;
	mode = Mode::DRAGDROP;
}

//-----------------------------------------------------------------------
void DragDropWidget::SetMode(Mode _mode)
{
	mode = _mode;
}

void DragDropWidget::AddTag(const QString& _tag)
{
	if (tags_.contains(_tag))
		return;
	tags_ << _tag;
}

bool DragDropWidget::HasTag(const QString& _tag)
{
	return tags_.contains(_tag);
}

void DragDropWidget::dragEnterEvent(QDragEnterEvent *event)
{
	if (mode == Mode::ONLY_DRAG)
	{
		event->ignore();
		return;
	}

	if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
		if (event->source() == this) {
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else {
			event->acceptProposedAction();
		}
	}
	else {
		event->ignore();
	}
}

void DragDropWidget::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
		if (event->source() == this) {
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else {
			event->acceptProposedAction();
		}
	}
	else {
		event->ignore();
	}
}

void DragDropWidget::dropEvent(QDropEvent *event)
{
	if (mode == ONLY_DRAG)
	{
		event->ignore();
		return;
	}

	if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
		QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		DDItem item;
		QPoint offset;
		dataStream >> item.value >> offset;

		if (event->source() == this) {
			item.widget->move(event->pos() - offset);
			event->setDropAction(Qt::MoveAction);
			event->accept();
			if (NULL != OnDropSelfDelegate)
				OnDropSelfDelegate(item.widget);
		}
		else {
			if (NULL != AppendCloneDelegate)
			{
				DragDropWidgeDelegate::AppendCloneParams params;
				params.source = dynamic_cast<DragDropWidget*>(event->source());
				params.item = item.widget;
				params.point = event->pos() - offset;
				AppendCloneDelegate(&params);
			}
			event->acceptProposedAction();
		}
	}
	else {
		event->ignore();
	}
}

//! [1]
void DragDropWidget::mousePressEvent(QMouseEvent *event)
{
	QWidget *child = childAt(event->pos());
	if (!child)
		return;

	if (NULL != OnSelectDelegate)
	{
		OnSelectDelegate(child);
	}

	if (mode == Mode::ONLY_DROP)
	{
		return;
	}

	DDItem item;
	item.widget = child;

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	dataStream << item.value << (event->pos() - child->pos());

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-dnditemdata", itemData);

	QPixmap pixmap(child->size());
	child->render(&pixmap);
	QPainter painter;
	painter.begin(&pixmap);
	painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
	painter.end();

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap);
	drag->setHotSpot(event->pos() - child->pos());

	if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) != Qt::MoveAction) {
		child->show();
	}
}

