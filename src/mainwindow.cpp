#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <cmath>
#include <QGuiApplication>
#include "global.h"
#include "mainwindow.h"
#include "view/clockview.h"
#include "SettingsWindow.h"

QT_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    timer = new QTimer(this);
    timer->setInterval(1000);
    settings = new Settings(this);
    menu = new ActionMenu(this);
    systemTrayIcon = new QSystemTrayIcon(Default::App::icon(), this);
    clock = new ClockView(this, menu, settings);
    settingsWindow = new SettingsWindow(this, settings);

    initActions();
    initDefaultMenu();

    connect(settingsWindow, SIGNAL(signalSizeChanged(QSize)), clock, SLOT(setSize(QSize)));
    connect(settingsWindow, SIGNAL(signalColorChange(ClockState, QColor)), clock, SLOT(setColor(ClockState, QColor)));
    connect(systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    connect(clock, SIGNAL(startClicked()), SLOT(actionStart()));
    connect(clock, SIGNAL(pauseClicked()), SLOT(actionPause()));
    connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateTimerDisplay()));
    actionStart();
}

MainWindow::~MainWindow() {
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
    if (state == WORK) {
        seconds++;

        clockStruct.hours = (quint64) floor(seconds / 3600);
        clockStruct.minutes = (quint64) floor((seconds % 3600) / 60);
        clockStruct.seconds = (quint64) floor(seconds % 60);

        auto clockText = QString("%1:%2:%3")
                .arg(clockStruct.hours, 3, 10, QChar('0'))
                .arg(clockStruct.minutes, 2, 10, QChar('0'))
                .arg(clockStruct.seconds, 2, 10, QChar('0'));

        clock->setText(clockText);
    }
}

void MainWindow::actionStart() {
    if (state != WORK) {
        timer->start();
        setState(WORK);
    }
}

void MainWindow::actionPause() {
    timer->stop();
    setState(PAUSE);
}

void MainWindow::actionStop() {
    timer->stop();
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