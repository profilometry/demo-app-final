
#include "pmlib.h"

void pmlib::edge_link(cv::Mat &source, dirEnum edge_normal){
  const std::vector<cv::Point> search_pattern_orth = {
    {2, 0}, {3, 0}, {4, 0}, {2, 1}, {2, -1}, {5, 0}, {6, 0}, {3, 1},
    {3, -1}, {7, 0}, {4, 1}, {4, -1}, {5, 1}, {5, -1}, {2, 2}, {2, -2},
    {6, 1}, {6, -1}, {7, 1}, {7, -1}, {3, 2}, {3, -2}, {4, 2}, {4, -2},
    {1, 2}, {1, -2}, {5, 2}, {5, -2}, {6, 2}, {6, -2}, {3, 3}, {3, -3},
    {7, 2}, {7, -2}, {4, 3}, {4, -3}, {0, 2}, {0, -2}, {5, 3}, {5, -3},
    {6, 3}, {6, -3}, {2, 3}, {2, -3}, {7, 3}, {7, -3}, {1, 3}, {1, -3},
    {0, 3}, {0, -3}
  };
  const std::vector<cv::Point> search_pattern_diag = {
    {2, 2}, {3, 3}, {4, 4}, {2, 1}, {1, 2}, {5, 5}, {6, 6}, {3, 2},
    {2, 3}, {4, 3}, {3, 4}, {3, 1}, {1, 3}, {5, 4}, {4, 5}, {6, 5},
    {5, 6}, {4, 2}, {2, 4}, {5, 3}, {3, 5}, {4, 1}, {1, 4}, {6, 4},
    {4, 6}, {5, 2}, {2, 5}, {2, 0}, {0, 2}, {6, 3}, {3, 6}, {3, 0},
    {0, 3}, {5, 1}, {1, 5}, {6, 2}, {2, 6}, {6, 1}, {1, 6}, {4, 0},
    {0, 4}, {5, 0}, {0, 5}, {6, 0}, {0, 6}
  };
  // find start pixel
  // find initial direction
  // while next point exists
  //   if neighbor is edge -> next point
  //   else try search pattern
  //     if edge pixel found draw line to it and -> next point
  //     else done
  // repeat for opposite to initial direction
  ;
}
