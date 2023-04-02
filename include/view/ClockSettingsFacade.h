#ifndef CLOCK_SETTINGS_FACADE_H
#define CLOCK_SETTINGS_FACADE_H

#include <QObject>
#include "Settings.h"

class ClockSettingsFacade : public QObject {
Q_OBJECT

public:
    explicit ClockSettingsFacade(QObject *parent = nullptr, Settings *s = nullptr);

    ~ClockSettingsFacade() override;

    QSize getWindowsSize() const;

private:
    Settings *settings;
};


#endif // CLOCK_SETTINGS_FACADE_H
