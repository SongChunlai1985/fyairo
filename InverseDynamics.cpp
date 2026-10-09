#include"mind/3d/3d.h"

double cosd(double a){
    return cos(a/agl);
}
double sind(double a){
    return sin(a/agl);
}
double asind(double a){
    return asin(a)*agl;
}

void cpt(double &theta1,double &theta2,double &theta3,double &theta4,
         double &TF,
         double x=90,
         double y=-190,
         double z=17
         ){
#if 0
    x=84;
    y=-186;
    z=0;
#endif
                                                         //定义各项数值，初始化
    double  a2=115,                                      //大臂长度
            a3=116.5,                                    //小臂长度
            a4=39.7,                                     //抓手长度
            sita5=75.5,                                  //抓手与小臂角度
            c5=cosd(sita5),
            s5=sind(sita5);

    std::vector<std::vector<double>> A;
    // A=zeros(2,1);
    double  i=1,
                                                         //给出目标点XYZ坐标
                                                         //计算部分
            si4=((x-a4)*(x-a4)+y*y+z*z-a2*a2-a3*a3)/(-2*a2*a3),
            sita4=asind(si4),
            c4=cosd(sita4),
            s4=sind(sita4);


    for(double sita2=-90; sita2<0; sita2+=0.001){
        double  c2=cosd(sita2),
                s2=sind(sita2),
                c3=(z-a2*s2+a3*s4*s2)/(c4*a3*c2),
                s3=(c4*c5*c2*c3-c5*s4*s2)/(s5*c2),
                sita3=asind(s3);
        if (abs(1-c3*c3-s3*s3)<0.001){
            std::vector<double> a;
            a.push_back(sita2);
            a.push_back(sita3);
            A.push_back(a);                      //A(:,i)=[sita2,sita3];
            i=i+1;
        }
    }
    double  B0=0.0,
            B1=0.0;
    for(ulong i=0;i<A.size();i++ ){
        B0+=A[i][0];
        B1+=A[i][1];
    }
    double  B[2];
    B[0]=B0/A.size();
    B[1]=B1/A.size();                            //B=mean(A,2);

    double  sita2=B[0],
            sita3=B[1],
            c2=cosd(sita2),
            s2=sind(sita2),
            c3=cosd(sita3),
            s3=sind(sita3),

            sia=a2*c2-a3*c4*c3*s2-a3*c2*s4,
            sib=-a3*c4*s3,
            sita1=asind((sib*x-sib*a4+sia*y)/(sib*sib+sia*sia)),
            c1=cosd(sita1),
            s1=sind(sita1);


    gm44d   T01=gset( c1,-s1,  0,  0,
                      s1, c1,  0,  0,
                       0,  0,  1,  0,
                       0,  0,  0,  1),

            T12=gset( c2,-s2,  0,  0,
                       0,  0, -1,  0,
                      s2, c2,  0,  0,
                       0,  0,  0,  1),

            T23=gset(  0,  0, -1, a2,
                      c3,-s3,  0,  0,
                     -s3,-c3,  0,  0,
                       0,  0,  0,  1),

            T34=gset( c4,-s4,  0,  0,
                       0,  0, -1,  0,
                      s4, c4,  0,  0,
                       0,  0,  0,  1),

            T45=gset( c5,-s5,  0, a3,
                       0,  0,  1,  0,
                     -s5,-c5,  0,  0,
                       0,  0,  0,  1),

            T56=gset(  1,  0,  0, a4,
                       0,  1,  0,  0,
                       0,  0,  1,  0,
                       0,  0,  0,  1),

            T02=T01*T12,
            T03=T02*T23,
            T04=T03*T34,
            T05=T04*T45,
            T06=T05*T56;

    gm4d    T06c4=T06.GetCol(3);
    double  x1=T06c4[0/*1*/],
            y1=T06c4[1/*2*/],
            z1=T06c4[2/*3*/];

                                                //判断是否为普通解，X，Y误差<2
    if (abs(x1-x)<1 && abs(z1-z)<1 && abs(y1-y)<1 && -sita1>0 && -sita1<90  &&
         abs(s1)<=1 &&    abs(s2)<=1 &&    abs(s3)<=1 &&    abs(s4)<=1 &&
         abs(c1)<=1 &&    abs(c2)<=1 &&    abs(c3)<=1 &&    abs(c4)<=1/**/){
        TF=1.0;
        theta1=-sita1;
        theta2=-sita2;
        theta3=-sita3;
        theta4= sita4+90;
    }else{
        TF=0.0;
#if 0
        theta1=-sita1;
        theta2=-sita2;
        theta3=-sita3;
        theta4= sita4+90;
#endif
    };
#if 0
    printg(T06);
    printf("\n x1:%9.4f y1:%9.4f z1:%9.4f",x1,y1,z1);
    printf("\n s1:%9.4f s2:%9.4f s3:%9.4f s4:%9.4f tf:%9.4f ",-sita1,-sita2,sita3,sita4+90,TF);
#endif
    //TF=1 有解，解赋值给theta1,theta2,theta3,theta4(弧度)，
                                        //TF=0 无解，超出工作空间
}

void cpt(double &theta1,double &theta2,double &theta3,double &theta4,
         double &TF, p3d p){
     cpt(theta1,theta2,theta3,theta4,TF,-p.y/mm,-p.z/mm,-p.x/mm);
#if 0
     printf("\n x1:%9.4f y1:%9.4f z1:%9.4f",-p.y/mm,-p.z/mm,-p.x/mm);
#endif
     usleep(200*1000);
}
