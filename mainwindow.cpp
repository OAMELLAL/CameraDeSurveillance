/*******************************************************
 Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>

 This file is part of 'CameraSurveillance' project.

 'CameraSurveillance' project can not be copied and/or distributed without the express permission of AMELLAL Oussama
*******************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objectcamvideo.h"
#include "outils.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "opencv2/opencv.hpp"

#include<QGridLayout>
#include<QCameraInfo>
#include<QMessageBox>
#include<QThread>
#include <QDateTime>
#include<QStandardPaths>
#include<QDir>
#include<QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,mObjectCamVideo(nullptr)
    , mQMenuFichier(nullptr)
{
    ui->setupUi(this);
    //this->setStyleSheet("background-color: blue;");// Changer la couleur de la fenêtre
    initialisation();
    FrameLock = new QMutex();
    mQActionOuvrirCamera->setEnabled(true);
    mQActionOuvrirVideo->setEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initialisation()
{
    this->resize(1000, 800);
    mQMenuFichier = menuBar()->addMenu("&Fichier");
    QGridLayout * QGridLayoutPrincipale = new QGridLayout;
    mQGraphicsSceneImage = new QGraphicsScene(this);
    mQGraphicsViewImage = new QGraphicsView(mQGraphicsSceneImage);
    QGridLayoutPrincipale->addWidget(mQGraphicsViewImage,0,0,12, 15);

     QGridLayout *QGridLayoutOutils = new QGridLayout();
     QGridLayoutPrincipale->addLayout(QGridLayoutOutils, 12, 0, 1, 15);
     mQCheckBoxActivDesacSurv = new QCheckBox(this);
     mQCheckBoxActivDesacSurv->setText("Surveillance On/Off");
     QGridLayoutOutils->addWidget(mQCheckBoxActivDesacSurv, 0, 0);
     mQPushButtonEnregistrerVideo = new QPushButton(this);
     mQPushButtonEnregistrerVideo->setText("Enregistrer");
     QGridLayoutOutils->addWidget(mQPushButtonEnregistrerVideo, 0, 1, Qt::AlignLeft);

     QWidget *widget = new QWidget();
      widget->setLayout(QGridLayoutPrincipale);
      setCentralWidget(widget);

      mQStatusBarPrinc = statusBar();
      mQLabelStatusBarPrinc = new QLabel(mQStatusBarPrinc);
      mQStatusBarPrinc->addPermanentWidget(mQLabelStatusBarPrinc);
      mQLabelStatusBarPrinc->setText("Pas d'enregitrement en cours.");

        ActionsInitialisation();

         mQListViewVidSauvg = new QListView(this);
         mQListViewVidSauvg->setViewMode(QListView::ListMode);
         mQListViewVidSauvg->setResizeMode(QListView::Adjust);
         mQListViewVidSauvg->setSpacing(5);
         mQListViewVidSauvg->setWrapping(false);
         list_model = new QStandardItemModel(this);
         mQListViewVidSauvg->setModel(list_model);
         QGridLayoutPrincipale->addWidget(mQListViewVidSauvg, 0, 15, 12, 6);

    connect(mQPushButtonEnregistrerVideo, SIGNAL(clicked()), this, SLOT(ArretStopEnregistrement()));
    connect(mQCheckBoxActivDesacSurv, SIGNAL(stateChanged(int)), this, SLOT(ActivDesacDetectionMouvement(int)));
    }
void MainWindow::ActionsInitialisation()
{

    mQActionOuvrirCamera = new QAction("&Ouvrir la caméra", this);
    mQMenuFichier->addAction(mQActionOuvrirCamera);

    mQActionOuvrirVideo = new QAction("&Ouvrir une vidéo", this);
    mQMenuFichier->addAction(mQActionOuvrirVideo);

    mQActionCaracCamera = new QAction("&Caractéristiques de la caméra", this);
    mQMenuFichier->addAction(mQActionCaracCamera);

    mQActionQuitter = new QAction("&Quitter", this);
    mQMenuFichier->addAction(mQActionQuitter);

    connect(mQActionOuvrirCamera, SIGNAL(triggered()), this, SLOT(OuvrirCamera()));
    connect(mQActionOuvrirVideo, SIGNAL(triggered()), this, SLOT(OuvrirVideo()));
    connect(mQActionCaracCamera, SIGNAL(triggered()), this, SLOT(AfficherCaracCamera()));
    connect(mQActionQuitter, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
}

void MainWindow::OuvrirCamera()
{
    if(mObjectCamVideo != nullptr) {
        mQGraphicsSceneImage->clear();
        mQGraphicsViewImage->resetTransform();

        disconnect(mObjectCamVideo, &ObjectCamVideo::EnvoyerFrame, this, &MainWindow::RecevoirFrame);
        connect(ObjectCamVideoThread, &QThread::finished, this, &QObject::deleteLater);
    }
    ObjectCamVideoThread = new QThread();
    mObjectCamVideo = new ObjectCamVideo(0, FrameLock );
    mObjectCamVideo->ConnexionThread(ObjectCamVideoThread);
    mObjectCamVideo->moveToThread(ObjectCamVideoThread);
    connect(mObjectCamVideo, &ObjectCamVideo::EnvoyerFrame, this, &MainWindow::RecevoirFrame);
    ObjectCamVideoThread->start();
    mQLabelStatusBarPrinc->setText(QString("Capture du caméra %1").arg(0));
    connect(mObjectCamVideo, &ObjectCamVideo::videoEnregistree, this, &MainWindow::AjoutVideoSauvegardee);

    mQCheckBoxActivDesacSurv->setCheckState(Qt::Unchecked);
    mQPushButtonEnregistrerVideo->setText("Enregistrer");
    mQPushButtonEnregistrerVideo->setEnabled(true);
    mQActionOuvrirCamera->setEnabled(false);
}

void MainWindow::OuvrirVideo()
{
    if(mObjectCamVideo != nullptr) {
        mQGraphicsSceneImage->clear();
        mQGraphicsViewImage->resetTransform();
        disconnect(mObjectCamVideo, &ObjectCamVideo::EnvoyerFrame, this, &MainWindow::RecevoirFrame);
        connect(ObjectCamVideoThread, &QThread::finished, this, &QObject::deleteLater);
    }
    QFileDialog dialog(this);
    dialog.setWindowTitle("Ouvrir une image");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Videos filtres (*.mp4 *.avi)"));
    QStringList CheminsFichiers;

    if (dialog.exec())
    {
        CheminsFichiers = dialog.selectedFiles();
        ObjectCamVideoThread = new QThread();
        mObjectCamVideo = new ObjectCamVideo(CheminsFichiers.at(0), FrameLock );

        mObjectCamVideo->ConnexionThread(ObjectCamVideoThread);
        mObjectCamVideo->moveToThread(ObjectCamVideoThread);
        connect(mObjectCamVideo, &ObjectCamVideo::EnvoyerFrame, this, &MainWindow::RecevoirFrame);
        ObjectCamVideoThread->start();
        qDebug() << CheminsFichiers.at(0);
        mQLabelStatusBarPrinc->setText(QString("Capture du vidéo %1").arg(0));
        connect(mObjectCamVideo, &ObjectCamVideo::videoEnregistree, this, &MainWindow::AjoutVideoSauvegardee);

        mQCheckBoxActivDesacSurv->setCheckState(Qt::Unchecked);
        mQPushButtonEnregistrerVideo->setText("Enregistrer");
        mQPushButtonEnregistrerVideo->setEnabled(true);
        mQActionOuvrirVideo->setEnabled(false);
    }
}
void MainWindow::RecevoirFrame(cv::Mat * mat)
{
    FrameLock->lock();
    FrameEnCours = *mat;
    FrameLock->unlock();
    QImage frame(
                FrameEnCours.data,
                FrameEnCours.cols,
                FrameEnCours.rows,
                FrameEnCours.step,
                QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(frame);
    mQGraphicsSceneImage->clear();
    mQGraphicsViewImage->resetTransform();
    mQGraphicsSceneImage->addPixmap(image);
    mQGraphicsSceneImage->update();
    mQGraphicsViewImage->setSceneRect(image.rect());
}

void MainWindow::ArretStopEnregistrement()
{
    QString text = mQPushButtonEnregistrerVideo->text();
    if(text == "Enregistrer" && mObjectCamVideo != nullptr)
    {
           mObjectCamVideo->setVideoSavingStatus(ObjectCamVideo::DebutEnregistrement);
           mQPushButtonEnregistrerVideo->setText("Stopper l'enregistrement");
            mQCheckBoxActivDesacSurv->setCheckState(Qt::Unchecked);
            mQCheckBoxActivDesacSurv->setEnabled(false);
    }
    else if(text == "Stopper l'enregistrement" && mObjectCamVideo != nullptr)
    {
           mObjectCamVideo->setVideoSavingStatus(ObjectCamVideo::ArretEnCoursEnregistrement);
           mQPushButtonEnregistrerVideo->setText("Enregistrer");
           mQCheckBoxActivDesacSurv->setEnabled(true);
    }
}

void MainWindow::AjoutVideoSauvegardee(QString name)
{
    QString cover = Outils::getCheminVideoSauvegardee(name, "jpg");
    QStandardItem *item = new QStandardItem();
    list_model->appendRow(item);
    QModelIndex index = list_model->indexFromItem(item);

    list_model->setData(index, QPixmap(cover).scaledToHeight(145), Qt::DecorationRole);
    list_model->setData(index, name, Qt::DisplayRole);
    mQListViewVidSauvg->scrollTo(index);
}

void MainWindow::ActivDesacDetectionMouvement(int etat)
{
    if(mObjectCamVideo == nullptr) {
     return;
     }
     if(etat) {
     mObjectCamVideo->setMotionDetectingStatus(true);
     mQPushButtonEnregistrerVideo->setEnabled(false);
     } else {
     mObjectCamVideo->setMotionDetectingStatus(false);
     mQPushButtonEnregistrerVideo->setEnabled(true);
     }
}

void MainWindow::AfficherCaracCamera()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    QString infoCams = QString("Caméras disponible : \n");
    for (const QCameraInfo &cameraInfo : cameras){
        infoCams +=  "Nom : " + cameraInfo.deviceName() + "\n";
        infoCams += "Description : " + cameraInfo.description() + "\n";
    }
    QMessageBox::information(this, "Caméras disponible", infoCams);

}
