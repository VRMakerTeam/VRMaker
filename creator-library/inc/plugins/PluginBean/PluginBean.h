#ifndef PLUGINBEAN_H
#define PLUGINBEAN_H

#include "IPlugin.h"

class PluginBeanInternal;

class PluginBean : public IPlugin
{
public:
	virtual void Initialize(AppCore* _appCore);
	virtual void Attach();
	virtual void Show();
	virtual void Hide();
	virtual void Detach();
	virtual void Release();

private:
	PluginBeanInternal* internal_;
};

#endif // PLUGINBEAN_H
