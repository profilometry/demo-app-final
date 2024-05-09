
#include "pmlib.h"

namespace pmlib
{
  void isolate_channel(cv::Mat &source, cv::Mat &target, channelEnum ce){
    if (ce == ce_none || ce == ce_all){
      target = source;
      return;
    }
    std::vector<cv::Mat> channels;
    cv::split(source, channels);
    switch(ce){
      case ce_red:
        channels[0] = cv::Mat::zeros(source.rows, source.cols, CV_8UC1);
        channels[1] = cv::Mat::zeros(source.rows, source.cols, CV_8UC1);
        break;
      case ce_green:
        channels[0] = cv::Mat::zeros(source.rows, source.cols, CV_8UC1);
        channels[2] = cv::Mat::zeros(source.rows, source.cols, CV_8UC1);
        break;
      case ce_blue:
        channels[1] = cv::Mat::zeros(source.rows, source.cols, CV_8UC1);
        channels[2] = cv::Mat::zeros(source.rows, source.cols, CV_8UC1);
        break;
      default:
        // dead branch
        break;
    }
    cv::merge(channels, target);
  }

  void isolate_channel(cv::Mat &source, channelEnum ce){
    isolate_channel(source, source, ce);
  }
}
