#include "mov.h"
using namespace cv;
mov::mov(void){
}

mov::~mov(void){
}

void mov::movrt( cv::Mat &frm0 ){
    cv::String mv;Scalar s;

    frmmp0=frm0.clone();

    if(frmmp0.cols==frmmp1.cols ){
        frmm3=(((frmmp0-frmmp1)-66)+66);
        s= cv::mean( frmm3 );
        putText(frmm3,
                std::to_string(s.val[0]+s.val[1]+s.val[2]),
               Point(10,20),1,1,Scalar(255,0,0));
    }

    frmmp10=frmmp9.clone();
    frmmp9 =frmmp8.clone();
    frmmp8 =frmmp7.clone();
    frmmp7 =frmmp6.clone();
    frmmp6 =frmmp5.clone();
    frmmp5 =frmmp4.clone();
    frmmp4 =frmmp3.clone();
    frmmp3 =frmmp2.clone();
    frmmp2 =frmmp1.clone();
    frmmp1 =frmmp0.clone();

    frmm3.convertTo(movRt,CV_8UC3);

    m=s.val[0]+s.val[1]+s.val[2];
    frmm4=frmmp2;

    fr4=t32fc3(frmm4,fr4) ;
    fr0=t32fc3(frmmp0,fr0) ;
    fr5=fr4 - fr0;

    fr6=abs(fr5,fr6,fr10);

    frmm6=t8uc3(fr6,frmm6);
    frmm7=(tgray(frmm6,frmm7)-18)*255;     //去噪
    frmm8=trgb(frmm7,frmm8)/255;           //二值化
    frmm5= frmm8.mul(frmmp0)  ;

    frmm9=tgray(frmm6,frmm9);

    double mpv0,mpv1;cv::Point mp0;

    minMaxLoc(frmm9,&mpv0,&mpv1,&mp0,&mp);

    if(m>7)mp2=mp;

   //if(mpv1>50) std::cout<<"<"<<mpv1<<">" <<mp;
             /*  <<fr0.at<float>(10,10)<<","
                  <<fr4.at<float>(10,10)<<","
                  <<fr5.at<float>(10,10)<<".";*/


}

//**********必须指定不同的mdst**********

cv::Mat mov::t8uc3(cv::Mat &m,cv::Mat &mdst){
    m.convertTo(mdst,CV_8UC3);
    return mdst;
}

cv::Mat mov::t32fc3(cv::Mat &m,cv::Mat &mdst){
    m.convertTo(mdst, CV_32FC3);
    return mdst;
}

cv::Mat mov::tgray(cv::Mat &m,cv::Mat &mdst){
    //atb("tgray.m",m);

    cv::cvtColor(m,mdst,cv::COLOR_RGB2GRAY);

    //atb("tgray.frtgya",frtgy);

    return mdst;
}

cv::Mat mov::trgb(cv::Mat &m,cv::Mat &mdst){
    //atb("tgray.m",m);

    cv::cvtColor(m,mdst,cv::COLOR_GRAY2RGB);

    //atb("tgray.frtgya",frtgy);

    return mdst;
}

cv::Mat mov::abs(cv::Mat &m,cv::Mat &mdst,cv::Mat &m0){
    //atb("abs.m",m);
    //atb("abs.frtgyo",frtgyo);
    cv::absdiff(m,m0,mdst);
    //atb("abs.frtgya",frtgya);
    return mdst;
}

cv::String mov::atb(std::string s,cv::Mat &m){
    atb_=" "+s+":<"+std::to_string(m.cols      )+","+std::to_string(m.rows   )+","
               +std::to_string(m.channels())+","+std::to_string(m.depth())+">";
    std::cout<<atb_;
    return atb_;
}











