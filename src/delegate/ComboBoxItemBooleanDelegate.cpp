#include <QDebug>
#include <QComboBox>
#include <QStringList>
#include "delegate/ComboBoxItemBooleanDelegate.h"
#include "model/BooleanItemModel.h"

ComboBoxItemBooleanDelegate::ComboBoxItemBooleanDelegate(QObject *parent) : QItemDelegate(parent) {
}

QWidget *ComboBoxItemBooleanDelegate::createEditor(QWidget *parent,
                                                   const QStyleOptionViewItem &option,
                                                   const QModelIndex &index) const {
    auto *comboBox = new QComboBox(parent);
    comboBox->setModel(new BooleanItemModel(parent));
    return comboBox;
}

/** update model widget */
void ComboBoxItemBooleanDelegate::setEditorData(QWidget *editor,
                                                const QModelIndex &index) const {
    QString value = index.model()->data(index, Qt::EditRole).toString();
    auto *comboBox = dynamic_cast<QComboBox *>(editor);
    int comboBoxValue = comboBox->findText(value);

    if (comboBoxValue == -1) {
        comboBox->setCurrentIndex(index.data().toInt());
    } else {
        comboBox->setCurrentIndex(comboBoxValue);
    }

}

/** store edited model data to model */
void ComboBoxItemBooleanDelegate::setModelData(QWidget *editor,
                                               QAbstractItemModel *model,
                                               const QModelIndex &index) const {
    auto *comboBox = dynamic_cast<QComboBox *>(editor);
    int value = comboBox->currentIndex();

    model->setData(index, value, Qt::EditRole);
}

void ComboBoxItemBooleanDelegate::updateEditorGeometry(QWidget *editor,
                                                       const QStyleOptionViewItem &option,
                                                       const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}