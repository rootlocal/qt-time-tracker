#include "SettingsWindow.h"
#include "ui_settingswindow.h"
#include <QColorDialog>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

SettingsWindow::SettingsWindow(QWidget *parent, Settings *mSettings) : QDialog(parent),
                                                                       ui(new Ui::SettingsWindow) {
    settings = mSettings;
    timerSize = settings->getTimerWindowsSize();
    timerColors = settings->getTimerColors();
    ui->setupUi(this);

    {
        QPalette pal = ui->btnClockWorkColor->palette();
        pal.setColor(QPalette::Button, timerColors.work);
        ui->btnClockWorkColor->setAutoFillBackground(true);
        ui->btnClockWorkColor->setPalette(pal);
        ui->btnClockWorkColor->update();
    }
    {
        QPalette pal = ui->btnClockPauseColor->palette();
        pal.setColor(QPalette::Button, timerColors.pause);
        ui->btnClockPauseColor->setAutoFillBackground(true);
        ui->btnClockPauseColor->setPalette(pal);
        ui->btnClockPauseColor->update();
    }
    {
        QPalette pal = ui->btnClockStopColor->palette();
        pal.setColor(QPalette::Button, timerColors.stop);
        ui->btnClockStopColor->setAutoFillBackground(true);
        ui->btnClockStopColor->setPalette(pal);
        ui->btnClockStopColor->update();
    }
}

SettingsWindow::~SettingsWindow() {
    delete ui;
}

void SettingsWindow::on_comboBox_currentIndexChanged(int index) {

    switch (index) {
        case 0:
            timerSize.setWidth(100);
            timerSize.setHeight(24);
            break;
        case 2:
            timerSize.setWidth(134);
            timerSize.setHeight(34);
            break;
        case 1:
        default:
            timerSize.setWidth(124);
            timerSize.setHeight(28);
            break;
    }

    emit signalSizeChanged(timerSize);
}

void SettingsWindow::slotSizeChanged() {
    int width = ui->boxWidth->value();
    int height = ui->boxHeight->value();

    timerSize = QSize(width, height);
    emit signalSizeChanged(timerSize);
}

void SettingsWindow::on_btnClockWorkColor_clicked() {
    const QColor color = QColorDialog::getColor(timerColors.work);
    if (!color.isValid()) return;
    timerColors.work = color;
    colorChange(WORK, color);
}

void SettingsWindow::on_btnClockPauseColor_clicked() {
    const QColor color = QColorDialog::getColor(timerColors.pause);
    if (!color.isValid()) return;
    timerColors.pause = color;
    colorChange(PAUSE, color);
}

void SettingsWindow::on_btnClockStopColor_clicked() {
    const QColor color = QColorDialog::getColor(timerColors.stop);
    if (!color.isValid()) return;
    timerColors.stop = color;
    colorChange(STOP, color);
}

void SettingsWindow::on_btnResetColor_clicked() {
    timerColors = settings->getTimerColorsDefault();
    colorChange(WORK, timerColors.work);
    colorChange(PAUSE, timerColors.pause);
    colorChange(STOP, timerColors.stop);
}

void SettingsWindow::colorChange(ClockState state, const QColor &color) {
    QPalette palette;

    switch (state) {
        case WORK:
            palette = ui->btnClockWorkColor->palette();
            palette.setColor(QPalette::Button, color);
            ui->btnClockWorkColor->setAutoFillBackground(true);
            ui->btnClockWorkColor->setPalette(palette);
            ui->btnClockWorkColor->update();
            break;
        case PAUSE:
            palette = ui->btnClockPauseColor->palette();
            palette.setColor(QPalette::Button, color);
            ui->btnClockPauseColor->setAutoFillBackground(true);
            ui->btnClockPauseColor->setPalette(palette);
            ui->btnClockPauseColor->update();
            break;
        case STOP:
            palette = ui->btnClockStopColor->palette();
            palette.setColor(QPalette::Button, color);
            ui->btnClockStopColor->setAutoFillBackground(true);
            ui->btnClockStopColor->setPalette(palette);
            ui->btnClockStopColor->update();
            break;
    }

    emit signalColorChange(state, color);
}

void SettingsWindow::accept() {
    settings->setTimerWindowsSize(timerSize);
    settings->setTimerColors(timerColors);
    settings->write();

    emit settingsWindowAccept();
    this->hide();
}

void SettingsWindow::reject() {
    emit settingsWindowReject();
    this->hide();
}

void SettingsWindow::hide() {
    settings->setSettingsGeometry(saveGeometry());
    QDialog::hide();
}

void SettingsWindow::show() {
    if (!settings->getSettingsGeometry().isEmpty()) {
        restoreGeometry(settings->getSettingsGeometry());
    }

    QDialog::show();
}