#pragma once

#ifdef _MSGMAIN_MODULE_
#define MSGMAIN_EXPORT __declspec(dllexport)
#else
#define MSGMAIN_EXPORT __declspec(dllimport)
#endif // _MSGMAIN_MODULE_

int MSGMAIN_EXPORT msgMainExec(int argc, char** argv);
