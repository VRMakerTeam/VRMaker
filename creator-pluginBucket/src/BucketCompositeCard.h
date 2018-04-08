#pragma once

#include "Composite.h"

namespace Bucket
{
	class CardUI;

	class CardView : public Composite::View
	{
	public:
		std::function<void(const QString& _guid)> onSelect;
	public:
		QWidget* GetRoot();
		void setRadioGroup(const QString& _group);
		void UseTheme(const QString& _theme);
	protected:
		~CardView();
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
	private:
		CardUI* ui_;
	};

	class CardComponent : public Composite::MVCComponent
	{
	public:
		CardComponent(CardView* _view, const QString& _uuid);
	protected:
		void initialize(Composite::Core* _core) override;
	private:
	};
}