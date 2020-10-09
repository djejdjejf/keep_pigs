#include "mainwindow.h"
#include "pig_and_sty.h"
#include "keep_pig.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("养猪模拟");
    w.kp.start(0);
    w.show();
    return a.exec();
}
