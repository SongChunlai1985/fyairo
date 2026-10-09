#include "mind.h"

mind::mind()
{
    cus[0].e = p3d(0, 0, 0);
    cus[0].sz = p3d(DBL_MAX, DBL_MAX, DBL_MAX);
    cus[0].ag = p3d(0, 0, 0);
    l[1] = l[0];
    l[2] = l[0];
    l[3] = l[0];
    p = 10;
}

mind::~mind() {}

static cv::Mat rsl = cv::Mat(24, 32, CV_8UC3, scalar(0)), rsr = cv::Mat(24, 32, CV_8UC3, scalar(0));

vr mind::fd(cv::Mat t, cv::Mat &el, cv::Mat &er)
{
    double mpv0, mpv1;
    vr l, v;
    cv::Point llp(0, 0), lrp(0, 0), vlp(0, 0), vrp(0, 0);

    cv::matchTemplate(el, t, rsl, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(rsl, &mpv0, &mpv1, &llp, &vlp);

    cv::matchTemplate(er, t, rsr, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(rsr, &mpv0, &mpv1, &lrp, &vrp);

    /*std::cout<<std::endl<<t.rows<<","<<t.cols<<" "
             <<std::endl<<el.rows<<","<<el.cols<<" "
             <<std::endl<<er.rows<<","<<er.cols<<" ";*/
    v.lp = vlp;
    v.rp = vrp;
    return v;
}

vr mind::fd2(cv::Mat t, cv::Mat &er)
{
    double mpv0, mpv1;
    vr l, v;
    cv::Point llp(0, 0), lrp(0, 0), vlp(0, 0), vrp(0, 0);

    cv::matchTemplate(er, t, rsr, cv::TM_CCOEFF_NORMED);
    minMaxLoc(rsr, &mpv0, &mpv1, &lrp, &vrp);

    // std::cout<<"["<<mpv1<<"]";
    v.lp.x = int(mpv1 * 10);
    v.rp = vrp;
    return v;
}

void mind::dbox(obj &b, scalar cl, int world)
{ //一GL帧刷新一次
    dline(b.linesb(cl), 1, world);
    return;
}

void mind::dboxf(obj &b, scalar cl, int world)
{ //一视频帧刷新一次
    dlinef(b.linesb(cl), 1, world);
    return;
}

void mind::dbox(p3d pw, p3d sz, scalar cl, double f, int world)
{
    obj tt;
    tt.e = pw;
    tt.box(sz, f);
    dbox(tt, cl, world);
}

void mind::dboxf(p3d pw, p3d sz, scalar cl, double f, int world)
{
    obj tt;
    tt.e = pw;
    tt.box(sz, f);
    dboxf(tt, cl, world);
}

void mind::dline(axis d, int linewidth, int world) { dline_id(std::move(d), linewidth, world, 1); }

void mind::dline(const std::vector<axis> &d, int linewidth, int world)
{
#ifdef FYAIRO_1_0_0_ARM
    return;
#else
    for (auto &i : d) { dline_id(i, linewidth, world, 1); }
#endif
}

void mind::dline20(const std::vector<axis> &d, int linewidth, int world)
{
#ifdef FYAIRO_1_0_0_ARM
    return;
#else
    for (auto &i : d) { dline_id(i, linewidth, world, 20); }
#endif
}

void mind::dline(p3d d, scalar cl, int linewidth, int world)
{
#ifdef FYAIRO_1_0_0_ARM
    return;
#else
    dline(axis(p3d(0, 0, 0), d, 0, cl), linewidth, world);
    return;
#endif
}

void mind::dlinef(axis d, int linewidth, int world) { dline_id(d, linewidth, world, 2); }
void mind::dline20(axis d, int linewidth, int world) { dline_id(d, linewidth, world, 20); }

void mind::dline_id(axis d, int linewidth, int world, uint id)
{
#ifdef FYAIRO_1_0_0_ARM
    return;
#else
    if (world) d = l[world].worldm * d;
    d.linewidth = (float)linewidth;
    switch (id)
    {
    case 1: ax11.push_back(d); break;
    case 2: ax21.push_back(d); break;
    case 20: ax201.push_back(d); break;
    default: assert(0); // error
    }
#endif
}

void mind::dlinef(p3d pa, p3d pb, scalar cl, int linewidth, int world)
{
#ifdef FYAIRO_1_0_0_ARM
    return;
#else
    dlinef(axis(pa, pb, 0, cl), linewidth, world);
    return;
#endif
}

void mind::dlinef(p3d d, scalar cl, int linewidth, int world)
{
#ifdef FYAIRO_1_0_0_ARM
    return;
#else
    dlinef(p3d(0, 0, 0), d, cl, linewidth, world);
    return;
#endif
}

void mind::dlinef(std::vector<axis> d, int linewidth, int world)
{
#ifdef FYAIRO_1_0_0_ARM
    return;
#else
    for (auto &i : d) { dline_id(i, linewidth, world, 2); }
#endif
}

void mind::dbox(struct vr v, p3d sz, scalar cl, int world) { dbox(v.e(), sz, cl, 2.5 * mm, world); }

void mind::dot(vr v, scalar cl, int world) { dot(v.e(), cl, world); }

void mind::dot(p3d p, scalar cl, int sz, int world)
{
#ifdef FYAIRO_1_0_0_ARM
    return;
#else
    if (world) p = l[world].worldm * p;
    dot11.push_back(ddot(p, cl, sz));
#endif
}

void mind::dotf(p3d p, scalar cl, int world)
{
#ifdef FYAIRO_1_0_0_ARM
    return;
#else
    if (world) p = l[world].worldm * p;
    dot21.push_back(ddot(p, cl));
#endif
}

void mind::ball(p3d p, scalar cl, int sz)
{
#ifdef FYAIRO_1_0_0_ARM
    return;
#else
    ball11.push_back(ddot(p, cl, sz));
#endif
}

void mind::dot(cv::Mat gh, p3d p, scalar cl)
{
    cv::Point mPo = cv::Point(int(l[0].lcsz.x / 2), int(l[0].lcsz.y / 2));
    cv::Point p2 = vr(getm(pm[2]) * p + pm[0] + pm[1]).lp;
    cv::circle(gh, p2 + mPo, 1, cl, 1);
}

/*void mind::txt( p3d p, string s, scalar cl){
   cv::Point mPo=cv::Point(l[0].lcsz.width/2,l[0].lcsz.height/2);
   cv::Point p2=l[0].d4d.e2v2(l[0].d4d.trspt(p,pm[2])+pm[0]+pm[1]).lp;
}*/

void mind::drt(obj st, scalar cl, int world)
{
    dbox(st.e, st.sz, cl, 25 * mm, world);
    dbox(st.e, st.sz * 1.1, scalar(0, 128, 255), 25 * 1.1 * mm, world);
}

void mind::drt(obj &st0, obj st8, scalar cl, double f, int world)
{
    st0.box(st0.sz, f);
    dbox(st8, cl, world);
}

void mind::drt(hbody &bd)
{
    cus[0].box(cus[0].sz, 20 * mm);
    bd.p[bdo][0].box(bd.p[bdo][0].sz, 1 * mm);
    bd.p[bdo][1].box(bd.p[bdo][1].sz, 1 * mm);
    bd.p[bdo][2].box(bd.p[bdo][2].sz, 1 * mm);
    bd.p[fac][0].box(bd.p[fac][0].sz, 10 * mm);
    bd.p[hed][0].box(bd.p[hed][0].sz, 50 * mm);

    bd.p[ubd][0].box(bd.p[ubd][0].sz, 50 * mm);
    bd.p[lh0][0].box(bd.p[lh0][0].sz, 50 * mm);
    bd.p[rh0][0].box(bd.p[rh0][0].sz, 50 * mm);
    bd.p[lh1][0].box(bd.p[lh1][0].sz, 30 * mm);
    bd.p[rh1][0].box(bd.p[rh1][0].sz, 50 * mm);

    bd.p[lh2][0].box(bd.p[lh2][0].sz, 10 * mm);
    bd.p[lh3][0].box(bd.p[lh3][0].sz, 8 * mm);
    bd.p[lh3][1].box(bd.p[lh3][1].sz, 8 * mm);

    bd.p[rh2][0].box(bd.p[rh2][0].sz, 50 * mm);
    bd.p[rh3][0].box(bd.p[rh3][0].sz, 8 * mm);
    bd.p[rh3][1].box(bd.p[rh3][1].sz, 20 * mm);
    bd.p[rh3][2].box(bd.p[rh3][2].sz, 2 * mm);
    bd.p[rh3][3].box(bd.p[rh3][2].sz, 2 * mm);

    bd.p[lf0][0].box(bd.p[lf0][0].sz, 50 * mm);
    bd.p[rf0][0].box(bd.p[rf0][0].sz, 50 * mm);
    bd.p[lf1][0].box(bd.p[lf1][0].sz, 50 * mm);
    bd.p[rf1][0].box(bd.p[rf1][0].sz, 50 * mm);
    bd.p[lf2][0].box(bd.p[lf2][0].sz, 10 * mm);
    bd.p[rf2][0].box(bd.p[rf2][0].sz, 10 * mm);
}

void mind::drt(hbody &bd, scalar cl)
{
    dbox(cus[0], scalar(0, 122, 225));
    dbox(bd.p[bdo][0].e, bd.p[bdo][0].sz, cl);
    dbox(bd.p[bdo][1], cl);
    dbox(bd.p[bdo][2], cl);
    dbox(bd.p[fac][0], cl);
    dbox(bd.p[hed][0], cl);

    dbox(bd.p[ubd][0], cl);
    dbox(bd.p[lh0][0], cl);
    dbox(bd.p[rh0][0], cl);
    dbox(bd.p[lh1][0], cl);
    dbox(bd.p[rh1][0], cl);
    dbox(bd.p[lh2][0], cl);
    dbox(bd.p[lh3][0], cl);
    dbox(bd.p[lh3][1], cl);

    dbox(bd.p[rh2][0], cl);
    dbox(bd.p[rh3][0], cl);
    dbox(bd.p[rh3][1], cl);
    dbox(bd.p[rh3][2], cl);
    dbox(bd.p[rh3][3], cl);

    dbox(bd.p[lf0][0], cl);
    dbox(bd.p[rf0][0], cl);
    dbox(bd.p[lf1][0], cl);
    dbox(bd.p[rf1][0], cl);
    dbox(bd.p[lf2][0], cl);
    dbox(bd.p[rf2][0], cl);
}

void mind::dgrid(obj t, double nwd, double nht, double ndp)
{
    p3d sz = t.sz / 2;
    t.e = t.e - sz;
    for (int x = 0; x < t.sz.x; x += nwd)
    {
        for (int y = 0; y < t.sz.y; y += nht)
        {
            p3d a(x, y, 0), b(x, y, +t.sz.z);
            dlinef(a + t.e, b + t.e, t.cl);
        }
    }

    for (int y = 0; y < t.sz.y; y += nht)
    {
        for (int z = 0; z < t.sz.z; z += ndp)
        {
            p3d a(0, y, z), b(t.sz.x, y, z);
            dlinef(a + t.e, b + t.e, t.cl);
        }
    }

    for (int z = 0; z < t.sz.z; z += ndp)
    {
        for (int x = 0; x < t.sz.x; x += nwd)
        {
            p3d a(x, 0, z), b(x, t.sz.y, z);
            dlinef(a + t.e, b + t.e, t.cl);
        }
    }
}

void mind::reftcr(int tpnew)
{
    int tpold;
    for (int i = 0; i < 4; i++)
    {
        tpold = l[i].tp;
        l[i].tp = tpnew;
        l[i].mov();
        gm44d m = l[1].b[2].p[tpold][0] >> l[1].b[2].p[tpnew][0];
        l[i].worldm = l[i].worldm * m;
    }
}
