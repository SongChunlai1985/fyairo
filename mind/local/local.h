#ifndef LOCAL_H
#define LOCAL_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>
#include "../3d/3d.h"

#define Mechanical_arm
                         /**  境 */
class local{
public:

    local(void);
   ~local(void);

private:

public:

  d4 d4d;

  double    ap=0,
            bt=0,
            gm=0,
                                                    /**目心距*/
             c=22.53*mm,                            /**目间距*/ //22.53*mm
            c2=c*2,                                 /**光学补偿:平移n像素*/
           rpx=0,                                   /**人眼平均间距*/
            hc=68.18/2*mm,
                                                    /**视野角 这个值越小 测得距离越大 */
           Ppl=73,//82.3 70.46,
           Ppr=73,//82.3 70.46,
          Pplc=82.3,
          Pprc=82.3;

   p3d     Pov=p3d(0,0,0),
            Po=p3d(0,0,0),
            Pl=p3d(-c,0,0),
            Pr=p3d(+c,0,0),
           Plc=p3d(-hc,0,0),
           Prc=p3d(+hc,0,0);

   bipolar po,
           pm;

                          /**  三身 0:原身 1:实身 2:表身*/
   hbody b[3];

   axis w;

   obj wd[100];

   obj *brg;

   int ewd,eht;

   p2d        lsz,
              rsz,
             lcsz,
             rcsz;
    double    sfl,
              sfr,
             sflc,
             sfrc;

    vver  vl,
          vr,
         vlc,             //Po,P,wd
         vrc;             //Po,P,wd

   int jg=0;

   struct vr  vc,
             vch;

   int tp; // 接触部位 touch point
   int  tp_;

   gm44d currentm,
         currentm_,
         worldm;

   fmeshb ms,
          ms_;

   void init(int ewd_, int eht_);
   void mov();
   int m(int i, int j, axis &v,int x=0);                    /**生成bd[1]*/
   void b0_b1();                                            /**产生行动指令mt和bd[1]*/
   int m();                                         /**产生bd[2]*/
   void b1_b2(gm44d m);
   void refmesh();
};
#endif // LOCAL_H
