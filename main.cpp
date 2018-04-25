#include <QtCore>
#include <QtNetwork>

static QNetworkAccessManager networkAccessManager;

void parseJson(const QByteArray &data)
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
            for (const auto &value : array) {
                Q_ASSERT(value.isObject());
                const auto object = value.toObject();
                const auto user = object.value("user").toObject().value("screen_name").toString();
                qDebug() << "Tweet: " << object.value("user").toObject().value("screen_name")
                         << ":" << object.value("text");
            }
        }
    }
}

void request()
{
    QUrl url("https://api.twitter.com/1.1/search/tweets.json");
    QNetworkRequest request;
    QUrlQuery query;
    query.addQueryItem("count", "1"); // Limit results
    query.addQueryItem("q", "#UbuconEU18");
    url.setQuery(query);
    request.setUrl(url);

    const auto authorizationString = QString("Bearer %1").arg(qgetenv("BEARER").data());
    request.setRawHeader(QByteArrayLiteral("Authorization"), authorizationString.toUtf8());

    auto reply = networkAccessManager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        parseJson(reply->readAll());
        qApp->quit();
    });
    QObject::connect(reply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error),
                     [reply](QNetworkReply::NetworkError) {
        // qOverload<> requires C++14
        qDebug() << reply->errorString();
        qApp->quit();
    });
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    qDebug() << "Application started";
    request();
    return app.exec();
}
