#include "helpers/TimeDateHelper.h"
#include <cmath>

QT_USE_NAMESPACE

QString TimeDateHelper::formatTimeToString(const quint64 &timeSec) {
    timerClockStruct t;
    t.hours = static_cast<quint32>(floor(timeSec / 3600));
    t.minutes = static_cast<quint64>(floor((timeSec % 3600) / 60));
    t.seconds = static_cast<quint8>(floor(timeSec % 60));

    auto clockText = QString("%1:%2:%3")
            .arg(t.hours, 3, 10, QChar('0'))
            .arg(t.minutes, 2, 10, QChar('0'))
            .arg(t.seconds, 2, 10, QChar('0'));

    return clockText;
}