#include <assert.h>
#include <QWidget>
#include <QLayout>
#include "Utils.h"
#include "Dispatcher.h"
#include "HandleBus.hxx"
#include "AppCore.h"
#include "PluginUnity/UnityActions.h"
#include "PluginUnity/PluginUnity.h"
#include "ScreenplayCompositeCore.h"
#include "ScreenplayCompositePlayer.h"

namespace Screenplay
{
	class PlayerUI
	{
	public:
		QWidget* root;
		QWidget* toolbar;
		QWidget* slotCanvas;
	};
}

QWidget* Screenplay::PlayerView::GetRoot()
{
	return ui_->root;
}

void Screenplay::PlayerView::AttachCanvas(QWidget* _canvas)
{
	ui_->slotCanvas->layout()->addWidget(_canvas);
}

void Screenplay::PlayerView::setupUI()
{
	ui_ = new PlayerUI();
	ui_->root = AppCore::CreateUI("screenplayPlayer");
	ui_->toolbar = ui_->root->findChild<QWidget*>("__toolbar");
	ui_->slotCanvas = ui_->root->findChild<QWidget*>("__slotCanvas");
	ui_->toolbar->hide();
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------
Screenplay::PlayerComponent::PlayerComponent(PlayerView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
void Screenplay::PlayerComponent::initialize(Composite::Core* _core)
{
	DYN_CAST(PlayerView, view_, view);
	DYN_CAST(Screenplay::CompositeCore, _core, core);
	IPlugin* plugin = core->GetAppCore()->FindCachePlugin("PluginUnity");
	assert(plugin);
	PluginUnity* unityPlugin = dynamic_cast<PluginUnity*>(plugin);
	QWidget* widget = unityPlugin->GetGameView();
	if (0 != widget)
		view->AttachCanvas(widget);
}
