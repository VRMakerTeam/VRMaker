#pragma once

#define APP_BUNDLE "com.vrmaker.vplayer"

#define ANDROID_DEVICE_PERSISTENT_PATH "/mnt/sdcard/Android/data/"APP_BUNDLE"/files"
#define PC_DEVICE_PERSISTENT_PATH QString("%1/AppData/LocalLow/vrmaker/vplayer").arg(QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation))
#define EMBEDDED_STREAM_PATH QString("player/editor_web/StreamingAssets")

// Plugin Bean
#define STORAGE_URI_PUSHSTRING "http://internal.zyncho.com:8000/storage/pushstring"
#define STORAGE_BUCKET "vcreator:v1:beans"