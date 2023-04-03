#ifndef BOOLEAN_ITEM_MODEL_H
#define BOOLEAN_ITEM_MODEL_H

#include <QAbstractListModel>

class BooleanItemModel : public QAbstractListModel {
Q_OBJECT

    typedef QPair<QVariant, QVariant> DataPair;
    QList<DataPair> m_content;

public:
    explicit BooleanItemModel(QObject *parent = nullptr);

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
};

#endif // BOOLEAN_ITEM_MODEL_H
