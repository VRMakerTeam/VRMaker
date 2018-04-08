/********************************************************************
Copyright (c) by Zyncho

Usage:

Dispatcher::Subscribe<NotifyReceiver, NotifyAction>();
NotifyAction* action = Dispatcher::New<NotifyAction>();
action->message = "notify";
Dispatcher::Invoke<NotifyAction>(action);

*********************************************************************/
#ifndef __DISPATCHER_H__
#define __DISPATCHER_H__

#include <assert.h>
//#include <QDebug>
#include <QMultiHash>

template<class T>
class ReceiverCreatorPrivate
{
	friend class Dispatcher;
public:
private:
	static void* create()
	{
		return static_cast<void*>(new T());
	}
};

class ReceiverInfoPrivate
{
	friend class Dispatcher;
public:
private:
	void* (*createObject)();
};

template< class T >
class ActionInfoPrivate
{
	friend class Dispatcher;
public:
private:
	static QList<ReceiverInfoPrivate> receivers;
};

template< class T >
QList<ReceiverInfoPrivate> ActionInfoPrivate<T>::receivers;

class IAction
{
public:
};

class IReceiver
{
public:
	virtual void Receive(IAction* _action) = 0;
};

class Dispatcher
{
public:
	template<typename A>
	static A* NewAction()
	{
		return new A();
	}

	/// \brief R is Receiver, A is Action
	/// \Note only support R:A is N:1
	template<typename _TReceiver, typename _TAction>
	static void Subscribe()
	{
		ReceiverInfoPrivate receiverInfo;
		receiverInfo.createObject = ReceiverCreatorPrivate<_TReceiver>::create;
		ActionInfoPrivate<_TAction>::receivers.append(receiverInfo);
	}

	template<typename A>
	static void Invoke(A*& _action)
	{
		assert(_action);
		//qDebug() << "receivers<" << typeid(A).name() << "> address is " << &(ActionInfoPrivate<A>::receivers);

		auto itr = ActionInfoPrivate<A>::receivers.cbegin();
		for (; itr != ActionInfoPrivate<A>::receivers.cend(); ++itr)
		{
			IReceiver* receiver = static_cast<IReceiver*>(itr->createObject());
			receiver->Receive(_action);
			if (NULL != receiver)
			{
				delete receiver;
				receiver = NULL;
			}
		}

		if (NULL != _action)
		{
			delete _action;
			_action = NULL;
		}
	}
private:

};


#define NEW_ACTION(classname, variable) classname* variable = Dispatcher::NewAction<classname>();
#define INVOKE_ACTION(classname, variable) Dispatcher::Invoke<classname>(variable);
#define CAST_ACTION(classname, src, dest) \
	classname* dest = static_cast<classname*>(src);\
	assert(dest);
#endif
