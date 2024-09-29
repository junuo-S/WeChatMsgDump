#include "entry.h"

#include <QApplication>
#include <QTranslator>
#include <QDir>

#include "msgmanager/msgmanager.h"

int MSGMAIN_EXPORT msgMainExec(int argc, char** argv)
{
	QApplication app(argc, argv);
	auto exePath = QCoreApplication::applicationDirPath();
	QDir currentDir(exePath);
	currentDir.cdUp();
	currentDir.cd("translation");
	QTranslator translator;
	bool b = translator.load(currentDir.absoluteFilePath("msgmain_zh_CN.qm"));
	Q_ASSERT(b);
	app.installTranslator(&translator);
	QObject::connect(qApp, &QApplication::aboutToQuit, MsgManager::instance(), &QObject::deleteLater);
	MsgManager::instance()->startWork();
	return app.exec();
}

