#pragma once

class QWidget;

class BeanNotifyUI;

class BeanNotifyContent
{
public:
	static QWidget* GetRoot(QString language = NULL);
	static void SwitchWait();
	static void SwitchOK();
	static void SwitchNoneDevice();
	static void SwitchCopyFailed();
private:
	static BeanNotifyUI* ui_;
};
