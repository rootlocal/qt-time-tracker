#include "ActionMenu.h"

QT_USE_NAMESPACE

ActionMenu::ActionMenu(QWidget *parent) : QWidget(parent) {
    menu = new QMenu(this);
    QList<QAction *> timerActions;
    QList<QAction *> appActions;
    actionPause = new QAction(tr("Pause"), this);
    actionStart = new QAction(tr("Start"), this);
    actionStop = new QAction(tr("Stop"), this);
    actionSettings = new QAction(tr("Settings"), this);
    actionExit = new QAction(tr("Exit"), this);

    timerActions.push_back(actionPause);
    timerActions.push_back(actionStart);
    timerActions.push_back(actionStop);
    menu->addActions(timerActions);
    menu->addSeparator();
    appActions.push_back(actionSettings);
    appActions.push_back(actionExit);
    menu->addActions(appActions);
}

ActionMenu::~ActionMenu() {
    delete menu;
    delete actionSettings;
    delete actionExit;
    delete actionStart;
    delete actionStop;
    delete actionPause;
}

QAction *ActionMenu::getAction(Action action) {
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
        default:
            return nullptr;
    }
}

QMenu *ActionMenu::getMenu() {
    return menu;
}