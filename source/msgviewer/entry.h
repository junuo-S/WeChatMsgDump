#pragma once

#ifdef _MSGVIEWER_MODULE_
#define MSGVIEWER_EXPORT __declspec(dllexport)
#else
#define MSGVIEWER_EXPORT __declspec(dllimport)
#endif // _MSGVIEWER_MODULE_

int MSGVIEWER_EXPORT msgViewerExec(int argc, char** argv);
