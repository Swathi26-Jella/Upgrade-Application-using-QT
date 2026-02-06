#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.resize(440,370);
    w.setWindowIcon(QIcon(":/new/img/icon/stm32icon.png"));
    w.setWindowTitle("STM32 Hex Upgrade 1.0");
    return a.exec();
}
