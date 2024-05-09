
#include "pmlib.h"

namespace pmlib
{

  int process_image(cv::Mat &source, cv::Mat &target, cv::Rect roi, processingEnum pe){
    static cv::Mat intermediateImage;
    if (roi.area() > 0){
      source(roi).copyTo(intermediateImage);
    } else {
      source.copyTo(intermediateImage);
    }
    quad_diff_amp(intermediateImage, target, amplifyTargetChannel);
    intermediateImage = target;
    isolate_channel(intermediateImage, target, isolateTargetChannel);
    intermediateImage = target;
    switch (pe){
      case pe_gray:
        cv::cvtColor(intermediateImage, target, cv::COLOR_BGR2GRAY);
        break;
      case pe_canny:
        cv::cvtColor(intermediateImage, target, cv::COLOR_BGR2GRAY);
        intermediateImage = target;
        cv::blur(intermediateImage, target, cv::Size(gaussKernelSize, gaussKernelSize));
        cannyLowThreshold = cannyLowThreshold <= cannyLowThresholdMax ? cannyLowThreshold : cannyLowThresholdMax;
        cv::Canny(target, target, cannyLowThreshold, cannyLowThreshold*cannyThresholdRatio, cannyKernelSize);
        break;
      case pe_sobel:
        cv::cvtColor(intermediateImage, target, cv::COLOR_BGR2GRAY);
        intermediateImage = target;
        cv::blur(intermediateImage, target, cv::Size(gaussKernelSize, gaussKernelSize));
        intermediateImage = target;
        cv::Sobel(intermediateImage, target, -1, 1, 0);
        cv::convertScaleAbs(target, target);
        break;
      case pe_region:
        cv::cvtColor(intermediateImage, target, cv::COLOR_BGR2GRAY);
        intermediateImage = target;
        region_growing(intermediateImage, target, seedPoint);
        break;
      case pe_none:
      default:
        target = intermediateImage;
    }
    return 0;
  }
}
