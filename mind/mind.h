#ifndef MIND_H
#define MIND_H

#include "local/local.h"
#include "3d/3d.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

enum{
    l0,l1,l2,l3
};

class mind{
public:
                                           /**在这里初始化两个bd[0]*/
    mind(void);
    ~mind(void);

private:

public:
                                           /**pm[0]:观察者位置
                                            * pm[1]:观察者真实位置
                                            * pm[2]:被观察世界全部旋转
                                            * pm[3]:被观察点位置
                                            * pm[4]:被观察世界全部平移
                                            * pm[5]:观察者的上
                                            *(这几个量不会改变被观察世界各点的坐标关系)*/
    p3d pm[10];

    obj cus[21];
    int p;
    obj *cusp[21];
                                           /** 四境*/
    local l[4];

    int n  =0,
        nt =0,
        dn =0,
        dnt=0;

    // 用于画线/临时
    std::vector<axis> ax1,
                      ax11,
                      ax2,
                      ax21,
                      ax20, // 20号线程专属 todo:修改刷新机制
                      ax201;
    std::vector<ddot> dot1,
                      dot11,
                      dot2,
                      dot21,
                      ball1,
                      ball11;

    vr fd(cv::Mat obj, cv::Mat &el,cv::Mat &er);

    vr fd2(cv::Mat obj ,cv::Mat &er);

    cv::Point points[2];
    cv::Point mPo=cv::Point(l[0].lcsz/2);
    p3d pab32,pats,pbts;
    void dline(axis d, int linewidth=1, int world=l0);           /**和dlinef不同*/
    void dline(const std::vector<axis>& d, int linewidth=1, int world=l0);
    void dline(p3d d,scalar cl=fc_black,int linewidth=1,int world=l0);
                                                  /** @brief 画一条线段 */
    void dlinef(p3d Pa, p3d pb , scalar cl, int linewidth=1, int world=l0);
                                                  /** @brief 画一条线段 */
    void dlinef(axis d, int linewidth=1, int world=l0);
    void dlinef(p3d d, scalar cl=scalar(0,0,0), int linewidth=1, int world=l0);

    void dlinef(std::vector<axis> d, int linewidth=1, int world=l0);
                                                  /** @brief 绘制立方体 */
    void dbox(obj &b, scalar cl , int world=l0);
                                                  /** @brief VR点对膨胀为立方体 */
    void dbox(struct vr v, p3d  sz, scalar cl, int world=l0);
                                                  /** @brief 点膨胀为立方体 */
    void dbox(p3d pw, p3d sz, scalar cl, double f=2.5*mm, int world=l0);
    void dboxf(p3d pw, p3d sz, scalar cl, double f=2.5*mm, int world=l0);
                                                  /** @brief 绘制VR点对转换成的3维点 */
    void dboxf(obj &b, scalar cl, int world=l0);
    void dot(vr v, scalar cl, int world=l0);
                                                  /** @brief 绘制3维点 */
    void dot(p3d p, scalar cl, int sz=3, int world=l0);
    void dotf(p3d p, scalar cl, int world=l0);
    void ball(p3d p, scalar cl,int sz);
                                                  /** @brief 绘制3维点(圆圈) */
    void dot(cv::Mat gh, p3d p, scalar cl);
                                                  /** @brief 由三维点绘制文字 */
    void txt( p3d p, string s, scalar cl);
                                                  /** @brief 所有部位初始化为24个点 */
    void drt(hbody &bd);
                                                  /** @brief 绘制 */
    void drt(obj st, scalar cl, int world=l0);
                                                  /** @brief 实时绘制动态物体 */
    void drt(obj &st0, obj st8, scalar cl, double f, int world=l0);
                                                  /** @brief 绘制身体 */
    void drt(hbody &bd, scalar cl);
                                                  /** @brief 画格子 */
    void dgrid(obj t, double nwd, double nht, double ndp);

    void reftcr(int tpnew);

    void dline_id(axis d, int linewidth, int world, uint id);
    void dline20(const std::vector<axis> &d, int linewidth=1, int world=l0);
    void dline20(axis d, int linewidth=1, int world=l0);
};

#endif // MIND_H
