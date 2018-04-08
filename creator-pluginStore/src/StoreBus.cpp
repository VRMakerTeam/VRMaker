#include <QList>
#include "PluginStore/PluginStore.h"
#include "StoreBus.h"

QList<PluginStore*> StoreBus::plugins;

//-----------------------------------------------------------------------
void StoreBus::Push(PluginStore* _plugin)
{
	plugins.append(_plugin);
}



