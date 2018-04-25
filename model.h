#ifndef MODEL_H
#define MODEL_H

#include <QtGui>

#include <tuple>
#include <vector>

class Model : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Column {
        User = 0,
        ProfileImage,
        Text,
    };
    Q_ENUM(Column)

    Model(QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const
        override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    void parseJson(const QByteArray &data);
    void update();

    std::vector<std::tuple<QString, QString, QString>> tweets;
};

#endif // MODEL_H
