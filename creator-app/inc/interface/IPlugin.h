#ifndef __IPLUGIN_H__
#define __IPLUGIN_H__

class AppCore;

class IPlugin
{
public:
	virtual void Initialize(AppCore* _appCore) = 0;
	virtual void Attach() = 0;
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual void Detach() = 0;
	virtual void Release() = 0;
};

#endif
