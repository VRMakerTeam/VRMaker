#include <assert.h>
#include <QWidget>
#include <QMediaPlayer>
#include <QDir>
#include <QString>
#include "ControllerGlobal.h"
#include "PackageManager.h"

QMediaPlayer* gMediaPlayer = NULL;

void ControllerUtil::PlayMusic(const QString& _alias, const QString musicPath)
{
	/*
	QDir dir = QDir::current();
	dir.cd(musicPath);
	const QString& package = PackageManager::GetMusicPackage(_alias);
	QString path = dir.filePath(package);
	path.append("\/");
	path.append(_alias);
	gMediaPlayer = new QMediaPlayer;
	gMediaPlayer->stop();
	gMediaPlayer->setMedia(QUrl::fromLocalFile(path));
	gMediaPlayer->play();
	qDebug() << "media : " << gMediaPlayer->state() << endl;
	*/
}

void ControllerUtil::StopMusic()
{
	//gMediaPlayer->stop();
}

void ControllerUtil::PlayVoice(const QString& _alias)
{
	//QDir dir = QDir::current();
	//dir.cd("package");
	//const QString& package = PackageManager::GetVoicePackage(_alias);
	//QString path = dir.filePath(package);
	//path.append(".wav");
	/*gMediaPlayer->stop();
	gMediaPlayer->setMedia(QUrl::fromLocalFile(path));
	gMediaPlayer->setVolume(100);
	gMediaPlayer->play();*/	
}

void ControllerUtil::StopVoice()
{
	/*gMediaPlayer->stop();*/
}
