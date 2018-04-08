#pragma once

#include "Composite.h"


class QLabel;
class ClickWidget;

namespace Bean
{
	class SourceUI;

	class SourceView : public Composite::View
	{
	public:
		~SourceView();
		QWidget* GetRoot();
		void setRadioGroup(const QString& _group);
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
		void reset() override;
	protected:
		SourceUI* ui_;
	};

	class SourceComponent : public Composite::MVCComponent
	{
	public:
		SourceComponent(SourceView* _view, const QString& _uuid);
	protected:
		void initialize(Composite::Core* _core) override;
	private:
	};
}//namespace Bean