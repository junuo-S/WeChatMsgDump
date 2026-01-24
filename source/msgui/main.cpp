#include <QDir>
#include <QTranslator>
#include <QLibrary>

#include "msgapplication.h"

static void attachTranslator(MsgApplication& app)
{
	auto exePath = QCoreApplication::applicationDirPath();
	QDir currentDir(exePath);
	currentDir.cdUp();
	currentDir.cd("translation");
	QTranslator* translator = new QTranslator(&app);
	bool b = translator->load(currentDir.absoluteFilePath("msgmain_zh_CN.qm"));
	Q_ASSERT(b);
	app.installTranslator(translator);
}

extern "C" __declspec(dllexport)
int MsgMainExec(int argc, char** argv)
{
	QLibrary lib("msgcore.dll");
	lib.load();
	auto getCoreEntryFunc = reinterpret_cast<IJCoreEntry*(*)()>(lib.resolve("GetCoreEntry"));
	if (!getCoreEntryFunc)
		return -1;
	MsgApplication app(argc, argv);
	attachTranslator(app);
	app.init(getCoreEntryFunc());
	app.startWork();
	return app.exec();
}