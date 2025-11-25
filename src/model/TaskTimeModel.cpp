#include <QDebug>
#include <QDateTime>
#include "model/TaskTimeModel.h"
#include "DataBase.h"
#include "helpers/TimeDateHelper.h"

TaskTimeModel::TaskTimeModel(QObject *parent) : QSqlRelationalTableModel(parent) {
    this->setTable(TABLE_TASK_TIME);
    this->setRelation(1, QSqlRelation(TABLE_TASK, "id", "name"));

    QStringList headersColumnsList;
    headersColumnsList.insert(attributesEnum::ID, tr("id"));
    headersColumnsList.insert(attributesEnum::TASK, tr("Task"));
    headersColumnsList.insert(attributesEnum::TIME, tr("Time"));
    headersColumnsList.insert(attributesEnum::CREATED, tr("Created"));
    headersColumnsList.insert(attributesEnum::UPDATED, tr("Updated"));
    headersColumnsList.insert(attributesEnum::DESCRIPTION, tr("Description"));

    for (int i = 0, j = 0; i < this->columnCount(); i++, j++) {
        this->setHeaderData(i, Qt::Horizontal, headersColumnsList[j]);
    }

    this->setSort(4, Qt::DescendingOrder);
    this->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

QVariant TaskTimeModel::data(const QModelIndex &item, int role) const {
    QVariant value = QSqlRelationalTableModel::data(item, role);

    if (role == Qt::DisplayRole) {
        if (item.column() == attributesEnum::CREATED || item.column() == attributesEnum::UPDATED) {

            if (value.canConvert<QDateTime>()) {
                QDateTime dateTime = value.toDateTime();
                return dateTime.toString("dd.MM.yyyy HH:mm:ss");
            }

        } else if (item.column() == attributesEnum::TIME) {
            return TimeDateHelper::formatTimeToString(value);
        }
    }


    return QSqlRelationalTableModel::data(item, role);
}

bool TaskTimeModel::setData(const QModelIndex &item, const QVariant &value, int role) {
    if (item.column() == attributesEnum::CREATED && value.toString().isEmpty()) {
        QVariant val(DataBase::getCurrentDateTime());
        return QSqlRelationalTableModel::setData(item, val, role);
    } else if (item.column() == attributesEnum::UPDATED) {
        QVariant val(DataBase::getCurrentDateTime());
        return QSqlRelationalTableModel::setData(item, val, role);
    }

    return QSqlRelationalTableModel::setData(item, value, role);
}
