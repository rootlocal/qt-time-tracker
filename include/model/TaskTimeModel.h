#ifndef TASK_TIME_MODEL_H
#define TASK_TIME_MODEL_H

#include <QSqlRelationalTableModel>

class TaskTimeModel : public QSqlRelationalTableModel {
Q_OBJECT
public:
    enum attributesEnum {
        ID = 0,
        TASK = 1,
        TIME = 2,
        CREATED = 3,
        UPDATED = 4,
        DESCRIPTION = 5,
    };

    explicit TaskTimeModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &item, const QVariant &value, int role = Qt::EditRole) override;
};

#endif // TASK_TIME_MODEL_H
