#ifndef __SCREENPLAYCOMPOSITEPANEL_H__
#define __SCREENPLAYCOMPOSITEPANEL_H__

#include "Composite.h"

class QWidget;

namespace Ui
{
	class PluginAstronomyPanel;
}

namespace Screenplay
{
	class PanelComponent;
	class PlayerComponent;
	class SlateComponent;
	class PreloadComponent;
	class AgentInspectorComponent;
	
	class PanelUI;

	class PanelView : public Composite::View
	{
		friend class PanelComponent;
	public:
		PanelView();
		~PanelView();
		QWidget* GetRoot();
		void TogglePreloadRadio(bool _toggled);
		void ToggleBlockRadio(bool _toggled);
		void ToggleTriggerRadio(bool _toggled);

		void AttachDiagramFlow(QWidget* _root);
		void AttachDiagramGroup(QWidget* _root);
		void AttachDiagramTrigger(QWidget* _root);
		void AttachDiagramProperty(QWidget* _root);

		void SwitchBlankInspector();
		void SwitchAgentInspector();
		void SwitchDiagramInspector();

	protected:
		void setupUI() override;
		void onAttach(View* _view) override;
	private:
		PanelUI* ui_;
	};

	class PanelComponent : public Composite::MVCComponent
	{
	public:
		PanelComponent(PanelView* _view);
		~PanelComponent();

		void HandleOnTriggerNew(IAction* _action);
		void HandleOnTriggerDelete(IAction* _action);
		void HandleOnPipeAdd(IAction* _action);
		void HandleOnPipeRemove(IAction* _action);
		void HandleOnSelectSlate(IAction* _action);
		void HandleOnDeleteSlate(IAction* _action);
		void HandleOnReloadBean(IAction* _action);
		void HandleSwitchAgentInspctor(IAction* _action);
		void HandleSwitchDiagramInspctor(IAction* _action);
	protected:
		void initialize(Composite::Core* _core) override;
	private:
		PlayerComponent* playerComponent_;
		SlateComponent* slateComponent_;
		PreloadComponent* preloadComponent_;
		AgentInspectorComponent* agentInspectorComponent_;
	};
}


#endif
