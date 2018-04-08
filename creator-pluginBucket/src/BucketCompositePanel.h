#pragma once

#include "Composite.h"

namespace Bucket
{
	class AssetsComponent;

	class PanelUI;

	class PanelView: public Composite::View
	{
	public:
		QWidget* GetRoot();
		/// \brief attach the widget of assets
		void AttachAssets(QWidget* _widget);
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
	protected:
		void initialize(Composite::Core* _core) override;
	private:
		AssetsComponent* assetsComponent_;
	};
}