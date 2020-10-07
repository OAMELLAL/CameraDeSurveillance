/*******************************************************
 Copyright (C) 2020 AMELLAL Oussama <ouss.amellal@gmail.com>

 This file is part of 'CameraSurveillance' project.

 'CameraSurveillance' project can not be copied and/or distributed without the express permission of AMELLAL Oussama
*******************************************************/
#include "objectcamvideo.h"
#include "outils.h"

#include<QThread>
#include<QTimer>
#include<QDebug>

ObjectCamVideo::ObjectCamVideo(int camera, QMutex *lock):
    mEnregisterement(false),FramelockObj(lock), mIndexcamera(camera), mCheminVideo("")
{
    frame_width = frame_height = 0;
    mEtatDetecctionMouvement = false;
}
ObjectCamVideo::ObjectCamVideo(QString videoPath, QMutex *lock):
    mEnregisterement(false), FramelockObj(lock), mIndexcamera(-1), mCheminVideo(videoPath)
{
    frame_width = frame_height = 0;
    mEtatDetecctionMouvement = false;

}
void ObjectCamVideo::CommencerSauvergardeVideo(cv::Mat &firstFrame)
{
        saved_video_name = Outils::nomVideoSauvegardee();
        QString cover = Outils::getCheminVideoSauvegardee(saved_video_name, "jpg");
          std::cout << "test " << std::endl;
         std::cout <<  cover.toStdString() ;
          std::cout << "test " << std::endl;
          std::vector<int> compression_params;
          compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
          compression_params.push_back(100);

        cv::imwrite(cover.toStdString(), firstFrame, compression_params);

        video_writer = new cv::VideoWriter(
        Outils::getCheminVideoSauvegardee(saved_video_name, "avi").toStdString(),
        cv::VideoWriter::fourcc('M','J','P','G'), 24,cv::Size(frame_width,frame_height));
        mEtatEnregistrementVideo = EnregistrementEnCours;
}

void ObjectCamVideo::StoperSauvegardeVideo()
{
    mEtatEnregistrementVideo = EnregistrementArrete;
      video_writer->release();
      delete video_writer;
      video_writer = nullptr;
      emit videoEnregistree(saved_video_name);
}

void ObjectCamVideo::DetectionDeMouvement(cv::Mat &frame)
{
    cv::Mat fgmask;
     cv::GaussianBlur(frame, frame, cv::Size(9, 9), 2, 2);
     segmentor->apply(frame, fgmask);
     if (fgmask.empty()) {
     return;
     }
     cv::threshold(fgmask, fgmask, 25, 255, cv::THRESH_BINARY);
      int noise_size = 2;
      cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(6, 6));
      cv::Mat kernelDilatation = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
     cv::erode(fgmask, fgmask, kernel);
      kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(noise_size, noise_size));
      cv::dilate(fgmask, fgmask, kernelDilatation, cv::Point(-1,-1), 3);
      std::vector<std::vector<cv::Point> > contours;
      cv::findContours(fgmask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

      bool Mouvement = contours.size() > 0;
       qDebug() << contours.size();
      if(!mMouvementDetecte && Mouvement) {
          mMouvementDetecte = true;
         setVideoSavingStatus(DebutEnregistrement);
         qDebug() << "Mouvement détecté";
        }
      else if (mMouvementDetecte && !Mouvement) {
         mMouvementDetecte = false;
         setVideoSavingStatus(ArretEnCoursEnregistrement);
          qDebug() << "Pas de mouvement";
        }
      //Décommenter pour afficher des rectangles englobant la partie en mouvement détectée
      //cv::Scalar color = cv::Scalar(0, 0, 255); // Rouge
      //for(size_t i = 0; i < contours.size(); i++) {
      // cv::Rect rect = cv::boundingRect(contours[i]);
      // cv::rectangle(frame, rect, color, 1);
      // }
}

void ObjectCamVideo::ConnexionThread(QThread *QThreadcamvid)
{
    connect(QThreadcamvid, SIGNAL(started()), this, SLOT(Traitement()));
}

void ObjectCamVideo::Traitement()
{
    mEnregisterement = true;
    cv::VideoCapture cap ;
    int delay;
    if(mCheminVideo != "")
    {
        cap.open(mCheminVideo.toStdString());
        frameRate = (int)cap.get(cv::CAP_PROP_FPS);
        delay = (1000/frameRate);
    }
    else
    {
        cap.open(mIndexcamera);
    }
    cv::Mat frameTemp;
    frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
     segmentor = cv::createBackgroundSubtractorMOG2(130, 16, true);

     while(mEnregisterement) {

        cap >> frameTemp;
        if (frameTemp.empty()) {
             if(mCheminVideo != "")
                StoperSauvegardeVideo();
            break;
        }
        if(mEtatDetecctionMouvement) {
         DetectionDeMouvement(frameTemp);
         }
        if(mEtatEnregistrementVideo == DebutEnregistrement) {
         CommencerSauvergardeVideo(frameTemp);
         }
         if(mEtatEnregistrementVideo == EnregistrementEnCours) {
         video_writer->write(frameTemp);
         }
         if(mEtatEnregistrementVideo == ArretEnCoursEnregistrement) {
         StoperSauvegardeVideo();
         }
        cvtColor(frameTemp, frameTemp, cv::COLOR_BGR2RGB);
        FramelockObj->lock();
        mFrame = frameTemp;
        FramelockObj->unlock();

        emit EnvoyerFrame(&mFrame);
        if(mCheminVideo != "")
        { QThread::msleep(delay);}
        else
        {
            cv::waitKey(30);
        }
    }

    cap.release();
    mEnregisterement = false;
    mCheminVideo = "";
}
