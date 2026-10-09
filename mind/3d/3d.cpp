#include "3d.h"
#include <ctime>

mel fyml;

void ushort2byte(ushort c, bytex a) { memcpy(a, &c, sizeof(ushort)); }

void short2byte(short c, bytex a) { memcpy(a, &c, sizeof(short)); }

void setbit(byte &c, byte /**0-32*/ p, bool v)
{
    if (v) { c |= (0x1 << (7 - p)); }
    else
    {
        c &= (0x1 << (7 - p));
    }
}

ushort byte2ushort(bytex a)
{
    ushort c = 0;
    memcpy(&c, a, sizeof(ushort));
    return c;
}

int byte2int(bytex a)
{
    uchar c = 0;
    memcpy(&c, a, sizeof(uchar));
    return (int)c;
}

double u(double a, double b)
{
    if (b == 0.0)
    {
        // printf(" u b=0 ");
    }
    return (b == 0.0) ? DBL_MAX : a / b;
}

double Tan(double a) { return tan(a / agl); }

double Sin(double a) { return sin(a / agl); }

double Cos(double a) { return cos(a / agl); }

double Atan(double y, double x) { return atan2(y, x) * agl; }

double Asin(double y, double r)
{
    double a(u(y, r));
    if (a < -1)
    {
        a = -1;
        printf(" asin y/r<-1 ");
    }
    if (a > 1)
    {
        a = 1;
        printf(" asin y/r>1 ");
    }
    return asin(a) * agl;
}

double sqr(double a)
{
    if (a > 0.0) return sqrt(a);
    if (a == 0.0) return 0.0;
    printf(" sqr(a) a<0 ");
    return -sqrt(abs(a));
}

double rnd(double a) { return double(rand()) / RAND_MAX * a; }

int rnd(int a) { return int(double(rand()) / RAND_MAX * a + 0.5); }

/**
 * @brief 判断a是否在[min, max]内
 * @param a
 * @param min
 * @param max
 * @return
 */
bool hav(double a, double min, double max) { return a >= min && a <= max; }

void setzero(bytex r, int len)
{
    for (int i = 0; i < len; i++) { r[i] = 0; }
}

string str(int s) { return std::to_string(s); }

string str(double s)
{
    char a[128];
    sprintf(a, "%8.3f", s);
    return a;
}

string str(uint s)
{
    char a[128];
    sprintf(a, "%8.3u", s);
    return a;
}

string soc(int a) { return a == 0 ? "关" : "开"; }

std::vector<string> spstr(string s, string c)
{
    string::size_type pos1, pos2;
    std::vector<string> v;
    pos2 = s.find(c);
    pos1 = 0;
    while (string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length()) v.push_back(s.substr(pos1));
    return v;
}

inline double dis2(p3d a, p3d b) { return (b.x - a.x) * (b.x - a.x) + (b.z - a.z) * (b.z - a.z); }

std::vector<p3d> cccp(p3d O1, double r1, p3d O2, double r2)
{
    fcircle a(O1, r1, p3d(0, 1, 0)), b(O2, r2, p3d(0, 1, 0));
    return a & b & a.plane();
}

//_Float128 Tan2 (_Float128 a){return tanf128 (a/agl2);}
//_Float128 Atan2(_Float128 y,_Float128 x){return atan2f128(y,x)*agl2;}
//_Float128 Sin2 (_Float128 a){return sinf128(a/agl2);}
//_Float128 Cos2 (_Float128 a){return cosf128(a/agl2);}

fplane operator*(gm44d mt, fplane c)
{
    fplane rt = c;
    rt.o = mt * c.o;
    rt.x = mt * c.x;
    rt.y = mt * c.y;
    rt.z = mt * c.z;
    rt.n = dnormv(rt.y - rt.o);
    rt.m = rous(mt) * c.m;
    return rt;
}

fcircle operator*(gm44d mt, fcircle c)
{
    fcircle rt = c;
    rt.o = mt * c.o;
    rt.x = mt * c.x;
    rt.y = mt * c.y;
    rt.z = mt * c.z;
    rt.n = dnormv(rt.y - rt.o);
    rt.m = rous(mt) * c.m;
    return rt;
}

std::vector<p3d> operator&(fplane a, fcircle b) { return b & a; }

/**
 * @param a 平面
 * @param b 圆环b
 * @return 被平面a切的圆环b
 */
farea shave(fplane a, fcircle b)
{
    if (a.n == p3d(0, 0, 0)) return farea();
    std::vector<p3d> dab = b.in() * (a & b);
    if (dab.empty())
    {
        if (a.dist(b.o) >= 0)
            return farea(-180, 180);
        else
            return farea();
    }
    pola pab0(dab[0]), pab1(dab[1]);
    return farea(pab0.st, pab1.st);
}

fball operator*(gm44d mt, fball b)
{
    fball rt = b;
    rt.o = mt * b.o;
    rt.x = mt * b.x;
    rt.y = mt * b.y;
    rt.z = mt * b.z;
    rt.n = dnormv(rt.y - rt.o);
    rt.m = rous(mt) * b.m;
    return rt;
}

/**
 * @param a
 * @param b
 * @return 被球面切的圆环b
 */
farea shave(fball a, fcircle b)
{ //刮剩的
    std::vector<p3d> dab = b.in() * (a & b);
    if (dab.empty())
    {
        if (a.contain(b.o))
            return farea(-180, 180);
        else
            return farea();
    }
    pola pab0(dab[0]), pab1(dab[1]);
    return farea(pab0.st, pab1.st);
}

conr operator*(gm44d &m, conr c)
{
    conr c1;
    c1.pt[0][1][1][0] = m * c.pt[0][1][1][0];
    c1.pt[0][1][0][0] = m * c.pt[0][1][0][0];
    c1.pt[0][0][1][0] = m * c.pt[0][0][1][0];
    c1.pt[0][0][0][0] = m * c.pt[0][0][0][0];

    c1.pt[1][1][1][0] = m * c.pt[1][1][1][0];
    c1.pt[1][1][0][0] = m * c.pt[1][1][0][0];
    c1.pt[1][0][1][0] = m * c.pt[1][0][1][0];
    c1.pt[1][0][0][0] = m * c.pt[1][0][0][0];

    c1.pt[0][1][1][1] = m * c.pt[0][1][1][1];
    c1.pt[0][1][0][1] = m * c.pt[0][1][0][1];
    c1.pt[0][0][1][1] = m * c.pt[0][0][1][1];
    c1.pt[0][0][0][1] = m * c.pt[0][0][0][1];

    c1.pt[1][1][1][1] = m * c.pt[1][1][1][1];
    c1.pt[1][1][0][1] = m * c.pt[1][1][0][1];
    c1.pt[1][0][1][1] = m * c.pt[1][0][1][1];
    c1.pt[1][0][0][1] = m * c.pt[1][0][0][1];

    c1.pt[0][1][1][2] = m * c.pt[0][1][1][2];
    c1.pt[0][1][0][2] = m * c.pt[0][1][0][2];
    c1.pt[0][0][1][2] = m * c.pt[0][0][1][2];
    c1.pt[0][0][0][2] = m * c.pt[0][0][0][2];

    c1.pt[1][1][1][2] = m * c.pt[1][1][1][2];
    c1.pt[1][1][0][2] = m * c.pt[1][1][0][2];
    c1.pt[1][0][1][2] = m * c.pt[1][0][1][2];
    c1.pt[1][0][0][2] = m * c.pt[1][0][0][2];

    c1.pt[0][1][1][3] = m * c.pt[0][1][1][3];
    c1.pt[0][1][0][3] = m * c.pt[0][1][0][3];
    c1.pt[0][0][1][3] = m * c.pt[0][0][1][3];
    c1.pt[0][0][0][3] = m * c.pt[0][0][0][3];

    c1.pt[1][1][1][3] = m * c.pt[1][1][1][3];
    c1.pt[1][1][0][3] = m * c.pt[1][1][0][3];
    c1.pt[1][0][1][3] = m * c.pt[1][0][1][3];
    c1.pt[1][0][0][3] = m * c.pt[1][0][0][3];

    return c1;
}

