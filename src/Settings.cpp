#include <QSettings>
#include <QColor>
#include <utility>
#include "Settings.h"

Settings::Settings(QObject *parent) : QObject(parent),
                                      qSettings(QSettings::IniFormat, QSettings::UserScope,
                                                "tracker", "timer", this) {
    colorWork = QColor(150, 255, 150);
    colorPause = QColor(255, 255, 120);
    colorStop = QColor(255, 100, 100);
    load();
}

Settings::~Settings() = default;

void Settings::load() {
    colorWork = qSettings.value("colors/work", colorWork).value<QColor>();
    colorPause = qSettings.value("colors/pause", colorPause).value<QColor>();
    colorStop = qSettings.value("colors/stop", colorStop).value<QColor>();
    width = qSettings.value("size/width", width).toInt();
    height = qSettings.value("size/height", height).toInt();
}

void Settings::write() {
    qSettings.setValue("colors/work", colorWork);
    qSettings.setValue("colors/pause", colorPause);
    qSettings.setValue("colors/stop", colorStop);
    qSettings.setValue("size/width", width);
    qSettings.setValue("size/height", height);
}

int Settings::getWidth() const {
    return width;
}

void Settings::setWidth(int size) {
    Settings::width = size;
}

int Settings::getHeight() const {
    return height;
}

void Settings::setHeight(int size) {
    Settings::height = size;
}

QColor Settings::getColorWork() {
    return colorWork;
}

void Settings::setColorWork(QColor color) {
    Settings::colorWork = std::move(color);
}

QColor Settings::getColorPause() {
    return colorPause;
}

void Settings::setColorPause(QColor color) {
    Settings::colorPause = std::move(color);
}

QColor Settings::getColorStop() {
    return colorStop;
}

void Settings::setColorStop(QColor color) {
    Settings::colorStop = std::move(color);
}
