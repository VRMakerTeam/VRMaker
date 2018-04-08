#ifndef PLUGINUNITY_H
#define PLUGINUNITY_H

#include "IPlugin.h"

class QWidget;

class PluginUnityInternal;

class PluginUnity : public IPlugin
{
public:
	virtual void Initialize(AppCore* _appCore);
	virtual void Attach();
	virtual void Show();
	virtual void Hide();
	virtual void Detach();
	virtual void Release();

	void LoadH5();

	QWidget* GetGameView();
private:
	PluginUnityInternal* internal_;
};

#endif // PLUGINUNITY_H
