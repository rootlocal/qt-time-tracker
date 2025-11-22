#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include <QSystemTrayIcon>

class ClockView;

class QTimer;

class ClockView;

class ActionMenu;

class SettingsWindow;

class Settings;

class TasksWindow;

class TaskListWindow;

class DataBase;

class MainWindow : public QWidget {
Q_OBJECT
public:

    enum struct workStateEnum {
        RUNNING,
        PAUSED,
        STOPPED,
    };

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

    void timerEvent(QTimerEvent *) override;

public slots:

    void clockShow();

    void clockHide();

private slots:

    void actionStart();

    void slotSelectTask();

    void slotSelectedTask(quint64 taskId);

    void actionPause();

    void actionStop();

    void setState(workStateEnum workState);

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void slotUpdateTimerDisplay();

private:
    void initDefaultMenu();

    void initActions();

    QSystemTrayIcon *systemTrayIcon;
    quint64 seconds = 0;
    ClockView *clock;
    SettingsWindow *settingsWindow;
    Settings *settings;
    workStateEnum state;
    ActionMenu *menu;
    QTimer *timer;
    TasksWindow *tasksWindow;
    quint64 currentTask = 0;
    DataBase *db;
};

#endif // MAIN_WINDOW_H
