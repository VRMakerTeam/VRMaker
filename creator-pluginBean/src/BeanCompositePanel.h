#ifndef __BEANCOMPONENTPANEL_H__
#define __BEANCOMPONENTPANEL_H__

#include <QHash>
#include "Composite.h"

class IAction;
class QStackedWidget;

class WallComponent;
class PickerComponent;
class ToolbarComponent;

namespace Bean
{
	class PanelUI;

	class PanelView : public Composite::View
	{
		friend class PanelComponent;
	public:
		QWidget* GetRoot();

		void OpenDashboardPage();
		void OpenPickPage();
	protected:
		~PanelView();
		virtual void setupUI();
		virtual void onUpdate(const QString& _tag);
		virtual void onAttach(View* _view);
	private:
		PanelUI* ui_;
	};

	class PanelComponent : public Composite::MVCComponent
	{
	public:
		PanelComponent(PanelView* _view);
		void HandleOpenPage(IAction* _action);
		void HandlePublishBean(IAction* _action);
		void HandleInstallBean(IAction* _action);
	protected:
		virtual void initialize(Composite::Core* _core);
	private:
		WallComponent* wallComponent_;
		ToolbarComponent* toolbarComponent_;
		PickerComponent* pickerComponent_;

		void readyPublishBean(const QByteArray& _bytes);
		void onPublishFinish(const QByteArray& _bytes);
		void onPublishErrro(const QString& _error);
		void readyInstallBean(const QByteArray& _bytes);
	};
}

#endif
