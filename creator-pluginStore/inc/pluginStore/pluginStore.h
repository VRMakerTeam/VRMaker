#ifndef PLUGSTORE_H
#define PLUGSTORE_H

#include "IPlugin.h"

class QWidget;

class PluginStoreInternal;

class PluginStore : public IPlugin
{
public:
	virtual void Initialize(AppCore* _appCore);
	virtual void Attach();
	virtual void Show();
	virtual void Hide();
	virtual void Detach();
	virtual void Release();
	
private:
	PluginStoreInternal* internal_;
};


#endif // PLUGSTORE_H
