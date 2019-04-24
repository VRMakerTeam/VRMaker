#include <QMovie>
#include <QLabel>
#include <QTextCodec>
#include <QStackedWidget>
#include "AppCore.h"
#include "BeanNotifyContent.h"


BeanNotifyUI* BeanNotifyContent::ui_ = 0;

class BeanNotifyUI
{
public:
	QWidget* root;
	QLabel* animWait;
	QLabel* labelSuccess;
	QLabel* labelNoDevice;
	QLabel* labelCopyFail;
	QStackedWidget* pages;
};

QWidget* BeanNotifyContent::GetRoot(QString language)
{
	if (0 == ui_)
	{
		ui_ = new BeanNotifyUI();
#if 1
		if (language != NULL) {
			if (language.compare("de") == 0) {
				ui_->root = AppCore::CreateUI("beanNotifyContent_de");
			}
			else if (language.compare("jp") == 0) {
				ui_->root = AppCore::CreateUI("beanNotifyContent_jp");
			}
			else if (language.compare("kr") == 0) {
				ui_->root = AppCore::CreateUI("beanNotifyContent_kr");
			}
			else if (language.compare("it") == 0) {
				ui_->root = AppCore::CreateUI("beanNotifyContent_it");
			}
			else if (language.compare("pt") == 0) {
				ui_->root = AppCore::CreateUI("beanNotifyContent_pt");
			}
			else if (language.compare("sp") == 0) {
				ui_->root = AppCore::CreateUI("beanNotifyContent_sp");
			}
			else if (language.compare("fr") == 0) {
				ui_->root = AppCore::CreateUI("beanNotifyContent_fr");
			}
			else if (language.compare("ru") == 0) {
				ui_->root = AppCore::CreateUI("beanNotifyContent_ru");
			}
			else if (language.compare("zh_CN") == 0) {
				ui_->root = AppCore::CreateUI("beanNotifyContent_zh_CN");
			}
			else {
				ui_->root = AppCore::CreateUI("beanNotifyContent");
			}
		}
		else {
			ui_->root = AppCore::CreateUI("beanNotifyContent");
		}
#else
		ui_->root = AppCore::CreateUI("beanNotifyContent");
#endif
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

