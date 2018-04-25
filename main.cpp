#include <QtCore>
#include <QtWidgets>

#include "model.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qDebug() << "Application started";
    QTableView view;
    view.horizontalHeader()->setStretchLastSection(true);
    view.setModel(new Model(&view));
    view.show();
    return app.exec();
}
