#include <QDir>
#include <QTranslator>
#include <QLibrary>

#include "msgapplication.h"
#include "msgmanager/msgmanager.h"

extern "C" __declspec(dllexport)
int MsgMainExec(int argc, char** argv)
{
	QLibrary lib("msgcore.dll");
	lib.load();
	auto getCoreEntryFunc = reinterpret_cast<IJCoreEntry*(*)()>(lib.resolve("GetCoreEntry"));
	if (!getCoreEntryFunc)
		return -1;
	MsgApplication app(argc, argv);
	app.init(getCoreEntryFunc());
	auto exePath = QCoreApplication::applicationDirPath();
	QDir currentDir(exePath);
	currentDir.cdUp();
	currentDir.cd("translation");
	QTranslator translator;
	bool b = translator.load(currentDir.absoluteFilePath("msgmain_zh_CN.qm"));
	Q_ASSERT(b);
	app.installTranslator(&translator);
	MsgManager msgManager;
	msgManager.startWork();
	return app.exec();
}