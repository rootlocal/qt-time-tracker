#include <QCloseEvent>
#include <QGuiApplication>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include "view/clockview.h"
#include "ActionMenu.h"
#include "SettingsWindow.h"
#include "Settings.h"
#include "TasksWindow.h"
#include "TaskListWindow.h"
#include "DataBase.h"
#include "icons.h"
#include "mainwindow.h"
#include "helpers/TimeDateHelper.h"

QT_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    db = new DataBase(this);
    db->connectToDataBase();
    timer = new QTimer(this);
    timer->setInterval(1000);
    settings = new Settings(this);
    menu = new ActionMenu(this);
    systemTrayIcon = new QSystemTrayIcon(QIcon(TRAY_ICON), this);
    clock = new ClockView(this, menu, settings);
    settingsWindow = new SettingsWindow(this, settings);
    state = workStateEnum::STOPPED;
    tasksWindow = new TasksWindow(settings, this);
    initActions();
    initDefaultMenu();

    connect(settingsWindow, &SettingsWindow::signalSizeChanged, clock, &ClockView::setSize);
    connect(settingsWindow, &SettingsWindow::signalColorChange, clock, &ClockView::setColor);
    connect(systemTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    connect(clock, &ClockView::startClicked, this, &MainWindow::actionStart);
    connect(clock, &ClockView::pauseClicked, this, &MainWindow::actionPause);
    connect(timer, &QTimer::timeout, this, &MainWindow::slotUpdateTimerDisplay);

    setState(workStateEnum::STOPPED);
    this->clockShow();
}

MainWindow::~MainWindow() {
    db->closeDataBase();
    delete systemTrayIcon;
    delete menu;
    delete settingsWindow;
    delete timer;
    delete tasksWindow;
    delete db;
}

void MainWindow::initDefaultMenu() {
    connect(menu->getAction(ActionMenu::Action::EXIT), SIGNAL(triggered()), qApp, SLOT(quit()));
    systemTrayIcon->setContextMenu(menu->getMenu());
    systemTrayIcon->show();
}

void MainWindow::initActions() {
    connect(menu->getAction(ActionMenu::Action::SETTINGS), SIGNAL(triggered()), settingsWindow, SLOT(show()));
    connect(menu->getAction(ActionMenu::Action::TASKS), SIGNAL(triggered()), tasksWindow, SLOT(show()));
    connect(menu->getAction(ActionMenu::Action::START), SIGNAL(triggered()), SLOT(slotSelectTask()));
    connect(menu->getAction(ActionMenu::Action::PAUSE), SIGNAL(triggered()), SLOT(actionPause()));
    connect(menu->getAction(ActionMenu::Action::STOP), SIGNAL(triggered()), SLOT(actionStop()));
}

void MainWindow::clockShow() {
    clock->show();
}

void MainWindow::clockHide() {
    clock->hide();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {

    if (reason == QSystemTrayIcon::Trigger) {
        if (!clock->isVisible()) {
            this->clockShow();
        } else {
            this->clockHide();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    this->clockHide();
}

void MainWindow::timerEvent(QTimerEvent *event) {

}

void MainWindow::slotUpdateTimerDisplay() {
    if (state == workStateEnum::RUNNING) {
        seconds++;
        clock->setText(TimeDateHelper::formatTimeToString(seconds));
    }
}

void MainWindow::slotSelectTask() {
    if (currentTask == 0) {
        auto *taskListWindow = new TaskListWindow(settings, this);
        QMetaObject::Connection con = connect(taskListWindow, &TaskListWindow::signalSelectedTask, this,
                                              &MainWindow::slotSelectedTask);
        tasksWindow->hide();
        taskListWindow->exec();
        disconnect(con);
        delete taskListWindow;
    } else {
        actionStart();
    }
}

void MainWindow::slotSelectedTask(quint64 taskId) {
    currentTask = taskId;
    actionStart();
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

    /** Сохраняем в базу что натикало в таймере */
    QVariantList variantList;
    variantList.append(currentTask);
    variantList.append(seconds);
    variantList.append("");
    db->insertIntoTimeTable(variantList);

    seconds = 0;
    currentTask = 0;
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