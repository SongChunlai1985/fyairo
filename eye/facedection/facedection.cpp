#include "facedection.h"

ResNetDetector:: ResNetDetector(void){
}

ResNetDetector::~ResNetDetector(void){
}

void ResNetDetector::netinit(int model){
    if(model==0){
        dnnnet  = cv::dnn::readNetFromCaffe(
            "/home/root/eye/dt/deployres10.prototxt",
            "/home/root/eye/dt/res10_300x300_ssd_iter_140000.caffemodel");
        inScaleFactor  = 1.0;
        meanVal=scalar(104.0, 177.0, 123.0);
    }
    if(model==1){
        dnnnet = cv::dnn::readNetFromCaffe(
            "/home/root/eye/dt/deployVGG.prototxt",
            "/home/root/eye/dt/VGG_VOC0712_SSD_300x300_iter_120000.caffemodel");
        meanVal=127.5;
    }
    if(model==2){
        dnnnet = cv::dnn::readNetFromCaffe(
            "/home/root/eye/dt/mobilenet_640x640_iter_120000_merge.prototxt",
            "/home/root/eye/dt/mobilenet_640x640_iter_120000_merge.caffemodel"
        );
        meanVal=127.5;
        inScaleFactor=1.0/256*2;
        confidenceThreshold=0.2f;
    }
}

void ResNetDetector::Detection(cv::Mat frame,
                 std::vector<nface> &Rects,  cv::Size size ){

        inputBlob = cv::dnn::blobFromImage(frame, inScaleFactor,
                                                    size, meanVal, false, false);
        dnnnet.setInput(inputBlob, "data");
        detection = dnnnet.forward("detection_out");

    std::ostringstream ss;
    cv::Mat dttMat= cv::Mat(detection.size[2],detection.size[3], CV_32F,
                                              detection.ptr<float>());

      for(int i = 0; i <dttMat.rows && i <20; i++){
          nface Rect;
          float   c = dttMat.at<float>(i,2);
          float  id = dttMat.at<float>(i,1);
          if(c > confidenceThreshold){
              int mc=frame.cols,mr=frame.rows;
              float x(dttMat.at<float>(i,3)*mc),
                    y(dttMat.at<float>(i,4)*mr),
                    r(dttMat.at<float>(i,5)*mc),
                    b(dttMat.at<float>(i,6)*mr);
              float w=r-x,h=b-y;
              if(0<=x&&0<=w&&x+w<=mc&&0<=y&&0<=h&&y+h<=mr&&c<=1){
                  Rect.sz =p2d(cv::Point2f(w,h));
                  Rect.p  =p2d(cv::Point2f(x,y))+Rect.sz/2;
                  Rect.mc =c;
                  Rect.id =id;
                  Rects.push_back(Rect);
              }
          }
      }
}
