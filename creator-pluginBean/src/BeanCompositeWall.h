#pragma once

#include "Composite.h"

class IAction;

namespace Bean
{
	class WallUI;

	class WallView : public Composite::View
	{
	public:
		QWidget* GetRoot();
		void AppendCard(QWidget* _root);
		void RemoveCard(QWidget* _root);
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
		
	private:
		WallUI* ui_;
	};

	class WallComponent : public Composite::MVCComponent
	{
	public:
		WallComponent(WallView* _view);

		void HandleNewBean(IAction* _action);
		void HandleDeleteBean(IAction* _action);
		void HandleSelectBean(IAction* _action);
		void HandleParseDisk(IAction* _action);
	protected:
		void initialize(Composite::Core* _core) override;
	private:
		void newBean(const QString& _guid, const QString& _alias);
		void deleteBean(const QString& _guid);
		void selectBean(const QString& _guid);
		Composite::Core* ptrCore_;
	};
}//namespace Bean