#include <QApplication>
#include <QMessageBox>
#include "App.h"
#include <ConfigUtil.h>

int main(int argc, char *argv[])
{
	QCoreApplication::addLibraryPath("plugins");

	QApplication a(argc, argv);

	if (0 != ConfigUtil::Initialization())
	{
		QMessageBox::warning(NULL, "Error", "load config failed!");
		return 0;
	}

	App app;
	app.Initialize();
	int result = app.Execute(&a);
	app.Release();
	return result;
}
