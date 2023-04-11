#include "model/TaskModel.h"
#include "DataBase.h"
#include <QDebug>

TaskModel::TaskModel(QObject *parent) : QSqlRelationalTableModel(parent) {
    this->setTable(TABLE_TASK);
    this->setRelation(2, QSqlRelation(TABLE_TASK_STATUS, "type", "name"));

    QStringList headersColumnsList;
    headersColumnsList.append(tr("id"));                // 0
    headersColumnsList.append(tr("Name"));              // 1
    headersColumnsList.append(tr("Status"));            // 2
    headersColumnsList.append(tr("Description"));       // 3
    headersColumnsList.append(tr("Created"));           // 4
    headersColumnsList.append(tr("Updated"));           // 5

    for (int i = 0, j = 0; i < this->columnCount(); i++, j++) {
        this->setHeaderData(i, Qt::Horizontal, headersColumnsList[j]);
    }

    this->setSort(5, Qt::DescendingOrder);
    this->setEditStrategy(QSqlTableModel::OnManualSubmit);
}


QVariant TaskModel::data(const QModelIndex &item, int role) const {
    return QSqlRelationalTableModel::data(item, role);
}

bool TaskModel::setData(const QModelIndex &item, const QVariant &value, int role) {

    if (item.column() == CREATED && value.toString().isEmpty()) {
        QVariant val(DataBase::getCurrentDateTime());
        return QSqlRelationalTableModel::setData(item, val, role);
    } else if (item.column() == UPDATED) {
        QVariant val(DataBase::getCurrentDateTime());
        return QSqlRelationalTableModel::setData(item, val, role);
    }

    return QSqlRelationalTableModel::setData(item, value, role);
}