#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <iostream>
#include "mainwindow.h"
#include "RunGuard.h"

int main(int argc, char *argv[]) {
    try {
        RunGuard guard("time_tracker");

        if (!guard.tryToRun()) {
            qWarning("Another process is running");
            return -1;
        }

        QApplication app(argc, argv);
        auto translator = new QTranslator(&app);
        auto translatorQtBase = new QTranslator(&app);

        translator->load(QLocale::system(), QString(":/lang/application"), QString("_"));
        QApplication::installTranslator(translator);
        translatorQtBase->load(QLocale::system(), QString(":/lang/qtbase"), QString("_"));
        QApplication::installTranslator(translatorQtBase);

        QApplication::setQuitOnLastWindowClosed(false);
        MainWindow mainWindow;
        //mainWindow.show();
        return QApplication::exec();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}