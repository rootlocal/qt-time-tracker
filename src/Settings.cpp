#include <QSettings>
#include <QColor>
#include <QDebug>
#include "Settings.h"

QT_USE_NAMESPACE

Settings::Settings(QObject *parent) : QObject(parent),
                                      qSettings(QSettings::IniFormat,
                                                QSettings::UserScope,
                                                SETTINGS_ORGANIZATION,
                                                SETTINGS_APPLICATION_NAME, this) {
    load();
}

Settings::~Settings() = default;

void Settings::load() {
    timerColors.work = qSettings.value("timer/color_work", timerColors.work).value<QColor>();
    timerColors.pause = qSettings.value("timer/color_pause", timerColors.pause).value<QColor>();
    timerColors.stop = qSettings.value("timer/color_stop", timerColors.stop).value<QColor>();

    timerWindowCustomSize = qSettings.value("timer/window_custom_size", timerWindowCustomSize).value<QSize>();
    timerWindowSize = qSettings.value("timer/window_size", timerWindowSize).toString();
    timerWindowPosition = qSettings.value("timer/window_position", timerWindowPosition).value<QPoint>();
    isTimerCustomSize = qSettings.value("timer/is_window_custom_size", isTimerCustomSize).toBool();
    settingsGeometry = qSettings.value("settings/window_geometry", settingsGeometry).value<QByteArray>();
}

void Settings::write() {
    qSettings.setValue("timer/color_work", timerColors.work);
    qSettings.setValue("timer/color_pause", timerColors.pause);
    qSettings.setValue("timer/color_stop", timerColors.stop);
    qSettings.setValue("timer/window_size", timerWindowSize);
    qSettings.setValue("timer/window_custom_size", timerWindowCustomSize);
    qSettings.setValue("timer/is_window_custom_size", isTimerCustomSize);
}

const QSize &Settings::getTimerWindowCustomSize() const {
    return timerWindowCustomSize;
}

void Settings::setTimerWindowCustomSize(const QSize &size) {
    Settings::timerWindowCustomSize = size;
}

const QString &Settings::getTimerWindowSize() const {
    return timerWindowSize;
}

void Settings::setTimerWindowSize(const QString &size) {
    Settings::timerWindowSize = size;
}

const QPoint &Settings::getTimerWindowPosition() const {
    return timerWindowPosition;
}

void Settings::setTimerWindowPosition(const QPoint &position) {
    Settings::timerWindowPosition = position;
    qSettings.setValue("timer/window_position", position);
}

const bool &Settings::getIsTimerCustomSize() const {
    return isTimerCustomSize;
}

void Settings::setIsTimerCustomSize(const bool &value) {
    isTimerCustomSize = value;
}

const QByteArray &Settings::getSettingGeometry() const {
    return settingsGeometry;
}

void Settings::setSettingGeometry(const QByteArray &geometry) {
    Settings::settingsGeometry = geometry;
    qSettings.setValue("settings/window_geometry", geometry);
}

// COLORS
const Settings::timerColorsStruct &Settings::getTimerColors() const {
    return timerColors;
}

void Settings::setTimerColors(Settings::timerColorsStruct &colors) {
    timerColors = colors;
}

const Settings::timerColorsStruct &Settings::getTimerColorsDefault() const {
    return timerColorsDefault;
}

QList<QString> Settings::getTimerWindowSizesItems() {

    if (timerWindowSizesItems.isEmpty()) {
        QMap<QString, QSize> sizesMap = getTimerWindowSizes();
                foreach (QString key, sizesMap.keys()) {
                //QSize size = sizesMap.value(key);
                timerWindowSizesItems.append(key);
            }
    }

    return timerWindowSizesItems;
}

QString Settings::getTimerWindowSizesItem(int index) {
    return getTimerWindowSizesItems().value(index);
}

QMap<QString, QSize> Settings::getTimerWindowSizes() {

    if (timerWindowSizes.isEmpty()) {
        timerWindowSizes.insert("Small", QSize(100, 24));
        timerWindowSizes.insert("Normal", QSize(124, 28));
        timerWindowSizes.insert("Big", QSize(134, 34));
    }

    return timerWindowSizes;
}

QSize Settings::getTimerWindowSize(const QString &name) {

    if (getTimerWindowSizes().contains(name)) {
        return getTimerWindowSizes().value(name);
    }

    return {124, 28};
}


