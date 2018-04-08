#pragma once

#include "Composite.h"

namespace Screenplay
{
	class PreloadCardUI;

	class PreloadCardView : public Composite::View
	{
	public:
		PreloadCardView();
		~PreloadCardView();

		QWidget* GetRoot();
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
	private:
		PreloadCardUI* ui_;
	};

	class PreloadCardComponent : public Composite::MVCComponent
	{
	public:
		static PreloadCardComponent* New();
		static PreloadCardComponent* New(const QString& _uuid);
		static void Destroy(PreloadCardComponent* _componet);

	protected:
		PreloadCardComponent(PreloadCardView* _view);
		PreloadCardComponent(PreloadCardView* _view, const QString& _uuid);
		void initialize(Composite::Core* _core) override;
	private:
		Composite::Core* ptrCore_;
	};
}