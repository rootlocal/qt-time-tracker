#ifndef ACTION_MENU_H
#define ACTION_MENU_H

#include <QWidget>
#include <QMenu>

class QAction;

class ActionMenu : public QWidget {
Q_OBJECT

public:

    enum struct Action {
        SETTINGS,
        EXIT,
        START,
        STOP,
        PAUSE,
    };

    explicit ActionMenu(QWidget *parent = nullptr);

    ~ActionMenu() override;

    QAction *getAction(Action action);

    QMenu *getMenu();

private:

    QMenu *menu;
    QAction *actionSettings;
    QAction *actionExit;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionPause;
};


#endif // ACTION_MENU_H
