#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include <QTime>
#include <QDir>
#include "mainwindow.h"
#include "RunGuard.h"
#include "config.h"
#include "DataBase.h"
#include "helpers/FileHelper.h"
#include "Utils.h"
#include "main.h"

int main(int argc, char *argv[]) {
    //qDebug() << Q_FUNC_INFO  << (QTime::currentTime()).toString("HH:mm:ss");
    RunGuard guard(QString(APP_NAME));
    QApplication app(argc, argv);
    QApplication::setApplicationName(QString(APP_NAME));
    QApplication::setApplicationVersion(QString(APP_VERSION));
    QString version = QCoreApplication::applicationVersion();
    QString pkgVersion = QString(APP_REVISION);

    logFile.reset(new QFile(FileHelper::getLogFile()));
    logFile.data()->open(QFile::WriteOnly | QFile::Append | QFile::Text);
    qInstallMessageHandler(messageHandler);

    qInfo() << QString("Start Version: %1 Package revision: %2").arg(version, pkgVersion);
    if (!guard.tryToRun()) {
        qCritical() << "Another process is running";
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QObject::tr("%1 is already running.").arg(APP_NAME) + "\n" +
                       QObject::tr("It is allowed to run only one instance."));
        msgBox.exec();
        return -1;
    }

    QTranslator translator(&app);
    QTranslator translatorQtBase(&app);
    translator.load(QLocale::system(), QString(":/lang/application"), QString("_"));
    QApplication::installTranslator(&translator);
    translatorQtBase.load(QLocale::system(), QString(":/lang/qtbase"), QString("_"));
    QApplication::installTranslator(&translatorQtBase);
    QApplication::setQuitOnLastWindowClosed(false);
    DataBase db;
    Utils utils;
    utils.backupDatabase();

    if (!db.connectToDataBase()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QObject::tr("Failed to connect to the database."));
        msgBox.exec();
        return 1;
    }

    MainWindow mainWindow(&db);
    return QApplication::exec();
}

void messageHandler(QtMsgType t, const QMessageLogContext &context, const QString &msg) {
    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString type;
    QTextStream out(logFile.data());

    switch (t) {
        case QtDebugMsg:
            type = "DEBUG";
            break;
        case QtInfoMsg:
            type = "INFO";
            break;
        case QtWarningMsg:
            type = "WARNING";
            break;
        case QtCriticalMsg:
            type = "CRITICAL";
            break;
        case QtFatalMsg:
            type = "FATAL";
            break;
    }

    out << QString("%1 %2: [%3] %4\n").arg(dateTime, APP_NAME, type, msg);
    out.flush();
}
