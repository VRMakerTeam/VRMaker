#include <QMovie>
#include <QLabel>
#include <QStackedWidget>
#include "AppCore.h"
#include "BeanNotifyContent.h"


BeanNotifyUI* BeanNotifyContent::ui_ = 0;

class BeanNotifyUI
{
public:
	QWidget* root;
	QLabel* animWait;
	QStackedWidget* pages;
};

QWidget* BeanNotifyContent::GetRoot()
{
	if (0 == ui_)
	{
		ui_ = new BeanNotifyUI();
		ui_->root = AppCore::CreateUI("beanNotifyContent");
		ui_->animWait = ui_->root->findChild<QLabel*>("__animWait");
		QMovie* movie = new QMovie(":/pluginBean/anim/wait");
		ui_->animWait->setMovie(movie);
		movie->start();
		ui_->pages = ui_->root->findChild<QStackedWidget*>("__pages");
	}
	ui_->pages->setCurrentIndex(0);
	return ui_->root;
}

void BeanNotifyContent::SwitchWait()
{
	if (0 == ui_)
		return;
	ui_->pages->setCurrentIndex(0);
}

void BeanNotifyContent::SwitchOK()
{
	if (0 == ui_)
		return;
	ui_->pages->setCurrentIndex(1);
}

void BeanNotifyContent::SwitchNoneDevice()
{
	if (0 == ui_)
		return;
	ui_->pages->setCurrentIndex(2);
}

void BeanNotifyContent::SwitchCopyFailed()
{
	if (0 == ui_)
		return;
	ui_->pages->setCurrentIndex(3);
}

