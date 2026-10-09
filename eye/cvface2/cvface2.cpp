#include "cvface2.h"
cvfacereg::cvfacereg(void){

}

cvfacereg::~cvfacereg(void){

}

void getFace0(cv::Mat &frm,std::vector<std::vector<cv::Point2f>> &landmarks,
              std::vector<cv::Mat> &gface0 , std::vector<std::vector<cv::Point2f>> &landmark,
              std::vector<cv::Point2f> &pos){
    landmark.clear();
    gface0.clear();
    pos.clear();
    for(ulong i=0;i<landmarks.size();i++){
        cv::Point2d eL=landmarks[i][45],
                    eR=landmarks[i][36],
                    ed=eL-eR,
                    o=ed/2+eR;
        std::vector<cv::Point2f> ldmkt;
        double agl=atan2(double(ed.y),double(ed.x));//*57.29577951308232;
        double cx=50/double(ed.x),
               cy=cx;
        cv::Point2d O=cv::Point2d(0,0),
                O2=O-o+cv::Point2d(46,56);
        cv::Mat rm32d(2,3,CV_64FC1);
        getRMM2D(rm32d,agl,o,O2,cx,cy);
        cv::Size sz=cv::Size(92, 112);
        cv::Mat gfacet;
        warpAffine(frm,gfacet,rm32d,sz);
        transform(landmarks[i],ldmkt,rm32d);
        landmark.push_back(ldmkt);
        gface0.push_back(gfacet);
        pos.push_back(o);
    }
}

void cvfacereg::eigenfaceRecognizer(std::vector<cv::Mat> testSample,std::vector<int> &predictLabel ) {
    //识别
    predictLabel.clear();
    if(traindone){
        for(ulong i=0;i<testSample.size();i++){
            int lb= model->predict(testSample[i]);
#if 0
            cout <<" *"<< lb << endl;
#endif
            predictLabel.push_back(lb);
        }
    }
#if 0
    //获得特征值，特征向量，均值    平均脸
    Mat eigenvalues = model->getEigenValues();
    Mat eigenvectors = model->getEigenVectors();
    Mat mean = model->getMean();
    Mat meanFace = mean.reshape(1,faceimgheight);
    Mat dst;
    dst= normal(meanFace,dst);
    imshow("Mean Face", dst);

    //特征脸
    for (int i = 0; i < min(10,eigenvectors.cols); i++){
        Mat ev = eigenvectors.col(i).clone();
        Mat eigenFace = ev.reshape(1, faceimgheight);
        Mat grayscale;
        grayscale = normal(eigenFace, grayscale);
        Mat colorface;
        applyColorMap(grayscale, colorface, COLORMAP_BONE);
        char* winTitle = new char[128];
        sprintf(winTitle, "eigenface_%d", i);
        imshow(winTitle, colorface);
    }

    //重建人脸
    for (int num = min(10, eigenvectors.cols); num < min(300, eigenvectors.cols); num+=15){
        Mat evs = Mat(eigenvectors, Range::all(), Range(0, num));
        Mat projection = LDA::subspaceProject(evs, mean, testSample.reshape(1, 1));
        Mat reconstruction= LDA::subspaceReconstruct(evs, mean, projection);

        Mat result = reconstruction.reshape(1, faceimgheight);
        reconstruction = normal(result, reconstruction);
        char* winTitle = new char[128];
        sprintf(winTitle, "recon_face_%d", num);
        imshow(winTitle, reconstruction);
    }

#endif
}

