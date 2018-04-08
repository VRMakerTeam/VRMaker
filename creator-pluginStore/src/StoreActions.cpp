#include <assert.h>
#include <QDebug>
#include "StoreBus.h"
#include "PluginStore/PluginStore.h"
#include "PluginStore/StoreActions.h"

void loadAgent(PluginStore* plugin, IAction* _action)
{
	StoreBus::Handle<StoreActions::LoadAgentReceiver>(plugin, _action);
}
//-----------------------------------------------------------------------
void StoreActions::LoadAgentReceiver::Receive(IAction* _action)
{
	assert(_action);
	StoreBus::Invoke(loadAgent, _action);
}

void loadZone(PluginStore* plugin, IAction* _action)
{
	StoreBus::Handle<StoreActions::LoadZoneReceiver>(plugin, _action);
}
//-----------------------------------------------------------------------
void StoreActions::LoadZoneReceiver::Receive(IAction* _action)
{
	assert(_action);
	StoreBus::Invoke(loadZone, _action);
}
