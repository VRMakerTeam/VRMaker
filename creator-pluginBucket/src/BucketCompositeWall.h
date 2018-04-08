#pragma once

#include "Composite.h"

namespace Bucket
{
	class WallUI;

	class WallView : public Composite::View
	{
	public:
		std::function<void(const QString& _guid)> onSelect;
	public:
		QWidget* GetRoot();
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
		void reset() override;
	private:
		void onCardSelect(const QString& _guid);
		WallUI* ui_;
	};

	class WallComponent : public Composite::MVCComponent
	{
	public:
		WallComponent(WallView* _view);
		/// \brief 使用group过滤
		void HandleFilterAssetsGroup(IAction* _action);
		/// \brief 使用group中的tag过滤
		void HandleFilterAssetsGroupTags(IAction* _action);

		void HandleFilterUGCAudios(IAction* _action);
		void HandleFilterUGCImages(IAction* _action);
	protected:
		void initialize(Composite::Core* _core) override;
	private:
		Composite::Core* core_;
	};
}