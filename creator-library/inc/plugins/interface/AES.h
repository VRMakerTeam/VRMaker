#pragma once

#include <QByteArray>

class AES
{
public:
	static QByteArray aesEncrypt(const QByteArray& _plaintext, const QByteArray& _secret);
	static QByteArray aesDecrypt(const QByteArray& _ciphertext, const QByteArray& _secret);
	static QByteArray aesDecryptFile(const QString& _filepath, const QByteArray& _secret);
};