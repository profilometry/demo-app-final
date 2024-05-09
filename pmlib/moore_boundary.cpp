
#include "pmlib.h"

namespace pmlib
{
  std::vector<cv::Point> moore_boundary(cv::Mat &image){
    std::vector<cv::Point> boundary;
    if (image.type() != CV_8UC1) {
      std::cerr << "Invalid image type for moore_boundary\n";
      return boundary;
    }
    cv::Point p, c, b, s = cv::Point(-1, -1);
    // find start point
    for (int y = 0; y < image.rows; y++){
      for (int x = 0; x < image.cols; x++){
        if (image.at<uchar>(y, x) > 0){
          s = cv::Point(x, y);
          break;
        }
      }
    }
    // if s is not set, the image is empty
    if (s == cv::Point(-1, -1)) return boundary;
    boundary.push_back(s);
    int iterations = 0;
    p = s;
    b = cv::Point(s.x - 1, s.y);
    std::vector<cv::Point> M = moore_neighborhood(p);
    int bindex = find(M.begin(), M.end(), b) - M.begin();
    c = M[(bindex + 1) % 8];
    while (c != s){
      iterations++;
      if (
        // edge condition for when the region touches edges of image
        !point_in_image(c, image) ||
        image.at<uchar>(c.y, c.x) == 0
      ){
        b = c;
      } else {
        boundary.push_back(c);
        b = p;
        p = c;
        M = moore_neighborhood(p);
      }
      // if we check the whole neighborhood and no new points were added to boundary, the boundary is a single pixel
      if ((iterations > 8) && (boundary.size() <= 1)) return boundary;
      bindex = find(M.begin(), M.end(), b) - M.begin();
      c = M[(bindex + 1) % 8];
    }
    return boundary;
  }
}
