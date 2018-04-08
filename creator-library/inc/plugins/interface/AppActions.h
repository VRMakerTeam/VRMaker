#ifndef __APPACTIONS_H__
#define __APPACTIONS_H__

#include "Dispatcher.h"

class NotifyAction : public IAction
{
public:
	QString message;
};

class NotifyReceiver : public IReceiver
{
public:
	virtual void Receive(IAction* _action);
};

#endif
