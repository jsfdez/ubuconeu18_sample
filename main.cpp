#include <QtCore>
#include <QtQuick>

#include "listmodel.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    qDebug() << "Application started";
    qmlRegisterType<ListModel>("qt.labs.twittertimeline", 1, 0, "TwitterTimeLineModel");
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile(":/main.qml"));
    view.show();
    return app.exec();
}
