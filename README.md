# 初始化环境
- 设置环境变量QT_ROOT，例如C:\qt-580-msvc2015-x86\5.8\msvc2015.
- 设置环境变量VRMAKER_ROOT，指向VRMaker文件夹.
- 执行init.bat创建运行目录.

# 编译资源

- 使用Unity打开pkg-maker
- 使用菜单栏的BuildTools/AssetBundle/WebGL编译Web平台的资源
- 使用菜单栏的BuildTools/AssetBundle/Win32编译Windows平台的资源
- 使用菜单栏的BuildTools/AssetBundle/Android编译Android平台的资源
- 使用菜单栏的BuildTools/Manifest编译清单文件

# 编译VPlayer

- 使用Unity打开player-app/unitysln/player
- 使用菜单栏的ReleaseTool编译各平台的Player

# 编译VRMaker
- 执行build.bat编译所有文件.

# 运行VRMaker
- 执行run.bat运行VRMaker.

