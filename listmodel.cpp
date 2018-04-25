#include "listmodel.h"

const auto columnEnumerator = Model::staticMetaObject.enumerator(
            Model::staticMetaObject.indexOfEnumerator("Column"));

ListModel::ListModel(QObject *parent) : QAbstractListModel(parent)
{
    connect(&model, &Model::rowsAboutToBeInserted, this, &ListModel::rowsAboutToBeInserted);
    connect(&model, &Model::rowsInserted, this, &ListModel::rowsInserted);
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    return {
        { Model::User, "user" },
        { Model::ProfileImage, "profileImage" },
        { Model::Text, "tweet" },
    };
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    return model.rowCount(parent);
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    return model.data(model.index(index.row(), role), Qt::DisplayRole);
}
