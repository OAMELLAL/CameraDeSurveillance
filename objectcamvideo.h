/*******************************************************
 Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>

 This file is part of 'CameraSurveillance' project.

 'CameraSurveillance' project can not be copied and/or distributed without the express permission of AMELLAL Oussama
*******************************************************/
#ifndef OBJECTCAMVIDEO_H
#define OBJECTCAMVIDEO_H

#include <QObject>
#include<QThread>
#include<QMutex>
#include<QTimer>
#include "opencv2/opencv.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


class ObjectCamVideo : public QObject
{
    Q_OBJECT
public :
    enum EtatEnregistrementVideo {
        DebutEnregistrement,
        EnregistrementEnCours,
        ArretEnCoursEnregistrement,
        EnregistrementArrete
    };
private:
    int frame_width, frame_height;
    bool mEnregisterement;
    QMutex *FramelockObj;
    cv::Mat mFrame;
    int mIndexcamera;
    QString mCheminVideo;
    int frameRate;
    EtatEnregistrementVideo mEtatEnregistrementVideo;
    bool mEtatDetecctionMouvement;
    bool mMouvementDetecte;
    cv::Ptr<cv::BackgroundSubtractorMOG2> segmentor ;

    QString saved_video_name;
    cv::VideoWriter *video_writer;

    void CommencerSauvergardeVideo(cv::Mat &firstFrame);
    void StoperSauvegardeVideo();
    void DetectionDeMouvement(cv::Mat &frame);

public:
    ObjectCamVideo(int camera, QMutex *lock); // Pour ouvrir une caméra
    ObjectCamVideo(QString videoPath, QMutex *lock); // Pour ouvrir une vidéo
    void ConnexionThread(QThread * QThreadcamvid); // Connexion entre un signal(QThread) et le slot 'Traitement'
    void setVideoSavingStatus(EtatEnregistrementVideo etat) {mEtatEnregistrementVideo = etat; };
    void setMotionDetectingStatus(bool etat) {
        mEtatDetecctionMouvement = etat;
        mMouvementDetecte = false;
        if(mEtatEnregistrementVideo != EnregistrementArrete) mEtatEnregistrementVideo = ArretEnCoursEnregistrement;
    };
signals:
    void EnvoyerFrame(cv::Mat *data);
    void videoEnregistree(QString name);
public slots:
    void Traitement();
};

#endif // OBJECTCAMVIDEO_H
