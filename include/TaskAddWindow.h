#ifndef ADD_TASK_WINDOW_H
#define ADD_TASK_WINDOW_H

#include <QDialog>
#include <QSqlTableModel>

class QDataWidgetMapper;

class QSqlRelationalDelegate;

class QSqlRelationalTableModel;

class QSqlQueryModel;

namespace Ui {
    class TaskAddWindow;
}

class TaskAddWindow : public QDialog {
Q_OBJECT
public:
    explicit TaskAddWindow(QSqlRelationalTableModel *parentModel, int row = -1, QWidget *parent = nullptr);

    ~TaskAddWindow() override;

signals:

    void signalReady();

public slots:

    void accept() override;

    void reject() override;

    void slotUpdateButtons(int index);

private:
    Ui::TaskAddWindow *ui;
    QSqlRelationalTableModel *model;
    QDataWidgetMapper *mapper;
    QSqlQueryModel *statusModel;
};


#endif // ADD_TASK_WINDOW_H
