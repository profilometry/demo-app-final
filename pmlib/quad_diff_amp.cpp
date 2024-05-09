
#include "pmlib.h"

namespace pmlib
{
  void quad_diff_amp(cv::Mat &source, cv::Mat &target, channelEnum ce){
    cv::Mat intermediateImage;
    if (ce == ce_none || ce == ce_all){
      target = source;
      return;
    } else {
      intermediateImage = cv::Mat::zeros(source.rows, source.cols, source.type());
    }
    unsigned ac, bc1, bc2;
    switch(ce){
      case ce_red:
        ac = 2; bc1 = 0; bc2 = 1;
        break;
      case ce_green:
        ac = 1; bc1 = 0; bc2 = 2;
        break;
      case ce_blue:
        ac = 0; bc1 = 1; bc2 = 2;
        break;
      default:
        // dead branch
        return;
    }
    cv::Vec3b pixel;
    unsigned quad_diff;
    for (int y = 0; y < source.rows; y++){
      for (int x = 0; x < source.cols; x++){
        pixel = source.at<cv::Vec3b>(y, x);
        quad_diff = (unsigned)pixel[ac] - ((pixel[bc1] + pixel[bc2]) / 2);
        if (quad_diff <= 0){
          // pixel has less target channel than the avg of the other two
          continue;
        } else if (quad_diff >= 16){
          // would overflow
          pixel[ac] = 255;
        } else {
          quad_diff *= quad_diff;
          pixel[ac] = pixel[ac] + quad_diff > 255 ? 255 : pixel[ac] + quad_diff;
        }
        intermediateImage.at<cv::Vec3b>(y, x) = pixel;
      }
    }
    target = intermediateImage;
  }

  void quad_diff_amp(cv::Mat &source, channelEnum ce){
    quad_diff_amp(source, source, ce);
  }
}
