#include <QFile>
#include <QDir>
#include <QUuid>
#include <QStandardPaths>
#include <QDataStream>
#include <QProcess>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "MemDB.h"
#include "MemDBUtil.h"
#include "PathUtil.h"
#include "Settings.h"
#include "HandleBus.hxx"
#include "PluginBean/BeanModel.h"
#include "NotifyBox.h"
#include "BeanNotifyContent.h"
#include "PluginBeanLogger.h"
#include "BeanCompositeWall.h"
#include "BeanCompositePanel.h"
#include "BeanCompositeCard.h"
#include "BeanCompositeToolbar.h"
#include "BeanCompositePicker.h"
#include "PluginBean/PluginBean.h"
#include "PluginUnity/UnityActions.h"
#include "PluginBean/BeanActions.h"

QString gSelectedBeanGUID = "";

//-----------------------------------------------------------------------
void BeanActions::ParseDiskReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");

	HandleBus<Bean::WallComponent>::Handle<BeanActions::ParseDiskReceiver>(_action);
}

//-----------------------------------------------------------------------
void BeanActions::NewBeanReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");

	HandleBus<Bean::WallComponent>::Handle<BeanActions::NewBeanReceiver>(_action);
}

//-----------------------------------------------------------------------
void BeanActions::DeleteBeanReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");

	HandleBus<Bean::WallComponent>::Handle<BeanActions::DeleteBeanReceiver>(_action);

}

//-----------------------------------------------------------------------
void BeanActions::ModifyBeanReceiver::Receive(IAction* _action)
{
	HandleBus<Bean::CardComponent>::Handle<BeanActions::ModifyBeanReceiver>(_action);
}

//-----------------------------------------------------------------------
void BeanActions::PlayBeanReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");

	MemDB* memDB = BeanModel::Runtime::GetMemDB();
	QString beanGUID = BeanModel::Runtime::KV::Query::bean_alive_guid(memDB);

	auto onFinish = [&](const QByteArray& _bytes) {
		//NEW_ACTION(UnityActions::RunPlayerAction, action);
		//INVOKE_ACTION(UnityActions::RunPlayerAction, action);

		
		QString filepath = QString("%1/latest.vx").arg(PC_DEVICE_PERSISTENT_PATH);
		QFile file(filepath);
		if (file.open(QIODevice::WriteOnly))
		{
			file.write(_bytes);
			file.flush();
		}
		file.close();
		QProcess* process = new QProcess();
		process->execute("player/reviewer_pc/reviewer_pc");
		
	};
	NEW_ACTION(BeanActions::CompileBeanAction, action);
	action->guid = beanGUID;
	action->onFinish = std::bind(onFinish, std::placeholders::_1);
	INVOKE_ACTION(BeanActions::CompileBeanAction, action);
}

//-----------------------------------------------------------------------
void BeanActions::PublishBeanReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");
	HandleBus<Bean::PanelComponent>::Handle<BeanActions::PublishBeanReceiver>(_action);
}

//-----------------------------------------------------------------------
void BeanActions::InstallBeanReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");
	HandleBus<Bean::PanelComponent>::Handle<BeanActions::InstallBeanReceiver>(_action);
}

