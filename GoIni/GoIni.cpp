// GoIni.cpp : Defines the exported functions for the DLL application.
//

#include "GoIni.h"

// This is an example of an exported variable
GoIni_API int nGoIni=0;

// This is an example of an exported function.
GoIni_API int fnGoIni(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see GoIni.h for the class definition
CGoIni::CGoIni()
{
    return;
}
