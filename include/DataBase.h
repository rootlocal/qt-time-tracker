#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

#define TABLE_TASK_STATUS       "task_status"
#define TABLE_TASK              "task"
#define TABLE_TASK_TIME         "task_time"

class DataBase : public QObject {
Q_OBJECT

public:

    enum struct taskStatusEnum {
        NEW = 10,
        WORK = 11,
        CLOSED = 12,
    };

    explicit DataBase(QObject *parent = nullptr);

    ~DataBase() override;

    void connectToDataBase();

    bool isStatusExits(const taskStatusEnum &type);

    bool insertIntoTaskStatusTable(const taskStatusEnum &type, const QString &name);

    bool isTaskExits(const QString &name);

    int taskCount();

    bool insertIntoTaskTable(const QString &name, const QString &description);

    int taskTimeCount();

    bool insertIntoTimeTable(const QVariantList &data);

    void closeDataBase();

    static QString getCurrentDateTime();

private:
    QSqlDatabase db;
    QString databaseName;

private:
    bool openDataBase();

    bool restoreDataBase();

    bool createTables();

    bool executeSqlList(const QList<QString> &sqlCommands);
};


#endif // DATABASE_H
