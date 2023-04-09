#ifndef LIST_TASK_WINDOW_H
#define LIST_TASK_WINDOW_H

#include "QDialog"

class Settings;

class QDataWidgetMapper;

class QSqlTableModel;

class QSqlQueryModel;

namespace Ui {
    class TaskListWindow;
}

class TaskListWindow : public QDialog {
Q_OBJECT

public:
    explicit TaskListWindow(Settings *mSettings, QWidget *parent = nullptr);

    ~TaskListWindow() override;

signals:

    void signalSelectedTask(quint64 taskId);

public slots:

    void accept() override;

    void reject() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::TaskListWindow *ui;
    Settings *settings;
    QSqlQueryModel *model;
};


#endif // LIST_TASK_WINDOW_H
