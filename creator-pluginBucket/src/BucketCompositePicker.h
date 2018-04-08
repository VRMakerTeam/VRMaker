#pragma once

#include <functional>
#include <QMap>
#include "Composite.h"

class IAction;

namespace Bucket
{
	class AssetsComponent;
	class PickerComponent;

	class PickerUI;

	class PickerView : public Composite::View
	{
	public:
		std::function<void(const QString& _guid)> onOK;
		void OnCardSelect(const QString& _uuid);
	public:
		PickerView();
		~PickerView();
		/// \brief attach the widget of assets
		void Popup();
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
		void onAttach(View* _view) override;
		void reset() override;
	private:
		QString selectUUID_;
		PickerUI* ui_;
	};

	class PickerComponent : public Composite::MVCComponent
	{
	public:
		PickerComponent(PickerView* _view);
		~PickerComponent();
		void HandlePopupPicker(IAction* _action);
	protected:
		void initialize(Composite::Core* _core) override;
	private:
		AssetsComponent* assetsComponent_;
	};
}