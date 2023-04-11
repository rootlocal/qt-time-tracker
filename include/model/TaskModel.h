#ifndef TASK_MODEL_H
#define TASK_MODEL_H

#include <QSqlRelationalTableModel>

class TaskModel : public QSqlRelationalTableModel {
Q_OBJECT
public:
    enum attributesEnum {
        ID = 0,
        NAME = 1,
        STATUS = 2,
        DESCRIPTION = 3,
        CREATED = 4,
        UPDATED = 5,
    };

    explicit TaskModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &item, const QVariant &value, int role = Qt::EditRole) override;

private:

};


#endif // TASK_MODEL_H