//-----------------------------------------------------------------------
void BeanActions::ExportBeanReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");

	CAST_ACTION(BeanActions::ExportBeanAction, _action, action);
	QString beanUUID = action->guid;

	QString outputpath = QFileDialog::getExistingDirectory(NULL, "Export", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
	QString assetPath = QString("%1/%2.asset").arg(outputpath).arg(beanUUID);
	QString zipPath = QString("%1/%2.zip").arg(outputpath).arg(beanUUID);

	NotifyUtil::Popup(BeanNotifyContent::GetRoot());

	auto clearTmp = []() {
		QDir tmp(QString("%1/tmp").arg(PathUtil::PackageLocation()));
		if (tmp.exists())
		{
			tmp.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
			QFileInfoList fileList = tmp.entryInfoList();
			foreach(QFileInfo fi, fileList)
			{
				if (fi.isFile())
					fi.dir().remove(fi.fileName());
			}
			tmp.rmpath(tmp.absolutePath());
		}
		tmp.mkpath(tmp.absolutePath());
	};

	auto saveVX = [](const QString& _path, const QByteArray& _data) {
		QFile vxfile(_path);
		if (vxfile.open(QIODevice::WriteOnly))
		{
			vxfile.write(_data);
			vxfile.flush();
		}
		vxfile.close();
	};

	auto onCompileFinish = [=](const QByteArray& _data) {
		QDir pkgLocation(PathUtil::PackageLocation());
		QString tmpLocation = pkgLocation.absoluteFilePath("tmp");

		//
		//  打包VPlayer使用的asset
		//

		//清空文件夹
		clearTmp();
		//存储vx
		saveVX(QString("%1/latest.vx").arg(tmpLocation), _data);
		//打包vx
		static QProcess processAsset;
		QString cmd = QString("7z.exe a %1 %2/*").arg(assetPath).arg(tmpLocation);
		processAsset.start(cmd);

		//
		// 打包Idealens教师端使用的zip
		//

		//清空文件夹
		clearTmp();

		QString srcLocation = QString("%1/manifests").arg(PathUtil::PackageLocation());

		QTimer* timer = new QTimer();
		timer->setInterval(1000);
		QObject::connect(timer, &QTimer::timeout, [=] {
			MemDB* memDB = BeanModel::Persistent::GetMemDB(beanUUID);

			QList<QVariant> slates = memDB->SetsMembers("screenplay_slates_guids");
			QJsonArray arySlate;
			int idx = 0;
			foreach(QVariant slate, slates)
			{
				QJsonObject objSlate;
				objSlate["id"] = idx;
				objSlate["name"] = memDB->KVGet(QString("screenplay_slate_guid_1_alias:[%1]").arg(slate.toString())).toString();

				QString file = memDB->KVGet(QString("screenplay_slate_guid_1_preload_skybox_file:[%1]").arg(slate.toString())).toString();
				QString package = memDB->KVGet(QString("screenplay_slate_guid_1_preload_skybox_package:[%1]").arg(slate.toString())).toString();
				QString srcFile = QString("%1/%2@%3.png").arg(srcLocation).arg(package).arg(file);
				QString destFile = QString("%1/%2.png").arg(tmpLocation).arg(idx);
				QString tmpFile = QString("%1/%2_.png").arg(tmpLocation).arg(idx);
				QString srcFileSmall = QString("%1/%2@%3.png").arg(srcLocation).arg(package).arg(file);
				QString tmpFileSmall = QString("%1/%2_small_.png").arg(tmpLocation).arg(idx);
				QString destFileSmall = QString("%1/%2_small.png").arg(tmpLocation).arg(idx);

				//generate thumb picture
				{
					QImage image(srcFileSmall);
					QImage newImage = image.copy(0, 7, 240, 120);
					newImage.save(tmpFileSmall);

					QImageReader reader;
					reader.setFileName(tmpFileSmall);
					if (!reader.canRead())
						continue;
					reader.setScaledSize(QSize(192, 96));

					QImageWriter writer;
					writer.setFileName(destFileSmall);
					if (!writer.canWrite())
						continue;
					writer.write(reader.read());
					QFile::remove(tmpFileSmall);
				}

				//generate picture
				{
					QImage image(srcFile);
					QImage newImage = image.copy(0, 7, 240, 120);
					newImage.save(tmpFile);

					QImageReader reader;
					reader.setFileName(tmpFile);
					if (!reader.canRead())
						continue;
					reader.setScaledSize(QSize(960, 336));
					QImageWriter writer;
					writer.setFileName(destFile);
					if (!writer.canWrite())
						continue;
					writer.write(reader.read());
					QFile::remove(tmpFile);
				}

				arySlate.append(objSlate);
				idx++;
			}//foreach

			QJsonDocument jsonDoc;
			jsonDoc.setArray(arySlate);
			QByteArray jsonData = jsonDoc.toJson();
			QFile json(tmpLocation + "/json.txt");
			if (json.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				json.write(jsonData);
			}
			json.flush();
			json.close();

			static QProcess processZip;
			QString cmd = QString("7z.exe a %1 %2/*").arg(zipPath).arg(tmpLocation);
			processZip.start(cmd);

			BeanNotifyContent::SwitchOK();

			NotifyUtil::ActivateOKButton();
			timer->stop();
			delete timer;
		});//QObject::connect

		timer->start();
	};//onCompileFinish

	NEW_ACTION(BeanActions::CompileBeanAction, compileAction);
	compileAction->guid = beanUUID;
	compileAction->onFinish = onCompileFinish;
	INVOKE_ACTION(BeanActions::CompileBeanAction, compileAction);


	
}

