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
#if 0
		ui_->labelSuccess = ui_->root->findChild<QLabel*>("label_5");
		QTextCodec* codec = QTextCodec::codecForName("UTF-8");

		if (ui_->labelSuccess != NULL && language != NULL) {
			if (language.compare("de") == 0) {
				ui_->labelSuccess->setText("Erfolg");
			}
			else if (language.compare("jp") == 0) {
				ui_->labelSuccess->setText(codec->toUnicode("成功"));
			}
			else if (language.compare("kr") == 0) {
				ui_->labelSuccess->setText(codec->toUnicode("성공"));
			}
			else if (language.compare("it") == 0) {
				ui_->labelSuccess->setText("successo");
			}
			else if (language.compare("pt") == 0) {
				ui_->labelSuccess->setText("sucesso");
			}
			else if (language.compare("sp") == 0) {
				ui_->labelSuccess->setText("éxito");
			}
			else if (language.compare("fr") == 0) {
				ui_->labelSuccess->setText("Succès");
			}
			else if (language.compare("ru") == 0) {
				ui_->labelSuccess->setText("успех");
			}
			else if (language.compare("zh_CN") == 0) {
				ui_->labelSuccess->setText(codec->toUnicode("成功"));
			}
			else {
				ui_->labelSuccess->setText("SUCCESS");
			}
		}
		ui_->labelNoDevice = ui_->root->findChild<QLabel*>("label");
		if (ui_->labelNoDevice != NULL && language != NULL) {
			if (language.compare("de") == 0) {
				ui_->labelNoDevice->setText("Kein Gerät gefunden");
			}
			else if (language.compare("jp") == 0) {
				ui_->labelNoDevice->setText(codec->toUnicode("デバイスが見つかりません"));
			}
			else if (language.compare("kr") == 0) {
				ui_->labelNoDevice->setText(codec->toUnicode("기기가 검색되지 않음"));
			}
			else if (language.compare("it") == 0) {
				ui_->labelNoDevice->setText("Nessun dispositivo rilevato");
			}
			else if (language.compare("pt") == 0) {
				ui_->labelNoDevice->setText("Nenhum dispositivo detectado");
			}
			else if (language.compare("sp") == 0) {
				ui_->labelNoDevice->setText("Ningún dispositivo detectado");
			}
			else if (language.compare("fr") == 0) {
				ui_->labelNoDevice->setText("Aucun appareil détecté");
			}
			else if (language.compare("ru") == 0) {
				ui_->labelNoDevice->setText("Устройств не обнаружено");
			}
			else if (language.compare("zh_CN") == 0) {
				ui_->labelNoDevice->setText(codec->toUnicode("未检测到设备"));
			}
			else {
				ui_->labelNoDevice->setText("No device detected");
			}
		}

		ui_->labelCopyFail = ui_->root->findChild<QLabel*>("label_3");
		if (ui_->labelCopyFail != NULL && language != NULL) {
			if (language.compare("de") == 0) {
				ui_->labelCopyFail->setText("Kopieren von Assets schlägt fehl");
			}
			else if (language.compare("jp") == 0) {
				ui_->labelCopyFail->setText(codec->toUnicode("コピーアセットが失敗する"));
			}
			else if (language.compare("kr") == 0) {
				ui_->labelCopyFail->setText(codec->toUnicode("복사 자산 실패"));
			}
			else if (language.compare("it") == 0) {
				ui_->labelCopyFail->setText("copia attività fallire");
			}
			else if (language.compare("pt") == 0) {
				ui_->labelCopyFail->setText("copiar recursos falhar");
			}
			else if (language.compare("sp") == 0) {
				ui_->labelCopyFail->setText("los activos de copia fallan");
			}
			else if (language.compare("fr") == 0) {
				ui_->labelCopyFail->setText("les actifs de copie échouent");
			}
			else if (language.compare("ru") == 0) {
				ui_->labelCopyFail->setText("сбой копирования активов");
			}
			else if (language.compare("zh_CN") == 0) {
				ui_->labelCopyFail->setText(codec->toUnicode("拷贝文件失败"));
			}
			else {
				ui_->labelCopyFail->setText("copy assets fail");
			}
		}
#endif
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

