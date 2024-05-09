
#include "pmlib.h"

namespace pmlib
{
  void add_roi_offset(std::vector<cv::Point> &edge, cv::Rect roi){
    for (cv::Point &p : edge){
      p.x += roi.x;
      p.y += roi.y;
    }
  }
}
