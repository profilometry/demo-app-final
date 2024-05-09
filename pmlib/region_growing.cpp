
#include "pmlib.h"

namespace pmlib
{
  void region_growing(cv::Mat &source, cv::Mat &target, cv::Point seed_point){
    // note that this is only guaranteed to expand along the external boundary,
    // so technically the final region could have holes that should be a part of it
    // shouldn't be a problem for anything that uses this, but better to keep it in mind
    if (!point_in_image(seed_point, source)){
      std::cerr << "Called region_growing with seed point not in image\n";
      seed_point = cv::Point(0, 0);
    }
    std::vector<cv::Point> boundary, nhood;
    cv::Mat intermediateImage = cv::Mat::zeros(source.size(), CV_8UC1);
    intermediateImage.at<uchar>(seed_point.y, seed_point.x) = 255;
    bool region_updated = true;
    cv::Point start = seed_point;
    boundary.push_back(start);
    cv::Point next, step;
    int pdiff;
    while (region_updated){
      region_updated = false;
      // perhaps this could be updated iteratively instead of rebuilding on every iteration
      boundary = moore_boundary(intermediateImage);
      for (cv::Point p : boundary){
        // between neighborhood overlap and pixels already inside the region
        // this has something like 4x access redundancy
        // not sure how to remedy that without throwing entirely too much memory at the problem
        nhood = moore_neighborhood(p);
        for (cv::Point n : nhood){
          if (point_in_image(n, intermediateImage) && intermediateImage.at<uchar>(n.y, n.x) == 0){
            pdiff = source.at<uchar>(n.y, n.x) - source.at<uchar>(p.y, p.x);
            pdiff = pdiff >= 0 ? pdiff : -pdiff;
            if (pdiff <= regionGrowingThreshold){
              intermediateImage.at<uchar>(n.y, n.x) = 255;
              region_updated = true;
            }
          }
        }
      }
    }
    target = intermediateImage;
  }

  void region_growing(cv::Mat &source, cv::Point seed_point){
    region_growing(source, source, seed_point);
  }
}
