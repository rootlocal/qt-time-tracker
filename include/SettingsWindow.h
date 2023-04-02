#ifndef ADVANCED_SETTINGS_H
#define ADVANCED_SETTINGS_H

#include <QDialog>
#include <QSettings>
#include "view/clockview.h"
#include "Settings.h"

namespace Ui {
    class SettingsWindow;
}

class SettingsWindow : public QDialog {
Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr, Settings *mSettings = nullptr);

    ~SettingsWindow() override;

    void loadSettings();

    void saveSettings();

signals:

    void signalSizeChanged(int, int);

    void signalColorChange(ClockState, QColor);

signals:

    void mainWindowShow();

    void settingsWindowAccept();

    void settingsWindowReject();

private slots:

    void accept() override;

    void reject() override;

    void on_comboBox_currentIndexChanged(int index);

    void slotSizeChanged();

    void on_btnClockWorkColor_clicked();

    void on_btnClockPauseColor_clicked();

    void on_btnClockBreakColor_clicked();

private:
    Ui::SettingsWindow *ui;
    Settings *settings;

    int width = 124;
    int height = 28;
    QColor colorWork;
    QColor colorPause;
    QColor colorStop;
};

#endif // ADVANCED_SETTINGS_H
