#include <QApplication>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    auto translator = new QTranslator(&app);
    translator->load(QLocale::system(), QString(":/lang/application"), QString("_"));
    QApplication::installTranslator(translator);
    QApplication::setQuitOnLastWindowClosed(false);
    MainWindow mainWindow;
    //mainWindow.show();
    return QApplication::exec();
}