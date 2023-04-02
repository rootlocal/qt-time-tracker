#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

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

private:
    QSettings qSettings;
    int width = 124;
    int height = 28;
    QColor colorWork;
    QColor colorPause;
    QColor colorStop;
};


#endif // SETTINGS_H
