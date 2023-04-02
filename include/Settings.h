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

    void load();

    void write();

    int getWidth() const;

    void setWidth(int size);

    int getHeight() const;

    void setHeight(int size);

    QColor getColorWork();

    void setColorWork(QColor color);

    QColor getColorPause();

    void setColorPause(QColor color);

    QColor getColorStop();

    void setColorStop(QColor color);

    void savePosition(QPoint pos);

    QPoint restorePosition();

private:
    QSettings qSettings;
    int width = 124;
    int height = 28;
    QColor colorWork;
    QColor colorPause;
    QColor colorStop;

    QPoint timerWindowsPosition;
    //QSize timerWindowsSize;
};


#endif // SETTINGS_H
