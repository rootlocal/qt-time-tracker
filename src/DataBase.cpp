#include <QDir>
#include <QDateTime>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "DataBase.h"
#include "helpers/FileHelper.h"

QT_USE_NAMESPACE

DataBase::DataBase(QObject *parent) : QObject(parent) {
    databaseName = FileHelper::getTaskDbFile();
}

DataBase::~DataBase() = default;

bool DataBase::connectToDataBase() {

    if (!QFile(databaseName).exists()) {
        qDebug() << "Database does not exist, creating ...";

        if (this->createDataBase()) {
            return this->openDataBase();
        }

    } else {
        return this->openDataBase();
    }

    return false;
}

bool DataBase::createDataBase() {
    qDebug() << "Create database";

    if (!this->createTables()) {
        qCritical() << "Failed to create tables " << databaseName << ": " << db.lastError().text();
        return false;
    }

    if (!isStatusExits(taskStatusEnum::NEW)) {
        insertIntoTaskStatusTable(taskStatusEnum::NEW, tr("New"));
    }

    if (!isStatusExits(taskStatusEnum::WORK)) {
        insertIntoTaskStatusTable(taskStatusEnum::WORK, tr("Work"));
    }

    if (!isStatusExits(taskStatusEnum::CLOSED)) {
        insertIntoTaskStatusTable(taskStatusEnum::CLOSED, tr("Closed"));
    }

    if (taskCount() == 0) {
        insertIntoTaskTable("Default", "Auto created Default task");
    }

    return true;
}

bool DataBase::openDataBase() {
    qDebug() << "Open Database: " << databaseName;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName);

    if (db.open()) {
        QSqlQuery query;
        query.exec("PRAGMA foreign_keys = ON;");
        return true;
    }

    qCritical() << QObject::tr("Database Error: ") << db.lastError().text();

    return false;
}

void DataBase::closeDataBase() {
    db.close();
}

bool DataBase::executeSqlList(const QList<QString> &sqlCommands) {
    QSqlQuery query;
    bool results = true;

            foreach (QString command, sqlCommands) {
            query.clear();

            if (results) {
                db.transaction();

                if (query.exec(command)) {
                    db.commit();
                } else {
                    db.rollback();
                    qDebug() << "Error execute command: " << query.lastError().text();
                    results = false;
                }
            }

        }

    return results;
}

bool DataBase::createTables() {
    qInfo() << "Create tables";

    QList<QString> sql;

    if (!db.tables().contains(TABLE_TASK_STATUS)) {
        sql.append(QString("CREATE TABLE %1 ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "type TINYINT NOT NULL, "
                           "name VARCHAR(255) NOT NULL"
                           ");").arg(TABLE_TASK_STATUS)
        );

        sql.append(QString("CREATE UNIQUE INDEX %1_type_ukey ON %1 (type);").arg(TABLE_TASK_STATUS));
    }

    if (!db.tables().contains(TABLE_TASK)) {
        sql.append(QString("CREATE TABLE %1 ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "name VARCHAR(255) NOT NULL, "
                           "status TINYINT NOT NULL, "
                           "description TEXT NULL, "
                           "created DATETIME NOT NULL, "
                           "updated DATETIME NOT NULL, "
                           "FOREIGN KEY (status) REFERENCES %2(type) ON DELETE SET NULL"
                           ");").arg(TABLE_TASK, TABLE_TASK_STATUS)
        );
    }

    if (!db.tables().contains(TABLE_TASK_TIME)) {
        sql.append(QString("CREATE TABLE %1 ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "task_id INTEGER NOT NULL, "
                           "time UNSIGNED BIG INT NOT NULL, "
                           "created DATETIME NOT NULL, "
                           "updated DATETIME NOT NULL, "
                           "description TEXT NULL, "
                           "FOREIGN KEY (task_id) REFERENCES %2(id) ON DELETE CASCADE ON UPDATE CASCADE"
                           ");").arg(TABLE_TASK_TIME, TABLE_TASK)
        );
    }

    if (!sql.isEmpty()) {
        return this->executeSqlList(sql);
    }

    return true;
}

