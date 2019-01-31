#include <QMainWindow>
#include <QPushButton>
#include <QFontDatabase>
#include <QApplication>
#include <QDebug>
#include <QComboBox>
#include "HeaderWidget.h"
#include "Utils.h"
#include "AppCore.h"
#include "App.h"
#include "ConfigUtil.h"

class AppInternal
{
public:
	void Initialize(QApplication* _application);
	void Run();
	void Release();
private:
	QMainWindow* mainWindow;
	AppCore* appCore;
};

//-----------------------------------------------------------------------
void AppInternal::Initialize(QApplication* _application)
{
	AppCore::LoadTS("app");
#if 0
	QString language = appCore->Language();
	if (language.compare("de") == 0) {
		AppCore::LoadRCC("app_de");
	}
	else if (language.compare("jp") == 0) {
		AppCore::LoadRCC("app_jp");
	}
	else if (language.compare("kr") == 0) {
		AppCore::LoadRCC("app_kr");
	}
	else if (language.compare("it") == 0) {
		AppCore::LoadRCC("app_it");
	}
	else if (language.compare("pt") == 0) {
		AppCore::LoadRCC("app_pt");
	}
	else if (language.compare("sp") == 0) {
		AppCore::LoadRCC("app_sp");
	}
	else if (language.compare("fr") == 0) {
		AppCore::LoadRCC("app_fr");
	}
	else if (language.compare("ru") == 0) {
		AppCore::LoadRCC("app_ru");
	}
	else if (language.compare("zh_CN") == 0) {
		AppCore::LoadRCC("app_zh_CN");
	}
	else {
		AppCore::LoadRCC("app");
	}
#else
	AppCore::LoadRCC("app");
#endif

	//load Font
	int loadedFontID = QFontDatabase::addApplicationFont(":/app/fonts/default");
	QStringList strList(QFontDatabase::applicationFontFamilies(loadedFontID));
	QFont fontThis(strList.at(0));
	_application->setFont(fontThis);

	mainWindow = dynamic_cast<QMainWindow*>(AppCore::CreateUI("app"));
	mainWindow->setWindowFlags(Qt::FramelessWindowHint);

	HeaderWidget* titlebar = mainWindow->findChild<HeaderWidget*>("__titlebar");
	titlebar->SetWindow(mainWindow);

	appCore = new AppCore(_application, mainWindow);
	appCore->Initialize();

	appCore->NotifyLoadingTip("connect app close button");
	QPushButton* btnClose = mainWindow->findChild<QPushButton*>("__btn_close");
	QObject::connect(btnClose, &QPushButton::clicked, [=] {
		QApplication::quit();
	});
	appCore->NotifyLoadingTip("connect app minimize button");
	QPushButton* btnMinimize = mainWindow->findChild<QPushButton*>("__btn_minimize");
	QObject::connect(btnMinimize, &QPushButton::clicked, [=] {
		if (mainWindow->isMaximized())
			mainWindow->showNormal();
		else
			mainWindow->showMaximized();
	});
	appCore->NotifyLoadingTip("connect go home button");
	QPushButton* btnGoHome = mainWindow->findChild<QPushButton*>("__btnGoHome");
	QObject::connect(btnGoHome, &QPushButton::clicked, [=] {
		appCore->EnterHomePage();
	});

	appCore->NotifyLoadingTip("connect language button");
	QComboBox* comboLanguage = mainWindow->findChild<QComboBox*>("__combo_language");
	comboLanguage->setCurrentIndex(ConfigUtil::GetLanguage());
	QObject::connect(comboLanguage, &QComboBox::currentTextChanged, [=] {
		appCore->ChangeLanguage(comboLanguage->currentIndex());
	});

}


//-----------------------------------------------------------------------
void AppInternal::Run()
{
	mainWindow->show();
	appCore->Run();
	appCore->NotifyLoadingTip("app internal run finish");

}

void AppInternal::Release()
{
	appCore->Release();
	SAFE_DELETE(appCore);
}

App::App()
{
}

App::~App()
{
    
}

void App::Initialize()
{
	internal_ = new AppInternal();
}

int App::Execute(QApplication* _application)
{
	internal_->Initialize(_application);
	internal_->Run();
	return _application->exec();
}

void App::Release()
{
	internal_->Release();
}
