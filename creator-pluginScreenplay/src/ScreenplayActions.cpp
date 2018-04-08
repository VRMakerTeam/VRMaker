#include <QDebug>
#include <QUuid>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "PluginScreenplayLogger.h"
#include "HandleBus.hxx"
#include "MemDB.h"
#include "PathUtil.h"
#include "HttpUtil.h"
#include "Settings.h"
#include "PluginDiagram/DiagramModel.h"
#include "PluginBean/BeanModel.h"
#include "PluginBean/BeanActions.h"
#include "PluginUnity/UnityActions.h"
#include "PluginUnity/UnityMessages.h"
#include "PluginDiagram/DiagramActions.h"
#include "PluginScreenplay/pluginScreenplay.h"
#include "pluginBucket/BucketUtils.h"
#include "ScreenplayCompositeSlate.h"
#include "ScreenplayCompositeSlateCard.h"
#include "ScreenplayCompositePreload.h"
#include "ScreenplayCompositePlayer.h"
#include "ScreenplayCompositePanel.h"
#include "PluginScreenplay/ScreenplayModel.h"
#include "PluginScreenplay/ScreenplayActions.h"

namespace ScreenplayActions
{
	QList<QString> sortActions(QList<QString> _actions)
	{
		MemDB* memDB = DiagramModel::Persistent::GetMemDB();
		QList<QString> sortedActions = _actions;
		qSort(sortedActions.begin(), sortedActions.end(), [&](const QString& _left, const QString& _right) {
			int index_left = DiagramModel::Persistent::KV::Query::diagram_block_1_index(memDB, _left).toInt();
			int index_right = DiagramModel::Persistent::KV::Query::diagram_block_1_index(memDB, _right).toInt();
			return index_left < index_right;
		});
		return sortedActions;
	}

