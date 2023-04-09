#ifndef TASKS_WINDOW_H
#define TASKS_WINDOW_H

#include <QMainWindow>

class TableView;

class QSqlRelationalTableModel;

class DataBase;

class Settings;

QT_BEGIN_NAMESPACE
namespace Ui {
    class TasksWindow;
}
QT_END_NAMESPACE

class TasksWindow : public QMainWindow {
Q_OBJECT
public:
    explicit TasksWindow(Settings *mSettings, QWidget *parent = nullptr);

    ~TasksWindow() override;

signals:

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:

    void customMenuRequested(QPoint pos);

    void customHeaderMenuRequested(QPoint pos);

private slots:

    void slotAddTask();

    void slotDeleteTask();

    void slotUpdateModels();

    void slotActionEditTask();

    void slotEditTask(QModelIndex index);

    void slotViewTimeTask(QModelIndex index);

    void slotSelectedTask(const QModelIndex &index);

    void slotSelectedTimeTask(const QModelIndex &index);

private:
    Ui::TasksWindow *ui;
    Settings *settings;
    DataBase *db;

    QSqlRelationalTableModel *modelTaskStatus;
    QSqlRelationalTableModel *modelTask;
    QSqlRelationalTableModel *modelTime;

    void setupTaskModel();

    void setupTimeModel();

    void createUI();

    void deleteTask(int row);

    int getSelectedTaskId(const QModelIndex &index);

    uint getSelectedTaskId();
};


#endif // TASKS_WINDOW_H
