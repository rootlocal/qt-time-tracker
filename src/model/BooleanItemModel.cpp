#include "model/BooleanItemModel.h"

BooleanItemModel::BooleanItemModel(QObject *parent) : QAbstractListModel(parent) {
    m_content << qMakePair(DataPair::first_type("Disabled"), DataPair::second_type(0))
              << qMakePair(DataPair::first_type("Enabled"), DataPair::second_type(1));
}

QVariant BooleanItemModel::data(const QModelIndex &index, int role) const {
    const DataPair &data = m_content.at(index.row());
    QVariant value;

    switch (role) {
        case Qt::DisplayRole: {
            value = data.first;
        }
            break;

        case Qt::UserRole: {
            value = data.second;
        }
            break;

        default:
            break;
    }

    return value;
}

int BooleanItemModel::rowCount(const QModelIndex &parent) const {
    return m_content.count();
}