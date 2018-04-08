#pragma once
#include <QString>

class QAudioRecorder;
class QMediaPlayer;
class QByteArray;
class QBuffer;

namespace AudioUtils
{
	class Recorder
	{
	public:
		static void Start(const QString& _uuid);
		static void Stop();
	private:
		static QAudioRecorder* recorder;
	};

	class Player
	{
	public:
		static void PlayCache(const QString& _uuid);
		static void PlayPKG(const QString& _package, const QString& _file);
		static void Stop();
	private:
		static QMediaPlayer* player;
		static QByteArray* data;
		static QBuffer* buffer;
	};
}
