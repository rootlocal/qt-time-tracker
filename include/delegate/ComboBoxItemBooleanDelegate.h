#ifndef COMBO_BOX_ITEM_BOOLEAN_DELEGATE_H
#define COMBO_BOX_ITEM_BOOLEAN_DELEGATE_H

#include <QItemDelegate>

class QStringList;

class ComboBoxItemBooleanDelegate : public QItemDelegate {
Q_OBJECT

public:
    explicit ComboBoxItemBooleanDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const override;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

#endif // COMBO_BOX_ITEM_BOOLEAN_DELEGATE_H
