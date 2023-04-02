#include <QSettings>
#include <QColor>
#include "Settings.h"

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

    timerWindowsSize = qSettings.value("timer/windows_size", timerWindowsSize).value<QSize>();
    timerWindowsPosition = qSettings.value("timer/windows_position", timerWindowsPosition).value<QPoint>();
    settingsGeometry = qSettings.value("settings/windows_geometry", settingsGeometry).value<QByteArray>();
}

void Settings::write() {
    qSettings.setValue("timer/color_work", timerColors.work);
    qSettings.setValue("timer/color_pause", timerColors.pause);
    qSettings.setValue("timer/color_stop", timerColors.stop);
    qSettings.setValue("timer/windows_size", timerWindowsSize);
}

const QSize &Settings::getTimerWindowsSize() const {
    return timerWindowsSize;
}

void Settings::setTimerWindowsSize(const QSize &size) {
    Settings::timerWindowsSize = size;
}

const QPoint &Settings::getTimerWindowsPosition() const {
    return timerWindowsPosition;
}

void Settings::setTimerWindowsPosition(const QPoint &position) {
    Settings::timerWindowsPosition = position;
    qSettings.setValue("timer/windows_position", position);
}

const QByteArray &Settings::getSettingsGeometry() const {
    return settingsGeometry;
}

void Settings::setSettingsGeometry(const QByteArray &geometry) {
    Settings::settingsGeometry = geometry;
    qSettings.setValue("settings/windows_geometry", geometry);
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
