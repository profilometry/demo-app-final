
#include "pmlib.h"

std::vector<cv::Point> pmlib::edge_extract_strongest(cv::Mat &source, dirEnum dir){
  std::vector<cv::Point> edge;
  if (dir == dir_ne || dir == dir_nw || dir == dir_sw || dir == dir_se){
    std::cerr << "Called " << __func__ << "with invalid dir argument\n";
    dir--;
  }
  if (source.type() != CV_8UC1){
    std::cerr << "Function " << __func__ << "expects source with type CV_8UC1\n";
    return edge;
  }
  int current_max_val;
  int current_max_index;
  if (dir == dir_e || dir == dir_w){
    for (int y = 0; y < source.rows; y++){
      current_max_val = 0;
      current_max_index = -1;
      for (int x = 0; x < source.cols; x++){
        if (source.at<uchar>(y, x) > current_max_val){
          current_max_val = source.at<uchar>(y, x);
          current_max_index = x;
          if (current_max_val == 255) break;
        }
      }
      edge.push_back(current_max_index >= 0 ? cv::Point(current_max_index, y) : cv::Point(0, y));
    }
  } else {
    for (int x = 0; x < source.cols; x++){
      current_max_val = 0;
      current_max_index = -1;
      for (int y = 0; y < source.rows; y++){
        if (source.at<uchar>(y, x) > current_max_val){
          current_max_val = source.at<uchar>(y, x);
          current_max_index = y;
          if (current_max_val == 255) break;
        }
      }
      edge.push_back(current_max_index >= 0 ? cv::Point(x, current_max_index) : cv::Point(x, 0));
    }
  }
  return edge;
}
