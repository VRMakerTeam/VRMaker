#pragma once

class PathUtil
{
public:
	//��Դ��λ��
	//��������·�������Unity��PersistentData·��һ��
	//%USER%\AppData\LocalLow\vrmaker\vplayer
	static QString PackageLocation();

	//%USER%\AppData\Roaming\creator\thumb
	static QString ThumbLocation();

	//VRMaker�İ�װλ��
	//VRMaker���������ݾ�λ�ڴ��ļ�����
	//%USER%\AppData\Roaming\creator
	static QString AppDataLocation();

	static QString TempLocation();

};