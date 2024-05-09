
#include "pmlib.h"

namespace pmlib
{
  bool point_in_image(cv::Point p, cv::Mat &image){
    return (p.x >= 0 && p.x < image.cols && p.y >= 0 && p.y < image.rows);
  }

  bool point_in_image(cv::Point p, cv::Size s){
    return (p.x >= 0 && p.x < s.width && p.y >= 0 && p.y < s.height);
  }
}
