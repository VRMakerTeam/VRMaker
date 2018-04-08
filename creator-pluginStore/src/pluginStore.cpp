#include <assert.h>
#include <QDebug>
#include<QCheckBox>
#include<qobject.h>
#include <QResource>
#include "Utils.h"
#include "ui_storeLancher.h"
#include "ui_panel.h"
#include "AppCore.h"
#include "PluginStore/StoreActions.h"
#include "PluginStore/pluginStore.h"
#include "PackageManager.h"


#define DATA_OFFSET Qt::UserRole+1
#define GROUP_ZONE "zone"
#define STYLE_GRED_BACKGROUND "background-color:rgb(153,153,153)"
class StoreUnitData : public QObjectUserData
{
public:
	QString guid;
};

class StoreUserData : public QObjectUserData
{
public:
	int value;
};

class PluginStoreInternal: public QObject
{
public:
	AppCore* ptrAppCore;
	QWidget* rootPanel;
	QWidget* rootLauncher;
	Ui::PluginStorePanel* uiPanel;
	Ui::PluginStoreLancher* uiLancher;
	QString launcherStyleSheet;

	PluginStoreInternal();
	void closeDownloadPage();
	void setSelectedButtonWidget(QPushButton* sender);
	//void EditStore();
	//void HideToolbars(bool bIsHide) const;
	void clearStoreListLayout();
	void clearStoryboardList();
	
	void FilterMusic();
	void FilterVoice();
	void FilterScene();
	void FilterStart();
	void FilterModel();
	void FilterAction();
	void HandleStoreItem();
	void showDefaultClassify();
	
	void SetStoreUserData(QWidget* _widget, const int& _value);
	const int& GetStoreUserData(QWidget* _widget);
	void setItemLayout(QPushButton* buttonItem, const QString& alias);
	void DisplayAllTags();
	void creatOneCheckItem(const QString &tagName);
private:
	QWidget* localStoreContainer;
	QGridLayout* storeList;
	QWidget* storyboardContainer;
	QHBoxLayout* storyboardList;
};

PluginStoreInternal::PluginStoreInternal()
{
	
}

//-----------------------------------------------------------------------
void PluginStore::Initialize(AppCore* _appCore)
{
	
	QResource::registerResource("res/pluginStore.rcc");
	internal_ = new PluginStoreInternal();
	internal_->ptrAppCore = _appCore;

	internal_->rootPanel = new QWidget();
	internal_->uiPanel = new Ui::PluginStorePanel;
	internal_->uiPanel->setupUi(internal_->rootPanel);

	internal_->rootLauncher = new QWidget();
	internal_->uiLancher = new Ui::PluginStoreLancher;
	internal_->uiLancher->setupUi(internal_->rootLauncher);
	internal_->launcherStyleSheet = internal_->rootLauncher->styleSheet();
	internal_->ptrAppCore->AppendTitlebarLauncher(internal_->rootLauncher);

	//hide downLoad widget
	internal_->uiPanel->stackedWidget_store->setCurrentIndex(0);

	QObject::connect(internal_->uiLancher->pb_shop, &QPushButton::clicked, [=] {
		internal_->ptrAppCore->LaunchPlugin(this);
	});

	//init control events
	QObject::connect(internal_->uiPanel->pb_close_downLoad_2, &QPushButton::clicked, internal_, &PluginStoreInternal::closeDownloadPage);
	QObject::connect(internal_->uiPanel->pb_store_scene_3, &QPushButton::clicked, internal_, &PluginStoreInternal::FilterScene);
	QObject::connect(internal_->uiPanel->pb_store_model_3, &QPushButton::clicked, internal_, &PluginStoreInternal::FilterModel);
	QObject::connect(internal_->uiPanel->pb_store_action_3, &QPushButton::clicked, internal_, &PluginStoreInternal::FilterAction);
	QObject::connect(internal_->uiPanel->pb_store_start_3, &QPushButton::clicked, internal_, &PluginStoreInternal::FilterStart);
	QObject::connect(internal_->uiPanel->pb_store_music_3, &QPushButton::clicked, internal_, &PluginStoreInternal::FilterMusic);
	QObject::connect(internal_->uiPanel->pb_store_voice_3, &QPushButton::clicked, internal_, &PluginStoreInternal::FilterVoice);
}

