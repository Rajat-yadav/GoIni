#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GoIni_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GoIni_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef GoIni_EXPORTS
#define GoIni_API __declspec(dllexport)
#else
#define GoIni_API __declspec(dllimport)
#endif

// This is an example of a class exported from the GoIni.dll
class GoIni_API CGoIni
{
public:
    CGoIni();
    // TODO: add your methods here.
};

// This is an example of an exported variable
extern GoIni_API int nGoIni;

// This is an example of an exported function.
GoIni_API int fnGoIni(void);
