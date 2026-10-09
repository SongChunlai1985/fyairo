#ifndef CVFACE2_H
#define CVFACE2_H
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>
#include <unistd.h>
#include "../../mind/3d/3d.h"
//对原图归一化

void getFace0(cv::Mat &frm,std::vector<std::vector<cv::Point2f>> &landmarks,
              std::vector<cv::Mat> &gface0 , std::vector<std::vector<cv::Point2f>> &landmark,
              std::vector<cv::Point2f> &pos);
class cvfacereg{
public:
     cvfacereg(void);
    ~cvfacereg(void);
private:
    cv::Ptr<cv::face::BasicFaceRecognizer>model = cv::face::EigenFaceRecognizer::create();
    int faceimgheight =92,
        faceimgwidth =112;
    int traindone=0,inwork=0;
    cv::CascadeClassifier face_cascade;
    cv::Ptr<cv::face::Facemark> facemark;
    cv::Mat frm0,frm1;
public:
    void eigenfaceRecognizer(std::vector<cv::Mat> testSample,std::vector<int> &predictLabel);
                                                                /**训练模块*/
    int trainmodel();
                                                                /**初始化模块*/
    int eigenfaceinit (int dv);
                                                                /**识别人脸*/
    int eigenfacereg (cv::Mat &frm , int r, std::vector<cv::Mat> &gface0,
                      std::vector<int> &predictLabel,std::vector<cv::Point2f> &pos,
                      std::vector<std::vector<cv::Point2f>> &landmarks,
                      std::vector<cv::Rect> &faces);
};

#endif //CVFACE2_H
