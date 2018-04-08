#ifndef __BUCKETBUS_H__
#define __BUCKETBUS_H__

#include <functional> 
#include <QHash>

class IAction; 
class PluginBucket;
class PluginBucketInternal;

template<class T>
class ReceiverHandlePrivate
{
public:
	typedef std::function<void(PluginBucketInternal*, IAction*)> handleFunc;
	struct Info
	{
		handleFunc handle;
		PluginBucketInternal* in;
	};
	QHash<PluginBucket*, Info> handleMap;
};

class BucketBus
{
public:
	static void Push(PluginBucket* _plugin);

	template<class _Method> inline
		static void Invoke(_Method _method, IAction* _action)
	{
		for each (PluginBucket* plugin in plugins)
		{
			_method(plugin, _action);
		}
	}

	template<class _Receiver, class _Method> inline
		static void Route(PluginBucket* _plugin, PluginBucketInternal* _internal, _Method _method)
	{
		ReceiverHandlePrivate<_Receiver>& receiver = getReceiverPrivate<_Receiver>();
		ReceiverHandlePrivate<_Receiver>::Info info;
		info.handle = _method;
		info.in = _internal;
		receiver.handleMap[_plugin] = info;
	}

	template<class _Receiver> inline
		static void Handle(PluginBucket* _plugin, IAction* _action)
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
	static QList<PluginBucket*> plugins;
};

#endif
