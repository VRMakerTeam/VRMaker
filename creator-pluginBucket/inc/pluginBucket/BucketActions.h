#ifndef __BUCKETACTIONS_H__
#define __BUCKETACTIONS_H__

#include <functional>
#include "Dispatcher.h"

class QWidget;

namespace BucketActions
{
	class PopupPickerAction : public IAction
	{
	public:
		QString slot;
		QStringList filter;
		std::function<void(const QString& _guid)> onOK;
	};

	class PopupPickerReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	// Filter Assets with a group
	//
	//////////////////////////////////////////////////////////////////////////
	class FilterAssetsGroupAction : public IAction
	{
	public:
		QString group;
	};

	class FilterAssetsGroupReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	// Filter Tags with a group
	//
	//////////////////////////////////////////////////////////////////////////
	class FilterTagsGroupAction : public IAction
	{
	public:
		QString group;
	};

	class FilterTagsGroupReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	// Filter Assets with tags
	//
	//////////////////////////////////////////////////////////////////////////
	class FilterAssetsGroupTagsAction : public IAction
	{
	public:
		QString group;
		QStringList tags;
	};

	class FilterAssetsGroupTagsReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	// Filter UGC Audio
	//
	//////////////////////////////////////////////////////////////////////////
	class FilterUGCAudioAction : public IAction
	{
	};

	class FilterUGCAudioReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	// Filter UGC Image
	//
	//////////////////////////////////////////////////////////////////////////
	class FilterUGCImageAction : public IAction
	{
	};

	class FilterUGCImageReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};


	//////////////////////////////////////////////////////////////////////////
	//
	// Start Record UGC Audio
	//
	//////////////////////////////////////////////////////////////////////////
	class StartRecordUGCAudioAction : public IAction
	{
	public:
		QString uuid;
	};

	class StartRecordUGCAudioReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	// Stop Record UGC Audio
	//
	//////////////////////////////////////////////////////////////////////////
	class StopRecordUGCAudioAction : public IAction
	{
	public:
		std::function<void(const QString& _guid)> onOK;
	};

	class StopRecordUGCAudioReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	// Play UGC audio
	//
	//////////////////////////////////////////////////////////////////////////
	class StartPlayUGCAudioAction : public IAction
	{
	public:
		QString uuid;
	};

	class StartPlayUGCAudioReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	// Stop UGC audio
	//
	//////////////////////////////////////////////////////////////////////////
	class StopPlayUGCAudioAction : public IAction
	{
	public:
	};

	class StopPlayUGCAudioReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	// Import Audio
	//
	//////////////////////////////////////////////////////////////////////////
	class ImportAudioAction : public IAction
	{
	public:
	};

	class ImportAudioReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	//
	// Import Image
	//
	//////////////////////////////////////////////////////////////////////////
	class ImportImageAction : public IAction
	{
	public:
	};

	class ImportImageReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};
}

#endif
