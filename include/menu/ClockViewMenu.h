#ifndef ACTION_MENU_H
#define ACTION_MENU_H

#include <QWidget>

class QAction;
class QMenu;

class ClockViewMenu : public QWidget {
Q_OBJECT

public:

    enum struct Action {
        SETTINGS,
        EXIT,
        START,
        STOP,
        PAUSE,
        TASKS,
    };

    explicit ClockViewMenu(QWidget *parent = nullptr);

    ~ClockViewMenu() override;

    QAction *getAction(Action action);

    QMenu *getMenu();

private:

    QMenu *menu;
    QAction *actionSettings;
    QAction *actionExit;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionPause;
    QAction *actionTasks;
};


#endif // ACTION_MENU_H
