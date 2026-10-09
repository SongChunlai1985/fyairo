#ifndef FACEDECTION_H
#define FACEDECTION_H
#include <opencv4/opencv2/dnn.hpp>
#include "../../mind/3d/3d.h"

class ResNetDetector
{
public:
         ResNetDetector(void);
        ~ResNetDetector(void);

private:
          float confidenceThreshold = 0.5f;
          scalar meanVal;
          cv::Mat inputBlob,detection;
          cv::dnn::Net dnnnet;

public:
          double inScaleFactor  = 255.0,
                 inScaleFactor1 = 0.007843;
      void netinit(int model=0);
      void Detection(cv::Mat frame,
                             std::vector<nface> &Rects, cv::Size size);

};
#endif
