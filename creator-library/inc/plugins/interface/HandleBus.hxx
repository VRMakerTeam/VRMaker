/********************************************************************
Copyright (c) by Zyncho

Usage:

namespace Actions
{
	class Action : public IAction
	{
	}

	class Receiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action){
			HandleBus<PluginXXXInternal>::Handle<Actions::Receiver>(_action);
		}
	}
}

class PluginXXXInternal
{
public:
	void Method(IAction* _action);
}

class PluginXXX
{
public:
	void Initialize(AppCore* _core) {
		Dispatcher::Subscribe<Actions::Receiver, Actions::Action>();
		HandleBus<PluginXXXInternal>::Route<Receiver>(pluginInternal, &PluginXXXInternal::Method);
	}
}


*********************************************************************/
#ifndef __HANDLEBUS_H__
#define __HANDLEBUS_H__

#include <functional> 
#include <QHash>

class IAction;
class IPlugin;

template<class T, class _TReceiver>
class HandleContainer
{
public:
	typedef std::function<void(T*, IAction*)> handleFunc;

	struct Handler
	{
		handleFunc handle;
		T* obj;
	};
	QList<Handler> handlers;
};

template<class T>
class HandleBus
{
public:
	template<class _TReceiver, class _Method> inline
		static void Route(T* _obj, _Method _method)
	{
		HandleContainer<T, _TReceiver>::Handler handler;
		handler.handle = _method;
		handler.obj = _obj;
		getContainer<_TReceiver>().handlers.append(handler);
	}

	template<class _TReceiver, class _Method> inline
		static void DisRoute(T* _obj, _Method _method)
	{
		HandleContainer<T, _TReceiver>::Handler handler;
		handler.handle = _method;
		handler.obj = _obj;
		int i = -1;
		bool has = false;
		for each (HandleContainer<T, _TReceiver>::Handler aHandler in getContainer<_TReceiver>().handlers)
		{
			i += 1;
			if (aHandler.obj == handler.obj && aHandler.handle.target_type() == handler.handle.target_type())
			{
				has = true;
				break;
			}
		}

		if (has)
		{
			getContainer<_TReceiver>().handlers.removeAt(i);
		}
	}


	template<class _TReceiver> inline
		static void Clean()
	{
		getContainer<_TReceiver>().handlers.clear();
	}

	template<class _TReceiver> inline
		static void Handle(IAction* _action)
	{
		for each (HandleContainer<T, _TReceiver>::Handler handler in getContainer<_TReceiver>().handlers)
		{
			handler.handle(handler.obj, _action);
		}
	}
private:
	template <typename _TReceiver>
	static HandleContainer<T, _TReceiver>& getContainer()
	{
		static HandleContainer<T, _TReceiver> container;
		return container;
	}
};

#endif
