#pragma once

#include "Composite.h"


namespace Diagram
{
	class FlowUI;

	class FlowView : public Composite::View
	{
	public:
		FlowView();
		virtual ~FlowView();
		QWidget* GetRoot();

		void ToggleToolbar(bool _toggled);
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
		void reset() override;
	private:
		FlowUI* ui_;
	};

	class FlowComponent : public Composite::MVCComponent
	{
	public:
		FlowComponent(FlowView* _view);
		void HandleToggleFlowToolbar(IAction* _action);
		void HandleCleanFlow(IAction* _action);
		void HandleRefreshFlow(IAction* _action);
		void HandleAddPipe(IAction* _action);
		void HandleRemovePipe(IAction* _action);
	protected:
		void initialize(Composite::Core* _core) override;
		void release() override;
	private:
		void newPipe(const QString& _guid);
		void deletePipe(const QString& _guid);
		void addPipe(const QString& _guid);
		void removePipe(const QString& _guid);
		void refreshPipe(const QString& _guid);
		Composite::Core* ptrCore_;
	};
}//namespace Diagram