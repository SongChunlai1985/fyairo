//extern "C"                                                                                       //需要把libbz2.so*复制到/home/root/eye/usr/lib
#include "dfrc.h"

DlibFaceRecog::DlibFaceRecog(void){
}

DlibFaceRecog::~DlibFaceRecog(void){
}

int DlibFaceRecog::DlibRgin(std::vector<dface> &rgface_){
    dlib::deserialize("/home/root/eye/dt/shape_predictor_68_face"
                      "_landmarks.dat") >> sp;
    dlib::deserialize("/home/root/eye/dt/dlib_face_recognition_resnet"
                      "_model_v1.dat") >> net5;
    detector= dlib::get_frontal_face_detector();
    rgf=rgface_;
    return 0;
}

int DlibFaceRecog::svfaces(std::vector<dface> &rgface_){
    rgface_=rgf;
    return 0;
}

dlib::matrix<float,0,1> DlibFaceRecog::fidm(std::vector<float> fid){
    dlib::matrix<float,128,1> id;
    for (ulong i=0;i<128;i++) {
         id(0,long(i))=fid[i];
    }
    return id;
}

std::vector<float> DlibFaceRecog::fidm(dlib::matrix<float,0,1> id){
    std::vector<float> fid;
    for (ulong i=0;i<128;i++) {
         fid.push_back(id(0,long(i)));
    }
    return fid;
}

int DlibFaceRecog::DlibRecognition(cv::Mat a, std::vector<dface> &sf ,float n){
    dlib::cv_image <dlib::rgb_pixel> img2(a);
    assign_image(img,img2);
    dets.clear();

    dets = detector(img);

    if (dets.empty()){
        if(debug){std::cout<<std::endl<<"x";}
        return 0;
    }
    faces.clear();

    for(unsigned long i=0;i<dets.size();i++){
        dface f;
        shape = sp(img, dets[i]);
        for(ulong  k=1;k<68;k++){
            f.d68[k]=cv::Point2f(shape.part(k).x(),shape.part(k).y());
        }

        float fl=dets[i].left(),
              ft=dets[i].top(),
              fr=dets[i].right(),
              fb=dets[i].bottom();

        f.lt=cv::Point2f(fl,ft);
        f.rb=cv::Point2f(fr,fb);
        f.sz=f.rb-f.lt;
        f.p =f.lt+f.sz/2;
        sf.push_back(f);
        extract_image_chip(img,get_face_chip_details(shape,150,0.25),face_chip);
        faces.push_back(std::move(face_chip));
    }

    std::vector<dlib::matrix<float,0,1>>fid = net5(faces);

    for (ulong i = 0; i < fid.size(); ++i) {
        sf[i].fid=fidm(fid[i]);
    }

    if (p2fid==1){
        sf[0].s=100.f;
        rgf.push_back(sf[0]);
        p2fid=0;
        return 0;
    }
    if (p2fid==2){
        sf[0].s=1000000.0f;
        rgf[p2f]=sf[0];
        p2fid=0;
        std::cout<<std::endl<<"id"<<p2f<<" update"<<std::endl;
        std::cout<<std::endl<<fid[0]<<std::endl;
        return 0;
    }

    if(rgf.empty()){
        for (ulong i = 0; i < sf.size(); ++i) {
            rgf.push_back(sf[i]);
        }
        return 0;
    }else{

        for (ulong i=0;i<sf.size() ;i++){
            sf[i].mc=10;

            for (ulong j=0;j<rgf.size();j++){
                dlib::matrix<float,128,1> fid0=fidm(rgf[j].fid);   //128维空间坐标
                float mc = length(fid[i]-fid0);                    //128维空间两点的距离
                if(mc<sf[i].mc){
                    sf[i].mc=mc;
                    sf[i].id=int(j);
                    sf[i].name=rgf[j].name;
                }
            }

            if(sf[i].mc>=0.52f and n>0.999f){
                if (nwf<5) {
                    nwf++;
                    std::cout<<std::endl<<sf[i].mc<<" add new face "
                             <<rgf.size()<<" "<<6-nwf<<" - ";
                }else{
                    rgf.push_back(sf[i]);
                    nwf=0;
                    std::cout <<sf[i].mc<<" add new face "<<rgf.size()-1<<" - ";
                }
            }

            if(0.35f<sf[i].mc && sf[i].mc<0.52f){
                rgf[ulong(sf[i].id)].s+=0.5f;
                nwf=0;
                if(debug)std::cout<<std::endl<<sf[i].mc<<"like"<<sf[i].id<< " - ";
            }

            if(sf[i].mc<=0.35f){
                rgf[ulong(sf[i].id)].s+=1.0f;
                nwf=0;
                if(debug)std::cout<<std::endl<<" recognition "<<sf[i].id<<" r: "
                                  <<sf[i].s  << " - "  <<sf[i].mc;
            }

        }
    }

return 0;
}

