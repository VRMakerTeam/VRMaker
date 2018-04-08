#pragma once

#include "Composite.h"

class IAction;
class QWidget;

namespace Diagram
{

	class TriggersUI;

	class TriggersView : public Composite::View
	{
	public:
		QWidget* GetRoot();
		void ToogleNewGaze(bool _visiable);
	protected:
		void setupUI() override;
		void reset() override;
		void onUpdate(const QString& _tag) override;

	private:
		TriggersUI* ui_;
	};

	class TriggersComponent : public Composite::MVCComponent
	{
	public:
		TriggersComponent(TriggersView* _view);
		void HandleNewTrigger(IAction* _action);
		void HandleDeleteTrigger(IAction* _action);
		void HandleOnReloadBean(IAction* _action);
		void HandleRefreshTriggers(IAction* _action);
	protected:
		virtual ~TriggersComponent();
		void initialize(Composite::Core* _core) override;
		QString newTrigger(const QString& _type);
		void deleteTrigger(const QString& _guid);
	private:
		Composite::Core* ptrCore_;
	};
}//namespace Diagram