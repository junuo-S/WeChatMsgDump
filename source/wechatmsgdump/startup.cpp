#include "msgmain/entry.h"

#include <windows.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    return msgMainExec(0, nullptr);
}
