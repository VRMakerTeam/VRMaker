#ifndef __BEANACTIONS_H__
#define __BEANACTIONS_H__

#include <functional>
#include "Dispatcher.h"

namespace BeanActions
{
	/*-----------------------------------------------------
	Register archetype
	-----------------------------------------------------*/
	class RegisterSourceAction : public IAction
	{
	public:
		QString guid;
		QString alias;
		QString category;
		QStringList extends;
	};
	class RegisterSourceReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};
	/*-----------------------------------------------------
		Parse Disk
	-----------------------------------------------------*/
	class ParseDiskAction : public IAction
	{

	};
	class ParseDiskReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	/*-----------------------------------------------------
		Open Dashboard
	-----------------------------------------------------*/
	class OpenPageAction : public IAction
	{
	public:
		enum Page
		{
			DASHBOARD,
			ARCHETYPE,
			CLONE
		};
		Page page;
	};
	class OpenPageReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	/*-----------------------------------------------------
		New Bean
		use clone or archetype to cerate a new bean
	-----------------------------------------------------*/
	class NewBeanAction : public IAction
	{
	public:
		QString guid;
		QString alias;
	};
	class NewBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	/*-----------------------------------------------------
	On New Bean
	当新建Bean成功时
	-----------------------------------------------------*/
	class OnNewBeanAction : public IAction
	{
	public:
		QString guid;
	};

	/*-----------------------------------------------------
	Delete Bean
	-----------------------------------------------------*/
	class DeleteBeanAction : public IAction
	{
	public:
		QString guid;
	};

	class DeleteBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	/*-----------------------------------------------------
	On Delete Bean
	当删除Bean成功时
	-----------------------------------------------------*/
	class OnDeleteBeanAction : public IAction
	{
	public:
		QString guid;
	};

	/*-----------------------------------------------------
	Modify Bean
	-----------------------------------------------------*/
	class ModifyBeanAction : public IAction
	{
	public:
		QString guid;
		QString alias;
	};

	class ModifyBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};


	/*-----------------------------------------------------
	Edit Bean
	-----------------------------------------------------*/
	class EditBeanAction : public IAction
	{
	public:
		QString guid;
	};
	class EditBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	/*-----------------------------------------------------
	Compile Bean
	encode the bean to json bytes that player can read
	Note: Receiver will implement by other plugin
	-----------------------------------------------------*/
	class CompileBeanAction : public IAction
	{
	public:
		QString guid;

		std::function<void(const QByteArray& _bytes)> onFinish;
	};

	/*-----------------------------------------------------
	Dump Bean
	save .json file
	-----------------------------------------------------*/
	class DumpBeanAction : public IAction
	{
	public:
		QString filename;
		QByteArray bytes;
	};
	class DumpBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	/*-----------------------------------------------------
	Play Bean
	copy .json file to player directory
	-----------------------------------------------------*/
	class PlayBeanAction : public IAction
	{
	public:
		QString guid;
	};
	class PlayBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	/*-----------------------------------------------------
	Publish Bean
	upload the .json file
	-----------------------------------------------------*/
	class PublishBeanAction : public IAction
	{
	public:
		QString guid;
	};
	class PublishBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	/*-----------------------------------------------------
	Install Bean
	copy the .json file to device
	-----------------------------------------------------*/
	class InstallBeanAction : public IAction
	{
	public:
		QString guid;
	};
	class InstallBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	/*-----------------------------------------------------
	Export Bean
	.zip file
	-----------------------------------------------------*/
	class ExportBeanAction : public IAction
	{
	public:
		QString guid;
	};
	class ExportBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	
	/*-----------------------------------------------------
	Select Bean
	-----------------------------------------------------*/
	class SelectBeanAction : public IAction
	{
	public:
		QString uuid;
	};
	class SelectBeanReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	/*-----------------------------------------------------
	Toggle bean editor
	-----------------------------------------------------*/
	class ToggleBeanEditorAction : public IAction
	{
	public:
		bool visible;
	};
	class ToggleBeanEditorReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

	/*-----------------------------------------------------
	Select Source
	-----------------------------------------------------*/
	class SelectSourceAction : public IAction
	{
	public:
		QString uuid;
		bool toggled;
	};
	class SelectSourceReceiver : public IReceiver
	{
	public:
		virtual void Receive(IAction* _action);
	};

}

#endif
