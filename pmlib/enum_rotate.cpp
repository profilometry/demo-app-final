
#include "pmlib.h"

void pmlib::enum_rotate(cv::Mat &mat, rotationEnum re){
  cv::Mat tmp;
  switch(re){
    case re_ccw:
      cv::transpose(mat, tmp);
      cv::flip(tmp, mat, 0);
      return;
    case re_180:
      cv::transpose(mat, tmp);
      cv::flip(tmp, mat, 1);
      // fall through
    case re_cw:
      cv::transpose(mat, tmp);
      cv::flip(tmp, mat, 1);
      return;
    case re_none:
    default:
      return;
  }
}
