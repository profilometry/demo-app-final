
#include "pmlib.h"

namespace pmlib
{
  std::vector<cv::Vec2d> mapSS2RS(std::vector<cv::Point> &edge){
    std::vector<cv::Vec2d> out;
    for (cv::Point p : edge){
      double phi_x = 2*std::atan((double)p.x/cntx*FOVxHalfTan);
      double phi_y = 2*std::atan((double)p.y/cnty*FOVyHalfTan);
      double cos_af = std::cos(viewAngle - phi_y);
      if (cos_af == 0) continue; //point at infinity
      double real_x = intersectDist*std::tan(phi_x/2)*cosVA*std::cos(phi_y)/cos_af;
      double real_y = intersectDist*std::sin(phi_y)/cos_af;
      /*std::cout << "point p: " << p.x << " " << p.y << "\n";
      std::cout << "\tphi_x: " << phi_x << "\n";
      std::cout << "\tphi_y: " << phi_y << "\n";
      std::cout << "\tcos_af: " << cos_af << "\n";
      std::cout << "\treal_x: " << real_x << "\n";
      std::cout << "\treal_y: " << real_y << "\n";*/
      out.push_back(cv::Vec2d(real_x, real_y));
    }
    return out;
  }
}
