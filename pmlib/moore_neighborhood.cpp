
#include "pmlib.h"

namespace pmlib
{
  std::vector<cv::Point> moore_neighborhood(cv::Point p){
    std::vector<cv::Point> out;
    // push clockwise
    out.push_back(cv::Point(p.x - 1, p.y - 1));
    out.push_back(cv::Point(p.x - 1, p.y));
    out.push_back(cv::Point(p.x - 1, p.y + 1));
    out.push_back(cv::Point(p.x, p.y + 1));
    out.push_back(cv::Point(p.x + 1, p.y + 1));
    out.push_back(cv::Point(p.x + 1, p.y));
    out.push_back(cv::Point(p.x + 1, p.y - 1));
    out.push_back(cv::Point(p.x, p.y - 1));
    return out;
  }

  std::vector<cv::Point> moore_neighborhood(cv::Point p, cv::Size s){
    std::vector<cv::Point> out;
    // push clockwise
    if (!point_in_image(p, s)) return out;
    if (p.x == 0 && p.y == 0){
      out.push_back(cv::Point(p.x, p.y + 1));
      out.push_back(cv::Point(p.x + 1, p.y + 1));
      out.push_back(cv::Point(p.x + 1, p.y));
      return out;
    } else if (p.x == s.width - 1 && p.y == 0){
      out.push_back(cv::Point(p.x, p.y + 1));
      out.push_back(cv::Point(p.x - 1, p.y + 1));
      out.push_back(cv::Point(p.x - 1, p.y));
      return out;
    } else if (p.x == s.width - 1 && p.y == s.height - 1){
      out.push_back(cv::Point(p.x - 1, p.y));
      out.push_back(cv::Point(p.x - 1, p.y - 1));
      out.push_back(cv::Point(p.x, p.y - 1));
      return out;
    } else if (p.x == 0 && p.y == s.height - 1){
      out.push_back(cv::Point(p.x + 1, p.y));
      out.push_back(cv::Point(p.x + 1, p.y - 1));
      out.push_back(cv::Point(p.x, p.y - 1));
      return out;
    }
    if (p.x == 0){
      out.push_back(cv::Point(p.x, p.y + 1));
      out.push_back(cv::Point(p.x + 1, p.y + 1));
      out.push_back(cv::Point(p.x + 1, p.y));
      out.push_back(cv::Point(p.x + 1, p.y - 1));
      out.push_back(cv::Point(p.x, p.y - 1));
      return out;
    } else if (p.x == s.width - 1){
      out.push_back(cv::Point(p.x - 1, p.y - 1));
      out.push_back(cv::Point(p.x - 1, p.y));
      out.push_back(cv::Point(p.x - 1, p.y + 1));
      out.push_back(cv::Point(p.x, p.y + 1));
      out.push_back(cv::Point(p.x, p.y - 1));
      return out;
    } else if (p.y == 0){
      out.push_back(cv::Point(p.x - 1, p.y));
      out.push_back(cv::Point(p.x - 1, p.y + 1));
      out.push_back(cv::Point(p.x, p.y + 1));
      out.push_back(cv::Point(p.x + 1, p.y + 1));
      out.push_back(cv::Point(p.x + 1, p.y));
      return out;
    } else if (p.y == s.height - 1){
      out.push_back(cv::Point(p.x - 1, p.y - 1));
      out.push_back(cv::Point(p.x - 1, p.y));
      out.push_back(cv::Point(p.x + 1, p.y));
      out.push_back(cv::Point(p.x + 1, p.y - 1));
      out.push_back(cv::Point(p.x, p.y - 1));
      return out;
    }
    out.push_back(cv::Point(p.x - 1, p.y - 1));
    out.push_back(cv::Point(p.x - 1, p.y));
    out.push_back(cv::Point(p.x - 1, p.y + 1));
    out.push_back(cv::Point(p.x, p.y + 1));
    out.push_back(cv::Point(p.x + 1, p.y + 1));
    out.push_back(cv::Point(p.x + 1, p.y));
    out.push_back(cv::Point(p.x + 1, p.y - 1));
    out.push_back(cv::Point(p.x, p.y - 1));
    return out;
  }
}
