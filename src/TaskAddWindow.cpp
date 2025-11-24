#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlRelationalDelegate>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>

#include "TaskAddWindow.h"
#include "ui_task_add_dialog.h"
#include "DataBase.h"

QT_USE_NAMESPACE

TaskAddWindow::TaskAddWindow(QSqlRelationalTableModel *parentModel, int row, QWidget *parent) : QDialog(parent),
                                                                                                ui(new Ui::TaskAddWindow),
                                                                                                model(parentModel) {
    ui->setupUi(this);
    statusModel = new QSqlQueryModel(this);
    QString statusModelQueryString = QString("SELECT t.id, t.type, t.name FROM %1 as t").arg(TABLE_TASK_STATUS);
    QSqlQuery statusModelQuery;
    statusModelQuery.exec(statusModelQueryString);
    statusModel->setQuery(statusModelQuery);
    statusModel->setHeaderData(0, Qt::Horizontal, "Status Id", Qt::DisplayRole);
    statusModel->setHeaderData(1, Qt::Horizontal, "Status Type", Qt::DisplayRole);
    statusModel->setHeaderData(2, Qt::Horizontal, "Status Name", Qt::DisplayRole);
    ui->comboBoxStatus->clear();
    ui->comboBoxStatus->setModel(statusModel);
    ui->comboBoxStatus->setModelColumn(2);
    model->setRelation(2, QSqlRelation(TABLE_TASK_STATUS, "type", "name"));
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    /** Инициализируем mapper и привязываем поля данных к объектам */
    mapper = new QDataWidgetMapper();
    mapper->setModel(model);
    mapper->addMapping(ui->lineEditName, 1);
    mapper->addMapping(ui->comboBoxStatus, 2);
    mapper->addMapping(ui->lineEditDescription, 3);
    mapper->addMapping(ui->lineEditCreated, 4);
    mapper->addMapping(ui->lineEditUpdated, 5);
    /** Ручное подтверждение изменения данных через mapper */
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));

    /**
     * Если строка не задана, то есть равна -1,
     * тогда диалог работает по принципу создания новой записи.
     * А именно, в модель вставляется новая строка и работа ведётся с ней.
     */
    if (row == -1) {
        model->insertRow(model->rowCount(QModelIndex()));
        mapper->toLast();
        ui->btnNext->setEnabled(false);
        ui->btnPrevious->setEnabled(false);
    } else {
        /** В противном случае диалог настраивается на заданную запись */
        mapper->setCurrentModelIndex(model->index(row, 0));

        /** Подключаем коннекты от кнопок пролистывания к прилистыванию модели данных в mapper */
        connect(ui->btnPrevious, &QPushButton::clicked, mapper, &QDataWidgetMapper::toPrevious);
        connect(ui->btnNext, &QPushButton::clicked, mapper, &QDataWidgetMapper::toNext);
        /** При изменении индекса в mapper изменяем состояние кнопок */
        connect(mapper, &QDataWidgetMapper::currentIndexChanged, this, &TaskAddWindow::slotUpdateButtons);
    }

}

TaskAddWindow::~TaskAddWindow() {
    delete ui;
    delete mapper;
    delete statusModel;
}

void TaskAddWindow::accept() {
    QVariant id = model->data(model->index(mapper->currentIndex(), 0), Qt::DisplayRole);
    if (ui->lineEditName->text().isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("Name is empty"));
        return;
    }

    QSqlQuery query;
    QString strIsExitsName = QString("SELECT EXISTS (SELECT name FROM %1 WHERE name = :name AND id NOT LIKE :id)")
            .arg(TABLE_TASK);

    query.prepare(strIsExitsName);
    query.bindValue(":name", ui->lineEditName->text());
    query.bindValue(":id", id.toString());
    query.exec();
    query.next();

    if (query.value(0) != 0) {
        QMessageBox::critical(this, tr("Warning"), tr("A task with the same name already exists"));
        return;
    }

    mapper->submit();
    model->database().transaction();

    if (!model->submitAll()) {
        model->database().rollback();
        QString error = model->database().lastError().text();
        QString message = QString(tr("Error Create Task. Rollback.\n%1").arg(error));
        QMessageBox::critical(this, "Error", message);
        return;
    }

    model->database().commit();
    emit signalReady();
    this->close();
}

void TaskAddWindow::reject() {
    emit signalReady();
    this->hide();
}

/** Метод изменения состояния активности кнопок пролистывания
 *
 * В том случае, если мы достигаем одного из крайних (самый первый или самый последний) из индексов в таблице данных,
 * то мы изменяем состояние соответствующей кнопки на состояние неактивна
 */
void TaskAddWindow::slotUpdateButtons(int index) {
    ui->btnPrevious->setEnabled(index > 0);
    ui->btnNext->setEnabled(index < model->rowCount() - 1);
}