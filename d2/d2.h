#ifndef D2_H
#define D2_H

#include "../mind/3d/3d.h"
#include "../mind/local/local.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <opencv2/calib3d.hpp>
//#include <opencv2/viz/vizcore.hpp>

#ifndef FYAIRO_1_0_0_ARM
#ifdef OPENCV_CORE_CUDA
#include "opencv2/cudastereo.hpp"
#endif
#endif

#include "opencv2/ximgproc.hpp"
#include <GL/glut.h>

#define _ewd_ 640 /*1280*/
#define _eht_ 480 /*720*/
#define _npic_ 28 /*26*/
typedef char *pchar;
typedef wchar_t *pwchar;

struct color9
{
    std::vector<std::vector<color>> c;

    color9(ulong ewd, ulong eht)
    {
        color a;
        std::vector<color> an;
        for (uint i = 0; i < eht; i++) { an.push_back(a); }
        for (uint i = 0; i < ewd; i++) { c.push_back(an); }
    }

    color9()
    {
        ulong ewd = 640, eht = 480;
        color a;
        std::vector<color> an;
        for (uint i = 0; i < eht; i++) { an.push_back(a); }
        for (uint i = 0; i < ewd; i++) { c.push_back(an); }
    }

    /**从图像中获取数据*/
    color9(cv::Mat g, cv::Mat h, cv::Mat l, ulong ewd = 640, ulong eht = 480)
    {
        color a;
        std::vector<color> an;
        for (uint i = 0; i < eht; i++) { an.push_back(a); }
        for (uint i = 0; i < ewd; i++) { c.push_back(an); }
#if 1
        for (uint v = 0 + 4; v < eht - 4; v++)
        {
            int v_ = int(eht) - int(v);
            short n[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            for (uint u = 0 + 4; u < ewd - 4; u++)
            {
                int u_ = int(u);
                c[u][v].r0(g.at<cv::Vec3b>(v_, u_).val[2], g.at<cv::Vec3b>(v_, u_).val[1],
                           g.at<cv::Vec3b>(v_, u_).val[0]);

                c[u][v].h0(h.at<cv::Vec3b>(v_, u_).val[2], h.at<cv::Vec3b>(v_, u_).val[1],
                           h.at<cv::Vec3b>(v_, u_).val[0]);

                if (l.at<cv::Vec3b>(v_, u_).val[2] > 30)
                {

                    c[u][v].l = 17;

                    if (l.at<cv::Vec3b>(v_, u_ - 3).val[2] > 30 && l.at<cv::Vec3b>(v_, u_ - 2).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_, u_ - 1).val[2] > 30 && l.at<cv::Vec3b>(v_, u_ + 1).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_, u_ + 2).val[2] > 30 && l.at<cv::Vec3b>(v_, u_ + 3).val[2] > 30)
                    {
                        uchar l = 1;

                        c[u - 3][v].l = l;
                        c[u - 2][v].l = l;
                        c[u - 1][v].l = l;
                        c[u + 1][v].l = l;
                        c[u + 2][v].l = l;
                        c[u + 3][v].l = l;

                        c[u - 3][v].i = n[l];
                        c[u - 2][v].i = n[l];
                        c[u - 1][v].i = n[l];
                        c[u + 1][v].i = n[l];
                        c[u + 2][v].i = n[l];
                        c[u + 3][v].i = n[l];

                        n[l]++;
                    }

                    if (l.at<cv::Vec3b>(v_ - 2, u_ - 3).val[2] > 30 && l.at<cv::Vec3b>(v_ - 1, u_ - 2).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ - 1, u_ - 1).val[2] > 30 && l.at<cv::Vec3b>(v_, u_ + 1).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ + 1, u_ + 2).val[2] > 30 && l.at<cv::Vec3b>(v_ + 1, u_ + 3).val[2] > 30)
                    {
                        uchar l = 2;

                        c[u - 3][v - 2].l = l;
                        c[u - 2][v - 1].l = l;
                        c[u - 1][v - 1].l = l;
                        c[u + 1][v].l = l;
                        c[u + 2][v + 1].l = l;
                        c[u + 3][v + 1].l = l;

                        c[u - 3][v - 2].i = n[l];
                        c[u - 2][v - 1].i = n[l];
                        c[u - 1][v - 1].i = n[l];
                        c[u + 1][v].i = n[l];
                        c[u + 2][v + 1].i = n[l];
                        c[u + 3][v + 1].i = n[l];

                        n[l]++;
                    }

                    if (l.at<cv::Vec3b>(v_ - 3, u_ - 3).val[2] > 30 && l.at<cv::Vec3b>(v_ - 2, u_ - 2).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ - 1, u_ - 1).val[2] > 30 && l.at<cv::Vec3b>(v_ + 1, u_ + 1).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ + 2, u_ + 2).val[2] > 30 && l.at<cv::Vec3b>(v_ + 3, u_ + 3).val[2] > 30)
                    {
                        uchar l = 3;

                        c[u - 3][v - 3].l = l;
                        c[u - 2][v - 2].l = l;
                        c[u - 1][v - 1].l = l;
                        c[u + 1][v + 1].l = l;
                        c[u + 2][v + 2].l = l;
                        c[u + 3][v + 3].l = l;

                        c[u - 3][v - 3].i = n[l];
                        c[u - 2][v - 2].i = n[l];
                        c[u - 1][v - 1].i = n[l];
                        c[u + 1][v + 1].i = n[l];
                        c[u + 2][v + 2].i = n[l];
                        c[u + 3][v + 3].i = n[l];

                        n[l]++;
                    }

                    if (l.at<cv::Vec3b>(v_ - 3, u_ - 2).val[2] > 30 && l.at<cv::Vec3b>(v_ - 2, u_ - 2).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ - 1, u_ - 1).val[2] > 30 && l.at<cv::Vec3b>(v_ + 1, u_ + 0).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ + 2, u_ + 0).val[2] > 30 && l.at<cv::Vec3b>(v_ + 3, u_ + 1).val[2] > 30)
                    {
                        uchar l = 4;

                        c[u - 2][v - 3].l = l;
                        c[u - 2][v - 2].l = l;
                        c[u - 1][v - 1].l = l;
                        c[u + 0][v + 1].l = l;
                        c[u + 0][v + 2].l = l;
                        c[u + 1][v + 3].l = l;

                        c[u - 2][v - 3].i = n[l];
                        c[u - 2][v - 2].i = n[l];
                        c[u - 1][v - 1].i = n[l];
                        c[u + 0][v + 1].i = n[l];
                        c[u + 0][v + 2].i = n[l];
                        c[u + 1][v + 3].i = n[l];

                        n[l]++;
                    }

                    if (l.at<cv::Vec3b>(v_ - 3, u_).val[2] > 30 && l.at<cv::Vec3b>(v_ - 2, u_).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ - 1, u_).val[2] > 30 && l.at<cv::Vec3b>(v_ + 1, u_).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ + 2, u_).val[2] > 30 && l.at<cv::Vec3b>(v_ + 3, u_).val[2] > 30)
                    {
                        uchar l = 5;

                        c[u][v - 3].l = l;
                        c[u][v - 2].l = l;
                        c[u][v - 1].l = l;
                        c[u][v + 1].l = l;
                        c[u][v + 2].l = l;
                        c[u][v + 3].l = l;

                        c[u][v - 3].i = n[l];
                        c[u][v - 2].i = n[l];
                        c[u][v - 1].i = n[l];
                        c[u][v + 1].i = n[l];
                        c[u][v + 2].i = n[l];
                        c[u][v + 3].i = n[l];

                        n[l]++;
                    }

                    if (l.at<cv::Vec3b>(v_ - 3, u_ - 1).val[2] > 30 && l.at<cv::Vec3b>(v_ - 2, u_ - 1).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ - 1, u_ - 1).val[2] > 30 && l.at<cv::Vec3b>(v_ + 1, u_ + 0).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ + 2, u_ + 1).val[2] > 30 && l.at<cv::Vec3b>(v_ + 3, u_ + 1).val[2] > 30)
                    {
                        uchar l = 6;

                        c[u - 1][v - 3].l = l;
                        c[u - 1][v - 2].l = l;
                        c[u - 1][v - 1].l = l;
                        c[u + 0][v + 1].l = l;
                        c[u + 1][v + 2].l = l;
                        c[u + 1][v + 3].l = l;

                        c[u - 1][v - 3].i = n[l];
                        c[u - 1][v - 2].i = n[l];
                        c[u - 1][v - 1].i = n[l];
                        c[u + 0][v + 1].i = n[l];
                        c[u + 1][v + 2].i = n[l];
                        c[u + 1][v + 3].i = n[l];

                        n[l]++;
                    }

                    if (l.at<cv::Vec3b>(v_ + 3, u_ - 3).val[2] > 30 && l.at<cv::Vec3b>(v_ + 2, u_ - 2).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ + 1, u_ - 1).val[2] > 30 && l.at<cv::Vec3b>(v_ - 1, u_ + 1).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ - 2, u_ + 2).val[2] > 30 && l.at<cv::Vec3b>(v_ - 3, u_ + 3).val[2] > 30)
                    {
                        uchar l = 7;

                        c[u - 3][v + 3].l = l;
                        c[u - 2][v + 2].l = l;
                        c[u - 1][v + 1].l = l;
                        c[u + 1][v - 1].l = l;
                        c[u + 2][v - 2].l = l;
                        c[u + 3][v - 3].l = l;

                        c[u - 3][v + 3].i = n[l];
                        c[u - 2][v + 2].i = n[l];
                        c[u - 1][v + 1].i = n[l];
                        c[u + 1][v - 1].i = n[l];
                        c[u + 2][v - 2].i = n[l];
                        c[u + 3][v - 3].i = n[l];

                        n[l]++;
                    }

                    if (l.at<cv::Vec3b>(v_ + 1, u_ - 3).val[2] > 30 && l.at<cv::Vec3b>(v_ + 1, u_ - 2).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ + 1, u_ - 1).val[2] > 30 && l.at<cv::Vec3b>(v_, u_ + 1).val[2] > 30 &&
                        l.at<cv::Vec3b>(v_ - 1, u_ + 2).val[2] > 30 && l.at<cv::Vec3b>(v_ - 1, u_ + 3).val[2] > 30)
                    {
                        uchar l = 8;

                        c[u - 3][v + 1].l = l;
                        c[u - 2][v + 1].l = l;
                        c[u - 1][v + 1].l = l;
                        c[u + 1][v].l = l;
                        c[u + 2][v - 1].l = l;
                        c[u + 3][v - 1].l = l;

                        c[u - 3][v + 1].i = n[l];
                        c[u - 2][v + 1].i = n[l];
                        c[u - 1][v + 1].i = n[l];
                        c[u + 1][v].i = n[l];
                        c[u + 2][v - 1].i = n[l];
                        c[u + 3][v - 1].i = n[l];

                        n[l]++;
                    }
                }

                c[u][v].t0();
            }
        }
