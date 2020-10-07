/*******************************************************
 Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>

 This file is part of 'CameraSurveillance' project.

 'CameraSurveillance' project can not be copied and/or distributed without the express permission of AMELLAL Oussama
*******************************************************/
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow Fenetre;
    Fenetre.setWindowTitle("Caméra de surveillance");
    Fenetre.show();
    return a.exec();
}
