#include "mainwindow.h"
#include <QApplication>
#include <iostream>

using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    cout << "Websocket client started..." ;
    w.show();

    return a.exec();
}
