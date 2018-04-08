#ifndef PLUGINSCREENPLAY_H
#define PLUGINSCREENPLAY_H

#include "IPlugin.h"

class QWidget;

class PluginScreenplayInternal;

class PluginScreenplay : public IPlugin
{
public:
	virtual void Initialize(AppCore* _appCore);
	virtual void Attach();
	virtual void Show();
	virtual void Hide();
	virtual void Detach();
	virtual void Release();

	static void RegisterToBeanSource();
private:
	PluginScreenplayInternal* internal_;
};

#endif // PLUGINSCREENPLAY_H
