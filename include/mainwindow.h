#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "view/clockview.h"
#include "ActionMenu.h"
#include "SettingsWindow.h"
#include "Settings.h"

class QSystemTrayIcon;

class ClockView;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

    void timerEvent(QTimerEvent *) override;

private slots:

    void actionStart();

    void actionPause();

    void actionStop();

    void setState(ClockState clockState);

private:
    void initDefaultMenu();

    void initActions();

    Ui::MainWindow *ui;
    QSystemTrayIcon *systemTrayIcon;
    quint64 seconds = 0;
    ClockView *clock;
    SettingsWindow *settingsWindow;
    Settings *settings;
    ClockState state = WORK;
    ActionMenu *menu;
};

#endif // MAIN_WINDOW_H
