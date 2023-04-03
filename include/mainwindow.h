#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QTimer>
#include "view/clockview.h"
#include "ActionMenu.h"
#include "SettingsWindow.h"
#include "Settings.h"

class QSystemTrayIcon;

class ClockView;


class MainWindow : public QWidget {
Q_OBJECT
public:

    struct timerClockStruct {
        quint64 hours = 0;
        quint64 minutes = 0;
        quint64 seconds = 0;
    };

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

    void timerEvent(QTimerEvent *) override;

public slots:
    void show();

    void hide();

private slots:

    void actionStart();

    void actionPause();

    void actionStop();

    void setState(ClockState clockState);

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
    ClockState state = STOP;
    ActionMenu *menu;
    timerClockStruct clockStruct;
    QTimer *timer;
};

#endif // MAIN_WINDOW_H
