#ifndef __PLUGINDIAGRAM_H__
#define __PLUGINDIAGRAM_H__

#include "IPlugin.h"

class QWidget;
class PluginDiagramInternal;

class PluginDiagram : public IPlugin
{
public:
	virtual void Initialize(AppCore* _appCore);
	virtual void Attach();
	virtual void Show();
	virtual void Hide();
	virtual void Detach();
	virtual void Release();

	QWidget* GetGroupUi();
	QWidget* GetTriggerUi();
	QWidget* GetFlowUi();
	QWidget* GetPropertyUi();
private:
	PluginDiagramInternal* internal_;
};

#endif // __PLUGINDIAGRAM_H__
