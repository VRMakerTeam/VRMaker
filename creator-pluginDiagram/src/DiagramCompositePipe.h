#pragma once

#include "Composite.h"

class IAction;

namespace DragDropWidgeDelegate {
	struct AppendCloneParams;
}

namespace Diagram
{
	class PipeUI;

	class PipeView : public Composite::View
	{
	public:
		virtual ~PipeView();
		QWidget* GetRoot();
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
	private:
		void onAppendClone(const DragDropWidgeDelegate::AppendCloneParams* _params);
		void onSelect(QWidget* _widget);
		void onDropSelf(QWidget* _widget);
		void onIndexUpdate(QWidget* _widget, int _index);
		PipeUI* ui_;
	};

	class PipeComponent : public Composite::MVCComponent
	{
	public:
		PipeComponent(PipeView* _view, const QString& _uuid);
		~PipeComponent();

		void HandleNewBlock(IAction* _action);
		void HandleDeleteBlock(IAction* _action);
	protected:
		void initialize(Composite::Core* _core) override;
		void release() override;
		void refresh() override;
	private:
		void newEntryBlock(const QString& _template);
		void newExitBlock(const QString& _template);

		/// \brief new a block, will update the store
		void newBlock(const QString& _group, const QString& _template);
		/// \brief delete a block, will update the store
		void deleteBlock(const QString& _uuid);
		void addBlock(const QString& _uuid, const QString& _group, const QString& _template);
		void removeBlock(const QString& _uuid);
		Composite::Core* ptrCore_;
	};
}