void PluginStoreInternal::showDefaultClassify()
{
	QWidget* zoneWiget = uiPanel->widget_57;
	QWidget* startWiget = uiPanel->widget_58;
	QWidget* musicWiget = uiPanel->widget_59;
	QWidget* actionWiget = uiPanel->widget_60;
	QWidget* agentWiget = uiPanel->widget_61;
	QWidget* voiceWiget = uiPanel->widget_62;

	zoneWiget->setStyleSheet(STYLE_GRED_BACKGROUND);
	voiceWiget->setStyleSheet("");
	agentWiget->setStyleSheet("");
	musicWiget->setStyleSheet("");
	actionWiget->setStyleSheet("");
	startWiget->setStyleSheet("");
}


void PluginStoreInternal::closeDownloadPage()
{
	uiPanel->stackedWidget_store->setCurrentIndex(0);
}

void PluginStoreInternal::setSelectedButtonWidget(QPushButton* sender)
{
	
	QWidget* zoneWiget = uiPanel->widget_57;
	QWidget* startWiget = uiPanel->widget_58;
	QWidget* musicWiget = uiPanel->widget_59;
	QWidget* actionWiget = uiPanel->widget_60;
	QWidget* agentWiget = uiPanel->widget_61;
	QWidget* voiceWiget = uiPanel->widget_62;

	QPushButton* SceneBtn = uiPanel->pb_store_scene_3;
	QPushButton* ModelBtn = uiPanel->pb_store_model_3;
	QPushButton* ActionBtn = uiPanel->pb_store_action_3;
	QPushButton* StartBtn = uiPanel->pb_store_start_3;
	QPushButton* MusicBtn = uiPanel->pb_store_music_3;
	QPushButton* VoiceBtn = uiPanel->pb_store_voice_3;
	if (sender == SceneBtn)
	{
		zoneWiget->setStyleSheet(STYLE_GRED_BACKGROUND);
		voiceWiget->setStyleSheet("");
		agentWiget->setStyleSheet("");
		musicWiget->setStyleSheet("");
		actionWiget->setStyleSheet("");
		startWiget->setStyleSheet("");
	}
	else if (sender == ModelBtn)
	{
		zoneWiget->setStyleSheet("");
		voiceWiget->setStyleSheet("");
		agentWiget->setStyleSheet(STYLE_GRED_BACKGROUND);
		musicWiget->setStyleSheet("");
		actionWiget->setStyleSheet("");
		startWiget->setStyleSheet("");
	}
	else if (sender == MusicBtn)
	{
		zoneWiget->setStyleSheet("");
		voiceWiget->setStyleSheet("");
		agentWiget->setStyleSheet("");
		musicWiget->setStyleSheet(STYLE_GRED_BACKGROUND);
		actionWiget->setStyleSheet("");
		startWiget->setStyleSheet("");
	}
	else if (sender == VoiceBtn)
	{
		zoneWiget->setStyleSheet("");
		voiceWiget->setStyleSheet(STYLE_GRED_BACKGROUND);
		agentWiget->setStyleSheet("");
		musicWiget->setStyleSheet("");
		actionWiget->setStyleSheet("");
		startWiget->setStyleSheet("");
	}
	else if (sender == StartBtn)
	{
		zoneWiget->setStyleSheet("");
		voiceWiget->setStyleSheet("");
		agentWiget->setStyleSheet("");
		musicWiget->setStyleSheet("");
		actionWiget->setStyleSheet("");
		startWiget->setStyleSheet(STYLE_GRED_BACKGROUND);
	}
	else
	{
		zoneWiget->setStyleSheet("");
		voiceWiget->setStyleSheet("");
		agentWiget->setStyleSheet("");
		musicWiget->setStyleSheet("");
		actionWiget->setStyleSheet(STYLE_GRED_BACKGROUND);
		startWiget->setStyleSheet("");
	}

}


