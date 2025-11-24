#include <QtTest>
#include <QtWidgets>
#include <QDebug>

#include "mainwindow.h"

class tst_MainWindow : public QObject {
Q_OBJECT

public:
    tst_MainWindow();

    ~tst_MainWindow();

private slots:
//    void initTestCase();
//    void cleanupTestCase();
//    void init();
//    void cleanup();

    void test_clockShow();

private:
    MainWindow *mainWindow;

public slots:
};

tst_MainWindow::tst_MainWindow() {
    mainWindow = new MainWindow();
}

tst_MainWindow::~tst_MainWindow() {
    delete mainWindow;
}

void tst_MainWindow::test_clockShow() {
    mainWindow->clockShow();
}


QTEST_MAIN(tst_MainWindow)

#include "tst_mainwindow.moc"