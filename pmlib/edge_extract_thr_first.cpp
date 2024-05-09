
#include "pmlib.h"

std::vector<cv::Point> pmlib::edge_extract_thr_first(cv::Mat &source, dirEnum dir, int thr){
  std::vector<cv::Point> edge;
  if (dir == dir_ne || dir == dir_nw || dir == dir_sw || dir == dir_se){
    std::cerr << "Called " << __func__ << "with invalid dir argument\n";
    dir--;
  }
  if (source.type() != CV_8UC1){
    std::cerr << "Function " << __func__ << "expects source with type CV_8UC1\n";
    return edge;
  }
  if (dir == dir_e || dir == dir_w){
    for (int y = 0; y < source.rows; y++){
      for (int x = 0; x < source.cols; x++){
        if (source.at<uchar>(y, x) > thr){
          edge.push_back(cv::Point(x, y));
          break;
        }
      }
    }
  } else {
    for (int x = 0; x < source.cols; x++){
      for (int y = 0; y < source.rows; y++){
        if (source.at<uchar>(y, x) > thr){
          edge.push_back(cv::Point(x, y));
          break;
        }
      }
    }
  }
  return edge;
}