int cvfacereg::trainmodel(){
    //读取文件并训练
    string imgdir = string("/home/root/eye/dt/faceimgdata/att_faces/s");
    std::vector<cv::Mat>image;
    std::vector<int>labels;
    printf("\n读取文件并训练:\n");
    for(int i=1;i<13;i++){
        string si=std::to_string(i);
        for(int j=1;j<11;j++){
            string sj=std::to_string(j),
                   path=imgdir+si+"/"+sj+".bmp";
            //printf("\n %s",path.c_str());
            printf("#");
            image.push_back(cv::imread(path, 0));
            labels.push_back(i);
        }
    }
    if (image.size() < 1 || labels.size() < 1){
        std::cout << "invalid image path..." << std::endl;
        return 0;
    }
    faceimgheight = image[0].rows;
    faceimgwidth = image[0].cols;
    std::cout << "height:" << faceimgheight << ",width:" << faceimgwidth<<std::endl;
    model->train(image, labels);
    model->write("/home/root/eye/dt/faceimgdata/att_faces/EigenFaceRecognizer.yml");
    return 1;
}

int cvfacereg::eigenfaceinit (int dv){
     face_cascade.load("/home/root/eye/dt/haarcascade_frontalface_alt2.xml");
     facemark = cv::face::FacemarkLBF::create();
     facemark->loadModel("/home/root/eye/dt/lbfmodel.yaml");
     if(dv==2){
        trainmodel();
     }
     printf("\n 开始读取人脸数据文件: "
            " /home/root/eye/dt/faceimgdata/att_faces/EigenFaceRecognizer.yml");
     model->read("/home/root/eye/dt/faceimgdata/att_faces/EigenFaceRecognizer.yml");
     traindone=1;
     printf("\n @done ");
     return 1;
}

int cvfacereg::eigenfacereg (cv::Mat &frm ,int r, std::vector<cv::Mat> &gface0,
                             std::vector<int> &predictLabel,std::vector<cv::Point2f> &pos,
                             std::vector<std::vector<cv::Point2f>> &landmarks,
                             std::vector<cv::Rect> &faces){
    if(traindone!=1)return 0;
    frm0=frm.clone();
    while (inwork) {
        usleep(10*1000);
    }
    inwork=1;
    frm1=frm0.clone();
    faces.clear();
    face_cascade.detectMultiScale(frm1,faces,1.3,2,0|cv::CASCADE_SCALE_IMAGE, cv::Size(120,120));
    facemark->fit(frm1,faces,landmarks);                     // Run landmark detector

#if 0
    if(landmarks.size()){
        Point2d eL=landmarks[0][45],
                eR=landmarks[0][36],
                ed=eL-eR,
                o=ed/2+eR;
        drawFacemarks(frm_,landmarks[0]);
        circle(frm_,landmarks[0][45],4,Scalar(0,0,255));
        circle(frm_,landmarks[0][36],4,Scalar(0,255,0));
        circle(frm_,o,4,Scalar(0,255,255));
        imshow("landmarks",frm_);
    }
#endif
    for ( size_t i = 0; i < faces.size(); i++ ){
        cv::Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
#if 0
        printf(" %d {%d,%d} ",r,center.x,center.y);
#endif
    }
    if (landmarks.size()){
        std::vector<std::vector<cv::Point2f>> landmark;
        getFace0(frm1, landmarks, gface0, landmark,pos);
        std::vector<cv::Mat> ldmkface;
        cv::Mat ldmkft;
        for(ulong i=0;i<gface0.size();i++){
            cvtColor(gface0[i],ldmkft,cv::COLOR_BGR2GRAY);
            ldmkface.push_back(ldmkft);
        }
#if 0
        for (ulong i=0;i<68;i++) {
            Point p=Point(landmark[i])-Point(landmark[37])+Point(40,100);
            circle(ldmkts,p,1,Scalar(0,255,255));
            printf("[%d,%d] ",p.x,p.y);
        };
#endif
#if 0
        if( r){
            imwrite("/www/pages/r.jpg",ldmkts);
        }
        if(!r){
            imwrite("/www/pages/l.jpg",ldmkts);
        }
#endif
        eigenfaceRecognizer(ldmkface,predictLabel);
        inwork=0;
        return 1;
    }
    inwork=0;
    return 0;
}