obj operator*(gm44d &m, obj t)
{
    obj T = t;
    T.o = m * t.o;
    T.x = m * t.x;
    T.y = m * t.y;
    T.z = m * t.z;
    T.e = m * t.e;

    T.c = m * t.c;

    T.d[0].a = m * t.d[0].a;
    T.d[0].b = m * t.d[0].b;
    T.d[1].a = m * t.d[1].a;
    T.d[1].b = m * t.d[1].b;
    T.d[2].a = m * t.d[2].a;
    T.d[2].b = m * t.d[2].b;
    T.d[3].a = m * t.d[3].a;
    T.d[3].b = m * t.d[3].b;
    T.d[4].a = m * t.d[4].a;
    T.d[4].b = m * t.d[4].b;

    T.d[0].s = t.d[0].s;
    T.d[1].s = t.d[1].s;
    T.d[2].s = t.d[2].s;
    T.d[3].s = t.d[3].s;
    T.d[4].s = t.d[4].s;

    T.m = m * t.m;
    return T;
}

hbody operator*(gm44d &m, hbody a)
{
    hbody b;
    b.p[bdo][0] = m * a.p[bdo][0];
    b.p[bdo][1] = m * a.p[bdo][1];
    b.p[bdo][2] = m * a.p[bdo][2];

    b.p[fac][0] = m * a.p[fac][0];
    b.p[hed][0] = m * a.p[hed][0];
    b.p[ubd][0] = m * a.p[ubd][0];
    b.p[lh0][0] = m * a.p[lh0][0];
    b.p[rh0][0] = m * a.p[rh0][0];
    b.p[lh1][0] = m * a.p[lh1][0];
    b.p[rh1][0] = m * a.p[rh1][0];

    b.p[lh2][0] = m * a.p[lh2][0];
    b.p[lh3][0] = m * a.p[lh3][0];
    b.p[lh3][0] = m * a.p[lh3][0];

    b.p[rh2][0] = m * a.p[rh2][0];
    b.p[rh3][0] = m * a.p[rh3][0];
    b.p[rh3][1] = m * a.p[rh3][1];
    b.p[rh3][2] = m * a.p[rh3][2];
    b.p[rh3][3] = m * a.p[rh3][3];

    b.p[lf0][0] = m * a.p[lf0][0];
    b.p[lf1][0] = m * a.p[lf1][0];
    b.p[lf2][0] = m * a.p[lf2][0];

    b.p[rf0][0] = m * a.p[rf0][0];
    b.p[rf1][0] = m * a.p[rf1][0];
    b.p[rf2][0] = m * a.p[rf2][0];
    return b;
}

std::vector<traingle> operator*(gm44d m, std::vector<traingle> t)
{
    std::vector<traingle> rt = t;
    for (uint i = 0; i < t.size(); i++)
    {
        rt[i].v0 = m * rt[i].v0;
        rt[i].v1 = m * rt[i].v1;
        rt[i].v2 = m * rt[i].v2;

        rt[i].n0 = m * rt[i].n0;
        rt[i].n1 = m * rt[i].n1;
        rt[i].n2 = m * rt[i].n2;
    }
    return rt;
}

fmesh operator*(gm44d m, fmesh ms)
{
    fmesh rt;
    rt.t = m * ms.t;
    return rt;
}

scalar rnds() { return scalar(rnd(255), rnd(255), rnd(255)); }

scalar hsv(scalar &rgb)
{
    // r,g,b values are from 0 to 1
    // h = [0,360], s = [0,1], v = [0,1]
    // if s == 0, then h = -1 (undefined)

    double R = rgb.val[2] / 255.0, G = rgb.val[1] / 255.0, B = rgb.val[0] / 255.0;
    double H, S, V, min, max, delta, tmp;
    tmp = R < G ? R : G;     // min(R, G);
    min = tmp < B ? tmp : B; // min( tmp, B );
    tmp = R > G ? R : G;     // max( R, G);
    max = tmp > B ? tmp : B; // max(tmp, B );
    V = max;                 // v

    delta = max - min;

    if (max != 0.0)
    {
        S = delta / max; // s
    }
    else
    {
        // r = g = b = 0 // s = 0, v is undefined
        S = 0;
        H = 0;
        return scalar(0, 0, 0);
    }
    if (R - max == 0.0)
        H = (G - B) / delta; // between yellow & magenta
    else if (G - max == 0.0)
        H = 2 + (B - R) / delta; // between cyan & yellow
    else
        H = 4 + (R - G) / delta; // between magenta & cyan

    H *= 60; // degrees
    if (H < 0) H += 360;
    return scalar(H / 360 * 255, S * 255, V * 255);
}

cv::Mat roberts(cv::Mat srcImage)
{
    cv::Mat dstImage = srcImage.clone();
    int nRows = dstImage.rows;
    int nCols = dstImage.cols;
    for (int i = 0; i < nRows - 1; i++)
    {
        for (int j = 0; j < nCols - 1; j++)
        {
            //根据公式计算
            int t1 = (srcImage.at<uchar>(i, j) - srcImage.at<uchar>(i + 1, j + 1)) *
                     (srcImage.at<uchar>(i, j) - srcImage.at<uchar>(i + 1, j + 1));
            int t2 = (srcImage.at<uchar>(i + 1, j) - srcImage.at<uchar>(i, j + 1)) *
                     (srcImage.at<uchar>(i + 1, j) - srcImage.at<uchar>(i, j + 1));
            //计算g（x,y）
            dstImage.at<uchar>(i, j) = uchar(sqrt(t1 + t2));
        }
    }
    return dstImage;
}

cv::Mat cvtcolor(cv::Mat m)
{
    cv::Mat mdst;
    if (m.channels() == 3) { cv::cvtColor(m, mdst, cv::COLOR_RGB2GRAY); }
    if (m.channels() == 1) { cv::cvtColor(m, mdst, cv::COLOR_GRAY2RGB); }
    return mdst;
}

cv::Mat sbmat(cv::Mat &m, cv::Rect rt)
{
    int wd = rt.width / 2, ht = rt.height / 2, w = m.cols, h = m.rows;

    rt.width = rt.width > w ? w : rt.width;
    rt.height = rt.height > h ? h : rt.height;

    rt.x = rt.x > wd ? rt.x : wd;
    rt.y = rt.y > ht ? rt.y : ht;

    rt.x = rt.x > w - wd ? w - wd : rt.x;
    rt.y = rt.y > h - ht ? h - ht : rt.y;

    rt.x = rt.x - wd;
    rt.y = rt.y - wd;
#if 0
    printf("< %d,%d,%d,%d >",rt.x,rt.y,rt.width,rt.height);
#endif
    return cv::Mat(m, rt);
}

cv::Mat submat(cv::Mat &m, cv::Point p, int d)
{
return sbmat(m, cv::Rect(p - cv::Point(d / 2 , d / 2), cv::Size(d, d)));
}

p3d operator*(gm44d mar, p3d p)
{
    gm4d u;
    u[0] = p.x;
    u[1] = p.y;
    u[2] = p.z; // z为零出现错误
    u[3] = 1.0;
    gm4d v = mar * u;
    return p3d(v[0], v[1], v[2]);
}

std::vector<p3d> operator*(gm44d mar, std::vector<p3d> p)
{
    std::vector<p3d> rt;
    for (uint i = 0; i < p.size(); i++) { rt.push_back(mar * p[i]); }
    return rt;
}

axis operator*(gm44d m, axis t)
{
    axis rt = t;
    rt.a = m * t.a;
    rt.b = m * t.b;
    return rt;
}

std::vector<axis> operator*(gm44d mar, std::vector<axis> p)
{
    std::vector<axis> rt;
    for (uint i = 0; i < p.size(); i++)
    {
        rt.push_back(axis(mar * p[i].a, mar * p[i].b, p[i].s, p[i].ca, p[i].linewidth));
    }
    return rt;
}

d4::d4() {}

d4::~d4() {}

