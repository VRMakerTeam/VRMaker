#pragma once

class QByteArray;
class QUdpSocket;
class QHostAddress;

#define SERVER_PORT 16600
#define CLIENT_PORT 18800

class UDPUtil
{
public:
	static QString deviceUUID_;
	static QString hostIP_;
	static int Initialize();
	static QString Host();

private:
	enum MsgID
	{
		ONLINE = 10,
		OFFLINE,
		HOSTIP_REQ = 20,
		HOSTIP_REP,
	}msgID;


	static QUdpSocket* udpsocket_;

	static void receive();
	static void processMessage(QByteArray _bytes, QHostAddress _sender, quint16 _senderPort);
	static QString generateDeviceUUID();

	static void online();
	static void offline();
	static void request_hostIP();
	
	static QByteArray intToByte(int _num);
	static int bytesToInt(QByteArray _bytes);
	static QString getCPUID1();
	static QString getCPUID2();
};
