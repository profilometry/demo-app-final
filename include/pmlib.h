
#ifndef PMLIB_H__
#define PMLIB_H__

#include <opencv2/opencv.hpp>
#include <cmath>
namespace pmlib
{

  const double pi = 3.14159265359;

  // enums
  enum processingEnum {
      pe_none,
      pe_gray,
      pe_canny,
      pe_sobel,
      pe_region
  };

  enum extractionEnum {
    ee_strongest,
    ee_strongest_and_thr,
    ee_first_thr,
    //ee_offset_thr
  };

  enum rotationEnum {
    re_none,
    re_ccw,
    re_180,
    re_cw
  };

  enum dirEnum {
    dir_e,
    dir_ne,
    dir_n,
    dir_nw,
    dir_w,
    dir_sw,
    dir_s,
    dir_se
  };

  enum channelEnum {
    ce_none,
    ce_red,
    ce_green,
    ce_blue,
    ce_all
  };

  rotationEnum operator+(rotationEnum lhs, rotationEnum rhs);
  rotationEnum operator-(rotationEnum lhs, rotationEnum rhs);
  dirEnum operator--(dirEnum &dir, int);

  // variables
  extern int gaussKernelSize;
  const int rot_cnt = 4;
  extern channelEnum isolateTargetChannel;
  extern channelEnum amplifyTargetChannel;
  extern int regionGrowingThreshold;
  extern cv::Point seedPoint;

  extern int cannyLowThresholdMax;
  extern int cannyLowThreshold;
  extern double cannyThresholdRatio;
  extern int cannyKernelSize;

  extern double viewAngle;
  extern double cosVA;
  const double viewAngleMin = 0;
  const double viewAngleMax = pi;
  extern double cameraElevation;
  extern double intersectDist;
  extern double FOVx;
  extern double FOVy;
  extern double FOVxHalfTan;
  extern double FOVyHalfTan;
  const double eps = 0.001;
  const double FOVMin = eps;
  const double FOVMax = pi - eps;
  extern int cntx;
  extern int cnty;

  // get/set
  int getGaussKernelSize();
  void setGaussKernelSize(int value);
  int get_rot_cnt();
  channelEnum getIsolateTargetChannel();
  void setIsolateTargetChannel(channelEnum ce);
  channelEnum getAmplifyTargetChannel();
  void setAmplifyTargetChannel(channelEnum ce);
  int getRegionGrowingThreshold();
  void setRegionGrowingThreshold(int value);
  cv::Point getSeedPoint();
  void setSeedPoint(cv::Point p);

  int getCannyLowThreshold();
  void setCannyLowThreshold(int value);
  int getCannyLowThresholdMax();
  void setCannyLowThresholdMax(int value);
  double getCannyThresholdRatio();
  void setCannyThresholdRatio(double value);
  int getCannyKernelSize();
  void setCannyKernelSize(int value);

  double getViewAngle();
  void setViewAngle(double value);
  double getCameraElevation();
  void setCameraElevation(double value);
  double getFOVx();
  void setFOVx(double value);
  double getFOVy();
  void setFOVy(double value);
  int getCntx();
  void setCntx(int value);
  int getCnty();
  void setCnty(int value);

  // functions
  // pre-processing
  void isolate_channel(cv::Mat &source, cv::Mat &target, channelEnum ce);
  void isolate_channel(cv::Mat &source, channelEnum ce);
  void quad_diff_amp(cv::Mat &source, cv::Mat &target, channelEnum ce);
  void quad_diff_amp(cv::Mat &source, channelEnum ce);
  // 1st stage processing
  int process_image(cv::Mat &source, cv::Mat &target, cv::Rect roi, processingEnum pe = pe_none);
  void edge_link(cv::Mat &source, dirEnum edge_normal);
  void region_growing(cv::Mat &source, cv::Mat &target, cv::Point seed_point);
  void region_growing(cv::Mat &source, cv::Point seed_point);
  std::vector<cv::Point> moore_neighborhood(cv::Point p);
  std::vector<cv::Point> moore_neighborhood(cv::Point p, cv::Size s);
  std::vector<cv::Point> moore_boundary(cv::Mat &image);
  // 2nd stage processing
    // TODO direction polarity
  std::vector<cv::Point> edge_extract_strongest(cv::Mat &source, dirEnum dir);
  std::vector<cv::Point> edge_extract_thr_strongest(cv::Mat &source, dirEnum dir, int thr);
  std::vector<cv::Point> edge_extract_thr_first(cv::Mat &source, dirEnum dir, int thr);
  //std::vector<cv::Point> edge_extract_thr_first(cv::Mat &source, dirEnum dir);
  std::vector<cv::Vec2d> mapSS2RS(std::vector<cv::Point> &edge);
  // utility/other
  void enum_rotate(cv::Mat &src, rotationEnum re);
  bool point_in_image(cv::Point p, cv::Mat &image);
  bool point_in_image(cv::Point p, cv::Size s);
  void add_roi_offset(std::vector<cv::Point> &edge, cv::Rect roi);
}


#endif
