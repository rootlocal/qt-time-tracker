#include "view/TableView.h"
#include <QDebug>
#include <QSqlTableModel>
#include <QDebug>

TableView::TableView(QWidget *parent) : QTableView(parent) {
    horizontalHeaderView = new HorizontalHeaderView(this);
    this->setHorizontalHeader(horizontalHeaderView);
}

TableView::TableView(QStandardItemModel *item_model, QWidget *parent) : QTableView(parent) {
    horizontalHeaderView = new HorizontalHeaderView(this);
    this->setHorizontalHeader(horizontalHeaderView);
    this->setModel(item_model);
}

TableView::~TableView() {
    delete horizontalHeaderView;
}

void TableView::keyPressEvent(QKeyEvent *event) {

    // insert
    if ((event->key() == Qt::Key_I) && (event->modifiers() == Qt::ControlModifier)) {
        //qDebug() << "Control+I";
        this->model()->insertRow(this->model()->rowCount());
    }

    // save all
    if ((event->key() == Qt::Key_S) && (event->modifiers() == Qt::ControlModifier)) {
        //qDebug() << "Control+S";
        ((QSqlTableModel *) model())->submitAll();
    }

    // revert all
    if ((event->key() == Qt::Key_U) && (event->modifiers() == Qt::ControlModifier)) {
        //qDebug() << "Control+U";
        ((QSqlTableModel *) model())->revertAll();
    }

    QTableView::keyPressEvent(event);
}

HorizontalHeaderView::HorizontalHeaderView(QWidget *parent) :
        QHeaderView(Qt::Horizontal, parent) {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setSectionsMovable(true);
    this->setSectionsClickable(true);
    this->setSectionResizeMode(QHeaderView::Interactive);
}

HorizontalHeaderView::~HorizontalHeaderView() = default;

void HorizontalHeaderView::mousePressEvent(QMouseEvent *event) {
    /*if (event->button() == Qt::MiddleButton) {
        this->sectionPressed(this->logicalIndexAt(event->pos()));
        qDebug() << "mousePressEvent MiddleButton: " << event->pos();
    } else if (event->button() == Qt::LeftButton) {
        qDebug() << "mousePressEvent LeftButton: " << event->pos();
    } else if (event->button() == Qt::RightButton) {
        qDebug() << "mousePressEvent RightButton: " << event->pos();
    }*/

    QHeaderView::mousePressEvent(event);
}

void HorizontalHeaderView::mouseMoveEvent(QMouseEvent *event) {
    QHeaderView::mouseMoveEvent(event);
}

void HorizontalHeaderView::mouseReleaseEvent(QMouseEvent *event) {
    QHeaderView::mouseReleaseEvent(event);
}