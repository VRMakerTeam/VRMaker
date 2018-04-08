#pragma once

#include <functional>

class HttpUtil
{
public:
	static QByteArray Base64Encode(const QByteArray& _bytes);
	static QByteArray Base64Decode(const QByteArray& _bytes);

	static void POST(const QString& _uri, const QByteArray& _form, 
		std::function<void (const QByteArray&)> _onFinish, std::function<void(const QString&)> _onError);

	//url:  "http:// <hostIP> :17362/upload/ <deviceUUID> /<filename.doc>"
	static void PUT(const QString& _url, const QByteArray& _data,
		std::function<void(const QByteArray&)> _onFinish = Onfinish, std::function<void(const QString&)> _onError = Onerror);
protected:
private:
	static void Onfinish(QByteArray _reply);
	static void Onerror(QString _err);
};
