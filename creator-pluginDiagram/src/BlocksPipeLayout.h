#pragma once

#include <QHBoxLayout>
#include <QRect>
#include <QStyle>
#include <QList>

class BlocksPipeLayout : public QHBoxLayout
{
public:
	explicit BlocksPipeLayout(QWidget *parent, int spacing = -1);
	explicit BlocksPipeLayout(int spacing = -1);
	~BlocksPipeLayout();

	void addEntry(QWidget* _entry);
	void addExit(QWidget* _exit);
	void addBond(QWidget* _bond);
	void removeBlock(QWidget* _block);

	void clean();

	void setGeometry(const QRect &rect) Q_DECL_OVERRIDE;
	void relayout();

	std::function<void(QWidget* _widget, int _index)> OnIndexUpdateDelegate;
private:
	void doLayout(const QRect &rect) const;
	int m_spacing;
	QWidget* entry_;
	QWidget* exit_;
	QList<QWidget*> bonds_;
};