vr d4::fd(cv::Mat t, cv::Mat &el, cv::Mat &er)
{
    double mpvl0, mpvl1, mpvr0, mpvr1;
    cv::Point llp, vlp, lrp, vrp;
    vr v;

    cv::matchTemplate(el, t, rsl, cv::TM_CCOEFF_NORMED);
    minMaxLoc(rsl, &mpvl0, &mpvl1, &llp, &vlp);

    if (mpvl1 < 0.35)
    {
        vlp = cv::Point(-2, -2);
        vrp = cv::Point(-2, -2);
        v.lp = vlp;
        v.rp = vrp;
        return v;
    }
    if (mpvl1 < 0.40)
    {
        vlp = cv::Point(-1, -1);
        vrp = cv::Point(-1, -1);
        v.lp = vlp;
        v.rp = vrp;
        return v;
    }

    cv::matchTemplate(er, t, rsr, cv::TM_CCOEFF_NORMED);
    minMaxLoc(rsr, &mpvr0, &mpvr1, &lrp, &vrp);

    if (mpvr1 < 0.35)
    {
        vlp = cv::Point(-2, -2);
        vrp = cv::Point(-2, -2);
        v.lp = vlp;
        v.rp = vrp;
        return v;
    }
    if (mpvr1 < 0.40)
    {
        vlp = cv::Point(-1, -1);
        vrp = cv::Point(-1, -1);
        v.lp = vlp;
        v.rp = vrp;
        return v;
    }
    return v;
}

cv::Point d4::fd2(cv::Mat &t, cv::Mat &er)
{
    double mpv0, mpv1;
    cv::Point l, v;

    cv::matchTemplate(er, t, rsr, cv::TM_CCOEFF_NORMED);
    minMaxLoc(rsr, &mpv0, &mpv1, &l, &v);

    if (mpv1 > 0.90) { return v; }
    else
    {
        return cv::Point(-1, 0);
    }
}

cv::Point d4::fdr(cv::Point lp, cv::Mat &ml)
{
    blk2 = cv::Mat(ml, cv::Rect(lp.x, eht - lp.y, blksz, blksz));
    // mea= mean(blk2);
    rp = fd2(blk2, blk3);
    return rp;
}

#if 0
                                                                                                                        p3d b2e128(bipolar b ){
    p3d e;
    _Float128
            bap=_Float128(b.ap),bbt=_Float128(b.bt),bgm=_Float128(b.gm),
          mel1c=_Float128(fyml.c),
            s=Sin2(bap-bbt),
           cs=_Float128(2.0)*mel1c*_Float128(2.0)*Sin2(bap)*Sin2(bbt);
    e.x=double(_Float128(-2.0)*mel1c*Sin2(bap+bbt)/s);
    e.y=double(cs*Sin2(bgm)/s);
    e.z=double(cs*Cos2(bgm)/s);
    return (e);}                     //arm没有128位计算单元SSE
#endif

vr flp(vr v, int eht)
{
    vr nv = v;
    nv.lp.y = eht - v.lp.y;
    nv.rp.y = eht - v.rp.y;
    return nv;
}

double dnorm(p3d a, p3d b)
{
    p3d c = a - b;
    return sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
}

double dnorm(cv::Point a, cv::Point b)
{
    cv::Point c = a - b;
    return sqrt(c.x * c.x + c.y * c.y);
}

double dnorm(p3d c) { return sqrt(c.x * c.x + c.y * c.y + c.z * c.z); }

p3d dnormv(p3d c) { return c / dnorm(c); }

cv::Rect cvRt(int _x, int _y, int _w, int _h)
{
    std::cout << _x - _w / 2 << "," << _y - _h / 2 << "," << _w << "," << _h;
    return cv::Rect(_x - _w / 2, _y - _h / 2, _h, _w);
}

cv::Rect cvRt(int _x, int _y, int _w) { return cv::Rect(_x - _w / 2, _y - _w / 2, _w, _w); }

cv::Rect cvRt(int _x, int _y)
{
    int _w = 200;
    return cv::Rect(_x - _w / 2, _y - _w / 2, _w, _w);
}

gm44d gset(double m00, double m10, double m20, double m30, double m01, double m11, double m21, double m31, double m02,
           double m12, double m22, double m32, double m03, double m13, double m23, double m33)
{
    gm44d U;
    U.SetRow(0, gm4d{m00, m10, m20, m30});
    U.SetRow(1, gm4d{m01, m11, m21, m31});
    U.SetRow(2, gm4d{m02, m12, m22, m32});
    U.SetRow(3, gm4d{m03, m13, m23, m33});
    return U;
}

gm44d gset(p3d X, p3d Y, p3d Z, p3d M, double m03, double m13, double m23, double m33)
{
    return (gset(X.x, Y.x, Z.x, M.x, X.y, Y.y, Z.y, M.y, X.z, Y.z, Z.z, M.z, m03, m13, m23, m33));
}

void printm(gm44d m)
{
    printf("\n");
    for (int i = 0; i < 4; i++)
    {
        gm4d row = m.GetRow(i);
        string rows;
        printf("\n");
        for (int j = 0; j < 4; j++) { printf(" %9.4f ", row[j]); }
    }
}

//除去矩阵的位移
gm44d rous(gm44d b)
{
    return gset(b.GetRow(0)[0], b.GetRow(0)[1], b.GetRow(0)[2], 0, b.GetRow(1)[0], b.GetRow(1)[1], b.GetRow(1)[2], 0,
                b.GetRow(2)[0], b.GetRow(2)[1], b.GetRow(2)[2], 0);
}

gm44d getm(/**三个角度*/ p3d ag, /*位移**/ p3d t, int turn)
{

    axis X(p3d(0, 0, 0), p3d(1, 0, 0), -ag.x);
    X.getM();
    axis Y(p3d(0, 0, 0), p3d(0, 1, 0), ag.y);
    Y.getM();
    axis Z(p3d(0, 0, 0), p3d(0, 0, 1), ag.z);
    Z.getM();

    gm44d m = gset(1, 0, 0, t.x, 0, 1, 0, t.y, 0, 0, 1, t.z);

    switch (turn)
    {
    case EL_XYZ: return X.m * Y.m * Z.m * m;
    case EL_XZY: return X.m * Z.m * Y.m * m;
    case EL_YXZ: return Y.m * X.m * Z.m * m;
    case EL_YZX: return Y.m * Z.m * X.m * m;
    case EL_ZXY: return Z.m * X.m * Y.m * m;
    case EL_ZYX: return Z.m * Y.m * X.m * m;
    }
    return getm(0, 0, 0);
}

gm44d getm(/**三个角度*/ double gm, double st, double fi, /**位移*/ p3d t, int turn)
{
    return getm(p3d(gm, st, fi), t, turn);
}

/**对图像进行滤波 */
void spimg(const cv::Mat &image, cv::Mat &result)
{
    cv::Mat kernel(3, 3, CV_32F, scalar(0)); //创建并初始化滤波模板
    kernel.at<float>(1, 1) = 5.0;
    kernel.at<float>(0, 1) = -1.0;
    kernel.at<float>(1, 0) = -1.0;
    kernel.at<float>(1, 2) = -1.0;
    kernel.at<float>(2, 1) = -1.0;
    result.create(image.size(), image.type());
    cv::filter2D(image, result, image.depth(), kernel); //对图像进行滤波
}

void dpsm(std::vector<cv::Vec4f> src, std::vector<fyline> &dst)
{
    std::vector<fyline> dst0;
    for (ulong i = 0; i < src.size(); i++)
    {
        fyline myl;
        myl.l = src[i];
        if (myl.len() >= 40
#if 0
            && myl.isstreak(m /*scalar(20,30,50)*/)
#endif
        )
        {
            dst0.push_back(myl);
        }
    }
    for (ulong i = 0; i < dst0.size(); i++)
    {
        int pass = 0;
        double d = 15;
        for (ulong j = 0; j < dst0.size(); j++)
        {
            if (j == i) continue;
            if (dnorm((dst0[i].pa()), dst0[j].pa()) < d || dnorm((dst0[i].pa()), dst0[j].pb()) < d ||
                dnorm((dst0[i].pb()), dst0[j].pa()) < d || dnorm((dst0[i].pb()), dst0[j].pb()) < d)
            {
                pass++;
            }
        }

        if (pass) { dst.push_back(dst0[i]); }
    }
}

