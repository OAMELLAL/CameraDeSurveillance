/*******************************************************
 Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>

 This file is part of 'CameraSurveillance' project.

 'CameraSurveillance' project can not be copied and/or distributed without the express permission of AMELLAL Oussama
*******************************************************/
#ifndef OUTILS_H
#define OUTILS_H

#include<QString>

class Outils
{
public:

    static QString nomVideoSauvegardee();
    static QString getCheminBureau();
    static QString getCheminVideoSauvegardee(QString nom, QString type);
};

#endif // OUTILS_H
