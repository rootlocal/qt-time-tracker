#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QVariantList>
#include <QMessageBox>
#include "ui_task_list_dialog.h"
#include "DataBase.h"
#include "TaskListWindow.h"
#include "Settings.h"

QT_USE_NAMESPACE

TaskListWindow::TaskListWindow(Settings *mSettings, QWidget *parent) : QDialog(parent),
                                                                       ui(new Ui::TaskListWindow) {
    settings = mSettings;
    ui->setupUi(this);

    if (!settings->getWindowGeometry("taskList").isEmpty()) {
        restoreGeometry(settings->getWindowGeometry("taskList"));
    }

    this->resize(420, 100);

    ui->comboBoxTasks->clear();
    QStringList statusesIds;
    statusesIds << QString::number(static_cast<uint>(DataBase::taskStatusEnum::NEW))
                << QString::number(static_cast<uint>(DataBase::taskStatusEnum::WORK));

    model = new QSqlQueryModel(this);
    QString const queryString = QString("SELECT id, name, created, updated, description status "
                                        "FROM %1 as task WHERE status IN (%2) "
                                        "ORDER BY updated DESC").arg(TABLE_TASK, statusesIds.join(","));
    QSqlQuery modelQuery;
    modelQuery.prepare(queryString);
    modelQuery.exec();
    model->setQuery(modelQuery);

    model->setHeaderData(0, Qt::Horizontal, "Task Id", Qt::DisplayRole);
    model->setHeaderData(1, Qt::Horizontal, "Task Name", Qt::DisplayRole);
    model->setHeaderData(2, Qt::Horizontal, "Task Created", Qt::DisplayRole);
    model->setHeaderData(3, Qt::Horizontal, "Task Updated", Qt::DisplayRole);
    model->setHeaderData(4, Qt::Horizontal, "Task Is Active", Qt::DisplayRole);
    model->setHeaderData(5, Qt::Horizontal, "Task Description", Qt::DisplayRole);
    model->setHeaderData(6, Qt::Horizontal, "Task Status", Qt::DisplayRole);

    ui->comboBoxTasks->setModel(model);
    ui->comboBoxTasks->setModelColumn(1);
}

TaskListWindow::~TaskListWindow() {
    delete ui;
    delete model;
}

void TaskListWindow::accept() {
    QSqlRecord record = model->record(ui->comboBoxTasks->currentIndex());

    if (record.value("id").isNull()) {
        QMessageBox::critical(this, tr("Error"), tr("Name is empty"));
        return;
    }

    quint64 taskId = record.value("id").toLongLong();
    emit signalSelectedTask(taskId);
    this->close();
}

void TaskListWindow::reject() {
    settings->setWindowGeometry("taskList", saveGeometry());
    this->hide();
}

void TaskListWindow::closeEvent(QCloseEvent *event) {
    settings->setWindowGeometry("taskList", saveGeometry());
}
