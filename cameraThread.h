#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QThread>
#include <QDebug>
#include <opencv2/opencv.hpp>

class CameraThread : public QThread {
  Q_OBJECT
public:
  explicit CameraThread(QObject *parent = nullptr);
  void setCameraId(int value);
  int getCameraId();
  int getFrameWidth();
  int getFrameHeight();
  void quit();

signals:
  void cameraFrameCaptured(const cv::Mat &image);

protected:
  void run() override;
  void run(int cameraId);

private:
  bool m_stop;
  int m_cameraId = 0;
  int m_cameraWidth = 0;
  int m_cameraHeight = 0;
};

#endif // CAMERATHREAD_H
