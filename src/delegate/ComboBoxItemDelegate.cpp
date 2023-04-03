#include "delegate/ComboBoxItemDelegate.h"
#include <QDebug>
#include <utility>

ComboBoxItemDelegate::ComboBoxItemDelegate(QStringList valueList,
                                           QObject *parent) : QItemDelegate(parent),
                                                              values(std::move(valueList)) {
}

ComboBoxItemDelegate::~ComboBoxItemDelegate() = default;

/**  create widget for use */
QWidget *ComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const {
    auto *comboBox = new QComboBox(parent);
    comboBox->addItems(values);
    return comboBox;
}

/** update model widget */
void ComboBoxItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
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
void ComboBoxItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                        const QModelIndex &index) const {
    auto *comboBox = dynamic_cast<QComboBox *>(editor);
    int value = comboBox->currentIndex();
    model->setData(index, value, Qt::EditRole);
}

void ComboBoxItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}
