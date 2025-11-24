#ifndef TASKLIST_MENU_H
#define TASKLIST_MENU_H

#include <QWidget>

class QAction;

class QMenu;

class TaskListMenu : public QWidget {
Q_OBJECT

public:

    enum struct Action {
        EDIT,
        DELETE,
    };

    explicit TaskListMenu(QWidget *parent = nullptr);

    ~TaskListMenu() override;

    QAction *getAction(Action action);

    QMenu *getMenu();

private:
    QMenu *menu;
    QAction *actionEdit;
    QAction *actionDelete;
};


#endif //TASKLIST_MENU_H
