#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "HttpUtil.h"

namespace HttpUtilInternal
{
	struct Callback
	{
		std::function<void(const QByteArray&)> onFinish;
		std::function<void(const QString&)> onError;
	};
	QHash<QNetworkReply*, Callback> callbacks;
	QNetworkAccessManager* mgr;
}

//-----------------------------------------------------------------------
QByteArray HttpUtil::Base64Encode(const QByteArray& _bytes)
{
	return _bytes.toBase64(QByteArray::Base64UrlEncoding);
}

//-----------------------------------------------------------------------
QByteArray HttpUtil::Base64Decode(const QByteArray& _bytes)
{
	 return QByteArray::fromBase64(_bytes, QByteArray::Base64UrlEncoding);
}

//-----------------------------------------------------------------------
void HttpUtil::POST(const QString& _uri, const QByteArray& _form, std::function<void(const QByteArray&)> _onFinish, std::function<void(const QString&)> _onError)
{
	static bool isInit = false;
	if (!isInit)
	{
		HttpUtilInternal::mgr = new QNetworkAccessManager(NULL);
		QObject::connect(HttpUtilInternal::mgr, &QNetworkAccessManager::finished, [&](QNetworkReply* _reply) {
			if (!HttpUtilInternal::callbacks.contains(_reply))
				return;
			HttpUtilInternal::Callback callback = HttpUtilInternal::callbacks.value(_reply);

			QVariant statusCodeV = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
			if (200 != statusCodeV.toInt())
			{
				if (NULL != callback.onError)
					callback.onError(QString("status code %1").arg(statusCodeV.toInt()));
			}
			else
			{
				if (_reply->error() == QNetworkReply::NoError)
				{
					if (NULL != callback.onFinish)
						callback.onFinish(_reply->readAll());
				}
				else
				{
					if (NULL != callback.onError)
						callback.onError(_reply->errorString());
				}
			}
			
			HttpUtilInternal::callbacks.remove(_reply);
			_reply->deleteLater();
		});
		isInit = true;
	}

	HttpUtilInternal::Callback callback;
	callback.onFinish = _onFinish;
	callback.onError = _onError;
	QUrl uri(_uri);
	QNetworkReply* reply = HttpUtilInternal::mgr->post(QNetworkRequest(uri), _form);
	HttpUtilInternal::callbacks.insert(reply, callback);
}


void HttpUtil::PUT(const QString& _url, const QByteArray& _data, std::function<void(const QByteArray&)> _onFinish, std::function<void(const QString&)> _onError)
{
	static bool isInit = false;
	if (!isInit)
	{
		HttpUtilInternal::mgr = new QNetworkAccessManager(NULL);
		QObject::connect(HttpUtilInternal::mgr, &QNetworkAccessManager::finished, [&](QNetworkReply* _reply) {
			if (!HttpUtilInternal::callbacks.contains(_reply))
				return;
			HttpUtilInternal::Callback callback = HttpUtilInternal::callbacks.value(_reply);

			QVariant statusCodeV = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
			if (200 != statusCodeV.toInt())
			{
				if (NULL != callback.onError)
					callback.onError(QString("status code %1").arg(statusCodeV.toInt()));
			}
			else
			{
				if (_reply->error() == QNetworkReply::NoError)
				{
					if (NULL != callback.onFinish)
						callback.onFinish("upload finish...");
				}
				else
				{
					if (NULL != callback.onError)
						callback.onError(_reply->errorString());
				}
			}

			HttpUtilInternal::callbacks.remove(_reply);
			_reply->deleteLater();
		});
		isInit = true;
	}

	HttpUtilInternal::Callback callback;
	callback.onFinish = _onFinish;
	callback.onError = _onError;
	QUrl url(_url);
	QNetworkReply* reply = HttpUtilInternal::mgr->put(QNetworkRequest(url), _data);
	HttpUtilInternal::callbacks.insert(reply, callback);
}


void HttpUtil::Onfinish(QByteArray _reply)
{
	qDebug() << _reply;
}

void HttpUtil::Onerror(QString _err)
{
	qDebug() << _err;
}
