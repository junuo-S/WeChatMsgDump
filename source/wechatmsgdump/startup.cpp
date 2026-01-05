#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    HMODULE msgMainHandle = LoadLibraryA("msgmain.dll");
    if (msgMainHandle == NULL)
        return -1;
	using MsgMainExecFunc = int(*)(int, char**);
    MsgMainExecFunc MsgMainExec = reinterpret_cast<MsgMainExecFunc>(GetProcAddress(msgMainHandle, "MsgMainExec"));
    if (!MsgMainExec)
		return -1;
    return MsgMainExec(0, nullptr);
}
