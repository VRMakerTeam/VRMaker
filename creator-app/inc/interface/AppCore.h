#ifndef __APPCORE_H__
#define __APPCORE_H__

#include <QString>

class IPlugin;
class QWidget;
class AppCoreInternal;
class QApplication;
class SVM;

class AppCore
{
public:
	AppCore(QApplication* _application, QWidget* _rootUi);
	static void Alert(const QString& _title, const QString& _message);
	void Initialize();
	void Run();
	void Release();

	void Maximum();

	void StartModelTask();
	void FinishModelTask();

	/// \brief Preload some plugins, append them into launcherbar
	void PreloadPlugins();

	void AppendTitlebarLauncher(QWidget* _launcher);
	void AttachPanel(QWidget* _root);
	/// \param _align
	/// 0 : none
	/// 1: left
	/// 2: center
	/// 3 : right
	void AttachToolbar(QWidget* _root, int _align = 0);
	void DetachToolbar();

	void EnterPluginPage();
	void EnterHomePage();

	IPlugin* FindPreloadPlugin(const char* _plugin);

	IPlugin* CreatePlugin(const char* _plugin);
	void LaunchPlugin(IPlugin* _plugin);

	/// \return 
	/// 0 : OK
	/// 1 : the name is exists
	int RegisterCachePlugin(const char* _name, IPlugin* _plugin);
	IPlugin* FindCachePlugin(const char* _name);
	/// \brief remove it form cache
	/// \NOTE not delete the plugin
	void RemoveCachePlugin(const char* _name);

	static QWidget* CreateUI(const QString& _uiFile);
	static void LoadRCC(const QString& _resourceFile);
	static void LoadTS(const QString& _translatorFile);
	static QString Translate(const QString& _txt);
	static QString Language();

	void NotifyLoadingTip(const QString& _text);

	SVM* GetSVM();
private:
	AppCoreInternal* internal_;
};

#endif
