#include "model.h"

#include <QtNetwork>

static const auto columnEnumerator = Model::staticMetaObject.enumerator(
            Model::staticMetaObject.indexOfEnumerator("Column"));
static QNetworkAccessManager networkAccessManager;

Model::Model(QObject *parent)
    : QAbstractItemModel(parent)
{
    QTimer::singleShot(0, this, &Model::update);
}

QModelIndex Model::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column);
}

QModelIndex Model::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int Model::rowCount(const QModelIndex &) const
{
    return int(tweets.size());
}

int Model::columnCount(const QModelIndex &) const
{
    return columnEnumerator.keyCount();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole) {
        switch (index.column()) {
        case User: return std::get<User>(tweets[std::size_t(index.row())]);
        case ProfileImage: return std::get<ProfileImage>(tweets[std::size_t(index.row())]);
        case Text: return std::get<Text>(tweets[std::size_t(index.row())]);
        }
    }
    return QVariant();
}

void Model::parseJson(const QByteArray &data)
{
    QJsonParseError parseError;
    const auto document = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error) {
        qCritical() << "TwitterTimelineModel::parseJson. Error at:" << parseError.offset
                    << parseError.errorString();
        return;
    } else if (document.isObject()) {
        // Error received :(
        const auto object = document.object();
        const auto errorArray = object.value("errors").toArray();
        if (errorArray.size() > 0){
            QStringList errors;
            for (const auto error : errorArray) {
                Q_ASSERT(error.isObject());
                Q_ASSERT(error.toObject().contains("message"));
                errors.append(error.toObject().value("message").toString());
            }
            qCritical() << errors.join("<br />");
            return;
        } else if (object.contains("statuses")) {
            const auto array = object.value("statuses").toArray();
            if (array.isEmpty())
                return;
            beginInsertRows(QModelIndex(),
                            int(tweets.size()),
                            int(tweets.size()) + array.size() - 1);
            for (const auto &value : array) {
                Q_ASSERT(value.isObject());
                const auto object = value.toObject();
                const auto user = object.value("user").toObject();
                tweets.emplace_back(user.value("screen_name").toString(),
                                    user.value("profile_image_url").toString(),
                                    object.value("text").toString());
            }
            endInsertRows();
        }
    }
}

void Model::update()
{
    QUrl url("https://api.twitter.com/1.1/search/tweets.json");
    QNetworkRequest request;
    QUrlQuery query;
    query.addQueryItem("count", "10"); // Limit results
    query.addQueryItem("q", "#ubucon");
    url.setQuery(query);
    request.setUrl(url);

    const auto authorizationString = QString("Bearer %1").arg(qgetenv("BEARER").data());
    request.setRawHeader(QByteArrayLiteral("Authorization"), authorizationString.toUtf8());

    auto reply = networkAccessManager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        parseJson(reply->readAll());
    });
    QObject::connect(reply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error),
                     [reply](QNetworkReply::NetworkError) {
        // qOverload<> requires C++14
        qDebug() << reply->errorString();
    });
}
