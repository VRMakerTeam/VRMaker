#ifndef PLUGINBUCKET_H
#define PLUGINBUCKET_H

#include "IPlugin.h"

class QWidget;

class PluginBucketInternal;

class PluginBucket : public IPlugin
{
public:
	virtual void Initialize(AppCore* _appCore);
	virtual void Attach();
	virtual void Show();
	virtual void Hide();
	virtual void Detach();
	virtual void Release();
private:
	PluginBucketInternal* internal_;
};

#endif // PLUGINASTRONOMY_H
