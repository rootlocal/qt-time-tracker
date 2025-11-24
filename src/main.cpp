#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include <QTime>
#include "mainwindow.h"
#include "RunGuard.h"
#include "config.h"

int main(int argc, char *argv[]) {
    try {
        //qDebug() << Q_FUNC_INFO  << (QTime::currentTime()).toString("HH:mm:ss");
        RunGuard guard("time_tracker");
        QApplication app(argc, argv);
        QApplication::setApplicationName(QString("time_tracker"));
        QApplication::setApplicationVersion(QString(APP_VERSION));

        QString version = QCoreApplication::applicationVersion();
        qDebug() << "Version: " << version;
        QString pkgVersion = QString(APP_REVISION);
        qDebug() << "Package version: " << pkgVersion;

        if (!guard.tryToRun()) {
            qWarning("Another process is running");
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(QObject::tr("time_tracker is already running.") + "\n" +
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
        MainWindow mainWindow;
        return QApplication::exec();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}