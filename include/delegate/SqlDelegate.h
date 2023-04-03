#ifndef SQL_DELEGATE_H
#define SQL_DELEGATE_H

#include <QSqlRelationalDelegate>

class SqlDelegate : public QSqlRelationalDelegate {
Q_OBJECT

public:
    explicit SqlDelegate(QObject *parent = nullptr);

    ~SqlDelegate() override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};


#endif // SQL_DELEGATE_H
