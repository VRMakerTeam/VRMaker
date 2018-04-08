#pragma once

class PathUtil
{
public:
	//资源包位置
	//！！！此路径必须和Unity的PersistentData路径一致
	//%USER%\AppData\LocalLow\vrmaker\vplayer
	static QString PackageLocation();

	//%USER%\AppData\Roaming\creator\thumb
	static QString ThumbLocation();

	//VRMaker的安装位置
	//VRMaker产生的数据均位于此文件夹下
	//%USER%\AppData\Roaming\creator
	static QString AppDataLocation();

	static QString TempLocation();

};