void PluginStoreInternal::HandleStoreItem()
{
	QPushButton* displayWidget = uiPanel->widget_pic_2;
	QPushButton *btnItem = static_cast<QPushButton *>(sender());
	uiPanel->stackedWidget_store->setCurrentIndex(1);
	const int& id = GetStoreUserData(btnItem);
	qDebug() << "id:" << id;

	const QString& alias = CacheManager::GetCacheAliasById(id);
	const QString& version = CacheManager::GetVersionById(id);
	const QString& date = CacheManager::GetDateById(id);
	const QString& author = CacheManager::GetAuthorById(id);
	const QString& pic = CacheManager::GetPicById(id);
	const QString& group = CacheManager::GetGroupById(id);
	const QString& guid = CacheManager::GetGuidById(id);
	int storyboardCnt = 0;

	//if (group.compare(GROUP_ZONE) == 0)
	//{
	storyboardCnt = CacheManager::GetCountById(id);
	//}

	QLabel* label_alias = uiPanel->label_alias_2;
	QLabel* label_version = uiPanel->label_version_2;
	QLabel* label_date = uiPanel->label_date_2;
	QLabel* label_author = uiPanel->label_author_2;

	label_alias->setText(alias);
	label_version->setText(version);
	label_date->setText(date);
	label_author->setText(author);

	QString path = "Package\/";
	path.append(pic);
	displayWidget->setIcon(QIcon(path));
	displayWidget->setIconSize(QSize(459, 246));

	//display storyboard
	clearStoryboardList();
	for (int i = 0; i < storyboardCnt; i++)
	{
		const QString& alias = CacheManager::GetCacheAliasById(id);
		const QString& pic = CacheManager::GetPicById(id);
		QPushButton* storyboardObj = new QPushButton();
		QString path = "Package\/";
		int num = i + 1;
		QString code = QString("%1@%2").arg(guid).arg(QString::number(num));
		path.append(code);
		path.append(".png");
		storyboardObj->setFlat(true);
		storyboardObj->setIcon(QIcon(path));
		storyboardObj->setIconSize(QSize(350, 250));
		storyboardObj->setMinimumSize(QSize(320, 200));
		storyboardObj->setMaximumSize(QSize(320, 200));
		storyboardList->addWidget(storyboardObj,  i);
	}

}

void PluginStoreInternal::SetStoreUserData(QWidget* _widget, const int& _value)
{
	StoreUserData* data = dynamic_cast<StoreUserData *>(_widget->userData(DATA_OFFSET));
	assert(NULL == data);
	data = new StoreUserData();
	data->value = _value;
	_widget->setUserData(DATA_OFFSET, data);
}

const int& PluginStoreInternal::GetStoreUserData(QWidget* _widget)
{
	StoreUserData* data = dynamic_cast<StoreUserData *>(_widget->userData(DATA_OFFSET));
	assert(data);
	return data->value;
}

void PluginStoreInternal::clearStoryboardList()
{
	storyboardContainer = uiPanel->scrollAreaWidgetdownloadContents_2;
	assert(storyboardContainer);
	storyboardList = dynamic_cast<QHBoxLayout*>(storyboardContainer->layout());
	int count = storyboardList->count();
	QList<QWidget*> widgets;
	int index_remove = 0;
	for (; index_remove < count; index_remove++)
	{
		QLayoutItem* item = storyboardList->itemAt(0);
		storyboardList->removeItem(item);
		item->widget()->deleteLater();
		delete item;
	}
}

void PluginStoreInternal::clearStoreListLayout()
{
	localStoreContainer = uiPanel->scrollAreaStoreWidgetContents_2;
	assert(localStoreContainer);
	storeList = dynamic_cast<QGridLayout*>(localStoreContainer->layout());
	int count = storeList->count();
	QList<QWidget*> widgets;
	int index_remove = 0;
	for (; index_remove < count; index_remove++)
	{
		QLayoutItem* item = storeList->itemAt(0);
		storeList->removeItem(item);
		item->widget()->deleteLater();
		delete item;
	}

}

void PluginStoreInternal::setItemLayout(QPushButton* buttonItem, const QString& alias)
{
	buttonItem->setIconSize(QSize(230, 200));
	buttonItem->setMinimumSize(QSize(230, 200));
	buttonItem->setMaximumSize(QSize(230, 200));
	QLabel* aliasLabel = new QLabel;
	aliasLabel->setText(alias);
	aliasLabel->setParent(buttonItem);
	aliasLabel->setStyleSheet("background-color:rgba(22,155,213);font:19px;color:rgb(255,255,255);");
	aliasLabel->setGeometry(QRect(0, 160, 230, 40));
	aliasLabel->setAlignment(Qt::AlignCenter);

	QLabel* label = new QLabel;
	label->setParent(buttonItem);
	label->setStyleSheet("background-color:rgb(240,240,240);");
	label->setGeometry(QRect(0, 0, 230, 40));
	label->setEnabled(false);
}


