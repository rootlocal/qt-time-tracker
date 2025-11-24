#include <QMenu>
#include <QAction>
#include <QList>
#include "menu/ClockViewMenu.h"

QT_USE_NAMESPACE

ClockViewMenu::ClockViewMenu(QWidget *parent) : QWidget(parent) {
    menu = new QMenu(this);
    QList<QAction *> timerActions;
    QList<QAction *> appActions;
    actionPause = new QAction(tr("Pause"), this);
    actionStart = new QAction(tr("Start"), this);
    actionStop = new QAction(tr("Stop"), this);
    actionSettings = new QAction(tr("Settings"), this);
    actionExit = new QAction(tr("Exit"), this);
    actionTasks = new QAction(tr("Tasks"), this);

    timerActions.push_back(actionPause);
    timerActions.push_back(actionStart);
    timerActions.push_back(actionStop);
    menu->addActions(timerActions);
    menu->addSeparator();
    appActions.push_back(actionTasks);
    appActions.push_back(actionSettings);
    appActions.push_back(actionExit);
    menu->addActions(appActions);
}

ClockViewMenu::~ClockViewMenu() {
    delete menu;
    delete actionSettings;
    delete actionExit;
    delete actionStart;
    delete actionStop;
    delete actionPause;
    delete actionTasks;
}

QAction *ClockViewMenu::getAction(Action action) {
    switch (action) {
        case Action::SETTINGS:
            return actionSettings;
        case Action::EXIT:
            return actionExit;
        case Action::START:
            return actionStart;
        case Action::STOP:
            return actionStop;
        case Action::PAUSE:
            return actionPause;
        case Action::TASKS:
            return actionTasks;
        default:
            return nullptr;
    }
}

QMenu *ClockViewMenu::getMenu() {
    return menu;
}