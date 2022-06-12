#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.scatter.setFlags(w.scatter.flags() ^ Qt::FramelessWindowHint);
    w.scatter.setShadowQuality(QAbstract3DGraph::ShadowQualityNone);

    return a.exec();
}