void dpsm(const std::vector<cv::Vec4f> &src, std::vector<fyline> &dst, cv::Mat &m, std::vector<cv::Point2d> &d)
{
    std::vector<fyline> dst0;
    fyline myl;
    // 挑出长的线
    for (const auto &i : src)
    {
        //      重新初始化
        myl.ab_ = p3d();
        myl.len_ = 0.0;
        myl.l = i;
        if (myl.len() >= 15
            //            && myl.isstreak(m /*scalar(20,30,50)*/)
        )
        {
            dst0.push_back(myl);
        }
    }
    // 找交叉点
    std::vector<std::vector<cv::Point2d>> ds1;
    for (auto &i : dst0)
    {
        std::vector<cv::Point2d> ds;
        for (auto &j : dst0)
        {
            if (i.l != j.l && i.ii(j))
            { // 防止j和i相等
                ds.push_back(i.i(j));
            }
        }
        //        printf(" %zu ",ds.size());
        i.crs = int(ds.size());
        ds1.push_back(ds);
    }

    for (ulong i = 0; i < dst0.size(); i++)
    {
        for (ulong j = 0; j < dst0.size(); j++)
        { //查与交点线段交点的数量
            if (dst0[i].l != dst0[j].l && dst0[i].ii(dst0[j]) && dst0[j].crs > 5)
            {
#if 1
                dst0[i].crsc++; // 与有多个交点的线的交点数量
#endif
            }
        }

        if (dst0[i].crsc > 5)
        {
            dst.push_back(dst0[i]);
#if 1
            for (auto &v : ds1[i])
            {
                d.push_back(v);
#if 0
                                                                                                                                        cv::circle(m,ds1[i][v],3,
                    /*srnd()*/scalar(0,255,0),1);
#endif
            }

            d.push_back(dst0[i].pa());
            d.push_back(dst0[i].pb());
#if 0
                                                                                                                                    cv::circle(m,dst0[i].pa(),3,
                /*srnd()*/scalar(255,0,255),1);
            cv::circle(m,dst0[i].pb(),3,
                /*srnd()*/scalar(255,0,255),1);
#endif

#endif
        }
    }
}

int BoardLinesFilter(const std::vector<cv::Vec4f> &src, std::vector<fyline> &dst)
{
    std::vector<fyline> dst0;
    fyline tmpl; // 用于检查
    // 挑出长的线
    for (const auto &i : src)
    {
        // 重新初始化
        tmpl.clear();
        tmpl.l = i;
        // 检查重复
        bool redundant = false;
        for (auto j = dst0.begin(); j != dst0.end(); j++)
        {
            // 夹角小 距离短
            if (j->langle(tmpl) < 20 && j->ldist(tmpl) < 5)
            {
                // 长很多就替换
                if (!redundant && tmpl.len() > 1.5 * j->len())
                {
                    dst0.erase(j);
                    break;
                }
                redundant = true;
            }
        }
        // 长度也符合就插入
        if (!redundant && tmpl.len() >= 15)
        {
            //            std::cout << tmpl.len() << std::endl;
            dst0.push_back(tmpl);
        }
    }
    // 找交叉点
    std::vector<std::vector<cv::Point2d>> ds1;
    for (auto &i : dst0)
    {
        std::vector<cv::Point2d> ds;
        for (auto &j : dst0)
        {
            if (i.l != j.l && i.ii(j))
            { // 防止j和i相等
                ds.push_back(i.i(j));
            }
        }
        //        printf(" %zu ",ds.size());
        i.crs = int(ds.size());
        ds1.push_back(ds);
    }

    for (ulong i = 0; i < dst0.size(); i++)
    {
        for (ulong j = 0; j < dst0.size(); j++)
        { //查与交点线段交点的数量
            if (dst0[i].l != dst0[j].l && dst0[i].ii(dst0[j]) && dst0[j].crs > 5)
            {
                dst0[i].crsc++; // 与有多个交点的线的交点数量
            }
        }

        if (dst0[i].crsc > 5) { dst.push_back(dst0[i]); }
    }
    // 检查dst中数据 太少或太多
    if (dst.size() <18)
        return 1;
    else if (dst.size() > 18)
        return 2;
    else
        return 0;
}

