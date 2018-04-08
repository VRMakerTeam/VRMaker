#pragma once

#include "Composite.h"

#define GROUP_ENTRY "Entry"
#define GROUP_EXIT "Exit"

#define EXTEND_BASIC "basic"

class IAction;
class QRadioButton;

namespace Diagram
{
	class GroupUI;

	class GroupView : public Composite::View
	{
		friend class GroupComponent;
	public:
		QWidget* GetRoot();
	protected:
		~GroupView();
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
	private:
		GroupUI* ui_;
		QList<QRadioButton*> tabs;
	};

	class GroupComponent : public Composite::MVCComponent
	{
	public:
		GroupComponent(GroupView* _view);
		void HandleFilterGroup(IAction* _action);
		void HandleFilterExtend(IAction* _action);
		void HandleRegisterBlock(IAction* _action);
	protected:
		virtual ~GroupComponent();
		void initialize(Composite::Core* _core) override;
	private:
		void registerBlock(const QString& _extend, const QString& _group, const QString& _template);
		Composite::Core* ptrCore_;
	};
}