#pragma once

#include "Composite.h"

class IAction;

class QRadioButton;

namespace Bean
{
	class PickerUI;

	class PickerView : public Composite::View
	{
	public:
		QWidget* GetRoot();
		void SwitchArchetypePage();
		void SwitchClonePage();
	protected:
		void setupUI() override;
		void reset() override;
		void onUpdate(const QString& _tag) override;
	private:
		PickerUI* ui_;
	};

	class PickerComponent : public Composite::MVCComponent
	{
	public:
		PickerComponent(PickerView* _view);
		void HandleRegisterSource(IAction* _action);
		void HandleSelectSource(IAction* _action);

		void SwitchArchetypePage();
		void SwitchClonePage();
	protected:
		virtual void initialize(Composite::Core* _core);
	private:
		QString pickedSource_;
		Composite::Core* ptrCore_;
	};
}//namespace Bean