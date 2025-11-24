#include <QCloseEvent>
#include <QGuiApplication>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include "view/clockview.h"
#include "menu/ClockViewMenu.h"
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
    //qDebug() << Q_FUNC_INFO  << (QTime::currentTime()).toString("HH:mm:ss");
    db = new DataBase(this);
    db->connectToDataBase();
    timer = new QTimer(this);
    timer->setInterval(1000);
    settings = new Settings(this);

    clockViewMenu = new ClockViewMenu(this);
    clock = new ClockView(this, clockViewMenu, settings);

    systemTrayIcon = new QSystemTrayIcon(QIcon(TRAY_ICON), this);

    settingsWindow = new SettingsWindow(this, settings);
    tasksWindow = new TasksWindow(settings, this);

    connect(settingsWindow, &SettingsWindow::signalSizeChanged, clock, &ClockView::setSize);
    connect(settingsWindow, &SettingsWindow::signalColorChange, clock, &ClockView::setColor);
    connect(systemTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    connect(clock, &ClockView::startClicked, this, &MainWindow::actionStart);
    connect(clock, &ClockView::pauseClicked, this, &MainWindow::actionPause);
    connect(timer, &QTimer::timeout, this, &MainWindow::slotUpdateTimerDisplay);

    connect(
            clockViewMenu->getAction(ClockViewMenu::Action::SETTINGS),
            SIGNAL(triggered()),
            settingsWindow,
            SLOT(show())
    );
    connect(
            clockViewMenu->getAction(ClockViewMenu::Action::TASKS), SIGNAL(triggered()),
            tasksWindow,
            SLOT(show())
    );
    connect(
            clockViewMenu->getAction(ClockViewMenu::Action::START), SIGNAL(triggered()),
            SLOT(slotSelectTask())
    );
    connect(
            clockViewMenu->getAction(ClockViewMenu::Action::PAUSE), SIGNAL(triggered()),
            SLOT(actionPause())
    );
    connect(
            clockViewMenu->getAction(ClockViewMenu::Action::STOP), SIGNAL(triggered()),
            SLOT(actionStop())
    );
    connect(
            clockViewMenu->getAction(ClockViewMenu::Action::EXIT), SIGNAL(triggered()),
            qApp,
            SLOT(quit())
    );

    this->setState(state);
    systemTrayIcon->setContextMenu(clockViewMenu->getMenu());
    systemTrayIcon->show();
    this->clockShow();
}

MainWindow::~MainWindow() {
    db->closeDataBase();
    delete systemTrayIcon;
    delete clockViewMenu;
    delete settingsWindow;
    delete timer;
    delete tasksWindow;
    delete db;
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
            clockViewMenu->getAction(ClockViewMenu::Action::START)->setVisible(false);
            clockViewMenu->getAction(ClockViewMenu::Action::PAUSE)->setVisible(true);
            clockViewMenu->getAction(ClockViewMenu::Action::STOP)->setVisible(true);
            clock->setState(ClockView::clockStateEnum::WORK);
            break;
        case workStateEnum::PAUSED:
            clockViewMenu->getAction(ClockViewMenu::Action::START)->setVisible(true);
            clockViewMenu->getAction(ClockViewMenu::Action::PAUSE)->setVisible(false);
            clockViewMenu->getAction(ClockViewMenu::Action::STOP)->setVisible(true);
            clock->setState(ClockView::clockStateEnum::PAUSE);
            break;
        default:
            clockViewMenu->getAction(ClockViewMenu::Action::START)->setVisible(true);
            clockViewMenu->getAction(ClockViewMenu::Action::PAUSE)->setVisible(false);
            clockViewMenu->getAction(ClockViewMenu::Action::STOP)->setVisible(false);
            clock->setState(ClockView::clockStateEnum::STOP);
            break;
    }

}