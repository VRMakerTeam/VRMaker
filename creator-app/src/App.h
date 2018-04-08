#ifndef APP_H
#define APP_H

class QApplication;
class AppCore;

class AppInternal;

class App
{
public:
    explicit App();
    ~App();
	void Initialize();
    int Execute(QApplication* _application);
	void Release();
private:
	AppInternal* internal_;
};

#endif // APP_H