//-----------------------------------------------------------------------
void BeanActions::SelectBeanReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");

	CAST_ACTION(SelectBeanAction, _action, action);
	//update model layer
	MemDB* memDB = BeanModel::Runtime::GetMemDB();
	BeanModel::Runtime::KV::Update::bean_alive_guid(memDB, action->uuid);

	HandleBus<Bean::WallComponent>::Handle<BeanActions::SelectBeanReceiver>(_action);
}

//-----------------------------------------------------------------------
void BeanActions::ToggleBeanEditorReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");

	HandleBus<Bean::ToolbarComponent>::Handle<BeanActions::ToggleBeanEditorReceiver>(_action);
}

//-----------------------------------------------------------------------
void BeanActions::EditBeanReceiver::Receive(IAction* _action)
{
	CAST_ACTION(BeanActions::EditBeanAction, _action, action);
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");
	//重新加载前清空Unity数据
	{
		NEW_ACTION(UnityActions::API::CleanAction, actionMessageToUnity);
		INVOKE_ACTION(UnityActions::API::CleanAction, actionMessageToUnity);
	}
	//刷新Unity
	{
		//WebSocket传递大数据时效率较低，所以先将内容写入到editor_web的stream目录下
		QDir dir(MemDBUtil::GetDataPath());
		QString srcPath = dir.absoluteFilePath(QString("bucket.bean.guid.[%1].vcb").arg(action->guid));
		QString distPath = QString("%1/latest.vcb").arg(EMBEDDED_STREAM_PATH);
		QFile::remove(distPath);
		QFile::copy(srcPath, distPath);

		NEW_ACTION(UnityActions::API::ImportAction, importAction);
		importAction->source = ".vcb";
		INVOKE_ACTION(UnityActions::API::ImportAction, importAction);
	}
	HandleBus<PluginBeanInternal>::Handle<BeanActions::EditBeanReceiver>(_action);
}

//-----------------------------------------------------------------------
void BeanActions::OpenPageReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");
	HandleBus<Bean::PanelComponent>::Handle<BeanActions::OpenPageReceiver>(_action);
}

//-----------------------------------------------------------------------
void BeanActions::RegisterSourceReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");
	HandleBus<Bean::PickerComponent>::Handle<BeanActions::RegisterSourceReceiver>(_action);
}
//-----------------------------------------------------------------------
void BeanActions::SelectSourceReceiver::Receive(IAction* _action)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "receive");

	HandleBus<Bean::PickerComponent>::Handle<BeanActions::SelectSourceReceiver>(_action);
}

//-----------------------------------------------------------------------
void BeanActions::DumpBeanReceiver::Receive(IAction* _action)
{
	CAST_ACTION(BeanActions::DumpBeanAction, _action, action);
	static QString dumpPath = PathUtil::AppDataLocation();
	QDir dir(dumpPath);
	if (!dir.exists("bean_dump"))
		dir.mkdir("bean_dump");
	dir.cd("bean_dump");
	
	QString filepath = dir.absoluteFilePath(action->filename.append(".json"));
	QFile file(filepath);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		out << QString::fromUtf8(action->bytes);
		out.flush();
	}
	
	file.close();
}
