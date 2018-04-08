#include <assert.h>
#include <QDir>
#include <QWidget>
#include <QFileInfo>
#include "Utils.h"
#include "MemDB.h"
#include "MemDBUtil.h"
#include "flowlayout.h"
#include "IPlugin.h"
#include "AppCore.h"
#include "HandleBus.hxx"
#include "Dispatcher.h"
#include "PluginBeanLogger.h"
#include "PluginBean/BeanActions.h"
#include "PluginBean/BeanModel.h"
#include "BeanCompositeCore.h"
#include "BeanCompositePanel.h"
#include "BeanCompositeCard.h"
#include "BeanCompositeWall.h"

namespace Bean
{
	class WallUI
	{
	public:
		QWidget* root;
		QWidget* container;
	};
}

//-----------------------------------------------------------------------
void Bean::WallView::setupUI()
{
	ui_ = new WallUI();
	ui_->root = AppCore::CreateUI("beanWall");
	ui_->container = ui_->root->findChild<QWidget*>("__container");
	FlowLayout* layout = new FlowLayout();
	ui_->container->setLayout(layout);
}

void Bean::WallView::onUpdate(const QString& _tag)
{
	if (0 != _tag.compare(BeanModel::Persistent::Tags::beans_update))
		return;

	QLayoutItem* item = NULL;
	while (item = ui_->container->layout()->takeAt(0))
	{
		ui_->container->layout()->removeItem(item);
	}

	for each (View* view in children_)
	{
		DYN_CAST(CardView, view, card);
		ui_->container->layout()->addWidget(card->GetRoot());
	}
}

QWidget* Bean::WallView::GetRoot()
{
	return ui_->root;
}

//-----------------------------------------------------------------------
void Bean::WallView::AppendCard(QWidget* _root)
{
	assert(_root);
	ui_->container->layout()->addWidget(_root);
}

//-----------------------------------------------------------------------
void Bean::WallView::RemoveCard(QWidget* _root)
{
	assert(_root);
	ui_->container->layout()->removeWidget(_root);
}

//////////////////////////////////////////////////////////////////////////
//
// Component
//
//////////////////////////////////////////////////////////////////////////
Bean::WallComponent::WallComponent(WallView* _view)
	: Composite::MVCComponent(_view)
{

}

//-----------------------------------------------------------------------
void Bean::WallComponent::initialize(Composite::Core* _core)
{
	LOGX_DEBUG(XC_PluginBean, XL_LOW_PluginBean, "initialize");
	ptrCore_ = _core;

	HandleBus<Bean::WallComponent>::Route<BeanActions::NewBeanReceiver>(this, &Bean::WallComponent::HandleNewBean);
	HandleBus<Bean::WallComponent>::Route<BeanActions::DeleteBeanReceiver>(this, &Bean::WallComponent::HandleDeleteBean);
	HandleBus<Bean::WallComponent>::Route<BeanActions::SelectBeanReceiver>(this, &Bean::WallComponent::HandleSelectBean);
	HandleBus<Bean::WallComponent>::Route<BeanActions::ParseDiskReceiver>(this, &Bean::WallComponent::HandleParseDisk);
}

//-----------------------------------------------------------------------
void Bean::WallComponent::HandleNewBean(IAction* _action)
{
	CAST_ACTION(BeanActions::NewBeanAction, _action, action);
	LOGX_DEBUG(XC_PluginBean, XL_MEDIMU_PluginBean, QString("add new bean %1").arg(action->guid));

	assert(!action->guid.isEmpty() && !action->guid.isNull());

	newBean(action->guid, action->alias);
}

//-----------------------------------------------------------------------
void Bean::WallComponent::HandleDeleteBean(IAction* _action)
{
	CAST_ACTION(BeanActions::DeleteBeanAction, _action, action);
	LOGX_DEBUG(XC_PluginBean, XL_MEDIMU_PluginBean, QString("delete bean %1").arg(action->guid));

	deleteBean(action->guid);
}

//-----------------------------------------------------------------------
void Bean::WallComponent::HandleSelectBean(IAction* _action)
{
	CAST_ACTION(BeanActions::SelectBeanAction, _action, action);
	LOGX_DEBUG(XC_PluginBean, XL_MEDIMU_PluginBean, QString("select bean %1").arg(action->uuid));
	selectBean(action->uuid);
}

