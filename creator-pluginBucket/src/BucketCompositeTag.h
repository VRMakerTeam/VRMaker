#pragma once

#include "Composite.h"

class QCheckBox;

namespace Bucket
{
	class TagUI;

	class TagView : public Composite::View
	{
	public:
		QWidget* GetRoot();
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
		void reset() override;
	private:
		void append(const QString& _tag, const QString& _alias);
		TagUI* ui_;
		QHash<QCheckBox*, QString> tagMap_;
	};

	class TagComponent : public Composite::MVCComponent
	{
	public:
		TagComponent(TagView* _view);
		void HandleFilterTagsGroup(IAction* _action);
	protected:
		void initialize(Composite::Core* _core) override;
	private:
	};
}