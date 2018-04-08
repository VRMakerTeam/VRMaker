#pragma once

class QWidget; 
class NotifyBoxInternal;

class NotifyUtil
{
public:
	static void Popup(QWidget* _content);
	static void ActivateOKButton();
private:
	static NotifyBoxInternal* internal_;
};
