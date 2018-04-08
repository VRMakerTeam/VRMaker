#pragma once

#include <QVariant>
#include "Composite.h"

class IAction;
class PropertyUiBuilder;
class QWidget;

namespace Diagram
{

	class PropertyUI;

	class PropertyView : public Composite::View
	{
	public:
		QWidget* GetRoot();
		void ToogleBlockToolbar(bool _visiable);
		void SwitchTriggerProperty();
		void SwitchBlockProperty();

		void UpdateTrigger(const QString& _triggerUUID, const QString& _alias);
		void UpdateGazeIcon(const QString& _triggerUUID, int _icon);
		void UpdateGazeColor(const QString& _triggerUUID, int _r, int _g, int _b, int _a);
		void Build(const QString& _group, const QString& _tpl, const QString& _invoker);
		void UpdateProperty(const QString& _invoker, const QHash<QString, QVariant>& _properties);
	protected:
		~PropertyView();
		void setupUI() override;
		void reset() override;
		void onUpdate(const QString& _tag) override;
	private:
		PropertyUI* ui_;
		PropertyUiBuilder* builder_;
	};

	class PropertyComponent : public Composite::MVCComponent
	{
	public:
		PropertyComponent(PropertyView* _view);
		void HandleOpenBlockPropertyPanel(IAction* _action);
		void HandleOpenTriggerPropertyPanel(IAction* _action);
		void HandleClosePropertyPanel(IAction* _action);
		void ProcessMessageFromUnity(const QString& _method, const QString& _activity, const QString& _param);
		void HandleUpdateProperty(IAction* _action);
	protected:
		virtual ~PropertyComponent();
		virtual void initialize(Composite::Core* _core);
	private:
	};
}//namespace Diagram