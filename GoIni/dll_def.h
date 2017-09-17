#ifndef _DLL_DEF_H
#define _DLL_DEF_H

#ifdef GO_BUILD
#define GOCALL _declspec(dllexport)
#else
#define GOCALL _declspec(dllimport)
#endif 

#endif // _DLL_DEF_H
