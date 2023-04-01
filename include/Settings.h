#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject {
public:
    explicit Settings(QObject *parent = nullptr);

    ~Settings() override;

private:


};


#endif // SETTINGS_H
