#include <QDebug>
#include <QAbstractItemModel>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QMessageBox>
#include "Settings.h"
#include "DataBase.h"
#include "TasksWindow.h"
#include "ui_tasks_window.h"
#include "TaskAddWindow.h"
#include "delegate/SqlDelegate.h"
#include "helpers/TimeDateHelper.h"

QT_USE_NAMESPACE

TasksWindow::TasksWindow(Settings *mSettings, QWidget *parent) : QMainWindow(parent),
                                                                 ui(new Ui::TasksWindow),
                                                                 settings(mSettings) {
    db = new DataBase(this);
    ui->setupUi(this);
    ui->btnDelete->setDisabled(true);
    ui->actionDeleteTask->setDisabled(true);
    ui->actionEditTask->setDisabled(true);
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    this->setupTaskModel();
    this->setupTimeModel();
    this->createUI();
    connect(ui->btnCreate, &QPushButton::clicked, this, &TasksWindow::slotAddTask);
    connect(ui->btnDelete, &QPushButton::clicked, this, &TasksWindow::slotDeleteTask);

    connect(ui->actionCreateTask, &QAction::triggered, this, &TasksWindow::slotAddTask);
    connect(ui->actionEditTask, &QAction::triggered, this, &TasksWindow::slotActionEditTask);
    connect(ui->actionDeleteTask, &QAction::triggered, this, &TasksWindow::slotDeleteTask);

    if (!settings->getWindowGeometry("tasks").isEmpty()) {
        restoreGeometry(settings->getWindowGeometry("tasks"));
    }
}

TasksWindow::~TasksWindow() {
    delete ui;
    delete db;
    delete modelTaskStatus;
    delete modelTask;
    delete modelTime;
}

void TasksWindow::slotUpdateModels() {
    modelTask->select();
    ui->btnDelete->setDisabled(true);
    ui->actionDeleteTask->setDisabled(true);
    ui->actionEditTask->setDisabled(true);
}