void PluginStoreInternal::FilterAction()
{
	clearStoreListLayout();
	closeDownloadPage();
	setSelectedButtonWidget(static_cast<QPushButton*>(sender()));
	QStringList actionIdGroup = CacheManager::ListCacheGroup(CACHE_GROUP_ANIMATION);
	int idx = 0;
	for each (QString actionId in actionIdGroup)
	{
		int id = (actionId).toInt();
		const QString& alias = CacheManager::GetCacheAliasById(id);
		const QString& pic = CacheManager::GetPicById(id);
		QPushButton* actionObj = new QPushButton();
		QString path = "Package\/";
		path.append(pic);
		actionObj->setIcon(QIcon(path));
		setItemLayout(actionObj, alias);
		SetStoreUserData(actionObj, id);
		QObject::connect(actionObj, &QPushButton::clicked, this, &PluginStoreInternal::HandleStoreItem);
		storeList->addWidget(actionObj, idx / 4, idx % 4);
		idx++;
	}
}

void PluginStoreInternal::FilterModel()
{
	clearStoreListLayout();
	closeDownloadPage();
	setSelectedButtonWidget(static_cast<QPushButton*>(sender()));
	QStringList agentIdGroup = CacheManager::ListCacheGroup(CACHE_GROUP_AGENT);
	int idx = 0;
	for each (QString agentId in agentIdGroup)
	{
		int id = (agentId).toInt();
		const QString& alias = CacheManager::GetCacheAliasById(id);
		const QString& pic = CacheManager::GetPicById(id);
		QPushButton* agentObj = new QPushButton();
		QString path = "Package\/";
		path.append(pic);
		agentObj->setIcon(QIcon(path));
		setItemLayout(agentObj, alias);
		SetStoreUserData(agentObj, id);
		QObject::connect(agentObj, &QPushButton::clicked, this, &PluginStoreInternal::HandleStoreItem);
		storeList->addWidget(agentObj, idx / 4, idx % 4);
		idx++;
	}
}

void PluginStoreInternal::FilterMusic()
{
	clearStoreListLayout();
	closeDownloadPage();
	setSelectedButtonWidget(static_cast<QPushButton*>(sender()));
	QStringList musicIdGroup = CacheManager::ListCacheGroup(CACHE_GROUP_MUSIC);
	int idx = 0;
	for each (QString musicId in musicIdGroup)
	{
		int id = (musicId).toInt();
		const QString& alias = CacheManager::GetCacheAliasById(id);
		const QString& pic = CacheManager::GetPicById(id);
		QPushButton* musicObj = new QPushButton();
		QString path = "Package\/";
		path.append(pic);
		musicObj->setIcon(QIcon(path));
		setItemLayout(musicObj, alias);
		SetStoreUserData(musicObj, id);
		QObject::connect(musicObj, &QPushButton::clicked, this, &PluginStoreInternal::HandleStoreItem);
		storeList->addWidget(musicObj, idx / 4, idx % 4);
		idx++;
	}

}

void PluginStoreInternal::FilterScene()
{
	uiPanel->stackedWidget_store->setCurrentIndex(0);
	clearStoreListLayout();
	closeDownloadPage();
	setSelectedButtonWidget(static_cast<QPushButton*>(sender()));
	QStringList senceIdGroup = CacheManager::ListCacheGroup(CACHE_GROUP_ZONE);
	int idx = 0;
	for each (QString sceneId in senceIdGroup)
	{
		int id = (sceneId).toInt();
		const QString& alias = CacheManager::GetCacheAliasById(id);
		const QString& pic = CacheManager::GetPicById(id);
		QPushButton* senceObj = new QPushButton();
		QString path = "Package\/";
		path.append(pic);
		senceObj->setIcon(QIcon(path));
		setItemLayout(senceObj, alias);
		SetStoreUserData(senceObj, id);
		QObject::connect(senceObj, &QPushButton::clicked, this, &PluginStoreInternal::HandleStoreItem);
		storeList->addWidget(senceObj, idx / 4, idx % 4);
		idx++;
	}
}