#endif
    }
};

void glv3d(p3d e);

void drtg(p3d a, p3d b, p3d c, p3d d);

void drtg(std::deque<obj> t);

void drtg(obj t);

void drtg(p3d p, p3d sz, GLuint tex, gm44d m = gset(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));

void drawSkewed(conr c, int MODE = 1);

void drawSkewed(p3d p, p3d sz, int MODE = 1, gm44d m = gset(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));

void drawBall(p3d p, p3d sz, int MODE = 2);

void drawHalfBall(p3d p, p3d sz, int MODE = 2);

void drawBall(ddot a);

void drtg1(obj &t0, int MODE = 0);

void dline(axis d);

void dotg(double ex, double ey, double ez, double cr, double cg, double cb);

void dotg(p3d &e, scalar &cl);

void dotg(ddot &d);

class d2l
{
  public:
    d2l(void);

    ~d2l(void);

  private:
    int length_threshold = 12; /*1*/
    float distance_threshold = 1.41421356f /*1.41421356f*/;
    double canny_th1 = 12.0 /*12.0*/;
    double canny_th2 = 12.0 /*12.0*/;
    int canny_aperture_size = 5; /*2*/
    bool do_merge = true;
    /*1,2影响性能*/
#ifndef FYAIRO_1_0_0_ARM
    cv::Ptr<cv::ximgproc::FastLineDetector> fld = cv::ximgproc::createFastLineDetector(
        length_threshold, distance_threshold, canny_th1, canny_th2, canny_aperture_size, do_merge);

