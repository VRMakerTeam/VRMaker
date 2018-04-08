#ifndef __UNITYACTIONS_H__
#define __UNITYACTIONS_H__

#include "Dispatcher.h"

namespace StoreActions
{
	class LoadAgentAction : public IAction
	{
	public:
		QString package;
		QString file;
		QString uuid;
	};

	class LoadAgentReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	class LoadZoneAction : public IAction
	{
	public:
		QString package;
		QString file;
	};

	class LoadZoneReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};
}

#endif
