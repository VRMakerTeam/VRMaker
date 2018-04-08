#pragma once

#include "Composite.h"

class IAction;
class QWidget;

namespace Diagram
{

	class TriggerUI;

	class TriggerView : public Composite::View
	{
	public:
		QWidget* GetRoot();
		void ToggleTrash(bool _toggle);
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
	private:
		TriggerUI* ui_;
	};

	class TriggerComponent : public Composite::MVCComponent
	{
	public:
		static TriggerComponent* New();
		static TriggerComponent* New(const QString& _guid);

		void HandleToggleTrigersTrash(IAction* _action);
	protected:
		TriggerComponent(TriggerView* _view);
		TriggerComponent(TriggerView* _view, const QString& _guid);
		virtual ~TriggerComponent();
		void initialize(Composite::Core* _core) override;
	private:
	};
}//namespace Diagram