	QByteArray compileBean(const QString& _guid)
	{
		QStringList ugcFiles;
		QStringList cacheFiles;

		MemDB* memDB = BeanModel::Persistent::GetMemDB(_guid);
		assert(memDB);
		//loop slates
		memDB = ScreenplayModel::Persistent::GetMemDB();
		QList<QString> slates = ScreenplayModel::Persistent::Sets::Query::screenplay_slates_guids(memDB);
		QJsonArray arySlates;
		for each (QString slate in slates)
		{
			QString slateAlias = ScreenplayModel::Persistent::KV::Query::screenplay_slate_guid_1_alias(memDB, slate);
			QJsonArray aryPreloads;
			// encode preload skybox
			QString fileUUID = ScreenplayModel::Persistent::KV::Query::screenplay_slate_guid_1_preload_skybox_file(memDB, slate);
			QString packageGUID = ScreenplayModel::Persistent::KV::Query::screenplay_slate_guid_1_preload_skybox_package(memDB, slate);
			QJsonObject objSkybox;
			objSkybox.insert("group", "skybox");
			objSkybox.insert("package", packageGUID);
			objSkybox.insert("file", fileUUID);
			objSkybox.insert("guid", "");
			aryPreloads.append(objSkybox);

			if (fileUUID.contains(".jpg") || fileUUID.contains(".png"))
				ugcFiles << fileUUID;

			// encode preload agents
			QList<QString> preloads = ScreenplayModel::Persistent::Sets::Query::screenplay_slate_guid_1_preloads_guids(memDB, slate);
			memDB = DiagramModel::Persistent::GetMemDB();
			for each (QString preload in preloads)
			{
				QString fileGUID = ScreenplayModel::Persistent::KV::Query::screenplay_slate_guid_1_preload_guid_2_asset_file(memDB, slate, preload);
				QString packageGUID = ScreenplayModel::Persistent::KV::Query::screenplay_slate_guid_1_preload_guid_2_asset_package(memDB, slate, preload);
				QString group = BucketUtils::QueryFileGroup(fileGUID);
				LOGX_DEBUG(XC_PluginScreenplay, XL_MEDIMU_PluginScreenplay, QString("%1").arg(preload));
				double px = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_px(memDB, preload);
				LOGX_DEBUG(XC_PluginScreenplay, XL_MEDIMU_PluginScreenplay, QString("%1").arg(px));
				double py = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_py(memDB, preload);
				double pz = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_pz(memDB, preload);
				double rx = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_rx(memDB, preload);
				double ry = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_ry(memDB, preload);
				double rz = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_rz(memDB, preload);
				double sx = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_sx(memDB, preload);
				double sy = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_sy(memDB, preload);
				double sz = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_sz(memDB, preload);
				bool gaze = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_gaze(memDB, preload).toBool();
				QString gazeAlias = ScreenplayModel::Persistent::KV::Query::screenplay_preload_guid_1_gaze_alias(memDB, preload).toString();
				QJsonObject objPreload;
				objPreload.insert("group", group);
				objPreload.insert("package", packageGUID);
				objPreload.insert("file", fileGUID);
				objPreload.insert("guid", preload);
				objPreload.insert("px", px);
				objPreload.insert("py", py);
				objPreload.insert("pz", pz);
				objPreload.insert("rx", rx);
				objPreload.insert("ry", ry);
				objPreload.insert("rz", rz);
				objPreload.insert("sx", sx);
				objPreload.insert("sy", sy);
				objPreload.insert("sz", sz);
				objPreload.insert("gaze", gaze);
				objPreload.insert("gaze.alias", gazeAlias);
				aryPreloads.append(objPreload);
			}

			// encode triggers
			QList<QString> triggers = ScreenplayModel::Persistent::Sets::Query::screenplay_slate_guid_1_triggers_guids(memDB, slate);
			memDB = DiagramModel::Persistent::GetMemDB();
			QJsonArray aryTriggers;
			for each (QString trigger in triggers)
			{
				QString alias = DiagramModel::Persistent::KV::Query::diagram_trigger_1_alias(memDB, trigger);
				double px = DiagramModel::Persistent::KV::Query::diagram_trigger_1_px(memDB, trigger);
				double py = DiagramModel::Persistent::KV::Query::diagram_trigger_1_py(memDB, trigger);
				double pz = DiagramModel::Persistent::KV::Query::diagram_trigger_1_pz(memDB, trigger);
				double rx = DiagramModel::Persistent::KV::Query::diagram_trigger_1_rx(memDB, trigger);
				double ry = DiagramModel::Persistent::KV::Query::diagram_trigger_1_ry(memDB, trigger);
				double rz = DiagramModel::Persistent::KV::Query::diagram_trigger_1_rz(memDB, trigger);
				int icon = DiagramModel::Persistent::KV::Query::diagram_gaze_1_icon(memDB, trigger).toInt();
				double colorR = DiagramModel::Persistent::KV::Query::diagram_gaze_1_color_r(memDB, trigger).toInt();
				double colorG = DiagramModel::Persistent::KV::Query::diagram_gaze_1_color_g(memDB, trigger).toInt();
				double colorB = DiagramModel::Persistent::KV::Query::diagram_gaze_1_color_b(memDB, trigger).toInt();
				double colorA = DiagramModel::Persistent::KV::Query::diagram_gaze_1_color_a(memDB, trigger).toInt();
				QJsonObject objTrigger;
				objTrigger.insert("uuid", trigger);
				objTrigger.insert("alias", alias);
				objTrigger.insert("icon", icon);
				objTrigger.insert("px", px);
				objTrigger.insert("py", py);
				objTrigger.insert("pz", pz);
				objTrigger.insert("rx", rx);
				objTrigger.insert("ry", ry);
				objTrigger.insert("rz", rz);
				objTrigger.insert("color.r", colorR);
				objTrigger.insert("color.g", colorG);
				objTrigger.insert("color.b", colorB);
				objTrigger.insert("color.a", colorA);
				aryTriggers.append(objTrigger);
			}

			//encode actions
			QList<QString> pipes = ScreenplayModel::Persistent::Sets::Query::screenplay_slate_guid_1_pipes_guids(memDB, slate);
			memDB = DiagramModel::Persistent::GetMemDB();
			QJsonArray aryPipes;
			for each (QString pipe in pipes)
			{
				QJsonArray aryActions;
				QList<QString> blcoksGUIDS = DiagramModel::Persistent::Sets::Query::diagram_pipe_guid_1_blocks_guids(memDB, pipe);
				//按 index 排序
				blcoksGUIDS = sortActions(blcoksGUIDS);

				for each (QString block in blcoksGUIDS)
				{
					QString group = DiagramModel::Persistent::KV::Query::diagram_block_1_group(memDB, block);
					QString tpl = DiagramModel::Persistent::KV::Query::diagram_block_1_template(memDB, block);
					QJsonArray aryParams;
					QJsonArray aryExtras;
					QList<QString> fields = DiagramModel::Persistent::Hash::List::diagram_block_guid_1_fields(memDB, block);
					for each (QString field in fields)
					{
						field = field.toLower();
						QVariant value = DiagramModel::Persistent::Hash::Query::diagram_block_guid_1_fields(memDB, block, field);
						if (field.compare("asset") == 0)
						{
							QString fileGUID = value.toString();
							QString package = BucketUtils::QueryFilePackage(fileGUID);
							QJsonObject objPackage;
							objPackage.insert("key", "package");
							objPackage.insert("value", package);
							aryParams.append(objPackage);
							QJsonObject objFile;
							objFile.insert("key", "file");
							objFile.insert("value", fileGUID);
							aryParams.append(objFile);
						}
						else
						{
							QJsonObject objParam;
							objParam.insert("key", field);
							objParam.insert("value", QJsonValue::fromVariant(value));
							aryParams.append(objParam);

							if (field.contains("ugc_"))
								ugcFiles.append(value.toString());
							if (field.contains("cache_"))
								cacheFiles.append(value.toString());
						}
					}
					QJsonObject objAction;
					objAction.insert("guid", block);
					objAction.insert("params", aryParams);
					objAction.insert("action", QString("%1:%2").arg(group).arg(tpl));
					aryActions.append(objAction);
				}
				QJsonObject objPipe;
				objPipe.insert("guid", pipe);
				objPipe.insert("actions", aryActions);
				aryPipes.append(objPipe);
			}

			QJsonObject objSlate;
			objSlate.insert("preloads", aryPreloads);
			objSlate.insert("triggers", aryTriggers);
			objSlate.insert("pipes", aryPipes);
			objSlate.insert("alias", slateAlias);
			objSlate.insert("guid", slate);
			arySlates.append(objSlate);
		}

		QJsonObject root;
		root.insert("slates", arySlates);
		QJsonDocument jsonDoc;
		jsonDoc.setObject(root);
		QByteArray& bean_bytes = jsonDoc.toJson();

		auto IntToBytes = [](int _value)->QByteArray
		{
			QByteArray bytes;
			bytes.resize(4);
			bytes[0] = (uchar)(0x000000ff & _value);
			bytes[1] = (uchar)((0x0000ff00 & _value) >> 8);
			bytes[2] = (uchar)((0x00ff0000 & _value) >> 16);
			bytes[3] = (uchar)((0xff000000 & _value) >> 24);
			return bytes;
		};

		// pkg format is 
		// | size_bean | data_bean | count_ugcfile |  size_ugcfile1_name |  ugcfile1_name |   size_ugcfile1+data | ugcfile1_data | ... 
		// |  int      | bytes     |   int         |     int             |    bytes       |      int             |     bytes     | ...
		QByteArray vxPKG;
		// size_bean
		vxPKG.append(IntToBytes(bean_bytes.length()));
		// data_bean
		vxPKG.append(bean_bytes);
		// count_ugcfile
		vxPKG.append(IntToBytes(ugcFiles.length()));
		for (int i = 0; i < ugcFiles.length(); ++i)
		{
			// size_ugcfile_name
			vxPKG.append(IntToBytes(ugcFiles.at(i).length()));
			// ugcfile_name
			vxPKG.append(ugcFiles.at(i).toUtf8());

			QString filepath = PathUtil::PackageLocation() + "/ugc/" + ugcFiles.at(i);
			QFile file(filepath);
			if (file.open(QIODevice::ReadOnly))
			{
				QByteArray fileData = file.readAll();
				file.close();
				vxPKG.append(IntToBytes(fileData.length()));
				vxPKG.append(fileData);
			}
			else
			{
				vxPKG.append(IntToBytes(0));
			}
		}

		// count_cachefile
		vxPKG.append(IntToBytes(cacheFiles.length()));
		for (int i = 0; i < cacheFiles.length(); ++i)
		{
			// size_ugcfile_name
			vxPKG.append(IntToBytes(cacheFiles.at(i).length()));
			// ugcfile_name
			vxPKG.append(cacheFiles.at(i).toUtf8());

			QString filepath = "cache/" + cacheFiles.at(i);
			QFile file(filepath);
			if (file.open(QIODevice::ReadOnly))
			{
				QByteArray fileData = file.readAll();
				file.close();
				vxPKG.append(IntToBytes(fileData.length()));
				vxPKG.append(fileData);
			}
			else
			{
				vxPKG.append(IntToBytes(0));
			}
		}
		return vxPKG;
	}

	}

