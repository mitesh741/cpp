#include "mainwindow.h"
#include <QDebug>
#include <iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    cout << "Mitesh" <<  endl;
    function();
}

MainWindow::~MainWindow()
{

}

void MainWindow::function()
{
    cout  << "function" ;

}
