#include "SettingsWindow.h"
#include "ui_settingswindow.h"
#include <QColorDialog>
#include <QDebug>
#include <QSettings>

QT_USE_NAMESPACE

SettingsWindow::SettingsWindow(QWidget *parent, Settings *mSettings) : QDialog(parent),
                                                                       ui(new Ui::SettingsWindow) {
    settings = mSettings;

    ui->setupUi(this);
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint); //  On top window (fix unix)

    timerSize = settings->getTimerWindowSize();
    timerColors = settings->getTimerColors();
    ui->groupBoxCustomSize->setChecked(settings->getIsTimerCustomSize());
    ui->boxWidth->setValue(settings->getTimerWindowCustomSize().width());
    ui->boxHeight->setValue(settings->getTimerWindowCustomSize().height());

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

    ui->comboBox->clear();
    QList<QString> windowsSizesItems = settings->getTimerWindowSizesItems();
    ui->groupBoxSize->setDisabled(settings->getIsTimerCustomSize());
    ui->comboBox->addItems(windowsSizesItems);
    // ui->comboBox->setCurrentIndex(windowsSizesItems.indexOf(timerSize));

    int index = ui->comboBox->findText(timerSize);

    if (index != -1) {
        ui->comboBox->setCurrentIndex(index);
    }

    connect(ui->groupBoxCustomSize,
            SIGNAL(toggled(bool)), this,
            SLOT(isCustomSizeGroupBoxActionClicked(bool))
    );

}

SettingsWindow::~SettingsWindow() {
    delete ui;
}

void SettingsWindow::isCustomSizeGroupBoxActionClicked(bool state) {
    ui->groupBoxSize->setDisabled(state);
}

void SettingsWindow::on_comboBox_currentIndexChanged(int index) {
    QString newTimerSize = settings->getTimerWindowSizesItem(index);
    emit signalSizeChanged(settings->getTimerWindowSize(newTimerSize));
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
    bool isCustomSize = ui->groupBoxCustomSize->isChecked();
    settings->setIsTimerCustomSize(isCustomSize);
    settings->setTimerColors(timerColors);

    if (isCustomSize) {
        int width = ui->boxWidth->value();
        int height = ui->boxHeight->value();
        QSize size = QSize(width, height);
        settings->setTimerWindowCustomSize(size);
        emit signalSizeChanged(size);
    } else {
        QString sizeStr = ui->comboBox->currentText();
        settings->setTimerWindowSize(sizeStr);
        emit signalSizeChanged(settings->getTimerWindowSize(sizeStr));
    }

    settings->write();

    emit settingsWindowAccept();
    this->hide();
}

void SettingsWindow::reject() {
    emit settingsWindowReject();
    this->hide();
}

void SettingsWindow::hide() {
    settings->setSettingGeometry(saveGeometry());
    QDialog::hide();
}

void SettingsWindow::show() {
    if (!settings->getSettingGeometry().isEmpty()) {
        restoreGeometry(settings->getSettingGeometry());
    }

    QDialog::show();
}