//-----------------------------------------------------------------------
void ScreenplayActions::NewSlateReceiver::Receive(IAction* _action)
{
	HandleBus<Screenplay::SlateComponent>::Handle<ScreenplayActions::NewSlateReceiver>(_action);
}

//-----------------------------------------------------------------------
void ScreenplayActions::DeleteSlateReceiver::Receive(IAction* _action)
{
	//Slate will talk to unity and hide the delete button
	HandleBus<Screenplay::SlateComponent>::Handle<ScreenplayActions::DeleteSlateReceiver>(_action);
	//Panel will hide the preloads and diagrams panel
	HandleBus<Screenplay::PanelComponent>::Handle<ScreenplayActions::DeleteSlateReceiver>(_action);
	//HandleBus<Astronomy::PreloadComponent>::Handle<AstronomyActions::DeleteSlateReceiver>(_action);

	NEW_ACTION(DiagramActions::ToggleFlowToolbarAction, action);
	action->visible = false;
	INVOKE_ACTION(DiagramActions::ToggleFlowToolbarAction, action);

	// Clean diagram
	{
		//NEW_ACTION(DiagramActions::CleanAction, action);
		//INVOKE_ACTION(DiagramActions::CleanAction, action);
	}
}

void ScreenplayActions::RenameSlateReceiver::Receive(IAction* _action)
{
	HandleBus<Screenplay::SlateCardComponent>::Handle<ScreenplayActions::RenameSlateReceiver>(_action);
}


