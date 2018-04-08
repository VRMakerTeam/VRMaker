#pragma once

#include "Composite.h"

namespace Screenplay
{
	class PreloadUI;

	class PreloadView : public Composite::View
	{
	public:
		~PreloadView();
		QWidget* GetRoot();
	protected:
		void setupUI() override;
		void onUpdate(const QString& _tag) override;
	private:
		PreloadUI* ui_;
		QIcon* ptrDefaultSkyboxIcon_;
		void onPickSkyboxOK(const QString& _guid);
		void onPickAgentOK(const QString& _guid);
	};

	class PreloadComponent : public Composite::MVCComponent
	{
	public:
		PreloadComponent(PreloadView* _view);
		~PreloadComponent();

		void HandleOnPickAssetOK(IAction* _action);

		void HandleDeletePreload(IAction* _action);
		void HandleMessageFromUnity(IAction* _action);
		void HandleRefresh(IAction* _action);
	protected:
		void newPreload(const QString& _guid, const QString& _assetGUID);
		void deletePreload(const QString& _uuid);
		void addPreload(const QString& _guid, const QString& _assetGUID);
		void removePreload(const QString& _uuid);

		void initialize(Composite::Core* _core) override;
	private:
		Composite::Core* ptrCore_;
	};
}
