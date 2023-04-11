#ifndef TASK_TIME_MODEL_H
#define TASK_TIME_MODEL_H

#include <QSqlRelationalTableModel>

class TaskTimeModel : public QSqlRelationalTableModel {
Q_OBJECT
public:
    explicit TaskTimeModel(QObject *parent = nullptr);
};


#endif // TASK_TIME_MODEL_H
