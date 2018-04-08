#include <QDir>
#include <QUrl>
#include <QAudioRecorder>
#include <QMediaPlayer>
#include <QBuffer>
#include <QDataStream>
#include "PathUtil.h"
#include "Utils.h"
#include "pluginBucket/BucketActions.h"
#include "PluginBucketLogger.h"
#include "AudioUtil.h"

QAudioRecorder* AudioUtils::Recorder::recorder = NULL;
QMediaPlayer* AudioUtils::Player::player = NULL;
QBuffer* AudioUtils::Player::buffer = new QBuffer();
QByteArray* AudioUtils::Player::data = new QByteArray();

void AudioUtils::Recorder::Start(const QString& _uuid)
{
	SAFE_DELETE(recorder);
	recorder = new QAudioRecorder;

	QAudioEncoderSettings audioSettings;
	audioSettings.setCodec("audio/pcm");
	audioSettings.setQuality(QMultimedia::NormalQuality);

	recorder->setEncodingSettings(audioSettings);
	QString path = QString("%1/%2").arg(QDir::current().absoluteFilePath("cache")).arg(_uuid);
	recorder->setOutputLocation(QUrl::fromLocalFile(path));
	recorder->record();
}

void AudioUtils::Recorder::Stop()
{
	if (NULL != recorder)
	{
		recorder->stop();
		delete recorder;
		recorder = NULL;
	}
}

void AudioUtils::Player::PlayCache(const QString& _uuid)
{
	SAFE_DELETE(player);
	player = new QMediaPlayer;

	QString path = QString("%1/%2.wav").arg(QDir::current().absoluteFilePath("cache")).arg(_uuid);
	player->setMedia(QUrl::fromLocalFile(path));
	player->play();
}

void AudioUtils::Player::PlayPKG(const QString& _package, const QString& _file)
{
	QString path = QString("%1/../zpackages/pkgs/any/%2.pkg").arg(PathUtil::AppDataLocation()).arg(_package);
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
		return;

	auto takeInt = [](QByteArray& _data, int _a)->int {
		if (_data.count() < _a + 4)
			return 0;

		int value = 0;
		value |= (unsigned char)(_data[_a]);
		value |= ((unsigned char)(_data[_a+1])) << 8;
		value |= ((unsigned char)(_data[_a+2])) << 16;
		value |= ((unsigned char)(_data[_a+3])) << 24;
		return value;
	};

	QByteArray datas;
	QDataStream out(&file);
	out >> datas;
	
	int pos = 0;
	int count = takeInt(datas, pos);
	pos += 4;
	for (int i = 0; i < count; ++i)
	{
		int namesize = takeInt(datas, pos);
		pos += 4;
		QString filename = QString::fromUtf8(datas.mid(pos, namesize));
		pos += namesize;
		int filesize = takeInt(datas, pos);
		pos += 4;
		if (0 != filename.compare(_file))
			continue;

		//data->clear();
		//data->append(datas.mid(pos, filesize));
		//buffer->setData(*data);
		//player->setMedia(QMediaContent(), buffer);
		//player->play();
		QString outputFilePath = QString("%1/../zpackages/tmp.wav").arg(PathUtil::AppDataLocation());
		QFile file(outputFilePath);
		if (file.open(QIODevice::WriteOnly))
		{
			file.write(datas.mid(pos, filesize));
			file.flush();
			file.close();

			SAFE_DELETE(player);
			player = new QMediaPlayer;
			player->setMedia(QUrl::fromLocalFile(outputFilePath));
			player->play();
		}
	}
}

void AudioUtils::Player::Stop()
{
	if (NULL != player)
	{
		player->stop();
		delete player;
		player = NULL;
	}
}
