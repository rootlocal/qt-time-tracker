#include "SettingsWindow.h"
#include "ui_settingswindow.h"
#include <QColorDialog>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

SettingsWindow::SettingsWindow(QWidget *parent, Settings *mSettings) : QDialog(parent),
                                                                       ui(new Ui::SettingsWindow) {
    settings = mSettings;
    width = settings->getWidth();
    height = settings->getHeight();
    colorWork = settings->getColorWork();
    colorPause = settings->getColorPause();
    colorStop = settings->getColorStop();

    ui->setupUi(this);

    {
        QPalette pal = ui->btnClockWorkColor->palette();
        pal.setColor(QPalette::Button, colorWork);
        ui->btnClockWorkColor->setAutoFillBackground(true);
        ui->btnClockWorkColor->setPalette(pal);
        ui->btnClockWorkColor->update();
    }
    {
        QPalette pal = ui->btnClockPauseColor->palette();
        pal.setColor(QPalette::Button, colorPause);
        ui->btnClockPauseColor->setAutoFillBackground(true);
        ui->btnClockPauseColor->setPalette(pal);
        ui->btnClockPauseColor->update();
    }
    {
        QPalette pal = ui->btnClockBreakColor->palette();
        pal.setColor(QPalette::Button, colorStop);
        ui->btnClockBreakColor->setAutoFillBackground(true);
        ui->btnClockBreakColor->setPalette(pal);
        ui->btnClockBreakColor->update();
    }
}

SettingsWindow::~SettingsWindow() {
    delete ui;
}


void SettingsWindow::on_comboBox_currentIndexChanged(int index) {

    switch (index) {
        case 0:
            width = 100;
            height = 24;
            break;
        case 2:
            width = 134;
            height = 34;
            break;
        case 1:
        default:
            width = 124;
            height = 28;
            break;
    };

    signalSizeChanged(width, height);
}

void SettingsWindow::slotSizeChanged() {
    width = ui->boxWidth->value();
    height = ui->boxHeight->value();

    emit signalSizeChanged(width, height);
}

void SettingsWindow::on_btnClockWorkColor_clicked() {
    const QColor color = QColorDialog::getColor(colorWork);
    if (!color.isValid()) return;
    colorWork = color;
    QPalette pal = ui->btnClockWorkColor->palette();
    pal.setColor(QPalette::Button, color);
    ui->btnClockWorkColor->setAutoFillBackground(true);
    ui->btnClockWorkColor->setPalette(pal);
    ui->btnClockWorkColor->update();

    emit signalColorChange(WORK, color);
}

void SettingsWindow::on_btnClockPauseColor_clicked() {
    const QColor color = QColorDialog::getColor(colorPause);
    if (!color.isValid()) return;
    colorPause = color;
    QPalette pal = ui->btnClockPauseColor->palette();
    pal.setColor(QPalette::Button, color);
    ui->btnClockPauseColor->setAutoFillBackground(true);
    ui->btnClockPauseColor->setPalette(pal);
    ui->btnClockPauseColor->update();

    emit signalColorChange(PAUSE, color);
}

void SettingsWindow::on_btnClockBreakColor_clicked() {
    const QColor color = QColorDialog::getColor(colorPause);
    if (!color.isValid()) return;
    colorStop = color;
    QPalette pal = ui->btnClockBreakColor->palette();
    pal.setColor(QPalette::Button, color);
    ui->btnClockBreakColor->setAutoFillBackground(true);
    ui->btnClockBreakColor->setPalette(pal);
    ui->btnClockBreakColor->update();

    emit signalColorChange(STOP, color);
}

void SettingsWindow::accept() {
    settings->setWidth(width);
    settings->setHeight(height);
    settings->setColorWork(colorWork);
    settings->setColorPause(colorPause);
    settings->setColorStop(colorStop);
    settings->write();

    emit settingsWindowAccept();
    this->hide();
}

void SettingsWindow::reject() {
    emit settingsWindowReject();
    this->hide();
}