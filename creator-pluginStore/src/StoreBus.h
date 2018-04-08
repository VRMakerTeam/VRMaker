#ifndef __UNITYBUS_H__
#define __UNITYBUS_H__

#include <functional> 
#include <QHash>

class IAction; 
class PluginStore;
class PluginStoreInternal;

template<class T>
class ReceiverHandlePrivate
{
public:
	typedef std::function<void(PluginStoreInternal*, IAction*)> handleFunc;
	struct Info
	{
		handleFunc handle;
		PluginStoreInternal* in;
	};
	QHash<PluginStore*, Info> handleMap;
};

class StoreBus
{
public:
	static void Push(PluginStore* _plugin);

	template<class _Method> inline
		static void Invoke(_Method _method, IAction* _action)
	{
		for each (PluginStore* plugin in plugins)
		{
			_method(plugin, _action);
		}
	}

	template<class _Receiver, class _Method> inline
		static void Route(PluginStore* _plugin, PluginStoreInternal* _internal, _Method _method)
	{
		ReceiverHandlePrivate<_Receiver>& receiver = getReceiverPrivate<_Receiver>();
		ReceiverHandlePrivate<_Receiver>::Info info;
		info.handle = _method;
		info.in = _internal;
		receiver.handleMap[_plugin] = info;
	}

	template<class _Receiver> inline
		static void Handle(PluginStore* _plugin, IAction* _action)
	{
		ReceiverHandlePrivate<_Receiver>& receiver = getReceiverPrivate<_Receiver>();
		ReceiverHandlePrivate<_Receiver>::Info info = receiver.handleMap[_plugin];
		info.handle(info.in, _action);
	}

private:
	template<class _Receiver>
	static ReceiverHandlePrivate<_Receiver>& getReceiverPrivate()
	{
		static ReceiverHandlePrivate<_Receiver> receiver;
		return receiver;
	}
	static QList<PluginStore*> plugins;
};

#endif
