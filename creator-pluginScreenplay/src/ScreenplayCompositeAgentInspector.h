#pragma once

#include "Composite.h"

class QWidget;
class IAction;

namespace Screenplay
{
	class AgentInspectorUI;

	class AgentInspectorView : public Composite::View
	{
	public:
		QWidget* GetRoot();
		void UpdateInvoker(const QString& _invoker);
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
		void reset() override;
	private:
		AgentInspectorUI* ui_;
	};

	class AgentInspectorComponent : public Composite::MVCComponent
	{
	public:
		AgentInspectorComponent(AgentInspectorView* _view);
		void SetInvoker(const QString& _invoker);
	protected:
		void initialize(Composite::Core* _core) override;
	private:
	};
}