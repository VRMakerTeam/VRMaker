#ifndef __UTILS_H__
#define __UTILS_H__

#define SAFE_DELETE(x) if(NULL != x){ delete x; x = NULL;}
#define DYN_CAST(clazz, src, dist) clazz* dist = dynamic_cast<clazz*>(src)
#endif