cv::Mat normal(cv::Mat src, cv::Mat dst)
{
    if (src.channels() == 1) //若原图单通道
        normalize(src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    else //否则，原图三通道
        normalize(src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
    return dst;
}

cv::Mat getm33d(cv::Mat m32d)
{
    cv::Mat m(3, 3, CV_64FC1);
    m.at<double>(0, 0, 0) = m32d.at<double>(0, 0, 0);
    m.at<double>(0, 1, 0) = m32d.at<double>(0, 1, 0);
    m.at<double>(0, 2, 0) = m32d.at<double>(0, 2, 0);
    m.at<double>(1, 0, 0) = m32d.at<double>(1, 0, 0);
    m.at<double>(1, 1, 0) = m32d.at<double>(1, 1, 0);
    m.at<double>(1, 2, 0) = m32d.at<double>(1, 2, 0);
    m.at<double>(2, 0, 0) = 0;
    m.at<double>(2, 1, 0) = 0;
    m.at<double>(2, 2, 0) = 1;
    return m;
}

cv::Mat getm33d(double d00, double d10, double d20, double d01, double d11, double d21, double d02, double d12,
                double d22)
{
    cv::Mat m(3, 3, CV_64FC1);
    m.at<double>(0, 0, 0) = d00;
    m.at<double>(0, 1, 0) = d10;
    m.at<double>(0, 2, 0) = d20;
    m.at<double>(1, 0, 0) = d01;
    m.at<double>(1, 1, 0) = d11;
    m.at<double>(1, 2, 0) = d21;
    m.at<double>(2, 0, 0) = d02;
    m.at<double>(2, 1, 0) = d12;
    m.at<double>(2, 2, 0) = d22;
    return m;
}

cv::Mat getm32d(cv::Mat m33d)
{
    cv::Mat m(2, 3, CV_64FC1);
    m.at<double>(0, 0, 0) = m33d.at<double>(0, 0, 0);
    m.at<double>(0, 1, 0) = m33d.at<double>(0, 1, 0);
    m.at<double>(0, 2, 0) = m33d.at<double>(0, 2, 0);
    m.at<double>(1, 0, 0) = m33d.at<double>(1, 0, 0);
    m.at<double>(1, 1, 0) = m33d.at<double>(1, 1, 0);
    m.at<double>(1, 2, 0) = m33d.at<double>(1, 2, 0);
    return m;
}

void getRMM2D(cv::Mat &RMM2D, double agl, cv::Point2d o, cv::Point2d m, double cx, double cy)
{
    double snap = sin(-agl), csap = cos(-agl);
    RMM2D.at<double>(0, 0, 0) = csap * cx;
    RMM2D.at<double>(0, 1, 0) = -snap * cy;
    RMM2D.at<double>(0, 2, 0) = o.x - o.x * cx * csap + o.y * cy * snap + m.x;
    RMM2D.at<double>(1, 0, 0) = snap * cx;
    RMM2D.at<double>(1, 1, 0) = csap * cy;
    RMM2D.at<double>(1, 2, 0) = o.y - o.x * cx * snap - o.y * cy * csap + m.y;
}

void getRTM33f(cv::Mat &RMM2D, double agl, cv::Point2d o, cv::Point2d m, double cx, double cy)
{
    double snap = sin(-agl), csap = cos(-agl);
    RMM2D.at<double>(0, 0, 0) = csap * cx;
    RMM2D.at<double>(0, 1, 0) = -snap * cy;
    RMM2D.at<double>(0, 2, 0) = o.x - o.x * cx * csap + o.y * cy * snap + m.x;
    RMM2D.at<double>(1, 0, 0) = snap * cx;
    RMM2D.at<double>(1, 1, 0) = csap * cy;
    RMM2D.at<double>(1, 2, 0) = o.y - o.x * cx * snap - o.y * cy * csap + m.y;
    RMM2D.at<double>(1, 0, 0) = 0;
    RMM2D.at<double>(1, 1, 0) = 0;
    RMM2D.at<double>(1, 2, 0) = 1;
}

bool fsorta(fyline a, fyline b) { return a.ag() < b.ag(); }

void lgrp(std::vector<fyline> src, std::vector<fyline> dst[2])
{
    // std::cout<<std::endl;
    const uint RESERVE_SPACE = 25;             // 提前声明的空间大小
    dst[0].reserve(RESERVE_SPACE);             // 提前获取空间
    dst[1].reserve(RESERVE_SPACE);             // 提前获取空间
    std::sort(src.begin(), src.end(), fsorta); // 按照角度排序
    int j = 0;
    for (ulong i = 0; i < src.size(); i++)
    {
        // std::cout<<src[i].ag()<<", ";
        if (j == 1) { dst[1].push_back(src[i]); }
        else
        {
            dst[0].push_back(src[i]);
        }
        // 不是最后一个 && 角度差大
        if (i < src.size() - 1 && abs((int)(src[i + 1].ag() - src[i].ag())) > 10)
        {
            j++;
            // std::cout<<int(src[i].ag())<<" ;*** ";
        }
    }
}

ulong lmx(std::vector<fyline> ll2[], ulong r0, ulong r1, ulong r2)
{
    ulong maxl = 0, maxi = 0;
    for (ulong i = 0; i < (360 / 10 + 1); i++)
    {
        ulong ll2sz = ll2[i].size();
        if (ll2sz > maxl and r0 != i and r1 != i and r2 != i)
        {
            maxl = ll2sz;
            if (r0 - i > 5) maxi = i;
        }
    }
    return maxi;
}

fyline _lc() { return lc_; }

bool fsorty(fyline a, fyline b) { return a.i(_lc()).y < b.i(_lc()).y; }

bool fsortx(fyline a, fyline b) { return a.i(_lc()).x < b.i(_lc()).x; }

void lst(std::vector<fyline> _src, std::vector<fyline> &dst, fyline lc)
{
    std::vector<fyline> src = _src;
    lc_ = lc;
    // 对于横着的线段
    if (hav(lc.ag(), -15, 15) || hav(lc.ag(), 165, 180) || hav(lc.ag(), -180, -165))
    {
        sort(src.begin(), src.end(), fsortx
             /*[](fyline &a,fyline &b){
         return a.i(_lc()).y < b.i(_lc()).y;
     }*/);
    }
    else
    {
        sort(src.begin(), src.end(), fsorty
             /*[](fyline &a,fyline &b){
         return a.i(_lc()).x < b.i(_lc()).x;
     }*/);
    }

    dst = src;
#ifdef dbg
    for (ulong i = 0; i < dst.size(); i++) { printf(" %2.2f,", src[i].i(lc).x /*y*/); }
#endif
}

void lL3(std::vector<fyline> src, std::vector<fyline> &dst, fyline lc, double dmin, double dmax)
{
#ifdef dbg
    printf("\n");
#endif
    std::vector<fyline> m0;
    if (src.size() > 0)
    {
        for (ulong j = 0; j < src.size(); j++)
        {
            double d0 = 0.0, d1 = 0.0;
            if (lc.l[0] > 0)
            {
                if (j > 0) d0 = src[j].i(lc).y - src[j - 1].i(lc).y; //与lc横线交点距离差
                if (j < src.size() - 1) d1 = src[j + 1].i(lc).y - src[j].i(lc).y;
            }
            else
            {
                if (j > 0) d0 = src[j].i(lc).x - src[j - 1].i(lc).x; //与lc竖线交点距离差
                if (j < src.size() - 1) d1 = src[j + 1].i(lc).x - src[j].i(lc).x;
            }
            double ag0 = 0.0, ag1 = 0.0;
            if (j > 0) ag0 = std::abs(src[j].ag() - src[j - 1].ag());
            if (j < src.size() - 1) ag1 = std::abs(src[j + 1].ag() - src[j].ag()); //相邻直线线角度差ag_
#ifdef dbg
            printf(" %2.2lu/%2.2lu,", src.size(), j);
#endif
            if (
#if 1
                ((dmin < d0 && d0 < dmax) && (ag0 < 8 || ag0 > 172)) ||
                ((dmin < d1 && d1 < dmax) && (ag1 < 8 || ag1 > 172))
#else
                1
#endif
            )
            { //选取间距在5-45之间,
                //角度差在+-20的相邻直线
                src[j].d_ = d0;
                m0.push_back(src[j]);
            }
        } //去重
#if 1
        if (m0.size() > 0)
        {
            for (ulong j = 0; j < m0.size() - 1; j++)
            {
                double d0 = 0.0;
                if (lc.l[0] > 0) { d0 = m0[j + 1].i(lc).y - m0[j].i(lc).y; }
                else
                {
                    d0 = m0[j + 1].i(lc).x - m0[j].i(lc).x;
                }
                if (d0 > dmin) { dst.push_back(m0[j]); }
            }
            dst.push_back(m0[m0.size() - 1]);
        }
#else
        dst = m0;
#endif
    }
}

cv::Mat gAt(float sax, float say, float sbx, float sby, float scx, float scy, float dax, float day, float dbx,
            float dby, float dcx, float dcy)
{
    cv::Mat wp(2, 3, CV_32FC1);
    cv::Point2f sT[3], dT[3];

    sT[0] = cv::Point2f(sax, say);
    sT[1] = cv::Point2f(sbx, sby);
    sT[2] = cv::Point2f(scx, scy);

    dT[0] = cv::Point2f(dax, day);
    dT[1] = cv::Point2f(dbx, dby);
    dT[2] = cv::Point2f(dcx, dcy);

    wp = getAffineTransform(sT, dT);
    return wp;
}

void lL4(std::vector<fyline> src[], fydot7070 &dst)
{
    ulong s0 = src[0].size(), s1 = src[1].size();
    if (s0 == 0 || s1 == 0) { return; }
    for (ulong i = 0; i < s0; i++)
    {
        for (ulong j = 0; j < s1; j++)
        {
            p2d pi = src[0][s0 - 1 - i].i(src[1][s1 - 1 - j]);
            fydot d;
            d.p = pi;
            d.d.x = j;
            d.d.y = i;
            d.l = 1;
            dst.d[i][j] = d;
        }
    }
}

int lL4(std::vector<fyline> ll2[], std::vector<fyline> lr2[], std::vector<std::vector<vr>> &gdv)
{
    ulong sl0 = ll2[0].size(), sl1 = ll2[1].size(), sr0 = ll2[0].size(), sr1 = ll2[1].size();
    // 检查
    if (sl0 == 0 || sl1 == 0 || sr0 == 0 || sr1 == 0 || sl0 != sr0 || sl1 != sr1) { return 0; }

    for (ulong i = 0; i < sl0; i++)
    {
        for (ulong j = 0; j < sl1; j++)
        {
            p2d pli = ll2[0][sl0 - 1 - i].i(ll2[1][sl1 - 1 - j]), pri = lr2[0][sr0 - 1 - i].i(lr2[1][sr1 - 1 - j]);
            gdv[i][j] = vr(pli, pri);
        }
    }
    return 1;
}

void minMaxLoc(cv::Mat src, scalar &minVals, scalar &maxVals, cv::Point minLoc[], cv::Point maxLoc[])
{
    double minVal[3];
    double maxVal[3];
    cv::Mat m[3];
    cv::split(src, m);

    minMaxLoc(m[0], &minVal[0], &maxVal[0], &minLoc[0], &maxLoc[0]);
    minMaxLoc(m[1], &minVal[1], &maxVal[1], &minLoc[1], &maxLoc[1]);
    minMaxLoc(m[2], &minVal[2], &maxVal[2], &minLoc[2], &maxLoc[2]);

    minVals.val[0] = minVal[0];
    minVals.val[1] = minVal[1];
    minVals.val[2] = minVal[2];

    maxVals.val[0] = maxVal[0];
    maxVals.val[1] = maxVal[1];
    maxVals.val[2] = maxVal[2];
}

void dL5(fydot7070 &gd, ulong s0, ulong s1, cv::Mat frmg, int ewd, int eht)
{
    int w = 10, h = 10;
    cv::Point p(w, h);
    static cv::Mat bc;
    if (s0 == 0 || s1 == 0) { return; }
    for (ulong i = 0; i < s0; i++)
    {
        for (ulong j = 0; j < s1; j++)
        {
            cv::Point p0 = gd.d[i][j].p;
            if (gd.d[i][j].l && w < p0.x && p0.x < ewd - w && h < p0.y && p0.y < eht - h)
            {
                cv::Rect r = cv::Rect(p0 - p, p0 + p);
                bc = cv::Mat(frmg, r);
                scalar s(cv::mean(bc));
                gd.d[i][j].s = s;
            }
        }
    }
}

/**hsv颜色相似*/
bool sbs(scalar c1, scalar c2)
{
#if 0
    std::cout<<c;
#endif
    color ca(c1), cb(c2);
    double dh = abs(ca.h - cb.h), dv = abs(ca.v - cb.v);
    return dh < 3 && dv < 3;
}

/**明度差*/
bool sms(scalar c1, scalar c2) { return abs(color(c1).v - color(c2).v) < /*sbsb+*/ 30; }

bool smnm(scalar c1, scalar c2) { return (color(c1).name() == color(c2).name()); }

void dL6(fydot7070 &gd, ulong s0, ulong s1)
{

    if (s0 == 0 || s1 == 0) { return; }
    for (ulong i = 0; i < s0; i++)
    {
        for (ulong j = 0; j < s1; j++)
        {
            //为每一个交叉点找到相同其交叉点的个数
            double t = s0 * s1;
            for (ulong u = 0; u < s0; u++)
            {
                for (ulong v = 0; v < s1; v++)
                {
#ifdef dbg
                    printf(" d:%3.2f s:%3.2f ", d, s);
#endif
                    if (/*sbs(gd.d[i][j].s,gd.d[u][v].s)**/ 1) { gd.d[i][j].m++; }
                }
            }

            double q = gd.d[i][j].m / t;
            if (q < 0.499) gd.d[i][j].l = 0;
        }
    }
}

void dL7(fydot7070 gd, fygird &dd, double s0, double s1, int ewd, int eht)
{

    for (ulong j = 0; j < s1; j++)
    {
        fycols d1;
        for (ulong i = 0; i < s0; i++)
        {
            p2d p0 = gd.d[i][j].p;
            int d = 10;
            if (gd.d[i][j].l > 0 && d < p0.x && p0.x < ewd - d && d < p0.y && p0.y < eht - d)
            {
                d1.r.push_back(gd.d[i][j]);
            }
        }
        if (d1.r.size() > 0)
        {
            dd.c.push_back(d1);
            // std::cout<<"["<<d1.r.size()<<"]";
        }
    }
    // std::cout<<std::endl;
}

void rpr01(fygird &dd1)
{
    long ex = -1;
    for (ulong y = 1; y < dd1.sz.h; y++)
    { //淘汰间距小交叉的竖线
        for (ulong x = 1; x < dd1.sz.w; x++)
        {
            double ix = double(dd1.c[x].r[y].i.x);
            if (-2 < ix && ix < 2) { ex = long(x); }
        }
    }
    if (ex > -1)
    {
        dd1.c.erase(dd1.c.begin() + ex);
        dd1.sz.w--;
    }
}

void rpr02(fygird &dd1)
{
    for (ulong y = 1; y < dd1.sz.h; y++)
    { //相邻横线角度差
        double ax = double(dd1.c[0].r[y].a.x - dd1.c[0].r[y - 1].a.x);
        if (std::abs(ax) > 14)
        {
            for (ulong x = 0; x < dd1.sz.w; x++)
            { //淘汰角度差过大的一条横线
                dd1.c[x].r.erase(dd1.c[x].r.begin() + long(y));
            }
            dd1.sz.h--;
        }
    }
    for (ulong x = 1; x < dd1.sz.w; x++)
    { //相邻竖线角度差
        double ay = double(dd1.c[x - 1].r[0].a.y - dd1.c[x].r[0].a.y);
        if (std::abs(ay) > 14)
        { //淘汰角度差过大的一条竖线
            dd1.c.erase(dd1.c.begin() + long(x));
            dd1.sz.w--;
        }
    }
}

void rpr(fygird &dd, fygird &dd1, fysize &sz, std::vector<fyline> ll0, std::vector<fyline> ll1)
{
    ulong /*mxc=0,*/ mxr = 0, mnr = 99999;

    /*计算可见点阵大小*/
    for (ulong i = 0; i < dd.c.size(); i++)
    {
        for (ulong j = 0; j < dd.c[i].r.size(); j++)
        {
            if (mxr < dd.c[i].r[j].d.y) { mxr = ulong(dd.c[i].r[j].d.y); }
            if (mnr > dd.c[i].r[j].d.y) { mnr = ulong(dd.c[i].r[j].d.y); }
        }
    }
    dd.sz = fysize(dd.c.size(), mxr - mnr + 1);

    if (dd.sz.h * dd.sz.w == 0.0) return;
    /*形成逻辑位置阵*/
    p2d /**空行数量*/ l(dd.c[0].r[0].d.x, mnr);
    fydot7070 f;

    ulong im = dd.c.size(), jm = 0;
    for (ulong i = 0; i < im; i++)
    {
        jm = dd.c[i].r.size();
        for (ulong j = 0; j < jm; j++)
        {
            dd.c[i].r[j].f = dd.c[i].r[j].d;
            dd.c[i].r[j].d -= l;
            if (int(dd.c[i].r[j].d.x) < 0 || int(dd.c[i].r[j].d.y) < 0)
            {
                std::cout << "[" << dd.c[i].r[j].d.x << "," << dd.c[i].r[j].d.y << "]";
            }
            else
            {
                f.d[int(dd.c[i].r[j].d.x)][int(dd.c[i].r[j].d.y)] = dd.c[i].r[j];
            }
        }
    }

    /*创建向量逻辑位置阵*/
    dd1.sz = dd.sz;
    for (ulong i = 0; i < dd.sz.w; i++)
    {
        fycols fc;
        for (ulong j = 0; j < dd.sz.h; j++)
        {
            if (!f.d[i][j].l)
            {
                f.d[i][j].d = p2d(i, j);
                f.d[i][j].p = ll0[ll0.size() - 1 - ulong(f.d[i][j].f.x)].i(ll1[ll1.size() - 1 - ulong(f.d[i][j].f.y)]);
            }
            fc.r.push_back(f.d[i][j]);
        }
        dd1.c.push_back(fc);
    }

    for (ulong x = 0; x < dd1.sz.w; x++)
    { //遍历每一列
        ulong fx = 0;
        for (ulong y = 0; y < dd1.sz.h; y++)
        { //遍历第i列每个元素
            if (dd1.c[x].r[y].l > 0)
            { //查找到此列的竖线号>>c
                fx = ulong(dd1.c[x].r[y].f.x);
                break;
            };
        }
        for (ulong y = 0; y < dd1.sz.h; y++)
        { //统一此列竖线号
            if (!(dd1.c[x].r[y].l > 0)) { dd1.c[x].r[y].f.x = fx; };
        }
    }

    for (ulong y = 0; y < dd1.sz.h; y++)
    { //遍历每一行
        ulong fy = 0;
        for (ulong x = 0; x < dd1.sz.w; x++)
        { //遍历第i行每个元素
            if (dd1.c[x].r[y].l > 0)
            { //查找到此行的横线号>>c
                fy = ulong(dd1.c[x].r[y].f.y);
                break;
            };
        }
        for (ulong x = 0; x < dd1.sz.w; x++)
        { //统一此行横线号
            if (!(dd1.c[x].r[y].l > 0)) { dd1.c[x].r[y].f.y = fy; };
        }
    }

    for (ulong x = 0; x < dd1.sz.w; x++)
    { //根据横竖线号求交点坐标
        for (ulong y = 0; y < dd1.sz.h; y++)
        {
            if (!dd1.c[x].r[y].l)
            {
                dd1.c[x].r[y].p =
                    ll0[ll0.size() - ulong(dd1.c[x].r[y].f.y) - 1].i(ll1[ll1.size() - ulong(dd1.c[x].r[y].f.x) - 1]);
            }
            dd1.c[x].r[y].a = cv::Point2d(ll0[ll0.size() - ulong(dd1.c[x].r[y].f.y) - 1].ag(),
                                          ll1[ll1.size() - ulong(dd1.c[x].r[y].f.x) - 1].ag());
        }
    }

    for (ulong x = 1; x < dd1.sz.w; x++)
    {
        for (ulong y = 1; y < dd1.sz.h; y++)
        { //求间距
            dd1.c[x].r[y].i.x = dd1.c[x].r[y].p.x - dd1.c[x - 1].r[y].p.x;
            dd1.c[x].r[y].i.y = dd1.c[x].r[y].p.y - dd1.c[x].r[y - 1].p.y;
        }
    }

    rpr01(dd1);
    /*    rpr01(dd1);
        rpr01(dd1);
        rpr01(dd1);

        rpr02(dd1);
        rpr02(dd1);
        rpr02(dd1);
        rpr02(dd1);
    */
    sz = sz - dd1.sz;
}

void dataview(std::vector<fygird> dd2)
{
    /*由于很大概率 左下点为角点或边点 定义其为角点或边点*/
    for (ulong d = 0; d < dd2.size(); d++)
    {
        if (dd2[d].sz.w > 2 && dd2[d].sz.h > 2 && dd2[d].c.size() > 2)
        {
            double c0x = double(dd2[d].c[0].r[dd2[d].c[0].r.size() - 1].p.x) /*,
                    c0y=double(dd2[d].c[0].r[dd2[d].c[0].r.size()-1].p.y),
                     c1=double(dd2[d].c[1].r[dd2[d].c[0].r.size()-1].i.x),
                     c2=double(dd2[d].c[2].r[dd2[d].c[0].r.size()-1].i.x)*/
                ;

            if (c0x < 14)
            {
                dd2[d].c.erase(dd2[d].c.begin());
                dd2[d].sz.w--;
            }
        }
    }

    std::vector<fydot> p[3][3];
    for (ulong d = 0; d < dd2.size(); d++)
    {
        if (dd2[d].sz.w > 2 && dd2[d].sz.h > 2 && dd2[d].c.size() > 2)
        {

            for (ulong x = 0; x < 3; ++x)
            {
                for (ulong y = 0; y < 3; ++y) { p[x][y].push_back(dd2[d].c[x].r[dd2[d].c[x].r.size() - 1 - y]); }
            }
        }
    }

    for (ulong x = 0; x < 3; ++x)
    {
        std::cout << std::endl;
        for (ulong y = 0; y < 3; ++y)
        {
            for (ulong d = 0; d < p[x][y].size(); d++)
            {
                fydot f = p[x][y][d];
                printf("% 4.0f,% 4.0f  ", double(f.p.x), double(f.p.y));
                for (ulong e = 0; e < p[x][y].size(); e++)
                {
                    if (p[x][y][d] == p[x][y][e]) { p[x][y][d].n++; }
                }
            }
            for (ulong d = 0; d < p[x][y].size(); d++) { printf("% 2.0d ", p[x][y][d].n); }

            std::cout << "   |   ";
        }
    }
}

int dL8(fygird &ddl, ulong cols, ulong rows)
{
#if 0
    printf("\n");
#endif
    std::vector<ulong> k;
    for (ulong i = 0; i < ddl.c.size(); i++)
    {
        int lt = 0;
        for (ulong j = 0; j < ddl.c[i].r.size(); j++) { lt += ddl.c[i].r[j].l; }
#if 0
        printf("{%2d}",lt);
#endif
        if (lt < 4) { k.push_back(i); }
    }
    for (ulong i = 0; i < k.size(); i++)
    {
        ddl.c.erase(ddl.c.begin() + long(i));
        ddl.sz.w--;
    }
#if 0
    printf("\n");
#endif
    k.clear();
    for (ulong y = 0; y < ddl.sz.h; y++)
    {
        int lt = 0;
        for (ulong x = 0; x < ddl.c.size(); x++) { lt += ddl.c[x].r[y].l; }
#if 0
        printf("{%2d}",lt);
#endif
        if (lt < 4) { k.push_back(y); }
    }

    std::sort(k.begin(), k.end());

    for (ulong y = 0; y < k.size(); y++)
    {
        for (ulong x = 0; x < ddl.c.size(); x++) { ddl.c[x].r.erase(ddl.c[x].r.begin() + long(k[y])); }
#if 0
        printf(" [%2lu] ",k[y]);
#endif
        ddl.sz.h--;
    }

    if (ddl.sz.w > cols || ddl.sz.h > rows)
    {
#ifdef dbg
        printf("\n 3d.cpp 1033: ddl1.sz.w>cols || ddl1.sz.h>rows");
#endif
    }

    return 0;
}

int fypair(fygird &ddl, fygird &ddr, double w, double h, double k)
{
    if (ddl.sz.w - w != 0.0 || ddl.sz.h - h != 0.0 || ddr.sz.w - w != 0.0 || ddr.sz.h - h != 0.0 || ddl.c.size() == 0 ||
        ddr.c.size() == 0)
        return 0;

    if (
#if 1
        std::abs(norm(ddl.c[2].r[ddl.c[2].r.size() - 1 - 0].p - ddl.c[1].r[ddl.c[1].r.size() - 1 - 0].p) -
                 norm(ddl.c[1].r[ddl.c[1].r.size() - 1 - 0].p - ddl.c[0].r[ddl.c[0].r.size() - 1 - 0].p)) < k &&
        std::abs(norm(ddl.c[2].r[ddl.c[2].r.size() - 1 - 1].p - ddl.c[1].r[ddl.c[1].r.size() - 1 - 1].p) -
                 norm(ddl.c[1].r[ddl.c[1].r.size() - 1 - 1].p - ddl.c[0].r[ddl.c[0].r.size() - 1 - 1].p)) < k &&
        std::abs(norm(ddl.c[2].r[ddl.c[2].r.size() - 1 - 2].p - ddl.c[1].r[ddl.c[1].r.size() - 1 - 2].p) -
                 norm(ddl.c[1].r[ddl.c[1].r.size() - 1 - 2].p - ddl.c[0].r[ddl.c[0].r.size() - 1 - 2].p)) < k

        && std::abs(norm(ddl.c[0].r[ddl.c[0].r.size() - 1 - 2].p - ddl.c[0].r[ddl.c[0].r.size() - 1 - 1].p) -
                    norm(ddl.c[0].r[ddl.c[0].r.size() - 1 - 1].p - ddl.c[0].r[ddl.c[0].r.size() - 1 - 0].p)) < k &&
        std::abs(norm(ddl.c[1].r[ddl.c[1].r.size() - 1 - 2].p - ddl.c[1].r[ddl.c[1].r.size() - 1 - 1].p) -
                 norm(ddl.c[1].r[ddl.c[1].r.size() - 1 - 1].p - ddl.c[1].r[ddl.c[1].r.size() - 1 - 0].p)) < k &&
        std::abs(norm(ddl.c[2].r[ddl.c[2].r.size() - 1 - 2].p - ddl.c[2].r[ddl.c[2].r.size() - 1 - 1].p) -
                 norm(ddl.c[2].r[ddl.c[2].r.size() - 1 - 1].p - ddl.c[2].r[ddl.c[2].r.size() - 1 - 0].p)) < k

        && std::abs(norm(ddr.c[2].r[ddr.c[2].r.size() - 1 - 0].p - ddr.c[1].r[ddr.c[1].r.size() - 1 - 0].p) -
                    norm(ddr.c[1].r[ddr.c[1].r.size() - 1 - 0].p - ddr.c[0].r[ddr.c[0].r.size() - 1 - 0].p)) < k &&
        std::abs(norm(ddr.c[2].r[ddr.c[2].r.size() - 1 - 1].p - ddr.c[1].r[ddr.c[1].r.size() - 1 - 1].p) -
                 norm(ddr.c[1].r[ddr.c[1].r.size() - 1 - 1].p - ddr.c[0].r[ddr.c[0].r.size() - 1 - 1].p)) < k &&
        std::abs(norm(ddr.c[2].r[ddr.c[2].r.size() - 1 - 2].p - ddr.c[1].r[ddr.c[1].r.size() - 1 - 2].p) -
                 norm(ddr.c[1].r[ddr.c[1].r.size() - 1 - 2].p - ddr.c[0].r[ddr.c[0].r.size() - 1 - 2].p)) < k

        && std::abs(norm(ddr.c[0].r[ddr.c[0].r.size() - 1 - 2].p - ddr.c[0].r[ddr.c[0].r.size() - 1 - 1].p) -
                    norm(ddr.c[0].r[ddr.c[0].r.size() - 1 - 1].p - ddr.c[0].r[ddr.c[0].r.size() - 1 - 0].p)) < k &&
        std::abs(norm(ddr.c[1].r[ddr.c[1].r.size() - 1 - 2].p - ddr.c[1].r[ddr.c[1].r.size() - 1 - 1].p) -
                 norm(ddr.c[1].r[ddr.c[1].r.size() - 1 - 1].p - ddr.c[1].r[ddr.c[1].r.size() - 1 - 0].p)) < k &&
        std::abs(norm(ddr.c[2].r[ddr.c[2].r.size() - 1 - 2].p - ddr.c[2].r[ddr.c[2].r.size() - 1 - 1].p) -
                 norm(ddr.c[2].r[ddr.c[2].r.size() - 1 - 1].p - ddr.c[2].r[ddr.c[2].r.size() - 1 - 0].p)) < k

    //&& ddl.c[0].r[ddl.c[0].r.size()-1-0]-ddr.c[0].r[ddr.c[0].r.size()-1-0]
#else
        1
#endif
    )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool like(double a, double b, double d)
{
    double c = std::abs(a - b);
    return -d < c && c < d;
}

bool like(p2d a, p2d b, double d) { return cv::norm(a - b) < double(d); }

bool like(p3d a, p3d b, double d) { return cv::norm(a - b) < double(d); }

void svmat(cv::Mat m, string fn, string mn)
{
    cv::Mat m0 = m;
    cv::FileStorage fs(fn, cv::FileStorage::WRITE);
    fs << mn << m0;
    fs.release();
    std::cout << "I";
}

void rdmat(cv::Mat &m, string fn, string mn)
{
    cv::Mat m0;
    cv::FileStorage fs(fn, cv::FileStorage::READ);
    fs[mn] >> m0;
    fs.release();
    m = m0;
    std::cout << std::endl << "i";
}

bool fsortl(const dface &a, const dface &b)
{
    return a.e.x > b.e.x; //降序排列
}

cv::Mat hsvreg(cv::Mat img, int iLowH, int iHighH, int iLowS, int iHighS, int iLowV, int iHighV
#ifdef cvmorphologyExopenclose
               ,
               int szw, int szh, int szwc, int szhc
#endif
)
{
    using namespace cv;

    Mat imgHSV;

    cvtColor(img, imgHSV, COLOR_BGR2HSV); //转为HSV

    Mat imgThresholded;

    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV),
            imgThresholded); // Threshold the image
#ifdef cvmorphologyExopenclose
                             //开操作 (去除一些噪点)  如果二值化后图片干扰部分依然很多，增大下面的size
    Mat element = getStructuringElement(MORPH_RECT, Size(szw, szh));
    morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);

    //闭操作 (连接一些连通域)
    Mat elementc = getStructuringElement(MORPH_RECT, Size(szwc, szhc));
    morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, elementc);