void ScreenplayActions::SwitchInspectorToAgentReceiver::Receive(IAction* _action)
{
	HandleBus<Screenplay::PanelComponent>::Handle<ScreenplayActions::SwitchInspectorToAgentReceiver>(_action);
}

void ScreenplayActions::SwitchInspectorToDiagramReceiver::Receive(IAction* _action)
{
	HandleBus<Screenplay::PanelComponent>::Handle<ScreenplayActions::SwitchInspectorToDiagramReceiver>(_action);
}

//-----------------------------------------------------------------------
void ScreenplayActions::SelectSlateReceiver::Receive(IAction* _action)
{
	//Slate will talk to unity and show the delete button
	HandleBus<Screenplay::SlateComponent>::Handle<ScreenplayActions::SelectSlateReceiver>(_action);
	//Panel will show the preloads and diagrams panel
	HandleBus<Screenplay::PanelComponent>::Handle<ScreenplayActions::SelectSlateReceiver>(_action);
	//Preload will refresh the skybox and agents
	HandleBus<Screenplay::PreloadComponent>::Handle<ScreenplayActions::SelectSlateReceiver>(_action);

	//open toolbar of flow
	{
		NEW_ACTION(DiagramActions::ToggleFlowToolbarAction, action);
		action->visible = true;
		INVOKE_ACTION(DiagramActions::ToggleFlowToolbarAction, action);
	}

	MemDB* runtimeDB = ScreenplayModel::Runtime::GetMemDB();
	MemDB* persistentDB = ScreenplayModel::Persistent::GetMemDB();
	QString aliveSlate = ScreenplayModel::Runtime::KV::Query::screenplay_slate_alive_uuid(runtimeDB);
	QList<QString> pipes = ScreenplayModel::Persistent::Sets::Query::screenplay_slate_guid_1_pipes_guids(persistentDB, aliveSlate);
	//refresh pipe
	{
		NEW_ACTION(DiagramActions::RefreshFlowAction, action);
		for each (QString pipe in pipes)
		{
			action->pipesGUID.append(pipe);
		}
		INVOKE_ACTION(DiagramActions::RefreshFlowAction, action);
	}

	//refresh trigger
	QList<QString> triggers = ScreenplayModel::Persistent::Sets::Query::screenplay_slate_guid_1_triggers_guids(persistentDB, aliveSlate);
	{
		NEW_ACTION(DiagramActions::RefreshTriggersAction, action);
		for each (QString trigger in triggers)
		{
			action->triggersGUID.append(trigger);
		}
		INVOKE_ACTION(DiagramActions::RefreshTriggersAction, action);
	}
}

//-----------------------------------------------------------------------
void ScreenplayActions::CompileReceiver::Receive(IAction* _action)
{
	CAST_ACTION(BeanActions::CompileBeanAction, _action, action);

	assert(action->onFinish);
	action->onFinish(compileBean(action->guid));
}

//-----------------------------------------------------------------------
void ScreenplayActions::MessageFromUnityReceiver::Receive(IAction* _action)
{
	HandleBus<Screenplay::PreloadComponent>::Handle<ScreenplayActions::MessageFromUnityReceiver>(_action);
	HandleBus<PluginScreenplayInternal>::Handle<ScreenplayActions::MessageFromUnityReceiver>(_action);
}