bool DataBase::isStatusExits(const taskStatusEnum &type) {
    QSqlQuery query;
    query.prepare(QString("SELECT EXISTS (SELECT t.type FROM %1 as t WHERE t.type = :type)").arg(TABLE_TASK_STATUS));
    query.bindValue(":type", static_cast<int>(type));
    query.exec();
    query.next();

    if (query.value(0) != 0) {
        return true;
    }

    return false;
}

bool DataBase::insertIntoTaskStatusTable(const taskStatusEnum &type, const QString &name) {
    QSqlQuery query;

    query.prepare(QString("INSERT INTO %1 (type, name) VALUES (:type, :name)").arg(TABLE_TASK_STATUS));
    query.bindValue(":type", static_cast<int>(type));
    query.bindValue(":name", name);

    db.transaction();
    if (query.exec()) {
        db.commit();
    } else {
        db.rollback();
        qDebug() << QString("Error insert into %1: ").arg(TABLE_TASK_STATUS) << query.lastError().text();
        return false;
    }

    return true;
}

int DataBase::taskCount() {
    QSqlQuery query;
    query.prepare(QString("SELECT count(*) FROM %1").arg(TABLE_TASK));
    query.exec();
    query.next();

    return query.value(0).toInt();
}

bool DataBase::isTaskExits(const QString &name) {
    QSqlQuery query;
    query.prepare(QString("SELECT EXISTS (SELECT t.name FROM %1 as t WHERE t.name = :name)").arg(TABLE_TASK));
    query.bindValue(":name", name);
    query.exec();
    query.next();

    if (query.value(0) != 0) {
        return true;
    }

    return false;
}

bool DataBase::insertIntoTaskTable(const QString &name, const QString &description) {
    QSqlQuery query;
    taskStatusEnum status = taskStatusEnum::NEW;

    query.prepare(QString("INSERT INTO %1 (name, created, updated, description, status) VALUES ("
                          ":name, :created, :updated, :description, :status"
                          ")").arg(TABLE_TASK));

    query.bindValue(":name", name);
    query.bindValue(":description", description);
    query.bindValue(":created", getCurrentDateTime());
    query.bindValue(":updated", getCurrentDateTime());
    query.bindValue(":status", static_cast<int>(status));

    db.transaction();
    if (query.exec()) {
        db.commit();
    } else {
        db.rollback();
        qDebug() << QString("Error insert into %1: ").arg(TABLE_TASK) << query.lastError().text();
        return false;
    }

    return true;
}

int DataBase::taskTimeCount() {
    QSqlQuery query;
    query.prepare(QString("SELECT count(*) FROM %1").arg(TABLE_TASK_TIME));
    query.exec();
    query.next();

    return query.value(0).toInt();
}

bool DataBase::insertIntoTimeTable(const QVariantList &data) {
    QSqlQuery query;

    query.prepare(QString("INSERT INTO %1 (task_id, time, description, created, updated) VALUES ("
                          ":task_id, :time, :description, :created, :updated"
                          ")").arg(TABLE_TASK_TIME));

    query.bindValue(":task_id", data[0].toInt());
    query.bindValue(":time", data[1].toULongLong());
    query.bindValue(":description", data[2].toString());
    query.bindValue(":created", getCurrentDateTime());
    query.bindValue(":updated", getCurrentDateTime());

    db.transaction();
    if (query.exec()) {
        db.commit();
    } else {
        db.rollback();
        qDebug() << QString("Error insert into %1: ").arg(TABLE_TASK_TIME) << query.lastError().text();
        return false;
    }

    return true;
}

QString DataBase::getCurrentDateTime() {
    QDateTime dateTime = QDateTime::currentDateTime().toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc());
    return dateTime.toString(Qt::ISODate);
}

