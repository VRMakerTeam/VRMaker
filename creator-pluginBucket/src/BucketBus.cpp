#include <QList>
#include "PluginBucket/PluginBucket.h"
#include "BucketBus.h"

QList<PluginBucket*> BucketBus::plugins;

//-----------------------------------------------------------------------
void BucketBus::Push(PluginBucket* _plugin)
{
	plugins.append(_plugin);
}



