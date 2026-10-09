#include<opencv2/core.hpp>
#include<iostream>
#include<fstream>
#include<unistd.h>
#include"dfrc/dfrc.h"
void rdname(std::string fname[])
{
    int i=0;std::string m;
    std::ifstream x;      //文件名
    char path[50]="/home/root/eye/dt/facename.xml";     //路径
    while(!x.is_open()) {
        x.open(path,std::ios::in);
        usleep(500*1000);
    }
    while(x>>m)
    {
    fname[i]=m;
    i++;
    }
    x.close();
    std::cout<<"n";
}

void rdname(std::vector<std::string> &fname)
{
    std::string m;
    std::ifstream x;      //文件名
    char path[50]="/home/root/eye/dt/facename.xml";     //路径
    while(!x.is_open()) {
        x.open(path,std::ios::in);
        usleep(500*1000);
    }
    ulong i=0;
    while(x>>m){
        i++;
        fname.push_back(m);
        std::cout<<i<<":<"<<m<<">,";
    }
    x.close();
    std::cout<<"n";
}

void svname(std::string fname[])
{
    int i=0;std::string m;
    std::ofstream x;      //文件名
    char path[50]="/home/root/eye/dt/facename.xml";     //路径

    if(!x.is_open()) {
         x.open(path,std::ios::out);
         usleep(500*1000);
    }
    while(i<1000){
        m=fname[i]+"\n";
        x<<m;
        i++;
    }
    x.close();
    std::cout<<"o";
}

void svname(std::vector<std::string> fname)
{
    ulong i=0;std::string m;
    std::ofstream x;      //文件名
    char path[50]="/home/root/eye/dt/facname.xml";     //路径
    if(!x.is_open()) {
        x.open(path,std::ios::out);
        usleep(500*1000);
    }
    while(i<fname.size()){
        m=fname[i]+"\n";
        x<<m;
        i++;
    }
    x.close();
    std::cout<<"o";
}

void readface(std::vector<dface> &faces,std::string fn){
    cv::FileStorage fs(fn,cv::FileStorage::READ);
    int N;
    fs["Total"]>>N;
    cv::Mat fidm;
    faces.clear();
    if(N>0){
        for (ulong i=0;i<ulong(N);i++) {
            dface d;
            std::string n=std::to_string(int(i));
            d.id=int(i);
            fs["Name_"+n]>>d.name;
            fs["Familiar_"+n]>>d.s;
            fs["Face_"+n]>>fidm;
            d.fidm(fidm);
            d.hi=0;
            d.ht=0;
            faces.push_back(d);
        }
        //std::cout<<std::endl<<"加载完成"<<N<<"张人脸";
    }
    fs.release();
}

void svface(std::vector<dface> faces,std::string fn)
{
    cv::FileStorage fs(fn,cv::FileStorage::WRITE);
    int N=int(faces.size());
    fs<<"Total"<<N;
    if(N>0){
        for (ulong i=0;i<faces.size();i++) {
            std::string n=std::to_string(int(i));
            fs<<"Name_"+n<<faces[i].name;
            fs<<"Familiar_"+n<<faces[i].s;
            fs<<"Face_"+n<<faces[i].fidm();
        }
        //std::cout<<std::endl<<"保存完成"<<N<<"张人脸";
    }
    fs.release();
}
