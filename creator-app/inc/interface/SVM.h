#ifndef __SVM_H__
#define __SVM_H__

#include <QString>

class SVMInternal;

/// \brief Script Vitual machine
class SVM
{
public:
	static void InitConsole();
	static void LogTrace(const QString& _message);
	static void LogDebug(const QString& _message);
	static void LogInfo(const QString& _message);
	static void LogWarn(const QString& _message);
	static void LogError(const QString& _message);

	void Init();
	void Run();
	void Release();

	void Invoke(const QString& _method);
	void Execute(const QString& _file);

private:
	SVMInternal* internal_;
};

#endif //__SVM_H__