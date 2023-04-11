#include "model/TaskTimeModel.h"
#include "DataBase.h"
#include <QDebug>

TaskTimeModel::TaskTimeModel(QObject *parent) : QSqlRelationalTableModel(parent) {
    this->setTable(TABLE_TASK_TIME);
    this->setRelation(1, QSqlRelation(TABLE_TASK, "id", "name"));

    QStringList headersColumnsList;
    headersColumnsList.append(tr("id"));                    // 0
    headersColumnsList.append(tr("Task"));                  // 1
    headersColumnsList.append(tr("Time"));                  // 2
    headersColumnsList.append(tr("Created"));               // 3
    headersColumnsList.append(tr("Updated"));               // 4
    headersColumnsList.append(tr("Description"));           // 5

    for (int i = 0, j = 0; i < this->columnCount(); i++, j++) {
        this->setHeaderData(i, Qt::Horizontal, headersColumnsList[j]);
    }

    this->setSort(4, Qt::DescendingOrder);
    this->setEditStrategy(QSqlTableModel::OnManualSubmit);
}