#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<assert.h>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<opencv2/core.hpp>
#include"mind/3d/3d.h"
#include<opencv2/opencv.hpp>
#include<GL/glut.h>
//#include<glm/glm.hpp>
using namespace std;

struct fMat{
    uchar* data;
    int cols;
    int rows;
    int empty(){
        return cols==0 || rows==0;
    }
};

fmesh loadobj(string path){
        fmesh ms;
        std::vector<traingle> tgs;
        std::vector<p3d> vertices, uvs, normals;

        fstream f;
        f.open(path, ios::in);
         //printf("Loading OBJ file %s...\n", path);
        while( !f.eof() ){
            string lines;
            getline(f, lines);
            if(lines=="")continue;
            vector<string> parameters=spstr(lines," ");
            if ( parameters[0] == "v"){
                    p3d vertex;
                    vertex.x=atof( parameters[1].c_str());
                    vertex.y=atof( parameters[2].c_str());
                    vertex.z=atof( parameters[3].c_str());
                    vertices.push_back(vertex);

            }

            if ( parameters[0] == "vt"){
                    p3d uv;
                    //fscanf_s(file, "%f %f\n", &uv.x, &uv.y );
                    uv.x= atof( parameters[1].c_str());
                    uv.y= atof( parameters[2].c_str());
                    /*std::cout  <<endl<<"["
                                 <<uv.x<<","
                                 <<uv.y<<"]"
                                 <<endl;*/
                    uvs.push_back(uv);

            }

            if ( parameters[0] == "vn"){
                p3d normal;
                normal.x=atof( parameters[1].c_str());
                normal.y=atof( parameters[2].c_str());
                normal.z=atof( parameters[3].c_str());
                normals.push_back(normal);
            }

            if ( parameters[0] == "f" ){
                vector<string> sa=spstr(parameters[1],"/"),
                               sb=spstr(parameters[2],"/"),
                               sc=spstr(parameters[3],"/");
                traingle tg;
                tg.v0=p3d(atof(sa[0].c_str())-1,0,0);
                tg.t0=p3d(atof(sa[1].c_str())-1,0,0);
                tg.n0=p3d(atof(sa[2].c_str())-1,0,0);

                tg.v1=p3d(atof(sb[0].c_str())-1,0,0);
                tg.t1=p3d(atof(sb[1].c_str())-1,0,0);
                tg.n1=p3d(atof(sb[2].c_str())-1,0,0);

                tg.v2=p3d(atof(sc[0].c_str())-1,0,0);
                tg.t2=p3d(atof(sc[1].c_str())-1,0,0);
                tg.n2=p3d(atof(sc[2].c_str())-1,0,0);
                tgs.push_back(tg);
            }
        }

        for( uint i=0; i<tgs.size(); i++ ){
            traingle t;
                           t.v0=vertices[ulong(tgs[i].v0.x)];
            if(uvs.size()){t.t0=     uvs[ulong(tgs[i].t0.x)];}
                           t.n0= normals[ulong(tgs[i].n0.x)];

                           t.v1=vertices[ulong(tgs[i].v1.x)];
            if(uvs.size()){t.t1=     uvs[ulong(tgs[i].t1.x)];}
                           t.n1= normals[ulong(tgs[i].n1.x)];

                           t.v2=vertices[ulong(tgs[i].v2.x)];
            if(uvs.size()){t.t2=     uvs[ulong(tgs[i].t2.x)];}
                           t.n2= normals[ulong(tgs[i].n2.x)];

            ms.t.push_back(t);
        }

        return ms;
}

cv::Mat translucent(cv::Mat img,pt4d tsl=pt4d(1,1,1,0.8)){
    if(img.empty())return img;
    cv::Mat img1,img14[4];
    cv::cvtColor(img,img1,cv::COLOR_RGB2BGRA);
    cv::split(img1,img14);
    if(tsl.x<1.0)img14[0]=img14[0]*tsl.x;
    if(tsl.y<1.0)img14[1]=img14[1]*tsl.y;
    if(tsl.z<1.0)img14[2]=img14[2]*tsl.x;
    if(tsl.a<1.0)img14[3]=img14[3]*tsl.a;
    cv::merge(img14,4,img1);
    return img1;
}

GLuint gettexture(cv::Mat &img, GLuint idTexture = 0){                  //制定纹理的函数
    if(!idTexture)glGenTextures(1, &idTexture);
    if(img.empty())return 0;
    if (idTexture){			                                //加载纹理映射
        glBindTexture(GL_TEXTURE_2D, idTexture);
        /*gluBuild2DMipmaps(GL_TEXTURE_2D, 3, img.cols, img.rows,
                          GL_BGR, GL_UNSIGNED_BYTE, img.data);*/
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.cols, img.rows,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
        glPixelStoref(GL_PACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
    return idTexture;
}

GLuint gettexture(fMat img, GLuint idTexture = 0){                  //制定纹理的函数
    if(!idTexture)glGenTextures(1, &idTexture);
    if(img.empty())return 0;
    if (idTexture){			                                //加载纹理映射
        glBindTexture(GL_TEXTURE_2D, idTexture);
        /*gluBuild2DMipmaps(GL_TEXTURE_2D, 3, img.cols, img.rows,
                          GL_BGR, GL_UNSIGNED_BYTE, img.data);*/
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.cols, img.rows,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
        glPixelStoref(GL_PACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
    return idTexture;
}

GLuint loadtexture(string fileName,pt4d tsl=pt4d(1,1,1,0.8)){
    cv::Mat img = translucent(cv::imread(fileName),tsl);
    return gettexture(img);
}

void DrawObj(fmesh ms,p3d e){
    glBegin(GL_TRIANGLES);
        for(uint i = 0; i< ms.t.size(); i++){

            glTexCoord2d(ms.t[i].t0.x, ms.t[i].t0.y);
            glNormal3d  (ms.t[i].n0.x, ms.t[i].n0.y, ms.t[i].n0.z);
            glVertex3d  (-(ms.t[i].v0.x+e.x), ms.t[i].v0.y+e.y, ms.t[i].v0.z+e.z);

            glTexCoord2d(ms.t[i].t1.x, ms.t[i].t1.y);
            glNormal3d  (ms.t[i].n1.x, ms.t[i].n1.y, ms.t[i].n1.z);
            glVertex3d  (-(ms.t[i].v1.x+e.x), ms.t[i].v1.y+e.y, ms.t[i].v1.z+e.z);

            glTexCoord2d(ms.t[i].t2.x, ms.t[i].t2.y);
            glNormal3d  (ms.t[i].n2.x, ms.t[i].n2.y, ms.t[i].n2.z);
            glVertex3d  (-(ms.t[i].v2.x+e.x), ms.t[i].v2.y+e.y, ms.t[i].v2.z+e.z);
        }
    glEnd();
}
