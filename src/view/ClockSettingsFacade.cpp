#include "view/ClockSettingsFacade.h"

QT_USE_NAMESPACE

ClockSettingsFacade::ClockSettingsFacade(QObject *parent, Settings *s) : QObject(parent), settings(s) {}

ClockSettingsFacade::~ClockSettingsFacade() = default;

QSize ClockSettingsFacade::getWindowsSize() const {

    if (settings->getIsTimerCustomSize()) {
        return settings->getTimerWindowCustomSize();
    } else {
        QString sizeStr = settings->getTimerWindowSize();
        return settings->getTimerWindowSize(sizeStr);
    }

}