#ifndef ADVANCED_SETTINGS_H
#define ADVANCED_SETTINGS_H

#include <QDialog>
#include "view/clockview.h"

class QSettings;

namespace Ui {
    class SettingsWindow;
}

class SettingsWindow : public QDialog {
Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);

    ~SettingsWindow() override;

    void loadSettings();

    void saveSettings();

signals:

    void signalSizeChanged(int, int);

    void signalColorChange(ClockState, QColor);

private slots:

    void on_comboBox_currentIndexChanged(int index);

    void slotSizeChanged();

    void on_btnClockWorkColor_clicked();

    void on_btnClockPauseColor_clicked();

    void on_btnClockBreakColor_clicked();

private:
    Ui::SettingsWindow *ui;
    QSettings *settings;
};

#endif // ADVANCED_SETTINGS_H