//-----------------------------------------------------------------------
void ScreenplayActions::DeletePreloadReceiver::Receive(IAction* _action)
{
	HandleBus<Screenplay::PreloadComponent>::Handle<ScreenplayActions::DeletePreloadReceiver>(_action);
}

void ScreenplayActions::OnReloadBeanReceiver::Receive(IAction* _action)
{
	//加载Slates
	HandleBus<Screenplay::SlateComponent>::Handle<ScreenplayActions::OnReloadBeanReceiver>(_action);
	HandleBus<Screenplay::PanelComponent>::Handle<ScreenplayActions::OnReloadBeanReceiver>(_action);
}

void ScreenplayActions::OnPipeAddReceiver::Receive(IAction* _action)
{
	HandleBus<Screenplay::PanelComponent>::Handle<ScreenplayActions::OnPipeAddReceiver>(_action);
}


void ScreenplayActions::OnTriggerAddReceiver::Receive(IAction* _action)
{
	HandleBus<Screenplay::PanelComponent>::Handle<ScreenplayActions::OnTriggerAddReceiver>(_action);
}

void ScreenplayActions::OnTriggerRemoveReceiver::Receive(IAction* _action)
{
	HandleBus<Screenplay::PanelComponent>::Handle<ScreenplayActions::OnTriggerRemoveReceiver>(_action);
}




void ScreenplayActions::OnPipeRemoveReceiver::Receive(IAction* _action)
{
	HandleBus<Screenplay::PanelComponent>::Handle<ScreenplayActions::OnPipeRemoveReceiver>(_action);
}

void ScreenplayActions::OnPickAssetReceiver::Receive(IAction* _action)
{
	HandleBus<Screenplay::PreloadComponent>::Handle<ScreenplayActions::OnPickAssetReceiver>(_action);
}

void ScreenplayActions::OnVXImportFinishReceiver::Receive(IAction* _action)
{
	HandleBus<Screenplay::SlateComponent>::Handle<ScreenplayActions::OnVXImportFinishReceiver>(_action);
}


void ScreenplayActions::UpdateClonePositionReceiver::Receive(IAction* _action)
{
	CAST_ACTION(ScreenplayActions::UpdateClonePositionAction, _action, action);
	LOGX_DEBUG(XC_PluginScreenplay, XL_MEDIMU_PluginScreenplay, QString("%1 (%2, %3, %4)").arg(action->uuid).arg(action->x).arg(action->y).arg(action->z));
	MemDB* memDB = ScreenplayModel::Persistent::GetMemDB();
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_px(memDB, action->uuid, action->x);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_py(memDB, action->uuid, action->y);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_pz(memDB, action->uuid, action->z);
}

void ScreenplayActions::UpdateCloneRotationReceiver::Receive(IAction* _action)
{
	CAST_ACTION(ScreenplayActions::UpdateCloneRotationAction, _action, action);
	LOGX_DEBUG(XC_PluginScreenplay, XL_MEDIMU_PluginScreenplay, QString("%1 (%2, %3, %4)").arg(action->uuid).arg(action->x).arg(action->y).arg(action->z));
	MemDB* memDB = ScreenplayModel::Persistent::GetMemDB();
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_rx(memDB, action->uuid, action->x);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_ry(memDB, action->uuid, action->y);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_rz(memDB, action->uuid, action->z);
}

void ScreenplayActions::UpdateCloneScaleReceiver::Receive(IAction* _action)
{
	CAST_ACTION(ScreenplayActions::UpdateCloneScaleAction, _action, action);
	LOGX_DEBUG(XC_PluginScreenplay, XL_MEDIMU_PluginScreenplay, QString("%1 (%2, %3, %4)").arg(action->uuid).arg(action->x).arg(action->y).arg(action->z));
	MemDB* memDB = ScreenplayModel::Persistent::GetMemDB();
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_sx(memDB, action->uuid, action->x);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_sy(memDB, action->uuid, action->y);
	ScreenplayModel::Persistent::KV::Update::screenplay_preload_guid_1_sz(memDB, action->uuid, action->z);
}

void ScreenplayActions::OnNewBeanReceiver::Receive(IAction* _action)
{
	CAST_ACTION(BeanActions::OnNewBeanAction, _action, action);
	MemDB* memDB = ScreenplayModel::Persistent::GetMemDB(action->guid);
	assert(memDB);

	//新建bean后将slate的序号设为0
	ScreenplayModel::Persistent::KV::Update::screenplay_slate_index(memDB, 0);
}

