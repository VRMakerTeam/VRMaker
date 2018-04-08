extern "C" {
#include "3rd/lua/lua.h"
#include "3rd/lua/lualib.h"
#include "3rd/lua/lauxlib.h"
}

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QColor>
#include <QDebug>
#include "Utils.h"
#include "ConfigUtil.h"

#include "API/CoreAPI.h"
#include "API/DiagramAPI.h"
#include "API/CanvasAPI.h"
#include "LogX.h"
#include "SVM.h"

//////////////////////////////////////////////////////////////////////////
//
// Logger Module
//
//////////////////////////////////////////////////////////////////////////
XH_CUSTOM(SVM, ROOT);


static int lua_trace(lua_State* _state)
{
	const char* msg = lua_tostring(_state, 1);
	SVM::LogTrace(msg);
	return 0;
}

static int lua_debug(lua_State* _state)
{
	const char* msg = lua_tostring(_state, 1);
	SVM::LogDebug(msg);
	return 0;
}

static int lua_info(lua_State* _state)
{
	const char* msg = lua_tostring(_state, 1);
	SVM::LogInfo(msg);
	return 0;
}


static int lua_warn(lua_State* _state)
{
	const char* msg = lua_tostring(_state, 1);
	SVM::LogWarn(msg);
	return 0;
}


static int lua_error(lua_State* _state)
{
	const char* msg = lua_tostring(_state, 1);
	SVM::LogError(msg);
	return 0;
}

static const struct luaL_Reg log_lib[] = {
	{ "trace", lua_trace },
	{ "debug", lua_debug },
	{ "info", lua_info },
	{ "warn", lua_warn },
	{ "err", lua_error },
	{ NULL, NULL },
};

int luaopen_log(lua_State* _state)
{
	luaL_newlib(_state, log_lib);
	return 1;
}

//////////////////////////////////////////////////////////////////////////

class SVMInternal
{
public:
	lua_State* lvm;
	static QListWidget* console;
};

QListWidget* SVMInternal::console = NULL;


void SVM::InitConsole()
{
	bool openConsole = false;
	if (!ConfigUtil::FindBool("svm.console", openConsole))
		openConsole = false;

	QWidget* w = new QWidget();
	w->setWindowTitle("SVM Console");
	w->setStyleSheet("QListWidget {  background-color: rgb(39, 40, 34); font: 12pt \"Consolas\";} ");
	w->resize(QSize(460, 680));
	QVBoxLayout* layout = new QVBoxLayout(w);
	w->setLayout(layout);
	SVMInternal::console = new QListWidget(w);
	layout->addWidget(SVMInternal::console);
	if(openConsole)
		w->show();
}

void SVM::LogTrace(const QString& _message)
{
	QListWidgetItem* txt = new QListWidgetItem();
	txt->setText(_message);
	txt->setTextColor(QColor::fromRgb(102, 217, 239));
	SVMInternal::console->addItem(txt);
}

void SVM::LogDebug(const QString& _message)
{
	QListWidgetItem* txt = new QListWidgetItem();
	txt->setText(_message);
	txt->setTextColor(QColor::fromRgb(25, 165, 233));
	SVMInternal::console->addItem(txt);
}

void SVM::LogInfo(const QString& _message)
{
	QListWidgetItem* txt = new QListWidgetItem();
	txt->setText(_message);
	txt->setTextColor(QColor::fromRgb(166, 226, 46));
	SVMInternal::console->addItem(txt);
}

void SVM::LogWarn(const QString& _message)
{
	QListWidgetItem* txt = new QListWidgetItem();
	txt->setText(_message);
	txt->setTextColor(QColor::fromRgb(255, 128, 0));
	SVMInternal::console->addItem(txt);
}

void SVM::LogError(const QString& _message)
{
	QListWidgetItem* txt = new QListWidgetItem();
	txt->setText(_message);
	txt->setTextColor(QColor::fromRgb(249,38,114));
	SVMInternal::console->addItem(txt);
}

void SVM::Init()
{
	internal_ = new SVMInternal();

	

	internal_->lvm = luaL_newstate();
	/*load Lua base libraries*/
	luaL_openlibs(internal_->lvm);

	luaL_requiref(internal_->lvm, "log", luaopen_log, 1);
	lua_pop(internal_->lvm, 1);

	CoreAPI::RegisterModule(internal_->lvm);
	DiagramAPI::RegisterModule(internal_->lvm);
	CanvasAPI::RegisterModule(internal_->lvm);
}

void SVM::Run()
{
	QString svmfotmat = "";
	if (!ConfigUtil::FindString("svm.format", svmfotmat))
		svmfotmat = "lua";
	if (luaL_dofile(internal_->lvm, QString("svm/app.%1").arg(svmfotmat).toStdString().data()))
		qDebug() << lua_tostring(internal_->lvm, -1);
}

void SVM::Release()
{
	lua_close(internal_->lvm);

	SAFE_DELETE(internal_)
}

void SVM::Invoke(const QString& _method)
{
	lua_getglobal(internal_->lvm, _method.toStdString().data());
	lua_pcall(internal_->lvm, 0, 0, 0);
}

void SVM::Execute(const QString& _file)
{
	if (luaL_dofile(internal_->lvm, _file.toStdString().data()))
		SVM::LogError(lua_tostring(internal_->lvm, -1));
}