void PluginStoreInternal::FilterStart()
{
	clearStoreListLayout();
	closeDownloadPage();
	setSelectedButtonWidget(static_cast<QPushButton*>(sender()));
	QStringList scrollIdGroup = CacheManager::ListCacheGroup(CACHE_GROUP_SCROLL);
	int idx = 0;
	for each (QString scrollId in scrollIdGroup)
	{
		int id = (scrollId).toInt();
		const QString& alias = CacheManager::GetCacheAliasById(id);
		const QString& pic = CacheManager::GetPicById(id);
		QPushButton* scrollObj = new QPushButton();
		QString path = "Package\/";
		path.append(pic);
		scrollObj->setIcon(QIcon(path));
		setItemLayout(scrollObj, alias);
		SetStoreUserData(scrollObj, id);
		QObject::connect(scrollObj, &QPushButton::clicked, this, &PluginStoreInternal::HandleStoreItem);
		storeList->addWidget(scrollObj, idx / 4, idx % 4);
		idx++;
	}
}

void PluginStoreInternal::FilterVoice()
{
	clearStoreListLayout();
	closeDownloadPage();
	setSelectedButtonWidget(static_cast<QPushButton*>(sender()));
	QStringList voiceIdGroup = CacheManager::ListCacheGroup(CACHE_GROUP_VOICE);
	int idx = 0;
	for each (QString voiceId in voiceIdGroup)
	{
		int id = (voiceId).toInt();
		const QString& alias = CacheManager::GetCacheAliasById(id);
		const QString& pic = CacheManager::GetPicById(id);
		QPushButton* voiceObj = new QPushButton();
		QString path = "Package\/";
		path.append(pic);
		voiceObj->setIcon(QIcon(path));
		setItemLayout(voiceObj, alias);
		SetStoreUserData(voiceObj, id);
		QObject::connect(voiceObj, &QPushButton::clicked, this, &PluginStoreInternal::HandleStoreItem);
		storeList->addWidget(voiceObj, idx / 4, idx % 4);
		idx++;
	}
}

void PluginStoreInternal::creatOneCheckItem(const QString &tagName)
{	
	QCheckBox *checkBox = new QCheckBox();
	checkBox->setStyleSheet("background-color: rgba(255, 255, 255, 0);border - radius:10;");
	//uiPanel->scrollAreaWidgetContents_3->addItem()
	//checkBox->setGeometry(QRect(9,9,96,36));
	QLabel *tagLabel = new QLabel();
	tagLabel->setText(tagName);
	tagLabel->setGeometry(QRect(10,0,80,30));
	tagLabel->setStyleSheet("color: rgb(255, 255, 255);font: 14pt ;");
	tagLabel->setParent(checkBox);
}

void PluginStoreInternal::DisplayAllTags()
{
	 QVector<QString> storeTagsV = CacheManager::getAllTags(CacheManager::getTagsName());
	 for (int i = 0; i < storeTagsV.size(); i++) {
		 qDebug() << storeTagsV[i];
		 creatOneCheckItem(storeTagsV[i]);
	 }
	// creatOneCheckItem(storeTagsV[1]);

}

//-----------------------------------------------------------------------
void PluginStore::Attach()
{
	CacheManager::ParseCache();
	internal_->FilterScene();
	internal_->showDefaultClassify();
	internal_->ptrAppCore->AttachPanel(internal_->rootPanel);
	internal_->DisplayAllTags();
	//internal_->ptrAppCore->AttachToolbar(internal_->rootToolbar);

	QString stylesheet = internal_->launcherStyleSheet;
	stylesheet.append("QWidget#PluginStoreLancher{background-color: rgb(63, 63, 70);}");
	internal_->rootLauncher->setStyleSheet(stylesheet);
}

//-----------------------------------------------------------------------
void PluginStore::Show()
{
	internal_->rootPanel->show();
	//internal_->rootToolbar->show();
}

//-----------------------------------------------------------------------
void PluginStore::Hide()
{
	internal_->rootPanel->hide();
	//internal_->rootToolbar->hide();
}

//-----------------------------------------------------------------------
void PluginStore::Detach()
{
	internal_->rootLauncher->setStyleSheet(internal_->launcherStyleSheet);
}

//-----------------------------------------------------------------------
void PluginStore::Release()
{
	SAFE_DELETE(internal_->rootLauncher);
	SAFE_DELETE(internal_->uiLancher);
	SAFE_DELETE(internal_->rootPanel);
	SAFE_DELETE(internal_->uiPanel);
	SAFE_DELETE(internal_);
}
