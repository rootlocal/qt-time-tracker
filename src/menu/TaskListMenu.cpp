#include <QMenu>
#include <QAction>
#include <QList>
#include "menu/TaskListMenu.h"

TaskListMenu::TaskListMenu(QWidget *parent) : QWidget(parent) {
    menu = new QMenu(this);
    QList<QAction *> actions;
    actionEdit = new QAction(tr("Edit"), this);
    actionDelete = new QAction(tr("Delete"), this);
    actions.push_back(actionEdit);
    menu->addSeparator();
    actions.push_back(actionDelete);
    menu->addActions(actions);
    menu->addActions(actions);
}

TaskListMenu::~TaskListMenu() {
    delete menu;
    delete actionEdit;
    delete actionDelete;
}

QAction *TaskListMenu::getAction(Action action) {
    switch (action) {
        case Action::EDIT:
            return actionEdit;
        case Action::DELETE:
            return actionDelete;
        default:
            return nullptr;
    }
}

QMenu *TaskListMenu::getMenu() {
    return menu;
}