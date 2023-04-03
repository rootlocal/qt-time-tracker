#include "delegate/SqlDelegate.h"

SqlDelegate::SqlDelegate(QObject *parent) : QSqlRelationalDelegate(parent) {
}

SqlDelegate::~SqlDelegate() = default;

QWidget *SqlDelegate::createEditor(QWidget *parent,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const {
    return QSqlRelationalDelegate::createEditor(parent, option, index);
}

void SqlDelegate::setEditorData(QWidget *editor,
                                const QModelIndex &index) const {
    QSqlRelationalDelegate::setEditorData(editor, index);
}

void SqlDelegate::setModelData(QWidget *editor,
                               QAbstractItemModel *model,
                               const QModelIndex &index) const {
    QSqlRelationalDelegate::setModelData(editor, model, index);
}

void SqlDelegate::updateEditorGeometry(QWidget *editor,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const {
    QItemDelegate::updateEditorGeometry(editor, option, index);
}