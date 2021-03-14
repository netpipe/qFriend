#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QSplashScreen>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QFile file("media/themes/qdarkstyle/qdarkstyle.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    w.setStyleSheet(styleSheet);


    w.show();
    return a.exec();
}