//-----------------------------------------------------------------------
void Bean::WallComponent::HandleParseDisk(IAction* _action)
{
	MemDB* runtimeDB = BeanModel::Runtime::GetMemDB();

	QString path = MemDBUtil::GetDataPath();
	QDir dir = QDir(path);
	QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	for each (QFileInfo file in files)
	{
		if (!file.fileName().contains("bucket.bean.guid"))
			continue;

		if (0 == MemDBUtil::LoadBucket(file.completeBaseName()))
		{
			QString guid = QString::fromUtf8(MemDBUtil::TakeUUID(file.completeBaseName()));
			CardComponent* card = new CardComponent(new CardView(), guid);
			this->AddMVCChild(card);
			card->Initialize(ptrCore_);
			//将alias加入到运行时库中
			MemDB* persistentDB = BeanModel::Persistent::GetMemDB(guid);
			QString alias = BeanModel::Persistent::KV::Query::bean_alias(persistentDB);
			BeanModel::Runtime::Sets::Update::Add::bean_alias(runtimeDB, alias);
		}
	}
	Composite::Store::Update(BeanModel::Persistent::Tags::beans_update);
	Composite::Store::Update(BeanModel::Persistent::Tags::bean_update);
}

//-----------------------------------------------------------------------
void Bean::WallComponent::newBean(const QString& _guid, const QString& _alias)
{
	LOGX_DEBUG(XC_PluginBean, XL_MEDIMU_PluginBean, QString("new a bean card, uuid is %1").arg(_guid));

	CardComponent* card = new CardComponent(new CardView(), _guid);
	this->AddMVCChild(card);
	card->Initialize(ptrCore_);

	MemDB* runtimeDB = BeanModel::Runtime::GetMemDB();
	BeanModel::Runtime::Sets::Update::Add::bean_alias(runtimeDB, _alias);
	//获取选中的源
	QStringList extends;
	foreach(QString source, BeanModel::Runtime::Sets::Query::bean_source_selected_guid(runtimeDB))
	{
		extends.append(BeanModel::Runtime::KV::Query::bean_source_guid_1_extends(runtimeDB, source).value< QStringList>());
	}

	BeanModel::Persistent::InitializeModel(_guid);
	MemDB* persistentDB = BeanModel::Persistent::GetMemDB(_guid);
	BeanModel::Persistent::KV::Update::bean_alias(persistentDB, _alias);
	//从源中取得扩展
	
	foreach(QString extend, extends)
	{
		BeanModel::Persistent::Sets::Update::Add::bean_extends(persistentDB, extend);
	}
	//默认包含基础扩展包
	BeanModel::Persistent::Sets::Update::Add::bean_extends(persistentDB, "basic");
	Composite::Store::Update(BeanModel::Persistent::Tags::bean_update);
	Composite::Store::Update(BeanModel::Persistent::Tags::beans_update);

	NEW_ACTION(BeanActions::OnNewBeanAction, action);
	action->guid = _guid;
	INVOKE_ACTION(BeanActions::OnNewBeanAction, action);
}

void Bean::WallComponent::deleteBean(const QString& _guid)
{
	Composite::Component* component = this->FindChild(_guid);
	LOGX_DEBUG(XC_PluginBean, XL_MEDIMU_PluginBean, QString("delete the bean card, uuid is %1").arg(_guid));
	component->Release();
	this->RemoveMVCChild(_guid);
	SAFE_DELETE(component);

	NEW_ACTION(BeanActions::ToggleBeanEditorAction, action);
	action->visible = false;
	INVOKE_ACTION(BeanActions::ToggleBeanEditorAction, action);

	//update model layer
	//移除存在的alias
	MemDB* persistentDB = BeanModel::Persistent::GetMemDB(_guid);
	QString alias = BeanModel::Persistent::KV::Query::bean_alias(persistentDB);
	MemDB* runtimeDB = BeanModel::Runtime::GetMemDB();
	BeanModel::Runtime::Sets::Update::Remove::bean_alias(runtimeDB, alias);
	//移除数据库
	BeanModel::Persistent::ReleaseModel(_guid);

	Composite::Store::Update(BeanModel::Persistent::Tags::beans_update);

	NEW_ACTION(BeanActions::OnDeleteBeanAction, deleteAction);
	deleteAction->guid = _guid;
	INVOKE_ACTION(BeanActions::OnDeleteBeanAction, deleteAction);
}

void Bean::WallComponent::selectBean(const QString& _guid)
{
	NEW_ACTION(BeanActions::ToggleBeanEditorAction, action);
	action->visible = true;
	INVOKE_ACTION(BeanActions::ToggleBeanEditorAction, action);

}
