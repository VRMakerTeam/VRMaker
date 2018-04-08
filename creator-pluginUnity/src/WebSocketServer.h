#pragma once
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

class UnityWebSocketServerInternal;

class UnityWebSocketServer
{
public:
	int Initialize();
	void Release();

	void SendMessageToUnity(const QString& _message);
private:
	UnityWebSocketServerInternal* internal_;
};