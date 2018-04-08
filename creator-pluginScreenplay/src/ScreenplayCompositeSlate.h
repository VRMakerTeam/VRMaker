#pragma once

#include "Composite.h"

class IAction;

namespace Screenplay
{
	class SlateUI;

	class SlateView : public Composite::View
	{
	public:
		QWidget* GetRoot();
		void ToggleDeleteButton(bool _visible);
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
	private:
		SlateUI* ui_;
	};

	class SlateComponent : public Composite::MVCComponent
	{
	public:
		SlateComponent(SlateView* _view);
		~SlateComponent();
		void HandleNewSlateCard(IAction* _action);
		void HandleDeleteSlateCard(IAction* _action);
		void HandleSelectSlateCard(IAction* _action);
		void HandleReloadSlates(IAction* _action);

	protected:
		void initialize(Composite::Core* _core) override;
	private:
		void newSlateCard();
		void newSlateCard(const QString& _uuid);
		void deleteSlateCard(const QString& _uuid);
		void addSlateCard(const QString& _uuid);
		void removeSlateCard(const QString& _uuid);
		Composite::Core* ptrCore_;
	};
}

