
#include "pmlib.h"

namespace pmlib
{

  // TODO add sign and size checks to setters

  int gaussKernelSize = 3;
  channelEnum isolateTargetChannel = ce_none;
  channelEnum amplifyTargetChannel = ce_none;
  int regionGrowingThreshold = 1;
  cv::Point seedPoint = cv::Point(0, 0);

  // Canny stuff
  int cannyLowThresholdMax = 100;
  int cannyLowThreshold = cannyLowThresholdMax/2;
  double cannyThresholdRatio = 3.0;
  int cannyKernelSize = 3;

  double viewAngle = pi/2;
  double cosVA = 0;
  double cameraElevation = 1;
  double intersectDist = cameraElevation/std::sin(viewAngle);
  double FOVx = pi/2;
  double FOVy = pi/2;
  double FOVxHalfTan = std::tan(FOVx/2);
  double FOVyHalfTan = std::tan(FOVy/2);
  int cntx = 1;
  int cnty = 1;

  int getGaussKernelSize() { return gaussKernelSize; }
  void setGaussKernelSize(int value) { gaussKernelSize = value; }
  int get_rot_cnt() { return rot_cnt; }
  channelEnum getIsolateTargetChannel() { return isolateTargetChannel; }
  void setIsolateTargetChannel(channelEnum ce) { isolateTargetChannel = ce; }
  channelEnum getAmplifyTargetChannel() { return amplifyTargetChannel; }
  void setAmplifyTargetChannel(channelEnum ce) { amplifyTargetChannel = ce; }

  int getRegionGrowingThreshold() { return regionGrowingThreshold; }
  void setRegionGrowingThreshold(int value) { regionGrowingThreshold = value; }
  cv::Point getSeedPoint() { return seedPoint; }
  void setSeedPoint(cv::Point p) { seedPoint = p; }

  int getCannyLowThreshold() { return cannyLowThreshold; }
  void setCannyLowThreshold(int value) { cannyLowThreshold = (value <= cannyLowThresholdMax) ? (value > 0 ? value : 1) : cannyLowThresholdMax; }
  int getCannyLowThresholdMax() { return cannyLowThresholdMax; }
  void setCannyLowThresholdMax(int value) {
    cannyLowThresholdMax = value > 0 ? value : 1;
    if (cannyLowThreshold > cannyLowThresholdMax) cannyLowThreshold = cannyLowThresholdMax;
  }
  double getCannyThresholdRatio() { return cannyThresholdRatio; }
  void setCannyThresholdRatio(double value) { cannyThresholdRatio = value; }
  int getCannyKernelSize() { return cannyKernelSize; }
  void setCannyKernelSize(int value) {
    if (value == 3 || value == 5 || value == 7) cannyKernelSize = value;
    else return;
  }

  double getViewAngle() { return viewAngle; }
  void setViewAngle(double value) {
    viewAngle = (value <= viewAngleMax) ? (value >= viewAngleMin ? value : viewAngleMin) : viewAngleMax;
    cosVA = std::cos(viewAngle);
  }
  double getCameraElevation() { return cameraElevation; }
  void setCameraElevation(double value) { cameraElevation = value; }
  double getFOVx() { return FOVx; }
  void setFOVx(double value) {
    FOVx = (value <= FOVMax) ? (value >= FOVMin ? value : FOVMin) : FOVMax;
    FOVxHalfTan = std::tan(FOVx/2);
  }
  double getFOVy() { return FOVy; }
  void setFOVy(double value) {
    FOVy = (value <= FOVMax) ? (value >= FOVMin ? value : FOVMin) : FOVMax;
    FOVyHalfTan = std::tan(FOVy/2);
  }
  int getCntx() { return cntx; }
  void setCntx(int value) { cntx = value > 0 ? value : 1; }
  int getCnty() { return cnty; }
  void setCnty(int value) { cnty = value > 0 ? value : 1; }
}
