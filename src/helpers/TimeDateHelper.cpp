#include "helpers/TimeDateHelper.h"
#include <cmath>
#include <QVariant>

QT_USE_NAMESPACE

QString TimeDateHelper::formatTimeToString(const quint64 &timeSec) {
    timerClockStruct t;
    t.hours = static_cast<quint32>(floor(timeSec / 3600));
    t.minutes = static_cast<quint64>(floor((timeSec % 3600) / 60));
    t.seconds = static_cast<quint8>(floor(timeSec % 60));

    return QString("%1:%2:%3")
            .arg(t.hours, 3, 10, QChar('0'))
            .arg(t.minutes, 2, 10, QChar('0'))
            .arg(t.seconds, 2, 10, QChar('0'));
}

QString TimeDateHelper::formatTimeToString(const QVariant &timeSec) {
    quint64 intTime = 0;

    if (timeSec.canConvert<quint64>()) {
        intTime = timeSec.toULongLong();
    }

    return formatTimeToString(intTime);
}
