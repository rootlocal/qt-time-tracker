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

    void signalSizeChanged(QSize);

    void signalColorChange(ClockState, QColor);

signals:

    void mainWindowShow();

    void settingsWindowAccept();

    void settingsWindowReject();

private slots:

    void hide();

    void show();

    void accept() override;

    void reject() override;

    void on_comboBox_currentIndexChanged(int index);

    void slotSizeChanged();

    void on_btnClockWorkColor_clicked();

    void on_btnClockPauseColor_clicked();

    void on_btnClockStopColor_clicked();

    void on_btnResetColor_clicked();

private:
    void colorChange(ClockState state, const QColor &color);

    Ui::SettingsWindow *ui;
    Settings *settings;
    QSize timerSize = QSize(124, 28);
    Settings::timerColorsStruct timerColors;
};

#endif // ADVANCED_SETTINGS_H
