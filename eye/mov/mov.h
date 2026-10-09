#ifndef FYMOV_H
#define FYMOV_H
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <unistd.h>
class mov{
public:
         mov(void);
        ~mov(void);

private:
         cv::String atb_;

public:
      void movrt(cv::Mat &frame);
      void Refresh();

      cv::Mat t8uc3 (cv::Mat &m,cv::Mat &mdst);
      cv::Mat t32fc3(cv::Mat &m,cv::Mat &mdst);
      cv::Mat tgray(cv::Mat &m,cv::Mat &mdst);
      cv::Mat trgb(cv::Mat &m,cv::Mat &mdst);
      cv::Mat abs(cv::Mat &m,cv::Mat &mdst,cv::Mat &m0);
      cv::String atb(std::string s,cv::Mat &m);

      int ewd=1280,eht=720;

      cv::Mat
      frmmp0 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmmp1 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmmp2 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmmp3 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmmp4 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmmp5 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmmp6 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmmp7 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmmp8 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmmp9 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmmp10=cv::Mat(eht,ewd, CV_8UC3, 0.0),

      frmm0 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmm1 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmm2 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmm3 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmm4 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmm5 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmm6 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmm7 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmm8 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmm9 =cv::Mat(eht,ewd, CV_8UC3, 0.0),
      frmm10=cv::Mat(eht,ewd, CV_8UC3, 0.0),

      fr0 =cv::Mat(eht,ewd, CV_32FC3, 0.0),
      fr1 =cv::Mat(eht,ewd, CV_32FC3, 0.0),
      fr2 =cv::Mat(eht,ewd, CV_32FC3, 0.0),
      fr3 =cv::Mat(eht,ewd, CV_32FC3, 0.0),
      fr4 =cv::Mat(eht,ewd, CV_32FC3, 0.0),
      fr5 =cv::Mat(eht,ewd, CV_32FC3, 0.0),
      fr6 =cv::Mat(eht,ewd, CV_32FC3, 0.0),
      fr7 =cv::Mat(eht,ewd, CV_32FC3, 0.0),
      fr8 =cv::Mat(eht,ewd, CV_32FC3, 0.0),
      fr9 =cv::Mat(eht,ewd, CV_32FC3, 0.0),
      fr10=cv::Mat(eht,ewd, CV_32FC3, 0.0);

      cv::Mat movRt;
      double  m;
      cv::Point mp,mp2;

};
#endif