void TasksWindow::setupTaskModel() {
    modelTaskStatus = new QSqlRelationalTableModel(this);
    modelTaskStatus->setTable(TABLE_TASK_STATUS);

    modelTask = new QSqlRelationalTableModel(this);
    modelTask->setTable(TABLE_TASK);
    modelTask->setRelation(2, QSqlRelation(TABLE_TASK_STATUS, "type", "name"));

    QStringList headersColumnsList;
    headersColumnsList.append(tr("id"));                // 0
    headersColumnsList.append(tr("Name"));              // 1
    headersColumnsList.append(tr("Status"));            // 2
    headersColumnsList.append(tr("Description"));       // 3
    headersColumnsList.append(tr("Created"));           // 4
    headersColumnsList.append(tr("Updated"));           // 5

    for (int i = 0, j = 0; i < modelTask->columnCount(); i++, j++) {
        modelTask->setHeaderData(i, Qt::Horizontal, headersColumnsList[j]);
    }

    modelTask->setSort(5, Qt::DescendingOrder);
    modelTask->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void TasksWindow::setupTimeModel() {
    modelTime = new QSqlRelationalTableModel(this);
    modelTime->setTable(TABLE_TASK_TIME);
    modelTime->setRelation(1, QSqlRelation(TABLE_TASK, "id", "name"));

    QStringList headersColumnsList;
    headersColumnsList.append(tr("id"));                    // 0
    headersColumnsList.append(tr("Task"));                  // 1
    headersColumnsList.append(tr("Time"));                  // 2
    headersColumnsList.append(tr("Created"));               // 3
    headersColumnsList.append(tr("Updated"));               // 4
    headersColumnsList.append(tr("Description"));           // 5

    for (int i = 0, j = 0; i < modelTime->columnCount(); i++, j++) {
        modelTime->setHeaderData(i, Qt::Horizontal, headersColumnsList[j]);
    }

    modelTime->setSort(4, Qt::DescendingOrder);
    modelTask->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void TasksWindow::createUI() {
    modelTask->select();
    modelTime->select();
    ui->tableViewtasks->setModel(modelTask);
    ui->tableViewtasks->setColumnHidden(0, true); // id
    ui->tableViewtasks->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewtasks->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->tableViewtasks->resizeColumnsToContents();
    ui->tableViewtasks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableViewtasks->setItemDelegate(new SqlDelegate(ui->tableViewtasks));
    //ui->tableViewtasks->setItemDelegateForColumn(4, new ComboBoxItemBooleanDelegate(ui->tableViewtasks));

    ui->tableViewtasks->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewtasks->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewtasks->setShowGrid(true);
    ui->tableViewtasks->setSortingEnabled(true);
    ui->tableViewtasks->setGridStyle(Qt::SolidLine);

    connect(ui->tableViewtasks, &QTableView::doubleClicked, this, &TasksWindow::slotEditTask);
    connect(ui->tableViewtasks, &QTableView::clicked, this, &TasksWindow::slotSelectedTask);

    ui->tableViewtasks->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableViewtasks, &TableView::customContextMenuRequested, this, &TasksWindow::customMenuRequested);

    ui->tableViewtasks->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableViewtasks->horizontalHeader(), &QHeaderView::customContextMenuRequested, this,
            &TasksWindow::customHeaderMenuRequested);

    ui->tableViewtaskTime->setModel(modelTime);
    ui->tableViewtaskTime->setColumnHidden(0, true);
    ui->tableViewtaskTime->setColumnHidden(4, true);
    ui->tableViewtaskTime->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewtaskTime->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->tableViewtaskTime->resizeColumnsToContents();
    ui->tableViewtaskTime->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewtaskTime->setShowGrid(true);
    ui->tableViewtaskTime->setSortingEnabled(true);
    ui->tableViewtaskTime->setGridStyle(Qt::SolidLine);
    ui->tableViewtaskTime->setItemDelegate(new SqlDelegate(ui->tableViewtaskTime));

    ui->tableViewtaskTime->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewtaskTime->horizontalHeader()->setStretchLastSection(true);

    connect(ui->tableViewtaskTime, &QTableView::doubleClicked, this, &TasksWindow::slotViewTimeTask);
    connect(ui->tableViewtaskTime, &QTableView::clicked, this, &TasksWindow::slotSelectedTimeTask);
}

void TasksWindow::slotActionEditTask() {
    QModelIndex index = ui->tableViewtasks->selectionModel()->currentIndex();
    slotEditTask(index);
    ui->btnDelete->setDisabled(true);
    ui->actionDeleteTask->setDisabled(true);
    ui->actionEditTask->setDisabled(true);
}

/**
 * Метод для активации диалога добавления записей в режиме редактирования
 * с передачей индекса выбранной строки
 */
void TasksWindow::slotEditTask(const QModelIndex index) {
    auto *addTaskWindow = new TaskAddWindow(modelTask, index.row(), this);
    QMetaObject::Connection con = connect(addTaskWindow, &TaskAddWindow::signalReady, this,
                                          &TasksWindow::slotUpdateModels);
    addTaskWindow->setWindowTitle(tr("Edit Task"));
    addTaskWindow->exec();
    disconnect(con);
    delete addTaskWindow;
}

// TODO: не сделана форма
void TasksWindow::slotViewTimeTask(QModelIndex index) {
}

