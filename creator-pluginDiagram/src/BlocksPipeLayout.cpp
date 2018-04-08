#include <QtWidgets>
#include <QObjectUserData>
#include "BlocksPipeLayout.h"

BlocksPipeLayout::BlocksPipeLayout(QWidget *parent, int spacing)
	: QHBoxLayout(parent)
	, m_spacing(spacing)
	, entry_(NULL)
	, exit_(NULL)
{
	setContentsMargins(0, 0, 0, 0);
}

BlocksPipeLayout::BlocksPipeLayout(int spacing)
	: m_spacing(spacing)
	, entry_(NULL)
	, exit_(NULL)
{
	setContentsMargins(0, 0, 0, 0);
}

BlocksPipeLayout::~BlocksPipeLayout()
{
	
}

void BlocksPipeLayout::addEntry(QWidget* _entry)
{
	entry_ = _entry;
	this->addWidget(entry_);
}

void BlocksPipeLayout::addExit(QWidget* _exit)
{
	exit_ = _exit;
	this->addWidget(exit_);
}

void BlocksPipeLayout::addBond(QWidget* _bond)
{
	if (bonds_.contains(_bond))
		return;
	bonds_.append(_bond);
	this->addWidget(_bond);
}

void BlocksPipeLayout::removeBlock(QWidget* _block)
{
	if (!bonds_.contains(_block))
		return;

	bonds_.removeAll(_block);
	this->removeWidget(_block);
}

void BlocksPipeLayout::clean()
{
	QLayoutItem* item = NULL;
	while (item = this->takeAt(0))
	{
		item->widget()->setParent(NULL);
		this->removeItem(item);
	}
	entry_ = NULL;
	exit_ = NULL;
	bonds_.clear();
}

void BlocksPipeLayout::setGeometry(const QRect &rect)
{
	QLayout::setGeometry(rect);
	doLayout(rect);
}

//-----------------------------------------------------------------------
void BlocksPipeLayout::relayout()
{
	//在未布局前将bond按新的位置进行排序
	qSort(bonds_.begin(), bonds_.end(), [=](const QWidget* _left, const QWidget* _right) {
		return _left->geometry().x() < _right->geometry().x();
	});

	QRect rect = geometry();
	doLayout(rect);
}

void BlocksPipeLayout::doLayout(const QRect &rect) const
{
	int left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
	int x = effectiveRect.x();
	int y = effectiveRect.y();
	int lineHeight = 0;

	int index = 0;

	if (NULL != entry_)
	{
		QSize sizeHint = entry_->sizeHint();
		entry_->setGeometry(QRect(QPoint(x, y), entry_->sizeHint()));
		x = entry_->sizeHint().width() + m_spacing;

		if (NULL != OnIndexUpdateDelegate)
			OnIndexUpdateDelegate(entry_, index++);
	}

	for each (QWidget* _block in bonds_)
	{
		int nextX = x + _block->sizeHint().width() + m_spacing;
		if (nextX - m_spacing > effectiveRect.right()) {
			x = effectiveRect.x();
			nextX = x + _block->sizeHint().width() + m_spacing;
		}
		_block->setGeometry(QRect(QPoint(x, y), _block->sizeHint()));
		x = nextX;

		if (NULL != OnIndexUpdateDelegate)
			OnIndexUpdateDelegate(_block, index++);
	}

	if (NULL != exit_)
	{
		exit_->setGeometry(QRect(QPoint(x, y), exit_->sizeHint()));

		if (NULL != OnIndexUpdateDelegate)
			OnIndexUpdateDelegate(exit_, index++);
	}
}
