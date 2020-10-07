/*******************************************************
 Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>

 This file is part of 'CameraSurveillance' project.

 'CameraSurveillance' project can not be copied and/or distributed without the express permission of AMELLAL Oussama
*******************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QListView>
#include<QLabel>
#include<QStandardItemModel>
#include<QCheckBox>
#include <QPushButton>

#include "opencv2/opencv.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "objectcamvideo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initialisation();
    void ActionsInitialisation();
private:
    Ui::MainWindow *ui;

    ObjectCamVideo *mObjectCamVideo;
    QThread* ObjectCamVideoThread;
    cv::Mat FrameEnCours;
    QMutex * FrameLock;

    QMenu * mQMenuFichier;
    QAction * mQActionOuvrirCamera;
    QAction *mQActionOuvrirVideo;
    QAction *mQActionCaracCamera;
    QAction *mQActionQuitter;

    QGraphicsScene * mQGraphicsSceneImage;
    QGraphicsView * mQGraphicsViewImage;

    QPushButton *mQPushButtonPrendreUnePhoto;//sup
    QListView *mQListViewVidSauvg;
    QStatusBar *mQStatusBarPrinc;
    QLabel *mQLabelStatusBarPrinc;

    QCheckBox *mQCheckBoxActivDesacSurv;
    QPushButton *mQPushButtonEnregistrerVideo;
    QStandardItemModel *list_model;

private slots:

    void AfficherCaracCamera(); //Afficher les caractéristiques de la caméra
    void OuvrirCamera();
    void OuvrirVideo();
    void RecevoirFrame(cv::Mat*);

    void ArretStopEnregistrement();
    void AjoutVideoSauvegardee(QString name);
    void ActivDesacDetectionMouvement(int etat);
};
#endif // MAINWINDOW_H
