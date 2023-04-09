#ifndef COMBO_BOX_ITEM_DELEGATE_H
#define COMBO_BOX_ITEM_DELEGATE_H

#include <QItemDelegate>

class QSqlRelationalDelegate;

class QComboBox;

class ComboBoxItemDelegate : public QItemDelegate {
Q_OBJECT

public:
    explicit ComboBoxItemDelegate(QStringList valueList, QObject *parent = nullptr);

    ~ComboBoxItemDelegate() override;

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

signals:

private:
    QStringList values;
};


#endif // COMBO_BOX_ITEM_DELEGATE_H
