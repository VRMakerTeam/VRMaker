#pragma once

#include "Composite.h"

class QTreeWidgetItem;

namespace Bucket
{
	class GroupUI;

	class GroupView : public Composite::View
	{
	public:
		QWidget* GetRoot();
		void Filter(const QStringList& _groups);
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
		void reset() override;
	private:
		void append(const QString& _group, const QString& _alias);
		GroupUI* ui_;
		QHash<QTreeWidgetItem*, QString> groupMap_;
	};

	class GroupComponent : public Composite::MVCComponent
	{
	public:
		GroupComponent(GroupView* _view);
		void FilterGroups(const QStringList& _groups);
	protected:
		void initialize(Composite::Core* _core) override;
	private:
	};
}