#pragma once

#include <QObjectUserData>
#include <QHash>
#include <QVariant>
#include "Composite.h"

class BlockUiBuilder;

namespace Diagram
{
	class BlockUI;

	class BlockView : public Composite::View
	{
	public:
		static const uint UserDataRole = Qt::UserRole - 1;
		class UIUserData : public QObjectUserData
		{
		public:
			QString group;
			QString tpl; // template
			QString component;
		};
	public:
		BlockView();
		virtual ~BlockView();

		void Show() override;
		void Hide() override;

		QWidget* GetRoot();
		void BuildArchetype(const QString& _group, const QString& _template);
		void BuildClone(const QString& _group, const QString& _template);
		QHash<QString, QVariant> GetParams();
		UIUserData GetUserData();
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
		void reset() override;
	private:
		BlockUiBuilder* builder_;
		UIUserData* userData_;
		BlockUI* ui_;
	};

	class BlockComponent : public Composite::MVCComponent
	{
	public:
		virtual ~BlockComponent();
		static BlockComponent* NewArchetype(Composite::Core* _core, const QString& _uuid, const QString& _group, const QString& _template);
		static BlockComponent* NewClone(Composite::Core* _core, const QString& _uuid, const QString& _group, const QString& _template);
		static void Destroy(BlockComponent* _componet);
	protected:
		BlockComponent(BlockView* _view);
		BlockComponent(BlockView* _view, const QString& _uuid);
		virtual void initialize(Composite::Core* _core);
		virtual void release();
		void buildArchetype(const QString& _group, const QString& _template);
		void buildClone(const QString& _group, const QString& _template);
	private:
		Composite::Core* ptrCore_;
	};
}