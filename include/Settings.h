#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QPoint>
#include <QSize>
#include <QColor>

#define SETTINGS_ORGANIZATION "rootlocal"
#define SETTINGS_APPLICATION_NAME "timer"

class Settings : public QObject {
Q_OBJECT

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

    const QSize &getTimerWindowCustomSize() const;

    void setTimerWindowCustomSize(const QSize &size);

    const QString &getTimerWindowSize() const;

    void setTimerWindowSize(const QString &size);

    const QPoint &getTimerWindowPosition() const;

    void setTimerWindowPosition(const QPoint &position);

    const bool &getIsTimerCustomSize() const;

    void setIsTimerCustomSize(const bool &value);

    const QByteArray &getSettingGeometry() const;

    void setSettingGeometry(const QByteArray &geometry);

    // COLORS
    const timerColorsStruct &getTimerColors() const;

    void setTimerColors(timerColorsStruct &colors);

    const timerColorsStruct &getTimerColorsDefault() const;

    QList<QString> getTimerWindowSizesItems();

    QString getTimerWindowSizesItem(int index);

    QMap<QString, QSize> getTimerWindowSizes();

    QSize getTimerWindowSize(const QString &name);

private:
    QSettings qSettings;
    QPoint timerWindowPosition = QPoint(0, 0);
    QSize timerWindowCustomSize = QSize(124, 28);
    QString timerWindowSize = "Normal";
    QByteArray settingsGeometry;
    timerColorsStruct timerColors;
    timerColorsStruct timerColorsDefault;
    bool isTimerCustomSize = false;

    QList<QString> timerWindowSizesItems;
    QMap<QString, QSize> timerWindowSizes;
};


#endif // SETTINGS_H
