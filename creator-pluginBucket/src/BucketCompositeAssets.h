#pragma once

#include <functional>
#include "Composite.h"

class IAction;

namespace Bucket
{
	class AssetsUI;

	class AssetsView : public Composite::View
	{
		friend class AssetsComponent;
	public:
		std::function<void(const QString& _guid)> onSelect;
	public:
		QWidget* GetRoot();
	protected:
		~AssetsView();
		void setupUI() override;
		void onAttach(View* _view) override;
	private:
		AssetsUI* ui_;
	};

	class AssetsComponent : public Composite::MVCComponent
	{
	public:
		AssetsComponent(AssetsView* _view);
	protected:
		void initialize(Composite::Core* _core) override;
	private:
	};
}