void TasksWindow::slotSelectedTask(const QModelIndex &index) {
    modelTime->setFilter(QString("task_id = %1").arg(getSelectedTaskId(index)));
    ui->btnDelete->setDisabled(false);
    ui->actionDeleteTask->setDisabled(false);
    ui->actionEditTask->setDisabled(false);

    QString const querySummaryString = QString("SELECT sum(time) FROM %1 WHERE task_id=:task_id").arg(TABLE_TASK_TIME);
    QSqlQuery modelSummaryTaskTimeQuery;
    QVariant task_id(getSelectedTaskId(index));
    modelSummaryTaskTimeQuery.prepare(querySummaryString);
    modelSummaryTaskTimeQuery.bindValue(":task_id", task_id);
    modelSummaryTaskTimeQuery.exec();
    modelSummaryTaskTimeQuery.next();
    quint64 summaryTime = modelSummaryTaskTimeQuery.value(0).toULongLong();

    if (summaryTime != 0) {
        ui->labelSummaryTime->setText(TimeDateHelper::formatTimeToString(summaryTime));
    } else {
        ui->labelSummaryTime->setText("000:00:00");
    }

}

void TasksWindow::slotSelectedTimeTask(const QModelIndex &index) {
}

void TasksWindow::slotAddTask() {
    auto *addTaskWindow = new TaskAddWindow(modelTask, -1, this);
    QMetaObject::Connection con = connect(addTaskWindow, &TaskAddWindow::signalReady, this,
                                          &TasksWindow::slotUpdateModels);
    addTaskWindow->setWindowTitle(tr("Add Task"));
    addTaskWindow->exec();
    disconnect(con);
    delete addTaskWindow;
}

void TasksWindow::slotDeleteTask() {
    int row = ui->tableViewtasks->selectionModel()->currentIndex().row();
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Confirmation"));
    msgBox.setText(tr("Do You really want to delete this Item?"));
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QMessageBox::Yes) {
        ui->btnDelete->setDisabled(true);
        ui->actionDeleteTask->setDisabled(true);
        ui->actionEditTask->setDisabled(true);
        deleteTask(row);
    }

    /*QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "", "Quit?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {

    }*/
}

void TasksWindow::deleteTask(int row) {
    modelTask->database().transaction();
    modelTask->removeRow(row);

    if (modelTask->submitAll()) {
        modelTask->database().commit();
        // QMessageBox::information(this, "Done", "Deleted");
    } else {
        modelTask->database().rollback();
        QString error = modelTask->database().lastError().text();
        QString message = QString(tr("Error Delete Task. Rollback:\n %1").arg(error));
        QMessageBox::warning(this, tr("Error"), message);
    }

    modelTask->select();
}

void TasksWindow::closeEvent(QCloseEvent *event) {
    settings->setWindowGeometry("tasks", saveGeometry());
}

uint TasksWindow::getSelectedTaskId() {
    return getSelectedTaskId(ui->tableViewtasks->currentIndex());
}

int TasksWindow::getSelectedTaskId(const QModelIndex &index) {
    return ui->tableViewtasks->model()->index(index.row(), 0).data().toInt();
}

void TasksWindow::customMenuRequested(QPoint pos) {
    //QModelIndex index = ui->tableViewtasks->indexAt(pos);
    auto *menu = new QMenu(this);
    auto *actionEdit = new QAction("Edit", this);
    auto *actionDelete = new QAction("Delete", this);

    menu->addAction(actionEdit);
    menu->addSeparator();
    menu->addAction(actionDelete);
    connect(actionEdit, &QAction::triggered, this, &TasksWindow::slotActionEditTask);
    connect(actionDelete, &QAction::triggered, this, &TasksWindow::slotDeleteTask);
    menu->popup(ui->tableViewtasks->viewport()->mapToGlobal(pos));
}

void TasksWindow::customHeaderMenuRequested(QPoint pos) {
    /*
    int column = ui->tableViewtasks->horizontalHeader()->logicalIndexAt(pos);
    auto *menu = new QMenu(this);
    menu->addAction(new QAction("Header Action 1", this));
    menu->addAction(new QAction("Header Action 2", this));
    menu->addAction(new QAction("Header Action 3", this));
    menu->popup(ui->tableViewtasks->horizontalHeader()->viewport()->mapToGlobal(pos));
     */
}