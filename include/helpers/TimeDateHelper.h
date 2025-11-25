#ifndef TIME_DATE_HELPER_H
#define TIME_DATE_HELPER_H

#include <QObject>

class QVariant;

class TimeDateHelper : public QObject {
Q_OBJECT

public:
    struct timerClockStruct {
        quint32 hours = 0;
        quint64 minutes = 0;
        quint8 seconds = 0;
    };

    static QString formatTimeToString(const quint64 &timeSec);

    static QString formatTimeToString(const QVariant &timeSec);

};

#endif // TIME_DATE_HELPER_H
