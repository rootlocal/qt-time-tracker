#ifndef UTILS_H
#define UTILS_H

#include <QObject>

#define MAX_BACKUPS_DB_TASK_FILES  5

class Utils : public QObject {
Q_OBJECT

public:
    explicit Utils(QObject *parent = nullptr);

    bool backupDatabase();

private:
    bool cleanBackupDatabase();

};


#endif //UTILS_H
