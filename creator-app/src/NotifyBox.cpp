#include "ui_Notify.h"
#include "NotifyBox.h"


class NotifyBoxInternal
{
public:
	void Init();
	Ui::NotifyRoot* notifyUI;
	QDialog* notify;
};

NotifyBoxInternal* NotifyUtil::internal_ = NULL;

void NotifyBoxInternal::Init()
{
	notifyUI = new Ui::NotifyRoot;
	notify = new QDialog();
	notify->setWindowFlags(Qt::FramelessWindowHint);
	notify->setAttribute(Qt::WA_TranslucentBackground, true);
	notifyUI->setupUi(notify);
	notifyUI->btnOK->setVisible(false);
	QObject::connect(notifyUI->btnOK, &QPushButton::clicked, [=] {
		notify->accept();
	});
}

void NotifyUtil::Popup(QWidget* _content)
{
	if (NULL == internal_)
	{
		internal_ = new NotifyBoxInternal();
		internal_->Init();
	}

	while (QLayoutItem* item = internal_->notifyUI->layout->itemAt(0))
	{
		item->widget()->setParent(NULL);
		internal_->notifyUI->layout->removeItem(item);
	}

	if (NULL != _content)
		internal_->notifyUI->layout->addWidget(_content);

	internal_->notifyUI->btnOK->setVisible(false);
	internal_->notifyUI->line->setVisible(false);
	internal_->notify->setModal(true);
	internal_->notify->show();
}

void NotifyUtil::ActivateOKButton()
{
	internal_->notifyUI->btnOK->setVisible(true);
	internal_->notifyUI->line->setVisible(true);
}