    cv::Ptr<cv::ximgproc::FastLineDetector> fld2 =
        cv::ximgproc::createFastLineDetector(3, distance_threshold, 150, 150, 3, do_merge);
#endif

    cv::Mat frmlg1, frmrg1, frmlg2, frmrg2;

  public:
    int SOLID = 1, WIRE = 2;
    ulong image_numl = 0, image_numr = 0;

    float ctr[10];

    std::vector<std::vector<cv::Point>> contoursl, contoursr;
    std::vector<cv::Vec4i> hierarchyl, hierarchyr;

    double args[100];
    /**保存图片大小*/
    cv::Size imgsz;
    /**标定板上每行、每列的角点数；
           测试图片中的标定板上内角点数为23*16*/
    cv::Size ptsz = cv::Size(10, 5);
    /**建一个数组缓存检测到的角点，
                      通常采用Point2f形式*/
    std::vector<cv::Point2f> corner_points_bufl, corner_points_bufr;

    std::vector<std::vector<cv::Point2f>> cornerpsl;
    std::vector<std::vector<cv::Point2f>> cornerpsr;

    e3 dots[_ewd_][_eht_];

    cv::Mat imageInputl[_npic_];
    cv::Mat imageInputr[_npic_];
    /**L内外参矩阵，H——单应性矩阵*/
    cv::Mat cameraMatrix[4] = {cv::Mat::eye(3, 3, CV_32F), cv::Mat::eye(3, 3, CV_32F), cv::Mat::eye(3, 3, CV_32F),
                               cv::Mat::eye(3, 3, CV_32F)};
    /**L摄像机的5个畸变系数：k1,k2,p1,p2,k3*/
    cv::Mat distCoefficients[2];

