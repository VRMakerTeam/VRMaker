#include <QObject>
#include "PluginUnityLogger.h"
#include "PluginUnity/UnityActions.h"
#include "WebSocketServer.h"
class UnityWebSocketServerInternal : public QObject
{
public:
	QWebSocketServer* server;
	QWebSocket* client;
	void onNewConnection();
	void messageFromUnity(const QString& _message);
	void socketDisconnected();
};

//-----------------------------------------------------------------------
void UnityWebSocketServerInternal::onNewConnection()
{
	if (NULL != client)
	{
		client->close();
	}

	client = server->nextPendingConnection();

	connect(client, &QWebSocket::textMessageReceived, this, &UnityWebSocketServerInternal::messageFromUnity);
	connect(client, &QWebSocket::disconnected, this, &UnityWebSocketServerInternal::socketDisconnected);
}

//-----------------------------------------------------------------------
void UnityWebSocketServerInternal::messageFromUnity(const QString& _message)
{
	LOGX_DEBUG(XC_PluginUnity, XL_MEDIMU_PluginUnity, QString("receive message from unity: %1").arg(_message));

	QString message = _message;
	// get method
	int pos = message.indexOf('|');
	if (pos < 0)
		return;
	QString method = message.left(pos);
	//get activity
	message = pos < message.size() - 1 ? message.remove(0, pos+1) : "";
	pos = message.indexOf('|');
	if (pos < 0)
		return;
	QString activity = message.left(pos);
	//get param
	QString param = pos < message.size() - 1 ? message.remove(0, pos+1) : "";

	QWebSocket *pSender = static_cast<QWebSocket *>(sender());
	NEW_ACTION(UnityActions::WSMessageFromUnityAction, action);
	action->method= method;
	action->activity = activity;
	action->param = param;
	INVOKE_ACTION(UnityActions::WSMessageFromUnityAction,action);
}

//-----------------------------------------------------------------------
void UnityWebSocketServerInternal::socketDisconnected()
{
	QWebSocket* client = qobject_cast<QWebSocket *>(sender());
	if (client)
	{
		client->disconnect();
		client->deleteLater();
	}
}

//-----------------------------------------------------------------------
int UnityWebSocketServer::Initialize()
{
	internal_ = new UnityWebSocketServerInternal();
	internal_->client = NULL;
	internal_->server = new QWebSocketServer("Chat Server", QWebSocketServer::NonSecureMode, NULL);
	if (!internal_->server->listen(QHostAddress::Any, 33168))
		return 1;

	QObject::connect(internal_->server, &QWebSocketServer::newConnection,
		internal_, &UnityWebSocketServerInternal::onNewConnection);
	return 0;
}

//-----------------------------------------------------------------------
void UnityWebSocketServer::Release()
{
	internal_->server->close();
}

//-----------------------------------------------------------------------
void UnityWebSocketServer::SendMessageToUnity(const QString& _message)
{
	LOGX_DEBUG(XC_PluginUnity, XL_MEDIMU_PluginUnity, QString("send message to unity: %1").arg(_message));
	if (NULL == internal_->client)
	{
		LOGX_ERROR(XC_PluginUnity, XL_HIGH_PluginUnity, "client is null");
		return;
	}
	if (!internal_->client->isValid())
	{
		LOGX_ERROR(XC_PluginUnity, XL_HIGH_PluginUnity, "client is invalid");
		return;
	}
	internal_->client->sendTextMessage(_message);
}
