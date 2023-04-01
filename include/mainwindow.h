#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "view/clockview.h"
#include "ActionMenu.h"

class QSystemTrayIcon;

class ClockView;

class AdvancedSettings;

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
    AdvancedSettings *settings;
    ClockState state = WORK;
    ActionMenu *menu;
};

#endif // MAIN_WINDOW_H
