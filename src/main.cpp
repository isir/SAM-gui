#include "ui/mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    qRegisterMetaType<Mosquittopp::Message>("Mosquittopp_Message");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
