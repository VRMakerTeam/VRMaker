#ifndef __DIAGRAMACTIONS_H__
#define __DIAGRAMACTIONS_H__

#include <functional>
#include <QHash>
#include <QVariant>
#include "Dispatcher.h"

namespace DiagramActions
{
	
	//////////////////////////////////////////////////////////////////////////
	// Filter blocks of group
	//////////////////////////////////////////////////////////////////////////
	class FilterGroupAction : public IAction
	{
	public:
		QString group;
	};

	class FilterGroupReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Filter blocks of extend
	//////////////////////////////////////////////////////////////////////////
	class FilterExtendAction : public IAction
	{
	public:
		QStringList extends;
	};

	class FilterExtendReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Toggle Flow Toolbar
	//////////////////////////////////////////////////////////////////////////
	class ToggleFlowToolbarAction : public IAction
	{
	public:
		bool visible;
	};

	class ToggleFlowToolbarReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Clean Flow
	//////////////////////////////////////////////////////////////////////////
	class CleanFlowAction : public IAction
	{
	};

	class CleanFlowReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Refresh Flow
	//////////////////////////////////////////////////////////////////////////
	class RefreshFlowAction : public IAction
	{
	public:
		QStringList pipesGUID;
	};

	class RefreshFlowReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// New Trigger
	//////////////////////////////////////////////////////////////////////////
	class NewTriggerAction : public IAction
	{
	public:
		QString type;
	};

	class NewTriggerReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	class OnTriggerNewAction : public IAction
	{
	public:
		QString uuid;
	};

	//////////////////////////////////////////////////////////////////////////
	// Delete Trigger
	//////////////////////////////////////////////////////////////////////////
	class DeleteTriggerAction : public IAction
	{
	public:
		QString guid;
	};

	class DeleteTriggerReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	class OnTriggerDeleteAction : public IAction
	{
	public:
		QString uuid;
	};

	//////////////////////////////////////////////////////////////////////////
	// Toggle Triggers Trash
	//////////////////////////////////////////////////////////////////////////
	class ToggleTriggersTrashAction : public IAction
	{
	public:
		bool toggle;
	};

	class ToggleTrigersTrashReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};


	//////////////////////////////////////////////////////////////////////////
	// Refresh Flow
	//////////////////////////////////////////////////////////////////////////
	class RefreshTriggersAction : public IAction
	{
	public:
		QStringList triggersGUID;
	};

	class RefreshTriggersReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};


	//////////////////////////////////////////////////////////////////////////
	// Add Pipe
	//////////////////////////////////////////////////////////////////////////
	class AddPipeAction : public IAction
	{
	public:
	};

	class AddPipeReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	class OnPipeAddAction : public IAction
	{
	public:
		QString uuidPipe;
	};

	//////////////////////////////////////////////////////////////////////////
	// Remove Pipe
	//////////////////////////////////////////////////////////////////////////
	class RemovePipeAction : public IAction
	{
	public:
		QString uuidPipe;
	};

	class RemovePipeReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	class OnPipeRemoveAction : public IAction
	{
	public:
		QString uuidPipe;
	};

	//////////////////////////////////////////////////////////////////////////
	// Refresh Pipe
	//////////////////////////////////////////////////////////////////////////
	class RefreshPipeAction : public IAction
	{
	public:
		QString uuidPipe;
	};

	class RefreshPipeReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Register Block
	//////////////////////////////////////////////////////////////////////////
	class RegisterBlockAction : public IAction
	{
	public:
		QString extend;
		QString group;
		QString name;
	};

	class RegisterBlockReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// New Block
	//////////////////////////////////////////////////////////////////////////
	class NewBlockAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString uuidPipe;
	};

	class NewBlockReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Delete Block
	//////////////////////////////////////////////////////////////////////////
	class DeleteBlockAction : public IAction
	{
	public:
		QString uuid;
	};

	class DeleteBlockReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Block register property
	//////////////////////////////////////////////////////////////////////////
	class BlockRegisterPropertyAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString property;
		QVariant defaultValue;
	};

	class BlockRegisterPropertyReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Block UI Add Text
	//////////////////////////////////////////////////////////////////////////
	class BlockUiAddTextAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString text;
	};

	class BlockUiAddTextReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Block UI Add Value
	//////////////////////////////////////////////////////////////////////////
	class BlockUiBindPropertyAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString property;
	};

	class BlockUiBindPropertyReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Block UI use image
	//////////////////////////////////////////////////////////////////////////
	class BlockUiUseImageAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString image;
	};

	class BlockUiUseImageReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Property bind callback
	//////////////////////////////////////////////////////////////////////////
	class PropertyBindCallbackAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString method;
		std::function< void(QHash<QString, QVariant>) > callback;
	};

	class PropertyBindCallbackReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Property bind picker
	//////////////////////////////////////////////////////////////////////////
	class PropertyUiBindPickerAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString property;
		QString butText;
		QStringList filter; /// < asset groups
	};

	class PropertyUiBindPickerReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Property bind recorder
	//////////////////////////////////////////////////////////////////////////
	class PropertyUiBindRecorderAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString property;
	};

	class PropertyUiBindRecorderReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Property bind line edit
	//////////////////////////////////////////////////////////////////////////
	class PropertyUiBindLineEditAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString property;
		QString label;
	};

	class PropertyUiBindLineEditReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Property bind Text edit
	//////////////////////////////////////////////////////////////////////////
	class PropertyUiBindTextEditAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString property;
		QString label;
	};

	class PropertyUiBindTextEditReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Property bind number spin
	//////////////////////////////////////////////////////////////////////////
	class PropertyUiBindNumberSpinAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString property;
		QString label;
		double min;
		double max;
	};

	class PropertyUiBindNumberSpinReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Property bind integer spin
	//////////////////////////////////////////////////////////////////////////
	class PropertyUiBindIntegerSpinAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString property;
		QString label;
		int min;
		int max;
	};

	class PropertyUiBindIntegerSpinReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Property bind button
	//////////////////////////////////////////////////////////////////////////
	class PropertyUiBindButtonAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString label;
		std::function< void() > onClick;
	};

	class PropertyUiBindButtonReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Property bind combobox
	//////////////////////////////////////////////////////////////////////////
	class PropertyUiBindComboBoxAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString label;
		QString property;
		QStringList lis;
	};

	class PropertyUiBindComboBoxReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Property bind color picker
	//////////////////////////////////////////////////////////////////////////
	class PropertyUiBindColorPickerAction : public IAction
	{
	public:
		QString group;
		QString tpl;
		QString label;
		QString property;
		int r;
		int g;
		int b;
		int a;
	};

	class PropertyUiBindColorPickerReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// Property Update Property
	//////////////////////////////////////////////////////////////////////////
	class PropertyUiUpdatePropertyAction : public IAction
	{
	public:
		QString invoker;
		QHash<QString, QVariant> properties;
	};

	class PropertyUiUpdatePropertyReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	class OpenBlockPropertyPanelAction : public IAction
	{
	public:
		QString group;
		QString tpl; //template
		QString invoker; ///< uuid of component
	};
	class OpenBlockPropertyPanelReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	class OpenTriggerPropertyPanelAction : public IAction
	{
	public:
		QString invoker; ///< uuid of component
	};
	class OpenTriggerPropertyPanelReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	class ClosePropertyPanelAction : public IAction
	{
	public:
	};
	class ClosePropertyPanelReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	class OnNewBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	class OnDeleteBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	class OnReloadBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};


	

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	class MessageFromUnityReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};


}//namespace DiagramActions

#endif
