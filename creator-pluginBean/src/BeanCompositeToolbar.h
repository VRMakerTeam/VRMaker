#pragma once

#include "Composite.h"

class IAction;
class QPushButton;

namespace Bean
{
	class ToolbarUI;

	class ToolbarView : public Composite::View
	{
	public:
		QWidget* GetRoot();
		void ToggleEditor(bool _visible);
	protected:
		virtual void setupUI();
	private:
		ToolbarUI* ui_;
	};

	class ToolbarComponent : public Composite::MVCComponent
	{
	public:
		ToolbarComponent(ToolbarView* _view);
		void HandleToggleEditorBar(IAction* _action);
	protected:
		virtual void initialize(Composite::Core* _core);
	private:
	};
}//namespace Bean