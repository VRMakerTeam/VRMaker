#pragma once

#include "Composite.h"

class QTimer;

namespace Bean
{
	class CardUI;
	class CardView : public Composite::View
	{
	public:
		virtual ~CardView();
		QWidget* GetRoot();
		void setRadioGroup(const QString& _group);

		void TogglePublishMark(bool _visible);
	protected:
		virtual void setupUI();
		virtual void onUpdate(const QString& _tag);
	private:
		CardUI* ui_;
		QTimer* publishTimer_;
	};

	class CardComponent : public Composite::MVCComponent
	{
	public:
		CardComponent(CardView* _view, const QString& _uuid);

		void HandleModifyBean(IAction* _action);
		void StartPublish();
		void FinishPublish();
		void AbortPublish();
	protected:
		virtual void initialize(Composite::Core* _core);
	private:
	};


}//namespace Bean