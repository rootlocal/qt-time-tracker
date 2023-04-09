#ifndef ADVANCED_SETTINGS_H
#define ADVANCED_SETTINGS_H

#include <QDialog>
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

    void signalColorChange(ClockView::clockStateEnum, QColor);

    void settingsWindowAccept();

    void settingsWindowReject();

public slots:

    void hide();

    void show();

    void accept() override;

    void reject() override;

private slots:

    void comboBoxChanged(int index);

    void btnWorkColorClicked();

    void btnPauseColorClicked();

    void btnStopColorClicked();

    void btnResetColorClicked();

    void isCustomSizeClicked(bool state);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void colorChange(ClockView::clockStateEnum state, const QColor &color);

    Ui::SettingsWindow *ui;
    Settings *settings;
    QString timerSize;
    Settings::timerColorsStruct timerColors;

};

#endif // ADVANCED_SETTINGS_H
