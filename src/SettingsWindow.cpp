#include <QColorDialog>
#include <QDebug>
#include <QSettings>
#include "SettingsWindow.h"
#include "ui_settings_window.h"

QT_USE_NAMESPACE

SettingsWindow::SettingsWindow(QWidget *parent, Settings *mSettings) : QDialog(parent),
                                                                       ui(new Ui::SettingsWindow) {
    settings = mSettings;
    ui->setupUi(this);
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
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

    connect(ui->groupBoxCustomSize, &QGroupBox::toggled, this, &SettingsWindow::isCustomSizeClicked);
    //connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsWindow::accept);
    //connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &SettingsWindow::reject);
    connect(ui->btnClockWorkColor, &QPushButton::clicked, this, &SettingsWindow::btnWorkColorClicked);
    connect(ui->btnClockPauseColor, &QPushButton::clicked, this, &SettingsWindow::btnPauseColorClicked);
    connect(ui->btnClockStopColor, &QPushButton::clicked, this, &SettingsWindow::btnStopColorClicked);
    connect(ui->btnResetColor, &QPushButton::clicked, this, &SettingsWindow::btnResetColorClicked);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxChanged(int)));
}

SettingsWindow::~SettingsWindow() {
    delete ui;
}

void SettingsWindow::isCustomSizeClicked(bool state) {
    ui->groupBoxSize->setDisabled(state);
}

void SettingsWindow::comboBoxChanged(int index) {
    QString newTimerSize = settings->getTimerWindowSizesItem(index);
    emit signalSizeChanged(settings->getTimerWindowSize(newTimerSize));
}

void SettingsWindow::btnWorkColorClicked() {
    const QColor color = QColorDialog::getColor(timerColors.work, this);
    if (!color.isValid()) return;
    timerColors.work = color;
    colorChange(ClockView::clockStateEnum::WORK, color);
}

void SettingsWindow::btnPauseColorClicked() {
    const QColor color = QColorDialog::getColor(timerColors.pause, this);
    if (!color.isValid()) return;
    timerColors.pause = color;
    colorChange(ClockView::clockStateEnum::PAUSE, color);
}

void SettingsWindow::btnStopColorClicked() {
    const QColor color = QColorDialog::getColor(timerColors.stop, this);
    if (!color.isValid()) return;
    timerColors.stop = color;
    colorChange(ClockView::clockStateEnum::STOP, color);
}

void SettingsWindow::btnResetColorClicked() {
    timerColors = settings->getTimerColorsDefault();
    colorChange(ClockView::clockStateEnum::WORK, timerColors.work);
    colorChange(ClockView::clockStateEnum::PAUSE, timerColors.pause);
    colorChange(ClockView::clockStateEnum::STOP, timerColors.stop);
}

void SettingsWindow::colorChange(ClockView::clockStateEnum state, const QColor &color) {
    QPalette palette;

    switch (state) {
        case ClockView::clockStateEnum::WORK:
            palette = ui->btnClockWorkColor->palette();
            palette.setColor(QPalette::Button, color);
            ui->btnClockWorkColor->setAutoFillBackground(true);
            ui->btnClockWorkColor->setPalette(palette);
            ui->btnClockWorkColor->update();
            break;
        case ClockView::clockStateEnum::PAUSE:
            palette = ui->btnClockPauseColor->palette();
            palette.setColor(QPalette::Button, color);
            ui->btnClockPauseColor->setAutoFillBackground(true);
            ui->btnClockPauseColor->setPalette(palette);
            ui->btnClockPauseColor->update();
            break;
        case ClockView::clockStateEnum::STOP:
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

void SettingsWindow::show() {
    if (!settings->getWindowGeometry("settings").isEmpty()) {
        restoreGeometry(settings->getWindowGeometry("settings"));
    }

    QDialog::show();
}

void SettingsWindow::moveEvent(QMoveEvent *event) {
    settings->setWindowGeometry("settings", saveGeometry());
}

void SettingsWindow::resizeEvent(QResizeEvent *event) {
    settings->setWindowGeometry("settings", saveGeometry());
}

void SettingsWindow::closeEvent(QCloseEvent *event) {

}