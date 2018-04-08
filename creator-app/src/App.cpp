#include <QMainWindow>
#include <QPushButton>
#include <QFontDatabase>
#include <QApplication>
#include <QDebug>
#include "HeaderWidget.h"
#include "Utils.h"
#include "AppCore.h"
#include "App.h"

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

	//load Font
	//int loadedFontID = QFontDatabase::addApplicationFont(":/CN/SourceHanSansCN-Medium.otf");
	//QStringList strList(QFontDatabase::applicationFontFamilies(loadedFontID));
	//QFont fontThis(strList.at(0));
	//_application->setFont(fontThis);

	AppCore::LoadTS("app");
	AppCore::LoadRCC("app");
	mainWindow = dynamic_cast<QMainWindow*>(AppCore::CreateUI("app"));
	mainWindow->setWindowFlags(Qt::FramelessWindowHint);

	HeaderWidget* titlebar = mainWindow->findChild<HeaderWidget*>("__titlebar");
	titlebar->SetWindow(mainWindow);

	appCore = new AppCore(_application, mainWindow);
	appCore->Initialize();

	QPushButton* btnClose = mainWindow->findChild<QPushButton*>("__btn_close");
	QObject::connect(btnClose, &QPushButton::clicked, [=] {
		QApplication::quit();
	});
	QPushButton* btnMinimize = mainWindow->findChild<QPushButton*>("__btn_minimize");
	QObject::connect(btnMinimize, &QPushButton::clicked, [=] {
		if (mainWindow->isMaximized())
			mainWindow->showNormal();
		else
			mainWindow->showMaximized();
	});
	QPushButton* btnGoHome = mainWindow->findChild<QPushButton*>("__btnGoHome");
	QObject::connect(btnGoHome, &QPushButton::clicked, [=] {
		appCore->EnterHomePage();
	});

}

//-----------------------------------------------------------------------
void AppInternal::Run()
{
	mainWindow->show();
	appCore->Run();
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
