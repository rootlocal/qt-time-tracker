#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <cmath>
#include <QGuiApplication>
#include "icons.h"
#include "mainwindow.h"
#include "view/clockview.h"
#include "SettingsWindow.h"

QT_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    timer = new QTimer(this);
    timer->setInterval(1000);
    settings = new Settings(this);
    menu = new ActionMenu(this);
    systemTrayIcon = new QSystemTrayIcon(QIcon(TRAY_ICON), this);
    clock = new ClockView(this, menu, settings);
    settingsWindow = new SettingsWindow(this, settings);
    state = workStateEnum::STOPPED;

    initActions();
    initDefaultMenu();

    connect(settingsWindow, &SettingsWindow::signalSizeChanged, clock, &ClockView::setSize);
    connect(settingsWindow, &SettingsWindow::signalColorChange, clock, &ClockView::setColor);
    connect(systemTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    connect(clock, &ClockView::startClicked, this, &MainWindow::actionStart);
    connect(clock, &ClockView::pauseClicked, this, &MainWindow::actionPause);
    connect(timer, &QTimer::timeout, this, &MainWindow::slotUpdateTimerDisplay);

    actionStart();
}

MainWindow::~MainWindow() {
    delete systemTrayIcon;
    delete menu;
    delete settingsWindow;
    delete timer;
}

void MainWindow::initDefaultMenu() {
    connect(menu->getAction(ActionMenu::Action::EXIT), SIGNAL(triggered()), qApp, SLOT(quit()));
    systemTrayIcon->setContextMenu(menu->getMenu());
    systemTrayIcon->show();
}

void MainWindow::initActions() {
    connect(menu->getAction(ActionMenu::Action::SETTINGS), SIGNAL(triggered()), settingsWindow, SLOT(show()));
    connect(menu->getAction(ActionMenu::Action::START), SIGNAL(triggered()), SLOT(actionStart()));
    connect(menu->getAction(ActionMenu::Action::PAUSE), SIGNAL(triggered()), SLOT(actionPause()));
    connect(menu->getAction(ActionMenu::Action::STOP), SIGNAL(triggered()), SLOT(actionStop()));
}

void MainWindow::show() {
    clock->show();
}

void MainWindow::hide() {
    clock->hide();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {

    if (reason == QSystemTrayIcon::Trigger) {
        if (!clock->isVisible()) {
            this->show();
        } else {
            this->hide();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    this->hide();
}

void MainWindow::timerEvent(QTimerEvent *event) {

}

void MainWindow::slotUpdateTimerDisplay() {
    if (state == workStateEnum::RUNNING) {
        seconds++;

        clockStruct.hours = static_cast<quint64>(floor(seconds / 3600));
        clockStruct.minutes = static_cast<quint64>(floor((seconds % 3600) / 60));
        clockStruct.seconds = static_cast<quint64>(floor(seconds % 60));

        auto clockText = QString("%1:%2:%3")
                .arg(clockStruct.hours, 3, 10, QChar('0'))
                .arg(clockStruct.minutes, 2, 10, QChar('0'))
                .arg(clockStruct.seconds, 2, 10, QChar('0'));

        clock->setText(clockText);
    }
}

void MainWindow::actionStart() {
    if (state != workStateEnum::RUNNING) {
        timer->start();
        setState(workStateEnum::RUNNING);
    }
}

void MainWindow::actionPause() {
    timer->stop();
    setState(workStateEnum::PAUSED);
}

void MainWindow::actionStop() {
    timer->stop();
    seconds = 0;
    clock->setText("000:00:00");
    setState(workStateEnum::STOPPED);
}

void MainWindow::setState(workStateEnum clockState) {
    clock->setState(ClockView::clockStateEnum::WORK);
    this->state = clockState;

    switch (clockState) {
        case workStateEnum::RUNNING:
            menu->getAction(ActionMenu::Action::START)->setVisible(false);
            menu->getAction(ActionMenu::Action::PAUSE)->setVisible(true);
            menu->getAction(ActionMenu::Action::STOP)->setVisible(true);
            clock->setState(ClockView::clockStateEnum::WORK);
            break;
        case workStateEnum::PAUSED:
            menu->getAction(ActionMenu::Action::START)->setVisible(true);
            menu->getAction(ActionMenu::Action::PAUSE)->setVisible(false);
            menu->getAction(ActionMenu::Action::STOP)->setVisible(true);
            clock->setState(ClockView::clockStateEnum::PAUSE);
            break;
        default:
            menu->getAction(ActionMenu::Action::START)->setVisible(true);
            menu->getAction(ActionMenu::Action::PAUSE)->setVisible(false);
            menu->getAction(ActionMenu::Action::STOP)->setVisible(false);
            clock->setState(ClockView::clockStateEnum::STOP);
            break;
    }

}