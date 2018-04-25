#include <QtCore>
#include <QtNetwork>

static QNetworkAccessManager networkAccessManager;

void request()
{
    QUrl url("https://api.twitter.com/1.1/search/tweets.json");
    QNetworkRequest request;
    QUrlQuery query;
    query.addQueryItem("q", "#UbuconEU18");
    url.setQuery(query);
    request.setUrl(url);

    const auto authorizationString = QString("Bearer %1").arg(qgetenv("BEARER").data());
    request.setRawHeader(QByteArrayLiteral("Authorization"), authorizationString.toUtf8());

    auto reply = networkAccessManager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        qDebug() << reply->readAll();
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
