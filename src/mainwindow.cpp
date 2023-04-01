#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <cmath>
#include "global.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view/clockview.h"
#include "SettingsWindow.h"

QT_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow) {
    systemTrayIcon = new QSystemTrayIcon(Default::App::icon(), this);
    menu = new ActionMenu(this);
    clock = new ClockView(this, menu);
    settingsWindow = new SettingsWindow(nullptr);
    ui->setupUi(this);

    initActions();
    initDefaultMenu();
    setState(WORK);

    connect(settingsWindow, SIGNAL(signalSizeChanged(int, int)), clock, SLOT(setSize(int, int)));
    connect(settingsWindow, SIGNAL(signalColorChange(ClockState, QColor)), clock, SLOT(setColor(ClockState, QColor)));
    connect(settingsWindow, SIGNAL(signalColorChange(ClockState, QColor)), clock, SLOT(setColor(ClockState, QColor)));
    connect(settingsWindow, SIGNAL(signalSizeChanged(int, int)), clock, SLOT(setSize(int, int)));
    settingsWindow->loadSettings();

    connect(clock, SIGNAL(startClicked()), SLOT(actionStart()));
    connect(clock, SIGNAL(pauseClicked()), SLOT(actionPause()));
    clock->show();

    this->setWindowIcon(Default::App::icon());
    this->setWindowTitle(tr("Computools Timer Cheker"));
    startTimer(1000);
}

MainWindow::~MainWindow() {
    delete ui;
    delete systemTrayIcon;
    delete menu;
    delete settingsWindow;
}

void MainWindow::initDefaultMenu() {
    connect(menu->getAction(ActionMenu::Action::EXIT), SIGNAL(triggered()), qApp, SLOT(quit()));
    systemTrayIcon->setContextMenu(menu->getMenu());
    systemTrayIcon->show();
}

void MainWindow::initActions() {
    connect(ui->actionAdvanced_Settings, SIGNAL(triggered()), menu->getAction(ActionMenu::Action::SETTINGS),
            SLOT(trigger()));
    connect(menu->getAction(ActionMenu::Action::SETTINGS), SIGNAL(triggered()), settingsWindow, SLOT(show()));
    connect(menu->getAction(ActionMenu::Action::START), SIGNAL(triggered()), SLOT(actionStart()));
    connect(menu->getAction(ActionMenu::Action::PAUSE), SIGNAL(triggered()), SLOT(actionPause()));
    connect(menu->getAction(ActionMenu::Action::STOP), SIGNAL(triggered()), SLOT(actionStop()));

}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    this->hide();
}

void MainWindow::timerEvent(QTimerEvent *) {
    if (state == WORK) {
        seconds++;
        const quint64 h = floor(seconds / 3600);
        const quint64 m = floor((seconds % 3600) / 60);
        const quint64 s = floor(seconds % 60);
        clock->setText(
                QString("%1:%2:%3").arg(h, 3, 10, QChar('0')).arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0')));
    }
}

void MainWindow::actionStart() {
    if (state != WORK) {
        setState(WORK);
    }
}

void MainWindow::actionPause() {
    setState(PAUSE);
}

void MainWindow::actionStop() {
    seconds = 0;
    clock->setText("000:00:00");
    setState(STOP);
}

void MainWindow::setState(ClockState clockState) {
    clock->setState(clockState);
    this->state = clockState;
    switch (clockState) {
        case WORK:
            menu->getAction(ActionMenu::Action::START)->setVisible(false);
            menu->getAction(ActionMenu::Action::PAUSE)->setVisible(true);
            menu->getAction(ActionMenu::Action::STOP)->setVisible(true);

            break;
        case PAUSE:
            menu->getAction(ActionMenu::Action::START)->setVisible(true);
            menu->getAction(ActionMenu::Action::PAUSE)->setVisible(false);
            menu->getAction(ActionMenu::Action::STOP)->setVisible(true);

            break;
        default:
            menu->getAction(ActionMenu::Action::START)->setVisible(true);
            menu->getAction(ActionMenu::Action::PAUSE)->setVisible(false);
            menu->getAction(ActionMenu::Action::STOP)->setVisible(false);
            break;
    }

}