#endif

    return imgThresholded;

    //这里是自定义的求取形心函数，当然用连通域计算更好
    // Point center;
    // center = GetCenterPoint(imgThresholded);//获取二值化白色区域的形心

    // circle(img, center, 100, Scalar(0,0,255), 5, 8, 0);//绘制目标位置
}

bool fsortarea(std::vector<cv::Point> a, std::vector<cv::Point> b) { return cv::contourArea(a) > cv::contourArea(b); }

int GoSubPix(cv::Mat frm, std::vector<nface> &r0, cv::Size sz, string name)
{
    for (uint i = 0; i < r0.size(); i++)
    {
        cv::Mat subm = cvtcolor(sbmat(frm, cv::Rect(r0[i].p, sz))), subt, subc, elem;
        std::vector<std::vector<cv::Point>> ct;

        if (int(r0[i].id) != 1)
        { //白:1 黑:2
            cv::threshold(subm, subt, 190, 255, cv::THRESH_BINARY);
            elem = cv::getStructuringElement(0, cv::Size(7, 7));
        }
        else
        {
            cv::threshold(subm, subt, 100, 255, cv::THRESH_BINARY_INV);
            elem = cv::getStructuringElement(0, cv::Size(7, 7));
        }
        // cv::imshow(name+str(int(i))+"s",subm);
        cv::erode(subt, subt, elem);
        cv::Canny(subt, subc, 100, 200, 3);
        cv::findContours(subc, ct, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
        std::vector<cv::Moments> mu(ct.size());
        for (uint j = 0; j < ct.size(); j++) { mu[j] = moments(ct[j], false); }
        std::vector<p2d> mc(ct.size());
        for (uint j = 0; j < ct.size(); j++) { mc[j] = p2d(mu[j].m10 / mu[j].m00, mu[j].m01 / mu[j].m00); }
        subt = cvtcolor(subc);
        for (uint j = 0; j < ct.size(); j++)
        {
            circle(subt, mc[j], 1, cv::Scalar(0, 255, 0), -1, 8, 0);
#if 1
            for (uint k = 0; k < ct[j].size(); k++) { circle(subt, ct[j][k], 1, cv::Scalar(0, j ? 0 : 255, 255)); }
            std::cout << "@" << j << "," << cv::contourArea(ct[j]);
#endif
        }
        // cv::imshow(name+str(int(i)),subt);
        // cv::waitKey(50);
        if (ct.size())
        {
            std::sort(ct.begin(), ct.end(), fsortarea
                    /*[](std::vector<cv::Point> &a,std::vector<cv::Point> &b){
                 return cv::contourArea(a)>cv::contourArea(b);
                    }*/);
            if (cv::contourArea(ct[0]) > 100)
            {
                r0[i].p = r0[i].p + mc[0] - p2d(sz / 2) /**/;
                std::cout << "@" << name << cv::contourArea(ct[0]);
            }
        }
    }
    return 1;
}


cv::Mat imgTranslate(cv::Mat &matSrc, int xOffset, int yOffset, bool bScale)
{
    // 判断是否改变图像大小,并设定被复制ROI
    int nRows = matSrc.rows;
    int nCols = matSrc.cols;
    int nRowsRet = 0;
    int nColsRet = 0;
    cv::Rect rectSrc;
    cv::Rect rectRet;
    if (bScale)
    {
        nRowsRet = nRows + abs(yOffset);
        nColsRet = nCols + abs(xOffset);
        rectSrc.x = 0;
        rectSrc.y = 0;
        rectSrc.width = nCols;
        rectSrc.height = nRows;
    }
    else
    {
        nRowsRet = matSrc.rows;
        nColsRet = matSrc.cols;
        if (xOffset >= 0)
        {
            rectSrc.x = 0;
        }
        else
        {
            rectSrc.x = abs(xOffset);
        }
        if (yOffset >= 0)
        {
            rectSrc.y = 0;
        }
        else
        {
            rectSrc.y = abs(yOffset);
        }
        rectSrc.width = nCols - abs(xOffset);
        rectSrc.height = nRows - abs(yOffset);
    }
    // 修正输出的ROI
    if (xOffset >= 0)
    {
        rectRet.x = xOffset;
    }
    else
    {
        rectRet.x = 0;
    }
    if (yOffset >= 0)
    {
        rectRet.y = yOffset;
    }
    else
    {
        rectRet.y = 0;
    }
    rectRet.width = rectSrc.width;
    rectRet.height = rectSrc.height;
    // 复制图像
    cv::Mat matRet(nRowsRet, nColsRet, matSrc.type(), cv::Scalar(0));
    matSrc(rectSrc).copyTo(matRet(rectRet));
    return matRet;
}



