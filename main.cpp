#include <QtCore>
#include <QtNetwork>

static QNetworkAccessManager networkAccessManager;

void request()
{
    const QUrl url("https://api.twitter.com/1.1/search/tweets.json");
    QNetworkRequest request;
    request.setUrl(url);
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