    cv::Mat Rl = cv::Mat::eye(3, 3, CV_32F), Rr = cv::Mat::eye(3, 3, CV_32F), ml, mr, newml, newmr;

    cv::Mat R0 = cv::Mat::eye(3, 3, CV_32F), T0 = cv::Mat::eye(3, 1, CV_32F), E0 = cv::Mat::eye(3, 3, CV_32F),
            F0 = cv::Mat::eye(3, 3, CV_32F), pVE = cv::Mat::eye(_npic_, 2, CV_32F);

    int calib3d(cv::Mat &matl, cv::Mat &matr, string msg[], cv::Mat &mrtl, cv::Mat &mrtr);

    int saveEyeMat();

    int readEyeMat();

    int findChessboard(cv::Mat &matl, cv::Mat &matr, string msg[], std::vector<vr> &cbc);

#ifndef FYAIRO_1_0_0_ARM
#ifdef OPENCV_CORE_CUDA
    /**CUDA Knn 匹配*/
    void dffd(cv::cuda::GpuMat image01, cv::cuda::GpuMat image02);
#endif
#endif

    cv::Point fdr2(cv::Point2d &lp, color9 /**匹配色0*/ &c0, color9 /**匹配色1*/ &c1);

    cv::Point fdr3(cv::Point2d &lp, color9 /**匹配色0*/ &cl, color9 /**匹配色1*/ &cr);

    p3d Stereo1(double lx, double ly, double rx, double ry);

    bool imatch(std::vector<cv::Point> a, std::vector<cv::Point> b);

    std::vector<vr> match(std::vector<cv::Point> a, std::vector<cv::Point> b);

    void fdcts(cv::Mat iml, cv::Mat imr);

    std::vector<obj> fdgochees(cv::Mat iml, cv::Mat imr);

    std::vector<std::vector<vr>> fdcts();

    /**
     * @brief 由四个角确定网格横线和竖线
     * @param ds 角点vr点对
     * @param gsz 格子的宽高
     * @param fml 左图
     * @param fmr 右图
     * @param ll2 左图的线
     * @param lr2 右图的线
     * @param ext
     * @param show 是否画出格线
     * @return 1 成功
     */
    int makeGrid2(std::vector<std::vector<vr>> ds, fysize gsz, cv::Mat &fml, cv::Mat &fmr, std::vector<fyline> *ll2,
                  std::vector<fyline> *lr2, uint ext = 0, int show = 1);

    /**
     * @brief 确定角点
     * @param gdv vr点对数组
     * @param dsl 角点左
     * @param dsr 角点右
     * @param fml 左图
     * @param fmr 右图
     * @param ext
     * @param show
     * @return 1 成功
     */
    int makeGrid3(std::vector<std::vector<vr>> &gdv, std::vector<std::vector<cv::Point2d>> dsl,
                  std::vector<std::vector<cv::Point2d>> dsr, cv::Mat &fml, cv::Mat &fmr, uint ext = 0, int show = 1);

    /**
     * @overload
     */
    int makeGrid3(std::vector<std::vector<vr>> &gdv, std::vector<cv::Point2f> dsl, std::vector<cv::Point2f> dsr,
                  cv::Mat &fml, cv::Mat &fmr, uint ext = 0, int show = 1);
    /**
     * @brief 识别围棋盘
     * @param frmlg 左灰度图像
     * @param frmrg 右灰度图像
     * @param gdv
     * @return
     *
     * 流程：
     * 得到灰度图
     * 快速线段检测获得线段
     * dpsm 扔掉太短和交点太少的线
     * lgrp 分成水平和竖直两组
     * lst 对横竖分别排序
     * 获取边框 到l3a
     * 获取边框的交点为角点
     * cornerSubPix 精确查找角点
     * makeGrid 根据角点获取二维和三维网格 & 画画
     */
    int findgrid(cv::Mat &frmlg, cv::Mat &frmrg, std::vector<std::vector<vr>> &gdv);

    /**
     * @brief 基于霍夫变换识别围棋盘
     * @author cc
     * @param frml 左灰度图像
     * @param frmr 右灰度图像
     * @param gdv
     * @return
     */
    int FindGridHough(cv::Mat &frml, cv::Mat &frmr, std::vector<std::vector<vr>> &gdv);

    /**
     * @test
     * @brief 进行霍夫变换前的预处理
     * @param src 未经处理的灰度图像
     * @param dst 输出图像
     */
    static void HoughPreProcess(const cv::Mat &src,cv::Mat &dst);

    /**
     * @test
     * @brief 霍夫变换 todo:添加自适应算法
     * @param src 未经处理的灰度图像
     * @param lines 识别到的线段
     */
    static bool HoughTransform(cv::InputArray src,std::vector<fyline> &fylines);

};

#endif // D2_H
