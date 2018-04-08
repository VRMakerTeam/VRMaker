#pragma once

#include "Composite.h"

namespace Screenplay
{
	class SlateCardUI;

	class SlateCardView : public Composite::View
	{
	public:
		~SlateCardView();
		QWidget* GetRoot();
	protected:
		void setupUI() override;
		void reset() override;
	private:
		SlateCardUI* ui_;
	};

	class SlateCardComponent : public Composite::MVCComponent
	{
	public:
		SlateCardComponent(SlateCardView* _view);
		SlateCardComponent(SlateCardView* _view, const QString& _uuid);
		void HandleRenameSlateCard(IAction* _action);
	protected:
		void initialize(Composite::Core* _core) override;
	private:
	};
}