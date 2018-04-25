#include <QtCore>
#include <QtNetwork>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    qDebug() << "Application started";
    return app.exec();
}
