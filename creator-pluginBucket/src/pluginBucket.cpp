#include <assert.h>
#include <QWidget>
#include <QCheckBox>
#include "Utils.h"
#include "AppCore.h"
#include "BucketCompositeCore.h"
#include "BucketCompositePicker.h"
#include "BucketCompositeAssets.h"
#include "BucketCompositePanel.h"
#include "UGCManager.h"
#include "PackageManager.h"
#include "PluginBucket/BucketActions.h"
#include "PluginBucket/BucketModel.h"
#include "PluginBucket/pluginBucket.h"

class BucketUnitData : public QObjectUserData
{
public:
	QString guid;
};

class BucketLauncherUI
{
public:
	QWidget* root;
	QCheckBox* launch;
};


class PluginBucketInternal:public QObject
{
public:
	Bucket::CompositeCore* compositeCore;
	Bucket::PanelComponent* panelComponent;
	Bucket::PickerComponent* pickerComponent;

	BucketLauncherUI* launcherUI;
public:
	PluginBucketInternal();
	~PluginBucketInternal();
private:
};

PluginBucketInternal::PluginBucketInternal()
{

}

PluginBucketInternal::~PluginBucketInternal()
{
	
}

//-----------------------------------------------------------------------
void PluginBucket::Initialize(AppCore* _appCore)
{
	AppCore::LoadTS("pluginBucket");
#if 0
	AppCore::LoadRCC("pluginBucket");
#else
	QString language = _appCore->Language();

	if (language.compare("de") == 0) {
		AppCore::LoadRCC("pluginBucket_de");
	}
	else if (language.compare("jp") == 0) {
		AppCore::LoadRCC("pluginBucket_jp");
	}
	else if (language.compare("kr") == 0) {
		AppCore::LoadRCC("pluginBucket_kr");
	}
	else if (language.compare("it") == 0) {
		AppCore::LoadRCC("pluginBucket_it");
	}
	else if (language.compare("pt") == 0) {
		AppCore::LoadRCC("pluginBucket_pt");
	}
	else if (language.compare("sp") == 0) {
		AppCore::LoadRCC("pluginBucket_sp");
	}
	else if (language.compare("fr") == 0) {
		AppCore::LoadRCC("pluginBucket_fr");
	}
	else if (language.compare("ru") == 0) {
		AppCore::LoadRCC("pluginBucket_ru");
	}
	else if (language.compare("zh_CN") == 0) {
		AppCore::LoadRCC("pluginBucket_zh_CN");
	}
	else {
		AppCore::LoadRCC("pluginBucket");
	}
#endif

	internal_ = new PluginBucketInternal();

	internal_->compositeCore = new Bucket::CompositeCore();
	internal_->compositeCore->SetAppCore(_appCore);


	internal_->pickerComponent = new Bucket::PickerComponent(new Bucket::PickerView());
	internal_->pickerComponent->Initialize(internal_->compositeCore);

	internal_->panelComponent = new Bucket::PanelComponent(new Bucket::PanelView());
	internal_->panelComponent->Initialize(internal_->compositeCore);

	internal_->launcherUI = new BucketLauncherUI();
	internal_->launcherUI->root = AppCore::CreateUI("bucketLauncher");
	internal_->compositeCore->GetAppCore()->AppendTitlebarLauncher(internal_->launcherUI->root);
	internal_->launcherUI->launch = internal_->launcherUI->root->findChild<QCheckBox*>("__launch");
	QObject::connect(internal_->launcherUI->launch, &QCheckBox::clicked, [=](bool _clicked) {
		if (!_clicked)
		{
			internal_->launcherUI->launch->setChecked(true);
			return;
		}

		internal_->compositeCore->GetAppCore()->LaunchPlugin(this);
	});

	Dispatcher::Subscribe<BucketActions::PopupPickerReceiver, BucketActions::PopupPickerAction>();
	Dispatcher::Subscribe<BucketActions::FilterAssetsGroupReceiver, BucketActions::FilterAssetsGroupAction>();
	Dispatcher::Subscribe<BucketActions::FilterAssetsGroupTagsReceiver, BucketActions::FilterAssetsGroupTagsAction>();
	Dispatcher::Subscribe<BucketActions::FilterTagsGroupReceiver, BucketActions::FilterTagsGroupAction>();
	Dispatcher::Subscribe<BucketActions::StartRecordUGCAudioReceiver, BucketActions::StartRecordUGCAudioAction>();
	Dispatcher::Subscribe<BucketActions::StopRecordUGCAudioReceiver, BucketActions::StopRecordUGCAudioAction>();
	Dispatcher::Subscribe<BucketActions::StartPlayUGCAudioReceiver, BucketActions::StartPlayUGCAudioAction>();
	Dispatcher::Subscribe<BucketActions::StopPlayUGCAudioReceiver, BucketActions::StopPlayUGCAudioAction>();
	Dispatcher::Subscribe<BucketActions::ImportAudioReceiver, BucketActions::ImportAudioAction>();
	Dispatcher::Subscribe<BucketActions::ImportImageReceiver, BucketActions::ImportImageAction>();
	Dispatcher::Subscribe<BucketActions::FilterUGCAudioReceiver, BucketActions::FilterUGCAudioAction>();
	Dispatcher::Subscribe<BucketActions::FilterUGCImageReceiver, BucketActions::FilterUGCImageAction>();


	BucketModel::Runtime::InitializeModel();
	PackageManager::Parse();
	UGCManager::Parse();
}

//-----------------------------------------------------------------------
void PluginBucket::Attach()
{
	DYN_CAST(Bucket::PanelView, internal_->panelComponent->GetView(), panelView);
	internal_->compositeCore->GetAppCore()->AttachPanel(panelView->GetRoot());
}

//-----------------------------------------------------------------------
void PluginBucket::Show()
{
	//internal_->rootPanel->show();
}

//-----------------------------------------------------------------------
void PluginBucket::Hide()
{
	//internal_->rootPanel->hide();
}

//-----------------------------------------------------------------------
void PluginBucket::Detach()
{
	internal_->launcherUI->launch->setChecked(false);
}

//-----------------------------------------------------------------------
void PluginBucket::Release()
{
	SAFE_DELETE(internal_);
}
