#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QtGui>

#include "model.h"

class ListModel : public QAbstractListModel
{
public:
    ListModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    Model model;
};

#endif // LISTMODEL_H
