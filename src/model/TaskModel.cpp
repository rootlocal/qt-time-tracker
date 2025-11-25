#include "model/TaskModel.h"
#include "DataBase.h"
#include <QDebug>
#include <QDateTime>

TaskModel::TaskModel(QObject *parent) : QSqlRelationalTableModel(parent) {
    this->setTable(TABLE_TASK);
    this->setRelation(2, QSqlRelation(TABLE_TASK_STATUS, "type", "name"));

    QStringList headersColumnsList;
    headersColumnsList.insert(attributesEnum::ID, tr("id"));
    headersColumnsList.insert(attributesEnum::NAME, tr("Name"));
    headersColumnsList.insert(attributesEnum::STATUS, tr("Status"));
    headersColumnsList.insert(attributesEnum::DESCRIPTION, tr("Description"));
    headersColumnsList.insert(attributesEnum::CREATED, tr("Created"));
    headersColumnsList.insert(attributesEnum::UPDATED, tr("Updated"));

    for (int i = 0, j = 0; i < this->columnCount(); i++, j++) {
        this->setHeaderData(i, Qt::Horizontal, headersColumnsList[j]);
    }

    this->setSort(5, Qt::DescendingOrder);
    this->setEditStrategy(QSqlTableModel::OnManualSubmit);
}


QVariant TaskModel::data(const QModelIndex &item, int role) const {

    if (role == Qt::DisplayRole &&
        (item.column() == attributesEnum::CREATED || item.column() == attributesEnum::UPDATED)) {
        QVariant value = QSqlRelationalTableModel::data(item, role);
        if (value.canConvert<QDateTime>()) {
            QDateTime dateTime = value.toDateTime();
            return dateTime.toString("dd.MM.yyyy HH:mm:ss");
        }
    }

    return QSqlRelationalTableModel::data(item, role);
}

bool TaskModel::setData(const QModelIndex &item, const QVariant &value, int role) {

    if (item.column() == attributesEnum::CREATED && value.toString().isEmpty()) {
        QVariant val(DataBase::getCurrentDateTime());
        return QSqlRelationalTableModel::setData(item, val, role);
    } else if (item.column() == attributesEnum::UPDATED) {
        QVariant val(DataBase::getCurrentDateTime());
        return QSqlRelationalTableModel::setData(item, val, role);
    }

    return QSqlRelationalTableModel::setData(item, value, role);
}