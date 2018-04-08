#ifndef __CONTROLLERGLOBAL_H__
#define __CONTROLLERGLOBAL_H__

class Bean;
class Story;
class App;
class QString;
class ControllerUtil
{
public:
	static void PlayVoice(const QString& _alias);
	static void StopVoice();
	static void PlayMusic(const QString& _alias, const QString musicPath = "package");
	static void StopMusic();
};
#endif
