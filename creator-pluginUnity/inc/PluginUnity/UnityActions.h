#ifndef __UNITYACTIONS_H__
#define __UNITYACTIONS_H__

#include "Dispatcher.h"
#include <functional>

namespace UnityActions
{
	namespace API
	{
		//////////////////////////////////////////////////////////////////////////
		// Clean 
		//////////////////////////////////////////////////////////////////////////
		class CleanAction : public IAction
		{
		public:
		};

		class CleanReceiver : public IReceiver
		{
		public:
			virtual void Receive(IAction* _action);
		};

		//////////////////////////////////////////////////////////////////////////
		// Import
		//////////////////////////////////////////////////////////////////////////
		class ImportAction : public IAction
		{
		public:
			QString source;
		};

		class ImportReceiver : public IReceiver
		{
		public:
			virtual void Receive(IAction* _action);
		};

		//////////////////////////////////////////////////////////////////////////
		// New Trigger
		//////////////////////////////////////////////////////////////////////////
		class NewTriggerAction : public IAction
		{
		public:
			QString uuid;
			QString type;
		};

		class NewTriggerReceiver : public IReceiver
		{
		public:
			virtual void Receive(IAction* _action);
		};

		//////////////////////////////////////////////////////////////////////////
		// Delete Trigger
		//////////////////////////////////////////////////////////////////////////
		class DeleteTriggerAction : public IAction
		{
		public:
			QString uuid;
		};

		class DeleteTriggerReceiver : public IReceiver
		{
		public:
			virtual void Receive(IAction* _action);
		};

		//////////////////////////////////////////////////////////////////////////
		// Modify Trigger
		//////////////////////////////////////////////////////////////////////////
		class ModifyTriggerAction : public IAction
		{
		public:
			QString uuid;
			QString type;
			QString alias;
		};

		class ModifyTriggerReceiver : public IReceiver
		{
		public:
			virtual void Receive(IAction* _action);
		};

		//////////////////////////////////////////////////////////////////////////
		// Modify Gaze Icon
		//////////////////////////////////////////////////////////////////////////
		class ModifyGazeIconAction : public IAction
		{
		public:
			QString uuid;
			int icon;
		};

		class ModifyGazeIconReceiver : public IReceiver
		{
		public:
			virtual void Receive(IAction* _action);
		};


		//////////////////////////////////////////////////////////////////////////
		// Modify Gaze Color
		//////////////////////////////////////////////////////////////////////////
		class ModifyGazeColorAction : public IAction
		{
		public:
			QString uuid;
			int r;
			int g;
			int b;
			int a;
		};

		class ModifyGazeColorReceiver : public IReceiver
		{
		public:
			virtual void Receive(IAction* _action);
		};


		void DispatchAll();
	}
	


	//////////////////////////////////////////////////////////////////////////
	// send message to unity
	//////////////////////////////////////////////////////////////////////////
	class WSMessageToUnityAction : public IAction
	{
	public:
		QString method;
		QString activity;
		QString param;
	};
	class WSMessageToUnityReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	//////////////////////////////////////////////////////////////////////////
	// receive message from unity
	//////////////////////////////////////////////////////////////////////////
	class WSMessageFromUnityAction : public IAction
	{
	public:
		QString method;
		QString activity;
		QString param;
	};


	//////////////////////////////////////////////////////////////////////////
	// Run Player
	//////////////////////////////////////////////////////////////////////////
	class RunPlayerAction : public IAction
	{
	public:
	};

	class RunPlayerReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};
}

#endif
