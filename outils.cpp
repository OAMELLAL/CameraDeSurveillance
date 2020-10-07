/*******************************************************
 Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>

 This file is part of 'CameraSurveillance' project.

 'CameraSurveillance' project can not be copied and/or distributed without the express permission of AMELLAL Oussama
*******************************************************/
#include "outils.h"
#include<QDateTime>
#include<QStandardPaths>
#include<QDir>


QString Outils::getCheminBureau()
{
    //Chemin des vidéos sauvegardé : Bureau
    QString CheminBureau = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0];
    QDir BureauDir(CheminBureau);
    BureauDir.mkpath("CameraSurveillanceSortie");
    return BureauDir.absoluteFilePath("CameraSurveillanceSortie");
}

QString Outils::getCheminVideoSauvegardee(QString nom, QString type)
{
    return QString("%1/%2.%3").arg(Outils::getCheminBureau(), nom, type);
}

QString Outils::nomVideoSauvegardee()
{
    QDateTime time = QDateTime::currentDateTime();
    return time.toString("HH-mm-ss-zzz_dd-MM-yyyy");
}
