#include "entry.h"

#include <QApplication>
#include <QTranslator>
#include <QDir>

#include "msgmanager/msgmanager.h"

int MSGVIEWER_EXPORT msgViewerExec(int argc, char** argv)
{
	QApplication app(argc, argv);
	auto exePath = QCoreApplication::applicationDirPath();
	QDir currentDir(exePath);
	currentDir.cdUp();
	currentDir.cd("translation");
	QTranslator translator;
	translator.load(currentDir.absoluteFilePath("msgviewer_zh_CN.qm"));
	app.installTranslator(&translator);
	QObject::connect(qApp, &QApplication::aboutToQuit, MsgManager::instance(), &QObject::deleteLater);
	MsgManager::instance()->startWork();
	return app.exec();
}

