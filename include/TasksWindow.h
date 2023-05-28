#ifndef TASKS_WINDOW_H
#define TASKS_WINDOW_H

#include <QMainWindow>

class TableView;

class QSqlRelationalTableModel;

class DataBase;

class Settings;

class QMenu;

class TaskModel;

class TaskTimeModel;

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
    void moveEvent(QMoveEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

public slots:
    void show();

    void customTaskMenuRequested(QPoint pos);

    void customTaskHeaderMenuRequested(QPoint pos);

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
    TaskModel *modelTask;
    TaskTimeModel *modelTime;
    QMenu *customTaskMenu;

    void createUI();

    void deleteTask(int row);

    int getSelectedTaskId(const QModelIndex &index);

    uint getSelectedTaskId();
};


#endif // TASKS_WINDOW_H
