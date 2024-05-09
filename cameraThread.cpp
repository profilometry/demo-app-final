#include "CameraThread.h"

CameraThread::CameraThread(QObject *parent) : QThread(parent), m_stop(false) {}

void CameraThread::run() {
    cv::VideoCapture cap(m_cameraId);
    if (!cap.isOpened()) {
        qDebug() << "Error: Failed to open camera";
        m_cameraWidth = 0;
        m_cameraHeight = 0;
        return;
    }
    m_cameraWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    m_cameraHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    m_stop = false;

    cv::Mat frame;
    while (!m_stop) {
        cap >> frame;
        if (frame.empty()) continue;
        emit cameraFrameCaptured(frame);
        QThread::msleep(10);
    }
}

void CameraThread::setCameraId(int value){
  m_cameraId = value >= 0 ? value : 0;
}

int CameraThread::getCameraId(){
  return m_cameraId;
}

int CameraThread::getFrameWidth(){
  return m_cameraWidth;
}

int CameraThread::getFrameHeight(){
  return m_cameraHeight;
}

void CameraThread::quit() {
    m_stop = true;
}
