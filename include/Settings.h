#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QPoint>
#include <QSize>

#define SETTINGS_ORGANIZATION "rootlocal"
#define SETTINGS_APPLICATION_NAME "timer"

class Settings : public QObject {
public:
    explicit Settings(QObject *parent = nullptr);

    ~Settings() override;

    struct timerColorsStruct {
        QColor work = QColor(150, 255, 150);
        QColor pause = QColor(255, 255, 120);
        QColor stop = QColor(255, 100, 100);
    };

    void load();

    void write();

    const QSize &getTimerWindowsSize() const;

    void setTimerWindowsSize(const QSize &size);

    const QPoint &getTimerWindowsPosition() const;

    void setTimerWindowsPosition(const QPoint &position);

    const QByteArray &getSettingsGeometry() const;

    void setSettingsGeometry(const QByteArray &geometry);

    // COLORS
    const timerColorsStruct &getTimerColors() const;

    void setTimerColors(timerColorsStruct &colors);

    const timerColorsStruct &getTimerColorsDefault() const;

private:
    QSettings qSettings;
    QPoint timerWindowsPosition = QPoint(0, 0);
    QSize timerWindowsSize = QSize(124, 28);
    QByteArray settingsGeometry;
    timerColorsStruct timerColors;
    timerColorsStruct timerColorsDefault;
};


#endif // SETTINGS_H
