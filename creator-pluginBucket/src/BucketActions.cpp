#include <QFileDialog>
#include "HandleBus.hxx"
#include "AudioUtil.h"
#include "PathUtil.h"
#include "BucketCompositePicker.h"
#include "BucketCompositeAssets.h"
#include "BucketCompositeWall.h"
#include "BucketCompositeTag.h"
#include "PluginBucketLogger.h"
#include "UGCManager.h"
#include "PackageManager.h"
#include "PluginBucket/BucketActions.h"

//-----------------------------------------------------------------------
void BucketActions::PopupPickerReceiver::Receive(IAction* _action)
{
	HandleBus<Bucket::PickerComponent>::Handle<BucketActions::PopupPickerReceiver>(_action);
}

//-----------------------------------------------------------------------
void BucketActions::FilterAssetsGroupReceiver::Receive(IAction* _action)
{
	HandleBus<Bucket::WallComponent>::Handle<BucketActions::FilterAssetsGroupReceiver>(_action);
}

void BucketActions::FilterTagsGroupReceiver::Receive(IAction* _action)
{
	HandleBus<Bucket::TagComponent>::Handle<BucketActions::FilterTagsGroupReceiver>(_action);
}

void BucketActions::FilterAssetsGroupTagsReceiver::Receive(IAction* _action)
{
	HandleBus<Bucket::WallComponent>::Handle<BucketActions::FilterAssetsGroupTagsReceiver>(_action);
}

void BucketActions::StartRecordUGCAudioReceiver::Receive(IAction* _action)
{
	CAST_ACTION(BucketActions::StartRecordUGCAudioAction, _action, action);
	AudioUtils::Recorder::Start(action->uuid);
}

void BucketActions::StopRecordUGCAudioReceiver::Receive(IAction* _action)
{
	AudioUtils::Recorder::Stop();
}

void BucketActions::StartPlayUGCAudioReceiver::Receive(IAction* _action)
{
	CAST_ACTION(BucketActions::StartRecordUGCAudioAction, _action, action);
	AudioUtils::Player::PlayCache(action->uuid);
}

void BucketActions::StopPlayUGCAudioReceiver::Receive(IAction* _action)
{
	AudioUtils::Player::Stop();
}

void BucketActions::ImportAudioReceiver::Receive(IAction* _action)
{
	QStringList files = QFileDialog::getOpenFileNames(NULL, QObject::tr("Select Audio"), ".", "Audio Files(*.wav)");
	QDir dir(PathUtil::PackageLocation());
	if (!dir.exists("ugc"))
		dir.mkdir("ugc");
	dir.cd("ugc");
	
	for each (QString file in files)
	{
		QFileInfo fileInfo(file);
		QFile::copy(file, dir.absoluteFilePath(fileInfo.fileName()));
	}
	if(0 != files.count())
		UGCManager::Parse();
}

void BucketActions::ImportImageReceiver::Receive(IAction* _action)
{
	//��ȡ��Դ���������ѡ�е��ļ�
	QStringList files = QFileDialog::getOpenFileNames(NULL, QObject::tr("Select Image"), ".", "Image Files(*.png;*.jpg)");

	//�л������ugc��·��
	QDir dir(PathUtil::PackageLocation());
	if (!dir.exists("ugc"))
		dir.mkdir("ugc");
	dir.cd("ugc");

	//����Ҫ������ļ�ȫ��������ugc·����
	for each (QString file in files)
	{
		QFileInfo fileInfo(file);
		QFile::copy(file, dir.absoluteFilePath(fileInfo.fileName()));
	}

	//���¸���UGC�б�
	if (0 != files.count())
		UGCManager::Parse();
}

//-----------------------------------------------------------------------
void BucketActions::FilterUGCAudioReceiver::Receive(IAction* _action)
{
	HandleBus<Bucket::WallComponent>::Handle<BucketActions::FilterUGCAudioReceiver>(_action);
}

//-----------------------------------------------------------------------
void BucketActions::FilterUGCImageReceiver::Receive(IAction* _action)
{
	HandleBus<Bucket::WallComponent>::Handle<BucketActions::FilterUGCImageReceiver>(_action);
}
