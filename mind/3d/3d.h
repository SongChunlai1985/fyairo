#ifndef D3D_H
#define D3D_H

#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <unistd.h>

#ifdef ARM_ROUND_DBL
#ifndef FYAIRO_1_0_0_ARM
#define FYAIRO_1_0_0_ARM
#endif
#endif
/**把GteGaussianElimination.h里LogError改成了printf*/
#include <Mathematics/GteConvertCoordinates.h>

#define varname(x) #x

typedef unsigned char byte;
typedef byte *bytex;
#ifndef FYAIRO_1_0_0_ARM
typedef long double longdouble;
typedef /*long*/ double fdouble;
#else
typedef double longdouble;
typedef double fdouble;
#endif
typedef gte::Matrix<4, 4, fdouble> gm44d;
typedef gte::Vector<4, fdouble> gm4d;
typedef gte::ConvertCoordinates<4, fdouble> gcc4d;
typedef cv::Point3d p3d;
typedef cv::Point2d p2d;
typedef cv::Scalar scalar;
typedef std::string string;

#define fc_black scalar(0, 0, 0)
#define fc_red scalar(0, 0, 255)
#define fc_green scalar(0, 255, 0)
#define fc_yellow scalar(0, 255, 255)
#define fc_blue scalar(255, 0, 0)
#define fc_purple scalar(255, 0, 255)
#define fc_cyan scalar(255, 255, 0)
#define fc_white scalar(255, 255, 255)

const double pi = 180, agl = pi / M_PI, mm = 3.685292;
const useconds_t ms_ = 1000, s_ = 1000 * 1000;

struct mel
{
    double c, /**2*c*/ c2, fl, fr, flc, frc, ap, bt, /*相机中心偏移补偿*/ rpx;
    p2d l_vc_lp, l_vc_rp, l_vch_lp, l_vch_rp, l_lcsz, l_rcsz;
    int ewd, eht;
};
extern mel fyml;

gm44d gset(double m00, double m10, double m20, double m30, double m01, double m11, double m21,
           double m31, double m02, double m12, double m22, double m32, double m03 = 0.0,
           double m13 = 0.0, double m23 = 0.0, double m33 = 1.0);

gm44d gset(p3d X, p3d Y, p3d Z, p3d M, double m03 = 0.0, double m13 = 0.0, double m23 = 0.0,
           double m33 = 1.0);

void printm(gm44d m);

gm44d rous(gm44d b);

void ushort2byte(ushort c, bytex a);

void short2byte(short c, bytex a);

void setbit(byte &c, byte p, bool v);

ushort byte2ushort(bytex a);

int byte2int(bytex a);

void ulong2uint(ulong c, uint *a);

double u(double a, double b);

double Tan(double a);

double Atan(double y, double x);

double Sin(double a);

double Asin(double y, double r);

double Cos(double a);

double sqr(double a);

double rnd(double a);

int rnd(int a);

bool hav(double a, double min, double max);

void setzero(bytex r, int len);

string str(int s);

string str(double s);

string str(uint s);

/**0:"关" 1:"开"*/
string soc(int a);

/**分解字符串*/
std::vector<string> spstr(string s, string c); /**计算平面上的圆相交点*/
std::vector<p3d> cccp(p3d O1, double r1, p3d O2, double r2);

p3d operator*(gm44d mar, p3d p);

std::vector<p3d> operator*(gm44d mar, std::vector<p3d> p);

double dnorm(p3d a, p3d b);

double dnorm(cv::Point a, cv::Point b);

double dnorm(p3d c);

p3d dnormv(p3d c);

/**数域*/
struct farea
{
    std::vector<double> /**最小值*/ a,
        /**最大值*/ b;

    farea() {}

    farea(double a_, double b_)
    {
        if (a_ > b_)
        {
            a.push_back(b_);
            b.push_back(a_);
            farea ar = *this;
            ar = ar.complement();
            *this = ar;
        }
        else
        {
            a.push_back(a_);
            b.push_back(b_);
        }
    }

    double min(uint i = 0)
    {
        if (a.size() > i)
            return a[i];
        else
            return 0;
    }

    /**中间数*/
    double mid(uint i = 0)
    {
        if (a.size() > i)
            return (b[i] - a[i]) / 2 + a[i];
        else
            return 0;
    }

    double max(uint i = 0)
    {
        if (a.size() > i)
            return b[i];
        else
            return 0;
    }

    double width(uint i = 0)
    {
        if (a.size() > i) return b[i] - a[i];
        return 0;
    }

    farea sub(uint i = 0)
    {
        if (a.size() > i) return farea(a[i], b[i]);
        return farea();
    }

    //包含
    bool operator&(double n)
    {
        if (a.size() > 0) return hav(n, a[0], b[0]);
        return false;
    }
    /**交集*/ //位置s(n)=a(n)&b(n)
    farea operator&(farea u)
    {
        if (a.empty() || u.a.empty()) return farea();
        farea rt;
        for (uint j = 0; j < a.size(); j++)
        {
            for (uint i = 0; i < u.a.size(); i++)
            {
                if (!(u.sub(i) & a[j]) && !(u.sub(i) & b[j]))
                { // 00
                    if (sub(j) & u.a[i] && sub(j) & u.b[i])
                    { //内
                        rt.a.push_back(u.a[i]);
                        rt.b.push_back(u.b[i]);
                    }
                }
                else if (!(u.sub(i) & a[j]) && u.sub(i) & b[j])
                { // 01
                    rt.a.push_back(u.a[i]);
                    rt.b.push_back(b[j]);
                }
                else if (u.sub(i) & a[j] && !(u.sub(i) & b[j]))
                { // 10
                    rt.a.push_back(a[j]);
                    rt.b.push_back(u.b[i]);
                }
                else if (u.sub(i) & a[j] && u.sub(i) & b[j])
                { // 11
                    rt.a.push_back(a[j]);
                    rt.b.push_back(b[j]);
                }
            }
        }
        return rt;
    }
    /**并集*/ //位置s(n)=a(n)|b(n)
    farea operator|(farea u)
    {
        if (a.empty()) return u;
        if (u.a.empty()) return *this;
        farea rt;
        for (uint j = 0; j < a.size(); j++)
        {
            for (uint i = 0; i < u.a.size(); i++)
            {
                if (!(u.sub(i) & a[j]) && !(u.sub(i) & b[j]))
                { // 00
                    if (sub(j) & u.a[i] && sub(j) & u.b[i])
                    { //内
                        rt.a.push_back(a[j]);
                        rt.b.push_back(b[j]);
                    }
                    else
                    {
                        rt.a.push_back(a[j]);
                        rt.b.push_back(b[j]);
                        rt.a.push_back(u.a[i]);
                        rt.b.push_back(u.b[i]);
                    }
                }
                else if (!(u.sub(i) & a[j]) && u.sub(i) & b[j])
                { // 01
                    rt.a.push_back(a[j]);
                    rt.b.push_back(u.b[i]);
                }
                else if (u.sub(i) & a[j] && !(u.sub(i) & b[j]))
                { // 10
                    rt.a.push_back(u.a[i]);
                    rt.b.push_back(b[j]);
                }
                else if (u.sub(i) & a[j] && u.sub(i) & b[j])
                { // 11
                    rt.a.push_back(u.a[i]);
                    rt.b.push_back(u.b[i]);
                }
            }
        }
        return rt;
    }

    /**补集*/
    farea complement(/**空间范围*/ farea om = farea(-180, 180))
    {
        farea rt;
        if (a.empty()) return rt;
        if (!(om & a[0]) && !(om & b[0]))
        { // 00
            rt.a.push_back(om.a[0]);
            rt.b.push_back(a[0]);
            return rt;
        }
        else if (om & a[0] && !(om & b[0]))
        { // 01
            rt.a.push_back(om.a[0]);
            rt.b.push_back(a[0]);
            return rt;
        }
        else if (!(om & a[0]) && om & b[0])
        { // 10
            rt.a.push_back(b[0]);
            rt.b.push_back(om.b[0]);
            return rt;
        }
        else if (om & a[0] && om & b[0])
        { // 11
            rt.a.push_back(om.a[0]);
            rt.b.push_back(a[0]);
            rt.a.push_back(b[0]);
            rt.b.push_back(om.b[0]);
            return rt;
        }

        std::cout << " what? ";
        return rt;
    }
};

struct bipolar
{
    double ap, bt, gm;

    bipolar()
    {
        ap = 0;
        bt = 0;
        gm = 0;
    }

    bipolar(double ap_, double bt_, double gm_)
    {
        ap = ap_;
        bt = bt_;
        gm = gm_;
    }

    bipolar(p3d e)
    {
        /*double dp=sqrt(e.y*e.y+e.z*e.z),
               db=2.0*fyml.c*2.0-e.x,
               ad=2.0*fyml.c*2.0+e.x;
               ap=pi-Atan(dp,ad);
               bt=Atan(dp,db);
               gm=Atan(e.y,e.z);*/
        ap = Atan(fyml.c + e.x, e.z);
        bt = Atan(fyml.c - e.x, e.z);
        gm = Atan(e.y, e.z);
    }

    p3d e()
    {
        /*double  m_=Tan(pi+pi/2-ap),
                 s=2*fyml.c*2/(-m_-Tan(bt+pi/2)),
                 x=-s*m_-fyml.c*2,
                dp=Tan(bt)*(fyml.c*2-x),
                 y=Sin(gm)*dp,
                 z=Cos(gm)*dp;
        return p3d(x,y,z);*/
        double st = ap - bt, c = fyml.c;
        p3d rt;
        if (ap < bt)
        {
            double l = 2 * Sin(bt + 90) / Sin(st) * c;
            rt.x = Sin(ap) * l - c;
            rt.z = Cos(ap) * l;
        }
        else
        {
            double r = 2 * Sin(90 - ap) / Sin(st) * c;
            rt.x = Sin(bt) * r + c;
            rt.z = Cos(bt) * r;
        };
        rt.y = Tan(gm) * rt.z;
        return rt;
    }
};

struct e3
{
    double x, y, z, r, g, b;

    e3(p3d e, cv::Vec3b v3)
    {
        x = e.x;
        y = e.y;
        z = e.z;
        r = v3.val[2];
        g = v3.val[1];
        b = v3.val[0];
    }

    e3()
    {
        x = 0;
        y = 0;
        z = 0;
        r = 0;
        g = 0;
        b = 0;
    }

    void dt(cv::Vec3b v3)
    {
        r = v3.val[2];
        g = v3.val[1];
        b = v3.val[0];
    }

    void dt(p3d e)
    {
        x = e.x;
        y = e.y;
        z = e.z;
    }

    p3d e() { return p3d(x, y, z); }
};

/*极坐标系统  从p3d(0,0,1)开始 朝前*/
struct pola
{
    double gm; // 仰角
    double st; // 偏角顺时针
    double r;  // 半径

    pola() {}

    pola(double gm_, double st_, double r_)
    {
        gm = gm_;
        st = st_;
        r = r_;
    }

    /**
     * @brief 从欧氏坐标转换
     */
    pola(p3d pe)
    {
        st = Atan(pe.x, pe.z);
        r = sqr(pe.x * pe.x + pe.y * pe.y + pe.z * pe.z);
        gm = Asin(pe.y, r);
    }

    // 转换成欧氏坐标
    p3d e() { return p3d(Sin(st) * Cos(gm) * r, r * Sin(gm), Cos(st) * Cos(gm) * r); }

    p3d p() { return p3d(gm, st, 0); }

    p3d self() { return p3d(gm, st, r); }
};

/**线段*/
struct axis
{
    p3d a, // 起点
        b, // 终点
        e, // 单位向量
        o; // 中点
    scalar ca, cb;
    double s, s_, /**power*/
        pw,       /**per power*/
        ppw;
    gm44d m;
    float linewidth;
    int ax;
    double l;

    p3d O()
    {
        o = a + (b - a) / 2;
        return o;
    }

    /**向量*/
    p3d v() { return b - a; }

    /**长度*/
    double d()
    {
        l = cv::norm(v());
        return l;
    }

    /**单位向量*/
    p3d n()
    {
        e = v() / d();
        return e;
    }

    /**直线与直线的间距*/
    double dst(axis d)
    {
        p3d /**垂直向量*/ e3 = n().cross(d.n());
        e3 = e3 / dnorm(e3);
        return fabs(e3.dot(a) - e3.dot(d.a));
    }

    /**直线与点的间距*/
    double dst(p3d p)
    {
        p3d v1 = axis(a, p).v().cross(v());
        return dnorm(v1) / d();
    }

    p3d foot(p3d p)
    {
        double ap = dnorm(p - a), h = dst(p);
        return sqr(ap * ap - h * h) * e + a;
    }

    /**靠拢目标角度*/
    int operator>>(axis d)
    {
        pw = d.pw;
        ppw = d.ppw;
        double tpw = pw * ppw;
        if (abs(d.s - s) <= tpw)
        {
            s = d.s;
            return 0;
        }
        if (s > d.s)
            s -= tpw;
        else
            s += tpw;
        return 1;
    }

    void w(double _s = 0, double _pw = 1, double _ppw = 0.1)
    {
#if 1
        if (_pw == 0.0) { printf("0"); }
#endif
        s = _s;
        pw = _pw;
        ppw = _ppw;
    }

    axis()
    {
        s_ = 1;
        pw = 1;
        ppw = 0.1;
    }

    axis(p3d pa, p3d pb, double st = 0, scalar cl = scalar(0, 0, 0), float linewidth_ = 1.0)
    {
        a = pa;
        b = pb;
        s = st;
        s_ = 1;
        pw = 1;
        ppw = 0.1;
        ca = cl / 2;
        cb = cl;
        linewidth = linewidth_;
        e = (b - a) / dnorm(b - a);
        l = d();
    }

    axis(double l_, p3d a_, p3d e_)
    {
        a = a_;
        e = e_ / dnorm(e_);
        l = l_;
        b = a_ + e * l;
    }

    /**获取角轴旋转平移矩阵
                                                         从正前(z=1)开始 顺时针旋转*/
    gm44d getM()
    {
        n();
        O();

        double x = e.x, y = e.y, z = e.z, r = s / agl,

               c = cos(r), sd = sin(r), i = 1 - c, xx = x * x * i, yy = y * y * i, xs = x * sd,
               xy = x * y * i, yz = y * z * i, ys = y * sd, xz = x * z * i, zz = z * z * i,
               zs = z * sd;

        gm44d mar0 = gset(1, 0, 0, -o.x, 0, 1, 0, -o.y, 0, 0, 1, -o.z),

              mar1 = gset(xx + c, xy - zs, xz + ys, 0, xy + zs, yy + c, yz - xs, 0, xz - ys,
                          yz + xs, zz + c, 0),

              mar2 = gset(1, 0, 0, o.x, 0, 1, 0, o.y, 0, 0, 1, o.z);

        m = mar2 * mar1 * mar0;
        s_ = s;
        return m;
    }

    p3d LineLineIntersection(p3d p1, p3d v1, p3d p2, p3d v2)
    {
        p3d intersection;
        p3d startPointSeg = p2 - p1;
        p3d vecS1 = v1.cross(v2);            // 有向面积1
        p3d vecS2 = startPointSeg.cross(v2); // 有向面积2
        double num2 = vecS2.dot(vecS1) / (dnorm(vecS1) * dnorm(vecS1));
        intersection = p1 + v1 * num2;
        return intersection;
    }

    p3d operator*(axis h)
    {
        p3d nn = n().cross(h.n());
        nn = nn / dnorm(nn);
        double ds = dst(h);
        p3d i = nn * ds;
        axis l03(h + i);
        return LineLineIntersection(a, n(), l03.a, l03.n());
    }

    axis operator+(p3d mv)
    {
        axis rt(*this);
        rt.a = a + mv;
        rt.b = b + mv;
        return rt;
    }

    void _save(cv::FileStorage &fs, string nm)
    {
        fs << nm + "_a" << a;
        fs << nm + "_b" << b;
        fs << nm + "_ca" << ca;
        fs << nm + "_cb" << cb;
        fs << nm + "_s" << s;

        for (int i = 0; i < 4; i++)
        {
            gm4d row = m.GetRow(i);
            string rows;
            for (int j = 0; j < 4; j++) { rows += str(double(row[j])) + " "; }
            fs << nm + "_m_" + str(i) << rows;
        }
    }

    void _read(cv::FileStorage &fs, string nm)
    {
        fs[nm + "_a"] >> a;
        fs[nm + "_b"] >> b;
        fs[nm + "_ca"] >> ca;
        fs[nm + "_cb"] >> cb;
        fs[nm + "_s"] >> s;
        for (int i = 0; i < 4; i++)
        {
            string rows;
            fs[nm + "_m_" + str(i)] >> rows;
            std::vector<string> rowv = spstr(rows, " ");
            if (rowv.size() == 4)
            {
                gm4d row;
                for (ulong j = 0; j < 4; j++) { row[int(j)] = double(std::atof(rowv[j].c_str())); }
                m.SetRow(i, row);
            }
        }
    }
};

axis operator*(gm44d m, axis t);

std::vector<axis> operator*(gm44d mar, std::vector<axis> p);

enum ELs
{
    EL_XYZ,
    EL_XZY,
    EL_YXZ,
    EL_YZX,
    EL_ZXY,
    EL_ZYX
};

gm44d getm(/**三个角度*/ p3d ag, /*位移**/ p3d t = p3d(0, 0, 0), int turn = EL_YXZ);

gm44d getm(/**三个角度*/ double gm, double st, double fi, p3d t = p3d(0, 0, 0), int turn = EL_YXZ);

struct pt4d
{
    double x, y, z, a;

    pt4d() { x = y = z = 0; }

    pt4d(double x_, double y_, double z_, double a_)
    {
        x = x_;
        y = y_;
        z = z_;
        a = a_;
    }

    pt4d operator-(const pt4d &r) const
    {
        pt4d tp;
        tp.x = x - r.x;
        tp.y = y - r.y;
        tp.z = z - r.z;
        tp.a = a - r.a;
        tp.x = tp.x > 0 ? tp.x : -tp.x;
        tp.y = tp.y > 0 ? tp.y : -tp.y;
        tp.z = tp.z > 0 ? tp.z : -tp.z;
        tp.a = tp.a > 0 ? tp.a : -tp.a;
        return tp;
    }

    double dnorm() { return sqr(x * x + y * y + z * z + a * a); }
};

struct fdot
{
    p3d o, x, y, z;

    fdot() { x = y = z = o = p3d(0, 0, 0); }

    fdot(p3d x_, p3d y_, p3d z_, p3d o_)
    {
        x = x_;
        y = y_;
        z = z_;
        o = o_;
    }

    /**通过变换前的4个点与变换后的4个点推算旋转矩阵*/
    gcc4d mkcvt(p3d a0, p3d b0, p3d c0, p3d m0, p3d a1, p3d b1, p3d c1, p3d m1,
                bool vectorOnRightU = false, bool vectorOnRightV = false)
    {
        gm44d U, V;
        gcc4d convert;
        U = gset(a1, b1, c1, m1);
        V = gset(a0, b0, c0, m0);
        convert(U, vectorOnRightU, V, vectorOnRightV);
        return convert;
    }

    /**坐标被乘变回原坐标系统*/
    gm44d operator<<(fdot src)
    {
        gcc4d cvt = mkcvt(src.x - src.o, src.y - src.o, src.z - src.o, src.o - src.o, x - o, y - o,
                          z - o, o - o);
        gm44d mo = gset(p3d(1, 0, 0), p3d(0, 1, 0), p3d(0, 0, 1), src.o - o);
        return cvt.GetC() * mo /**/;
    }

    /**坐标被乘变至目标坐标系统*/
    gm44d operator>>(fdot src)
    {
        gcc4d cvt = mkcvt(src.x - src.o, src.y - src.o, src.z - src.o, src.o - src.o, x - o, y - o,
                          z - o, o - o);
        gm44d mo = gset(p3d(1, 0, 0), p3d(0, 1, 0), p3d(0, 0, 1), src.o - o);
        return mo * cvt.GetC() /**/;
    }
};

struct fplane
{
    p3d o, x, y, z, n;
    double a, b, c, d;
    gm44d m;
    farea arx, arz;

    gm44d n_m()
    {
        m = getm(pola(n).p()) * getm(-90, 0, 0); /*getm(-n*M_PI);*/
        return m;
    }

    p3d normv()
    {
        n = (x - o).cross(z - o);
        return n / dnorm(n);
    }

    p3d normv(p3d p1, p3d p2, p3d p3)
    {
        d = 0 - (a * p1.x + b * p1.y + c * p1.z);
        n = (p3 - p1).cross((p2 - p1));
        return n / dnorm(n);
    }

    gm44d in()
    { // ok
        return fdot(x, y, z, o) << fdot(p3d(dnorm(x - o), 0, 0), p3d(0, dnorm(y - o), 0),
                                        p3d(0, 0, dnorm(z - o)), p3d(0, 0, 0));
    }

    gm44d out()
    { // ok
        return fdot(p3d(dnorm(x - o), 0, 0), p3d(0, dnorm(y - o), 0), p3d(0, 0, dnorm(z - o)),
                    p3d(0, 0, 0)) >>
               fdot(x, y, z, o);
    }

    fplane() { o = x = y = z = p3d(0, 0, 0); }

    fplane(p3d o_, p3d x_, p3d y_, p3d z_)
    {
        o = o_;
        x = x_;
        y = y_;
        z = z_;
        n = normv(o, x, z);
        a = n.x;
        b = n.y;
        c = n.z;
        n_m();
    }

    fplane(p3d p1, p3d p2, p3d p3)
    {
        o = p1;
        n = normv(p1, p2, p3);
    }

    fplane(p3d o_, p3d n_)
    {
        o = o_;
        if (n_ == p3d(0, 0, 0)) return;
        n = n_ / dnorm(n_);
        n_m();
        x = o + m * p3d(200 * mm, 0, 0);
        y = o + m * p3d(0, 200 * mm, 0);
        z = o + m * p3d(0, 0, 200 * mm);
    }

    fplane(p3d pt, p3d nx_, p3d ny_, p3d nz_, double r, farea arx_, farea arz_)
    {
        n = ny_;
        o = pt + n * r;
        arx = arx_;
        arz = arz_;
        x = o + nx_ * 200 * mm; //三轴要等长
        y = o + ny_ * 200 * mm;
        z = o + nz_ * 200 * mm;
        m = n_m();
    }

    fplane(p3d pt, p3d n_, double r)
    {
        n = n_ / dnorm(n_);
        o = pt + n * r;
        n_m();
        x = o + m * p3d(200 * mm, 0, 0);
        y = o + m * p3d(0, 200 * mm, 0);
        z = o + m * p3d(0, 0, 200 * mm);
    }

    std::vector<axis> lines(scalar cl = scalar(0, 255, 0), farea s = farea(-180, 180))
    {
        std::vector<axis> rt;
        double r = 200 * mm;
        p3d a = m * pola(0, s.a[0], r).e() + o, b;
        for (double st = s.a[0]; st <= s.b[0]; st += 45)
        {
            b = m * pola(0, st, r).e() + o;
            rt.push_back(axis(a, b, 0, cl));
            a = b;
        }
        b = m * pola(0, s.b[0], r).e() + o;
        rt.push_back(axis(a, b, 0, cl));
        return rt;
    }

    std::vector<axis> linesa(scalar cl = fc_green, farea s = farea(-180, 180))
    {
        std::vector<axis> rt = lines(cl, s);
        rt.push_back(axis(o, x, 0, scalar(0, 0, 255)));
        rt.push_back(axis(o, y, 0, scalar(0, 255, 255)));
        rt.push_back(axis(o, z, 0, scalar(255, 0, 0)));
        rt.push_back(axis(o, o + n * 200 * mm * 1.2, 0, scalar(255, 0, 255)));
        return rt;
    }

    double dist(p3d pt)
    {
        double t = (pt - o).dot(n) / dnorm(n);
        return t;
    }

    p3d foot(p3d pt) { return pt - dist(pt) * n; }

    fplane operator*(gm44d mt)
    {
        fplane rt = *this;
        rt.x = mt * x;
        rt.y = mt * y;
        rt.z = mt * z;
        rt.n = mt * n;
        rt.m = mt * m;

        rt.a = n.x;
        rt.b = n.y;
        rt.c = n.z;
        return rt;
    }

    /**平面与直线相交的点*/
    p3d operator&(axis l)
    {
        axis q(l.a, foot(l.a));
        double cst = l.v().dot(q.v()) / (l.d() * q.d()), // cosθ=向量a.向量b/|向量a|×|向量b|
            ap = q.d() / cst;
        return l.a + ap * l.n();
    }

    /**面片与线段相交*/
    bool operator&(std::vector<axis> l)
    {
        if (arx.a.empty() || arz.a.empty()) return 0;
        for (uint i = 0; i < l.size(); i++)
        {
            p3d rt = *this & l[i];
            if (dist(l[i].a) * dist(l[i].b) <= 0)
            {
                p3d rt1 = in() * rt;
                if (hav(rt1.x, arx.a[0], arx.b[0]) && hav(rt1.z, arz.a[0], arz.b[0])) { return 1; }
            }
        }
        return 0;
    }

    /**平面与平面相交的直线*/
    axis operator&(fplane p)
    {
        p3d rtn = n.cross(p.n), rna = rtn.cross(n), rnb = rtn.cross(p.n),
            rto = axis(o, o + rna) * axis(p.o, p.o + rnb);
        return axis(rto - 200 * mm * rtn, rto + 200 * mm * rtn);
    }
};

fplane operator*(gm44d mt, fplane c);

/*xz圆**/
struct fcircle
{
    p3d o, x, y, z, n;
    double r;
    gm44d m;

    void n_m() { m = getm(pola(n).p()) * getm(-90, 0, 0); }

    fcircle() {}

    fcircle(p3d o_, p3d x_, p3d y_, p3d z_)
    {
        o = o_;
        x = x_;
        y = y_;
        z = z_;
        n = y / dnorm(y);
        r = dnorm(z - o);
    }

    /**圆心 半径 法向量*/
    fcircle(p3d o_, double r_, p3d n_)
    {
        o = o_;
        r = r_;
        n = n_ / dnorm(n_);
        n_m();
        x = o + m * p3d(r, 0, 0);
        y = o + m * p3d(0, r, 0);
        z = o + m * p3d(0, 0, r);
    }

    fcircle(p3d pt, double d_, p3d n_, double r_)
    {
        n = n_ / dnorm(n_);
        r = r_;
        o = pt + n * d_;
        n_m();
        x = o + m * p3d(r, 0, 0);
        y = o + m * p3d(0, r, 0);
        z = o + m * p3d(0, 0, r);
    }

    /**半径*/
    double getr()
    {
        r = dnorm(o, z);
        return r;
    }

    /**法向量*/
    p3d normv() { return (x - o).cross(y - o); }

    fplane plane() { return fplane(o, n); }

    gm44d in()
    {
        return fdot(x, y, z, o) << fdot(p3d(r, 0, 0), p3d(0, r, 0), p3d(0, 0, r), p3d(0, 0, 0));
    }

    gm44d out()
    {
        return fdot(p3d(r, 0, 0), p3d(0, r, 0), p3d(0, 0, r), p3d(0, 0, 0)) >> fdot(x, y, z, o);
    }

    std::vector<axis> lines(scalar cl = scalar(0, 255, 0), farea s = farea(-180, 180),
                            double ofs = +0)
    {
        std::vector<axis> rt;
        for (uint i = 0; i < s.a.size(); i++)
        {
            p3d a = m * pola(0, s.a[i], r + ofs).e() + o, b;
            for (double st = s.a[i]; st <= s.b[i]; st += 9)
            {
                b = m * pola(0, st, r + ofs).e() + o;
                rt.push_back(axis(a, b, 0, cl));
                a = b;
            }
            b = m * pola(0, s.b[i], r + ofs).e() + o;
            rt.push_back(axis(a, b, 0, cl));
        }
        return rt;
    }

    std::vector<axis> linesa(scalar cl = scalar(0, 255, 0), farea s = farea(-180, 180))
    {
        std::vector<axis> rt = lines(cl, s);
        rt.push_back(axis(o, x, 0, scalar(0, 0, 255)));
        rt.push_back(axis(o, y, 0, scalar(0, 255, 255)));
        rt.push_back(axis(o, z, 0, scalar(255, 0, 0)));
        rt.push_back(axis(o, o + n * r * 1.2, 0, scalar(255, 0, 255)));
        return rt;
    }

    fcircle operator+(p3d o_)
    {
        fcircle rt = *this;
        rt.o = o + o_;
        rt.x = x + o_;
        rt.y = y + o_;
        rt.z = z + o_;
        return rt;
    }

    /**圆与直线交点*/
    std::vector<p3d> operator&(axis b)
    {
        std::vector<p3d> rt;
        double d = b.dst(o); //圆心到直线的距离
        if (d > r) return rt;
        double u = sqr(r * r - d * d); //垂足到交点的距离
        p3d foot = b.foot(o);
        rt.push_back(foot - u * b.e);
        rt.push_back(foot + u * b.e);
        return rt;
    }

    /**圆与平面交点*/
    std::vector<p3d> operator&(fplane b)
    {
        if (n == p3d(0, 0, 0)) return std::vector<p3d>();
        return *this & (plane() & b);
    }

    /**同平面上两圆相交的圆*/
    fcircle operator&(fcircle b)
    {
        axis ab(o, b.o);
        p3d e = ab.n();
        double d = ab.d();
        if (d > r + b.r) return fcircle();
        double p = (r + b.r + d) / 2, s = sqr(p * (p - r) * (p - b.r) * (p - d)), po = 2 * s / d,
               apr = Asin(po, r), cpr = Cos(apr), nAO = cpr * r;
        p3d AO = nAO * e;
        return fcircle(AO + o, po, e);
    }
};

fcircle operator*(gm44d mt, fcircle c);

/**圆与平面交点*/
std::vector<p3d> operator&(fplane a, fcircle b);

farea shave(fplane a, fcircle b);

/**球体*/
struct fball
{
    p3d o, x, y, z, n;
    double r;
    gm44d m;

    void n_m() { m = getm(pola(n).p()) * getm(-90, 0, 0); /*对齐实际法向量z 旋转至y*/ }

    fball() { o = x = y = z = p3d(0, 0, 0); }

    fball(p3d o_, double r_)
    {
        o = o_;
        r = r_;
        x = o + p3d(r, 0, 0);
        y = o + p3d(0, r, 0);
        z = o + p3d(0, 0, r);
        n = y / dnorm(y);
        m = getm(0, 0, 0);
    }

    /**球心 半径 法向量*/
    fball(p3d o_, double r_, p3d n_)
    {
        o = o_;
        r = r_;
        n = n_ / dnorm(n_);
        n_m();
        x = o + m * p3d(r, 0, 0);
        y = o + m * p3d(0, r, 0);
        z = o + m * p3d(0, 0, r);
    }

    fball(p3d o_, p3d x_, p3d y_, p3d z_)
    {
        o = o_;
        x = x_;
        y = y_;
        z = z_;
        r = dnorm(z - o);
        n = y / dnorm(y);
        m = getm(0, 0, 0);
    }

    double getr()
    {
        r = dnorm(o, x);
        return r;
    }

    void n_()
    {
        p3d n_((x - o).cross(y - o));
        n = n_ / dnorm(n_);
    }

    std::vector<axis> lines(scalar cl = scalar(0, 255, 0), farea g = farea(-90, 90),
                            farea s = farea(-180, 180))
    {
        std::vector<axis> rt;
        for (uint i = 0; i < s.a.size(); i++)
        {
            p3d a = m * pola(g.a[i], s.a[i], r).e() + o, b;
            for (double gm = g.a[i]; gm <= g.b[i]; gm += 9)
            {
                a = m * pola(gm, s.a[i], r).e() + o;
                for (double st = s.a[i]; st <= s.b[i]; st += 9)
                {
                    b = m * pola(gm, st, r).e() + o;
                    rt.push_back(axis(a, b, 0, cl));
                    a = b;
                }
                b = m * pola(gm, s.b[i], r).e() + o;
                rt.push_back(axis(a, b, 0, cl));
            }
            for (double st = s.a[i]; st <= s.b[i]; st += 9)
            {
                a = m * pola(g.a[i], st, r).e() + o;
                for (double gm = g.a[i]; gm <= g.b[i]; gm += 9)
                {
                    b = m * pola(gm, st, r).e() + o;
                    rt.push_back(axis(a, b, 0, cl));
                    a = b;
                }
                b = m * pola(g.b[i], st, r).e() + o;
                rt.push_back(axis(a, b, 0, cl));
            }
        }
        return rt;
    }

    std::vector<axis> linesa(scalar cl = scalar(0, 255, 0), farea g = farea(-90, 90),
                             farea s = farea(-180, 180))
    {
        std::vector<axis> rt = lines(cl, g, s);
        rt.push_back(axis(o, x, 0, scalar(0, 0, 255)));
        rt.push_back(axis(o, y, 0, scalar(0, 255, 255)));
        rt.push_back(axis(o, z, 0, scalar(255, 0, 0)));
        rt.push_back(axis(o, o + n * r * 1.2, 0, scalar(255, 0, 255)));
        return rt;
    }

    bool contain(p3d p) { return dnorm(p - o) < r; }

    /**与平面相交的圆*/
    fcircle operator&(fplane p)
    {
        getr();
        double d = (o - p.o).dot(p.n); //球心到平面的距离 n为单位法向量
        p3d o2(o - d * p.n);           //沿球心反向移动d个n到达垂足
        return fcircle(o2, sqr(r * r - d * d), p.n);
    }

    /**与圆相交点 顺序不能错*/
    std::vector<p3d> operator&(fcircle c)
    {
#if 0
                                                                                                                                fcircle s1=*this*c.plan(),
                s2=s1*c;
        return s2*c.plan();
#endif
        return *this & c.plane() & c & c.plane();
    }

    /**与球相交的圆*/
    fcircle operator&(fball b)
    {
        axis ab(o, b.o);
        p3d e = ab.n();
        double d = ab.d(), p = (r + b.r + d) / 2, s2 = p * (p - r) * (p - b.r) * (p - d);
        if (s2 < 0) return fcircle();
        double s = sqr(s2), po = 2 * s / d, cpr = Cos(Asin(po, r));
        double nAO = cpr * r;

        p3d AO = nAO * e;
        return fcircle(o + AO, po, e);
    }
};

fball operator*(gm44d mt, fball b);

farea shave(fball a, fcircle b);

struct ddot
{
    p3d e;
    scalar cl;
    int sz;

    ddot()
    {
        e = p3d(0, 0, 0);
        cl = scalar(0, 0, 0);
    }

    ddot(p3d e_, scalar cl_, int sz_ = 1)
    {
        e = e_;
        cl = cl_;
        sz = sz_;
    }
};

struct conr
{
    /**维1:左右两面 维2:下上两面 维3:后前两面 维4:3爪1心 */
    p3d pt[2][2][2][4];

    std::vector<axis> lines(scalar cl = scalar(0, 255, 0))
    {
        std::vector<axis> rt;
        rt.push_back(axis(pt[0][0][0][0], pt[1][0][0][0], 0, cl)); //横
        rt.push_back(axis(pt[0][0][1][0], pt[1][0][1][0], 0, cl));
        rt.push_back(axis(pt[0][1][0][0], pt[1][1][0][0], 0, cl));
        rt.push_back(axis(pt[0][1][1][0], pt[1][1][1][0], 0, cl));

        rt.push_back(axis(pt[0][0][0][0], pt[0][1][0][0], 0, cl)); //竖
        rt.push_back(axis(pt[0][0][1][0], pt[0][1][1][0], 0, cl));
        rt.push_back(axis(pt[1][0][0][0], pt[1][1][0][0], 0, cl));
        rt.push_back(axis(pt[1][0][1][0], pt[1][1][1][0], 0, cl));

        rt.push_back(axis(pt[0][0][0][0], pt[0][0][1][0], 0, cl)); //前后
        rt.push_back(axis(pt[0][1][0][0], pt[0][1][1][0], 0, cl));
        rt.push_back(axis(pt[1][0][0][0], pt[1][0][1][0], 0, cl));
        rt.push_back(axis(pt[1][1][0][0], pt[1][1][1][0], 0, cl));

        return rt;
    }

    std::vector<axis> linesb(scalar cl = scalar(128, 128, 128))
    {
        std::vector<axis> rt;
        rt.push_back(axis(pt[1][1][1][0], pt[1][1][1][1], 0, cl));
        rt.push_back(axis(pt[1][1][0][0], pt[1][1][0][1], 0, cl));
        rt.push_back(axis(pt[1][0][1][0], pt[1][0][1][1], 0, cl));
        rt.push_back(axis(pt[1][0][0][0], pt[1][0][0][1], 0, cl));

        rt.push_back(axis(pt[0][1][1][0], pt[0][1][1][1], 0, cl));
        rt.push_back(axis(pt[0][1][0][0], pt[0][1][0][1], 0, cl));
        rt.push_back(axis(pt[0][0][1][0], pt[0][0][1][1], 0, cl));
        rt.push_back(axis(pt[0][0][0][0], pt[0][0][0][1], 0, cl));

        rt.push_back(axis(pt[1][1][1][0], pt[1][1][1][2], 0, cl));
        rt.push_back(axis(pt[1][1][0][0], pt[1][1][0][2], 0, cl));
        rt.push_back(axis(pt[1][0][1][0], pt[1][0][1][2], 0, cl));
        rt.push_back(axis(pt[1][0][0][0], pt[1][0][0][2], 0, cl));

        rt.push_back(axis(pt[0][1][1][0], pt[0][1][1][2], 0, cl));
        rt.push_back(axis(pt[0][1][0][0], pt[0][1][0][2], 0, cl));
        rt.push_back(axis(pt[0][0][1][0], pt[0][0][1][2], 0, cl));
        rt.push_back(axis(pt[0][0][0][0], pt[0][0][0][2], 0, cl));

        rt.push_back(axis(pt[1][1][1][0], pt[1][1][1][3], 0, cl));
        rt.push_back(axis(pt[1][1][0][0], pt[1][1][0][3], 0, cl));
        rt.push_back(axis(pt[1][0][1][0], pt[1][0][1][3], 0, cl));
        rt.push_back(axis(pt[1][0][0][0], pt[1][0][0][3], 0, cl));

        rt.push_back(axis(pt[0][1][1][0], pt[0][1][1][3], 0, cl));
        rt.push_back(axis(pt[0][1][0][0], pt[0][1][0][3], 0, cl));
        rt.push_back(axis(pt[0][0][1][0], pt[0][0][1][3], 0, cl));
        rt.push_back(axis(pt[0][0][0][0], pt[0][0][0][3], 0, cl));

        return rt;
    }
};

conr operator*(gm44d &m, conr c);

struct vver
{
    p3d Po, P;
    p2d wd;
};

extern int sbsa, sbsb;

//(lsz.width/ewdmm+lsz.width/ehtmm)/2,
// const  _Float128 pi2 =_Float128(180.0);
// const  _Float128 agl2=_Float128(180.0)/_Float128(M_PI);

struct vr
{
    p2d lp, //
        rp, //
        lsz, rsz;
    scalar cl;

    vr()
    {
        lp = p2d(0);
        rp = p2d(0);
        lsz = p2d(0);
        rsz = p2d(0);
        cl = scalar(0);
    }

    vr(p2d lp_, p2d rp_)
    {
        lp = lp_;
        rp = rp_;
    }

    /*
        vr (p3d e){
            double k=u(fyml.fl,e.z);
            lp.x=int(k*e.x);
            lp.y=int(k*e.y);
            rp.x=int(k*(e.x+fyml.c2));
            rp.y=lp.y;
        }
    */
    vr(p3d e)
    {
        double kl = u(fyml.fl, e.z), kr = u(fyml.fr, e.z);
        lp.x = kl * (e.x + fyml.c);
        lp.y = kl * e.y;
        rp.x = kr * (e.x - fyml.c);
        rp.y = kr * e.y;
    }

    vr(bipolar b)
    {
        lp = p2d(fyml.fl / Tan(b.ap - fyml.ap), Tan(b.gm) * fyml.fl);
        rp = p2d(fyml.fr / Tan(b.bt - fyml.bt), Tan(b.gm) * fyml.fr);
    }

    /**中心为原点*/
    vr vo()
    {
        vr v;
        v.lp = lp - fyml.l_vc_lp;
        v.rp = rp - fyml.l_vc_rp;
        v.lsz = lsz;
        v.rsz = rsz;
        v.cl = cl;
        return (v);
    }

    /**
     * @brief 光轴平行的理想情况的简单计算
     * @return 该点的三维坐标
     */
    p3d e()
    {
        double xx = lp.x - rp.x, k = u(fyml.c2, xx);
        return p3d(lp.x * k - fyml.c, lp.y * k, fyml.fl * k);
    }

    /**
     * @brief 光轴不平行的情况
     */
    bipolar b()
    {
        return bipolar(Atan(lp.x, fyml.fl) + fyml.ap, Atan(rp.x, fyml.fl) + fyml.bt,
                       Atan(lp.y, fyml.fl));
    }

    p3d e2() { return b().e(); }

    p3d voe() { return vo().e(); }

    /**左上角为原点*/
    vr vo_()
    {
        vr v = *this;
        v.lp = lp + fyml.l_vc_lp;
        v.rp = rp + fyml.l_vc_rp;
        return (v);
    }

    /**垂直翻转*/
    vr f()
    {
        vr f_;
        f_.lp.x = lp.x;
        f_.rp.x = rp.x;
        f_.lp.y = fyml.eht - lp.y;
        f_.rp.y = fyml.eht - rp.y;
        return f_;
    }

    p3d fvoe() { return f().voe(); }
};

/**翻转2维坐标系 */
vr flp(vr v, int eht);

/**物体 */
struct obj
{
    /**欧氏坐标*/
    p3d o, x, y, z, e, eb, /**尺寸   */
        sz,                /**转角   */
        ag;
    /**编号   */
    int id, /**源号   */
        l,  /**源号   */
        l1, l2;
    uint l3, l4;

    axis d[5];
    /**24个角点*/
    conr c;

    bipolar b;

    pola p_;

    vr v;

    scalar cl;
    /**形状 0:长方体 1:圆形 2:半圆形 */
    int sp;
    int sp2;
    /**匹配度   */
    float mcl, /**匹配度   */
        mcr;

    gm44d m;
    string name;

    float mc() { return (mcl + mcr) / 2; }

    /**从欧氏坐标计算极坐标*/
    pola p()
    {
        p_ = pola(e);
        return p_;
    }

    /**从vr计算欧氏坐标*/
    void v2e() { e = v.f().voe(); }

    void e2v() { v = vr(e); }

    /**移动指定距离*/
    obj operator+(p3d ntag)
    {

        p3d tag = ntag;

        e = e + tag;
        o = o + tag;
        x = x + tag;
        y = y + tag;
        z = z + tag;

        c.pt[0][1][1][0] = c.pt[0][1][1][0] + tag;
        c.pt[0][1][0][0] = c.pt[0][1][0][0] + tag;
        c.pt[0][0][1][0] = c.pt[0][0][1][0] + tag;
        c.pt[0][0][0][0] = c.pt[0][0][0][0] + tag;

        c.pt[1][1][1][0] = c.pt[1][1][1][0] + tag;
        c.pt[1][1][0][0] = c.pt[1][1][0][0] + tag;
        c.pt[1][0][1][0] = c.pt[1][0][1][0] + tag;
        c.pt[1][0][0][0] = c.pt[1][0][0][0] + tag;

        c.pt[0][1][1][1] = c.pt[0][1][1][1] + tag;
        c.pt[0][1][0][1] = c.pt[0][1][0][1] + tag;
        c.pt[0][0][1][1] = c.pt[0][0][1][1] + tag;
        c.pt[0][0][0][1] = c.pt[0][0][0][1] + tag;

        c.pt[1][1][1][1] = c.pt[1][1][1][1] + tag;
        c.pt[1][1][0][1] = c.pt[1][1][0][1] + tag;
        c.pt[1][0][1][1] = c.pt[1][0][1][1] + tag;
        c.pt[1][0][0][1] = c.pt[1][0][0][1] + tag;

        c.pt[0][1][1][2] = c.pt[0][1][1][2] + tag;
        c.pt[0][1][0][2] = c.pt[0][1][0][2] + tag;
        c.pt[0][0][1][2] = c.pt[0][0][1][2] + tag;
        c.pt[0][0][0][2] = c.pt[0][0][0][2] + tag;

        c.pt[1][1][1][2] = c.pt[1][1][1][2] + tag;
        c.pt[1][1][0][2] = c.pt[1][1][0][2] + tag;
        c.pt[1][0][1][2] = c.pt[1][0][1][2] + tag;
        c.pt[1][0][0][2] = c.pt[1][0][0][2] + tag;

        c.pt[0][1][1][3] = c.pt[0][1][1][3] + tag;
        c.pt[0][1][0][3] = c.pt[0][1][0][3] + tag;
        c.pt[0][0][1][3] = c.pt[0][0][1][3] + tag;
        c.pt[0][0][0][3] = c.pt[0][0][0][3] + tag;

        c.pt[1][1][1][3] = c.pt[1][1][1][3] + tag;
        c.pt[1][1][0][3] = c.pt[1][1][0][3] + tag;
        c.pt[1][0][1][3] = c.pt[1][0][1][3] + tag;
        c.pt[1][0][0][3] = c.pt[1][0][0][3] + tag;

        d[0].a = d[0].a + tag;
        d[0].b = d[0].b + tag;
        d[1].a = d[1].a + tag;
        d[1].b = d[1].b + tag;
        d[2].a = d[2].a + tag;
        d[2].b = d[2].b + tag;
        d[3].a = d[3].a + tag;
        d[3].b = d[3].b + tag;
        d[4].a = d[4].a + tag;
        d[4].b = d[4].b + tag;
        return *this;
    }

    /**根据ag调整姿态 */
    void d3()
    {
        axis X(e + p3d(1, 0, 0), e + p3d(-1, 0, 0), ag.x);
        X.getM();
        axis Y(e + p3d(0, 1, 0), e + p3d(0, -1, 0), ag.y);
        Y.getM();
        axis Z(e + p3d(0, 0, 1), e + p3d(0, 0, -1), ag.z);
        Z.getM();
        m = X.m * Y.m * Z.m;
        c = m * c;
        x = vx();
        y = vy();
        z = vz();
    }

    /**初始化为24个点 */
    void box(p3d sz_, double f)
    {
        p3d p = e;
        double dx = sz_.x / 2, dy = sz_.y / 2, dz = sz_.z / 2;

        p3d p3[2][2][2];
        p3d p4[2][2][2];

        p3[1][1][1] = p3d(+dx, +dy, +dz);
        p3[1][1][0] = p3d(+dx, +dy, -dz);
        p3[1][0][1] = p3d(+dx, -dy, +dz);
        p3[1][0][0] = p3d(+dx, -dy, -dz);

        p3[0][1][1] = p3d(-dx, +dy, +dz);
        p3[0][1][0] = p3d(-dx, +dy, -dz);
        p3[0][0][1] = p3d(-dx, -dy, +dz);
        p3[0][0][0] = p3d(-dx, -dy, -dz); //尺寸

        p4[0][1][1] = p3d(-f, -0, -0);
        p4[1][0][1] = p3d(-0, -f, -0);
        p4[1][1][0] = p3d(-0, -0, -f);

        p4[1][0][0] = p3d(+f, +0, +0);
        p4[0][1][0] = p3d(+0, +f, +0);
        p4[0][0][1] = p3d(+0, +0, +f); //长短

        c.pt[1][1][1][0] = p + p3[1][1][1];
        c.pt[1][1][0][0] = p + p3[1][1][0];
        c.pt[1][0][1][0] = p + p3[1][0][1];
        c.pt[1][0][0][0] = p + p3[1][0][0];

        c.pt[0][1][1][0] = p + p3[0][1][1];
        c.pt[0][1][0][0] = p + p3[0][1][0];
        c.pt[0][0][1][0] = p + p3[0][0][1];
        c.pt[0][0][0][0] = p + p3[0][0][0]; //八个顶点

        c.pt[1][1][1][1] = p + p3[1][1][1] + p4[0][1][1];
        c.pt[1][1][0][1] = p + p3[1][1][0] + p4[0][1][1];
        c.pt[1][0][1][1] = p + p3[1][0][1] + p4[0][1][1];
        c.pt[1][0][0][1] = p + p3[1][0][0] + p4[0][1][1];

        c.pt[0][1][1][1] = p + p3[0][1][1] + p4[1][0][0];
        c.pt[0][1][0][1] = p + p3[0][1][0] + p4[1][0][0];
        c.pt[0][0][1][1] = p + p3[0][0][1] + p4[1][0][0];
        c.pt[0][0][0][1] = p + p3[0][0][0] + p4[1][0][0];

        c.pt[1][1][1][2] = p + p3[1][1][1] + p4[1][0][1];
        c.pt[1][1][0][2] = p + p3[1][1][0] + p4[1][0][1];
        c.pt[1][0][1][2] = p + p3[1][0][1] + p4[0][1][0];
        c.pt[1][0][0][2] = p + p3[1][0][0] + p4[0][1][0];

        c.pt[0][1][1][2] = p + p3[0][1][1] + p4[1][0][1];
        c.pt[0][1][0][2] = p + p3[0][1][0] + p4[1][0][1];
        c.pt[0][0][1][2] = p + p3[0][0][1] + p4[0][1][0];
        c.pt[0][0][0][2] = p + p3[0][0][0] + p4[0][1][0];

        c.pt[1][1][1][3] = p + p3[1][1][1] + p4[1][1][0];
        c.pt[1][1][0][3] = p + p3[1][1][0] + p4[0][0][1];
        c.pt[1][0][1][3] = p + p3[1][0][1] + p4[1][1][0];
        c.pt[1][0][0][3] = p + p3[1][0][0] + p4[0][0][1];

        c.pt[0][1][1][3] = p + p3[0][1][1] + p4[1][1][0];
        c.pt[0][1][0][3] = p + p3[0][1][0] + p4[0][0][1];
        c.pt[0][0][1][3] = p + p3[0][0][1] + p4[1][1][0];
        c.pt[0][0][0][3] = p + p3[0][0][0] + p4[0][0][1]; //三个朝向

        d3();
    }

    void box(double f) { box(sz, f); }

    obj()
    {
        double x_ = 0 * mm, y_ = 0 * mm, z_ = 0 * mm, w_ = 1 * mm, h_ = 1 * mm, d_ = 1 * mm,
               u_ = 0 * mm, v_ = 0 * mm, m_ = 0 * mm, f_ = 5 * mm;
        scalar cl_ = scalar(0);
        e = p3d(x_, y_, z_);
        sz = p3d(w_, h_, d_);
        ag = p3d(u_, v_, m_);
        cl = cl_;
        o = p3d(0, 0, 0);
        x = p3d(1, 0, 0);
        y = p3d(0, 1, 0);
        z = p3d(0, 0, 1);
        box(sz, f_);
    }

    obj(double x_, double y_, double z_, double w_ = 0 * mm, double h_ = 0 * mm, double d_ = 0 * mm,
        double u_ = 0, double v_ = 0, double m_ = 0, scalar cl_ = scalar(255, 0),
        double f_ = 5 * mm)
    {
        e = p3d(x_, y_, z_);
        sz = p3d(w_, h_, d_);
        ag = p3d(u_, v_, m_);
        cl = cl_;
        box(sz, f_);
    }

    obj(p3d e_, p3d sz_ = p3d(0 * mm, 0 * mm, 0 * mm), p3d ag_ = p3d(0, 0, 0),
        scalar cl_ = scalar(255, 0), double f_ = 5 * mm)
    {
        e = e_;
        sz = sz_;
        ag = ag_;
        cl = cl_;
        box(sz, f_);
    }

    /**坐标被乘变回原坐标系统*/
    gm44d operator<<(obj &src) { return fdot(x, y, z, o) << fdot(src.x, src.y, src.z, src.o); }

    /**坐标被乘变至目标坐标系统*/
    gm44d operator>>(obj &src) { return fdot(src.x, src.y, src.z, src.o) >> fdot(x, y, z, o); }

    /*x向单位向量**/
    p3d vx() { return p3d(axis(c.pt[0][0][0][0], c.pt[1][0][0][0]).n()); }

    /*y向单位向量**/
    p3d vy() { return p3d(axis(c.pt[0][0][0][0], c.pt[0][1][0][0]).n()); }

    /*z向单位向量**/
    p3d vz() { return p3d(axis(c.pt[0][0][0][0], c.pt[0][0][1][0]).n()); }

    fplane ls()
    {
        return fplane(e, vz(), -vx(), -vy(), sz.x / 2, farea(-sz.z / 2, sz.z / 2),
                      farea(-sz.y / 2, sz.y / 2));
    }

    fplane rs()
    {
        return fplane(e, -vz(), vx(), -vy(), sz.x / 2, farea(-sz.z / 2, sz.z / 2),
                      farea(-sz.y / 2, sz.y / 2));
    }

    fplane ds()
    {
        return fplane(e, vx(), -vy(), -vz(), sz.y / 2, farea(-sz.x / 2, sz.x / 2),
                      farea(-sz.z / 2, sz.z / 2));
    }

    fplane us()
    {
        return fplane(e, vx(), vy(), vz(), sz.y / 2, farea(-sz.x / 2, sz.x / 2),
                      farea(-sz.z / 2, sz.z / 2));
    }

    fplane bs()
    {
        return fplane(e, -vx(), -vz(), -vy(), sz.z / 2, farea(-sz.x / 2, sz.x / 2),
                      farea(-sz.y / 2, sz.y / 2));
    }

    fplane fs()
    {
        return fplane(e, vx(), vz(), -vy(), sz.z / 2, farea(-sz.x / 2, sz.x / 2),
                      farea(-sz.y / 2, sz.y / 2));
    }

    // pt在表面以内
    bool operator&(p3d pt)
    {
        return ls().dist(pt) <= 0 && //长度是负数在背面
               rs().dist(pt) <= 0 && ds().dist(pt) <= 0 && us().dist(pt) <= 0 &&
               bs().dist(pt) <= 0 && fs().dist(pt) <= 0;
    }

    //被ot穿透表面
    bool operator&(obj ot)
    {
        fplane lsp = ls(), rsp = rs(), dsp = ds(), usp = us(), bsp = bs(), fsp = fs();
        std::vector<axis> lss = ot.c.lines();
        return ls() & lss || rs() & lss || ds() & lss || us() & lss || bs() & lss || fs() & lss;
    }

    gm44d in()
    {
        return fdot(x, y, z, o) << fdot(p3d(1, 0, 0), p3d(0, 1, 0), p3d(0, 0, 1), p3d(0, 0, 0));
    }

    gm44d out()
    {
        return fdot(p3d(1, 0, 0), p3d(0, 1, 0), p3d(0, 0, 1), p3d(0, 0, 0)) >> fdot(x, y, z, o);
    }

    std::vector<axis> linesb(scalar cl)
    {
        std::vector<axis> rt = c.linesb(cl);

        d[0].ca = scalar(0, 0, 255);
        d[1].ca = scalar(0, 255, 255);
        d[2].ca = scalar(255, 0, 0);
        d[3].ca = scalar(0, 255, 0);
        d[4].ca = scalar(255, 255, 255);

        rt.push_back(d[0]);
        rt.push_back(d[1]);
        rt.push_back(d[2]);
        rt.push_back(d[3]);
        rt.push_back(d[4]);

        return rt;
    }

    void _save(cv::FileStorage &fs, string nm)
    {
        fs << nm + "_e" << e;
        fs << nm + "_eb" << eb;
        fs << nm + "_sz" << sz;
        fs << nm + "_ag" << ag;
        fs << nm + "_id" << id;
        fs << nm + "_l" << l;
        fs << nm + "_l1" << l1;
        fs << nm + "_l2" << l2;
        for (int i = 0; i < 5; i++) { d[i]._save(fs, nm + "_d_" + str(i)); }

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    for (int l = 0; l < 2; l++)
                    {
                        fs << nm + "_c_pt_" + str(i) + "_" + str(j) + "_" + str(k) + "_" + str(l)
                           << l2;
                    }
                }
            }
        }

        fs << nm + "_b_ap" << b.ap;
        fs << nm + "_b_bt" << b.bt;
        fs << nm + "_b_gm" << b.gm;

        fs << nm + "_p__st" << p_.st;
        fs << nm + "_p__gm" << p_.gm;
        fs << nm + "_p__r" << p_.r;

        fs << nm + "_v_lp" << v.lp;
        fs << nm + "_v_rp" << v.rp;
        fs << nm + "_v_lsz" << v.lsz;
        fs << nm + "_v_rsz" << v.rsz;
        fs << nm + "_v_cl" << v.cl;

        fs << nm + "_cl" << cl;

        fs << nm + "_sp" << sp;
        fs << nm + "_sp2" << sp2;

        fs << nm + "_mcl" << mcl;
        fs << nm + "_mcr" << mcr;

        fs << nm + "_name" << name;
    }

    void _read(cv::FileStorage &fs, string nm)
    {
        fs[nm + "_e"] >> e;
        fs[nm + "_eb"] >> eb;
        fs[nm + "_sz"] >> sz;
        fs[nm + "_ag"] >> ag;
        fs[nm + "_id"] >> id;
        fs[nm + "_l"] >> l;
        fs[nm + "_l1"] >> l1;
        fs[nm + "_l2"] >> l2;
        for (int i = 0; i < 5; i++) { d[i]._read(fs, nm + "_d_" + str(i)); }
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    for (int l = 0; l < 2; l++)
                    {
                        fs[nm + "_c_pt_" + str(i) + "_" + str(j) + "_" + str(k) + "_" + str(l)] >>
                            l2;
                    }
                }
            }
        }
        fs[nm + "_b_ap"] >> b.ap;
        fs[nm + "_b_bt"] >> b.bt;
        fs[nm + "_b_gm"] >> b.gm;

        fs[nm + "_p__st"] >> p_.st;
        fs[nm + "_p__gm"] >> p_.gm;
        fs[nm + "_p__r"] >> p_.r;

        fs[nm + "_v_lp"] >> v.lp;
        fs[nm + "_v_rp"] >> v.rp;
        fs[nm + "_v_lsz"] >> v.lsz;
        fs[nm + "_v_rsz"] >> v.rsz;
        fs[nm + "_v_cl"] >> v.cl;

        fs[nm + "_cl"] >> cl;

        fs[nm + "_sp"] >> sp;
        fs[nm + "_sp2"] >> sp2;

        fs[nm + "_mcl"] >> mcl;
        fs[nm + "_mcr"] >> mcr;

        fs[nm + "_name"] >> name;
    }
};

obj operator*(gm44d &m, obj t);

/**随机颜色*/
scalar rnds();

scalar hsv(scalar &rgb);

struct color
{
    uchar r, g, b, a,
        /**色度  */ h,
        /**饱和度*/ s,
        /**明度  */ v,
        /**边缘类型*/ l;
    short /**已匹配*/ i;
    short /**已匹配*/ j;
    short t;

    color() {}

    color(scalar cl)
    {
        r = uchar(cl.val[2]);
        g = uchar(cl.val[1]);
        b = uchar(cl.val[0]);
        cl = hsv(cl);
        h = uchar(cl.val[0]);
        s = uchar(cl.val[1]);
        v = uchar(cl.val[2]);
    }

    void r0(uchar r_, uchar g_, uchar b_)
    {
        r = r_;
        g = g_;
        b = b_;
    }

    void h0(uchar h_, uchar s_, uchar v_)
    {
        h = h_;
        s = s_;
        v = v_;
    }

    void t0()
    {
        t = r + g + b;
        i = 0;
    }

    string name()
    {
        if (234 <= h && 43 <= s && 45 <= v)
            return ("红");
        else if (h <= 6 && 43 <= s && 45 <= v)
            return ("红");
        else if (7 <= h && h <= 33 && 43 <= s && 45 <= v)
            return ("橙");
        else if (34 <= h && h <= 54 && 43 <= s && 45 <= v)
            return ("黄");
        else if (55 <= h && h <= 105 && 43 <= s && 45 <= v)
            return ("绿");
        else if (106 <= h && h <= 136 && 43 <= s && 45 <= v)
            return ("青");
        else if (137 <= h && h <= 176 && 43 <= s && 45 <= v)
            return ("蓝");
        else if (177 <= h && h <= 233 && 43 <= s && 45 <= v)
            return ("紫");
        else if (s <= 43 && 45 <= v && v <= 190)
            return ("灰");
        else if (191 <= v)
            return ("白");
        else if (v < 45)
            return ("黑");
        else
            return ("??");
    }

    double r0() { return r / t; }

    double g0() { return g / t; }

    double b0() { return b / t; }

    pt4d rgb0()
    {
        pt4d tp;
        if (t == 0) return tp;
        tp.x = r0();
        tp.y = g0();
        tp.z = b0();
        tp.a = t;
        return tp;
    }

    bool like(pt4d rgb1, double u = 0.07, double v = 45)
    {
        pt4d rgb;
        rgb = rgb0() - rgb1;
        double tp = 0;
        tp = tp > rgb.x ? tp : rgb.x;
        tp = tp > rgb.y ? tp : rgb.y;
        tp = tp > rgb.z ? tp : rgb.z;

        return (tp < u && rgb.a < v);
    }

    double dnorm() { return sqr(r * r + g * g + b * b); }
};

/**求色彩相似度*/
bool sbs(scalar c1, scalar c2);

bool sms(scalar c1, scalar c2);

bool smnm(scalar c1, scalar c2);

struct fyline
{
    cv::Vec4d l;
    double ag_ = 0, len_ = 0, k_ = 0, b_ = 0, C_ = 0, d_ = 0;
    /**进栈*/
    int p = 0, p1 = 0, crs = 0, crsc = 0;
    p3d ab_;

    explicit fyline() : l(cv::Vec4d()) {}

    fyline(cv::Vec4d _l) { l = _l; }

    cv::Point2d pa() const { return cv::Point2d(l[0], l[1]); }

    cv::Point2d pb() const { return cv::Point2d(l[2], l[3]); }
    /** @brief 重新初始化fyline,可以重新赋值 */
    void clear() { ab_ = p3d(), len_ = 0.0; }
    p3d ab()
    {
        if (ab_.x == 0.0 && ab_.y == 0.0)
        {
            p3d a(l[0], l[1], 0), b(l[2], l[3], 0);
            ab_ = a - b;
            if (ab_.x == 0.0) { ab_.x = 0.000000001; /*故意歪一点*/ }
        }
        return ab_;
    }

    /**角度*/
    double ag()
    {
        if (ag_ == 0.0)
        {
            ag_ = atan2(ab().y, ab().x) * agl;
            if (ag_ < 0) { ag_ = ag_ + 180; }
        }
        return ag_;
    }

    /**长度*/
    double len()
    {
        if (len_ == 0.0) { len_ = cv::norm(ab()); }
        return len_;
    }

    /**斜率*/
    double k()
    {
        if (k_ == 0.0) { k_ = u(ab().y, ab().x); }
        return k_;
    }

    /**截距*/
    double b()
    {
        if (b_ == 0.0)
        {
            if (ag() == 90.0) { b_ = DBL_MAX; }
            else
            {
                b_ = l[1] - l[0] * k();
            }
        }
        return b_;
    }

    double A() { return l[3] - l[1]; }

    double B() { return l[0] - l[2]; }

    double C()
    {
        if (C_ == 0.0) { C_ = l[1] * l[2] - l[0] * l[3]; }
        return C_;
    }

    /**平行线距离*/
    double d(fyline u)
    {
        double C1 = C(), C2 = u.C(), C12 = C1 > C2 ? C1 - C2 : C2 - C1, A1 = A(), B1 = B();
        return C12 * sqr(A1 * A1 + B1 * B1);
    }

    double d(int lx, int ly)
    {
        double d_ = u((A() * lx + B() * ly + C()), sqr(A() * A() + B() * B()));
        d_ = d_ < 0 ? -d_ : d_;
        return d_;
    }

    cv::Point2d y(double x) { return cv::Point2d(x, k() * x + b()); }

    cv::Point2d x(double y) { return cv::Point2d((y - b()) / k(), y); }

    /**
     * @brief 获得交点坐标 intersect
     * @param f 另外一条线s
     * @return 交点
     */
    cv::Point2d i(fyline f)
    {
        return cv::Point2d(u((B() * f.C() - f.B() * C()), (A() * f.B() - f.A() * B())),
                           u((f.A() * C() - A() * f.C()), (A() * f.B() - f.A() * B())));
    }

    /**返回与f有交叉*/
    bool ii(fyline f, double e = 5.0)
    {
        cv::Point2d p2 = i(f);
        double x0 = l[0] < l[2] ? l[0] : l[2], x1 = l[0] > l[2] ? l[0] : l[2],
               y0 = l[1] < l[3] ? l[1] : l[3], y1 = l[1] > l[3] ? l[1] : l[3],
               fx0 = f.l[0] < f.l[2] ? f.l[0] : f.l[2], fx1 = f.l[0] > f.l[2] ? f.l[0] : f.l[2],
               fy0 = f.l[1] < f.l[3] ? f.l[1] : f.l[3], fy1 = f.l[1] > f.l[3] ? f.l[1] : f.l[3];
        return x0 - e < p2.x && p2.x < x1 + e && y0 - e < p2.y && p2.y < y1 + e && fx0 - e < p2.x &&
               p2.x < fx1 + e && fy0 - e < p2.y && p2.y < fy1 + e;
    }

    /**
     * @param p 点
     * @return 点线距离
     */
    double pdist(cv::Point2d _p)
    {
        p3d p(_p.x, _p.y, 0);
        p3d v1 = axis(p3d(l[0], l[1], 0), p).v().cross(p3d(l[2] - l[0], l[3] - l[1], 0));
        return dnorm(v1) / len();
    }

    /**
     * @param l 线
     * @return 线段之间最短距离
     */
    double ldist(fyline l)
    {
        double out = 99999;
        out = fmin(out, pdist(l.pa()));
        out = fmin(out, pdist(l.pb()));
        out = fmin(out, l.pdist(pa()));
        out = fmin(out, l.pdist(pb()));
        return out;
    }

    /**
     * @param l fyline线
     * @return 两线之间夹角
     */
    double langle(fyline l)
    {
        return (abs(l.ag() - ag()) > 90) ? (180 - abs(ag() - l.ag())) : abs(ag() - l.ag());
    }

    cv::Point2d y(double x, /**偏移*/ double s)
    {
        cv::Point2d sp(+s * Sin(ag()), -s * Cos(ag()));
        return y(x) + sp;
    }

    cv::Point2d x(double y, /**偏移*/ double s)
    {
        cv::Point2d sp(+s * Sin(ag()), -s * Cos(ag()));
        return x(y) + sp;
    }

    bool isstreak(cv::Mat &m)
    {
#if 0
        printf(" \n ");
#endif
        int u0 = 0, u1 = 0;
        if (ag() <= 45.0 || ag() >= 90 + 45.0)
        {
            int strk = 0, cnr = 0, lwn = 0;
            u0 = l[0] <= l[2] ? l[0] : l[2];
            u1 = l[0] <= l[2] ? l[2] : l[0];
#if 0
            printf(" %1.1d>> ",u1-u0);
#endif
            for (int u = u0; u < u1; u++)
            { /**线宽*/
                int lw = 0;
                for (int s = -6; s < 6; s++)
                {
                    if (sms(m.at<cv::Vec3b>(y(u, s)), m.at<cv::Vec3b>(y(u, s + 1)))) { lw++; }
                }
#if 0
                printf(" %1.0d ",lw);
#endif
                if (lw > 0 && sms(m.at<cv::Vec3b>(y(u, 12)), m.at<cv::Vec3b>(y(u, -12))))
                {
                    strk++;
                    if (lw < 5) { lwn++; }
                }
                if (lw >= 5 && lwn > 3)
                { /**突起*/
                    cnr++;
                    lwn = 0;
                }
            }

            if (strk > 3 && cnr >= 3) { return 1; }
        }

        if (ag() > 45.0 && ag() < 90 + 45.0)
        {
            int strk = 0, cnr = 0, lwn = 0;
            u0 = l[1] <= l[3] ? l[1] : l[3];
            u1 = l[1] <= l[3] ? l[3] : l[1];

#ifdef dbg
            printf(" %1.0d ", u0 < u1);
#endif
            for (int u = u0; u < u1; u++)
            {
                int lw = 0;
                for (int s = -6; s < 6; s++)
                {
                    if (sms(m.at<cv::Vec3b>(x(u, s)), m.at<cv::Vec3b>(x(u, s + 1)))) { lw++; }
                }
                if (lw > 0 && sms(m.at<cv::Vec3b>(x(u, 12)), m.at<cv::Vec3b>(x(u, -12))))
                {
                    strk++;
                    if (lw < 5) { lwn++; }
                }
                if (lw >= 5 && lwn > 3)
                { /**突起*/
                    cnr++;
                    lwn = 0;
                }
            }

            if (strk > 3 && cnr >= 3)
            {
#if 0
                printf(" %1.0d ",/*strk*/  cnr);
#endif
                return 1;
            }
        }
        return 0;
    }
};

struct fydot
{

    p2d /**逻辑线号*/ f,
        /**逻辑位置*/ d,
        /**位置   */ p,
        /**差距   */ i,
        /**线角度 */ a;
    int /**存在   */ l = 0,
                     /**存在   */ m = 0,
                     /**相邻   */ n = 0,
                     /**空格   */ o = 0;

    scalar /**背景色 */ s;

    /**位置匹配*/
    bool operator==(const fydot &r) { return norm(p - r.p) < 3.0; }

    /**位置不匹配*/
    bool operator!=(const fydot &r) { return !(*this == r); }

    /**垂直匹配*/
    bool operator-(const fydot &r)
    {
        double d = p.y - r.p.y;
        return -2 < d && d < 2;
    }
};

struct fycols
{
    std::vector<fydot> /**平*/ r;
    double /**最小间距*/ s0,
        /**最大间距*/ s1,
        /**步进系数*/ k,
        /**不存在  */ l;
};

struct fysize
{
    double w, h;

    fysize() {}

    fysize(double w_, double h_)
    {
        w = w_;
        h = h_;
    }

    fysize operator-(const fysize &r) { return fysize(w - r.w, h - r.h); }

    fysize operator+(const fysize &r) { return fysize(w + r.w, h + r.h); }

    bool operator==(const fysize &r) { return w - r.w == 0.0 && h - r.h == 0.0; }

    bool operator!=(const fysize &r) { return !(*this == r); }
};

struct fygird
{
    std::vector<fycols> /**列*/ c;
    fysize sz;
};

struct fydot7070
{
    fydot d[70][70];
};

struct nface
{
    p2d p, sz; /**匹配度        */
    float mc,  /**类别          */
        id;
    std::vector<cv::Point2f> landmarks;
};

struct dface
{                /**68个特征点    */
    p2d d68[69], /**人脸坐标      */
        p,       /**左上          */
        lt,      /**右下          */
        rb,      /**人脸成像大小   */
        sz;      /**相似度        */
    float mc,    /**相似度        */
        lk,      /**见面次数      */
        s;       /**最佳匹配ID    */
    int id,      /**是否打过招呼  */
        hi,      /**打招呼的时间  */
        ht;
    /**128字唯一特征码*/
    std::vector<float> fid;
    string name; /**vr坐标        */
    vr v,        /**vr坐标resnet  */
        rv;
    p3d e, e3, Sz, ag;
    /**匹配度   */
    float mcl, /**匹配度   */
        mcr;

    float mca() { return (mcl + mcr) / 2; }

    /**fid2Mat*/
    cv::Mat fidm()
    {
        cv::Mat fidm_(1, 128, CV_32FC1);
        for (ulong i = 0; i < 128; i++) { fidm_.at<float>(0, int(i)) = fid[i]; }
        return fidm_;
    }

    /**Mat2fid*/
    void fidm(cv::Mat fidm_)
    {
        for (int j = 0; j < fidm_.cols; j++) { fid.push_back(fidm_.at<float>(0, j)); }
    }

    /**从vr计算欧氏坐标*/
    void v2e() { e = v.f().voe(); }

    obj _obj()
    {
        obj ob;
        ob.v = v;
        ob.e = e;
        return ob;
    }

    void obj_(obj ob)
    {
        v = ob.v;
        e = ob.e;
    }
};

enum
{
    bdo,
    fac,
    hed,
    ubd, // 上身
    lh0,
    lh1,
    lh2,
    lh3,
    rh0,
    rh1,
    rh2,
    rh3,
    lf0,
    lf1,
    lf2,
    rf0,
    rf1,
    rf2,
    lh4,
    lh5,
    lh6,
    lh7,
    rh4,
    rh5,
    rh6,
    rh7
};

struct hbody
{
    /**部位 */
    obj p[20][5];
    /**命令 子命令 方向 误差 */
    double od[100][4];
    /**角度限制 */
    int mm[100][4];
    /**行 */
    short mt[100],
        /**感 */
        mtp[100];

    p3d j[100]; /**储存关节坐标*/

    axis w;

    void setpower(double fpw)
    {
        p[hed][0].d[0].pw = fpw * 24;
        p[ubd][0].d[0].pw = fpw * 24;

        p[ubd][0].d[1].pw = fpw;
        p[lh0][0].d[0].pw = fpw;
        p[lh0][0].d[1].pw = fpw;
        p[lh1][0].d[0].pw = fpw;
        p[lh2][0].d[0].pw = fpw;
        p[lh3][0].d[0].pw = fpw;
        p[lh3][0].d[0].pw = fpw;

        p[ubd][0].d[2].pw = fpw;
        p[rh0][0].d[0].pw = fpw;
        p[rh0][0].d[1].pw = fpw;
        p[rh1][0].d[0].pw = fpw;
        p[rh1][0].d[1].pw = fpw;
        p[rh2][0].d[0].pw = fpw;
        p[rh3][0].d[0].pw = fpw;
        p[rh3][1].d[0].pw = fpw;
        p[rh3][2].d[0].pw = fpw;
        p[rh3][3].d[0].pw = fpw;

        p[ubd][0].d[3].pw = fpw;
        p[lf0][0].d[0].pw = fpw;
        p[lf0][0].d[1].pw = fpw;
        p[lf0][0].d[2].pw = fpw;
        p[lf1][0].d[0].pw = fpw;
        p[lf2][0].d[0].pw = fpw;

        p[ubd][0].d[4].pw = fpw;
        p[rf0][0].d[0].pw = fpw;
        p[rf0][0].d[1].pw = fpw;
        p[rf0][0].d[2].pw = fpw;
        p[rf1][0].d[0].pw = fpw;
        p[rf2][0].d[0].pw = fpw;
    }

    void save(string filename)
    {
        cv::FileStorage fs(filename, cv::FileStorage::WRITE);

        p[bdo][0]._save(fs, "bd_o");
        p[bdo][1]._save(fs, "bd_eyel");
        p[bdo][2]._save(fs, "bd_eyer");
        for (int i = 0; i < 4 /*10*/; i++)
        {
            p[fac][i]._save(fs, "bd_fac_" + str(i));
            p[hed][i]._save(fs, "bd_hed_" + str(i));
            p[ubd][i]._save(fs, "bd_ubd_" + str(i));
            p[lh0][i]._save(fs, "bd_lh0_" + str(i));
            p[rh0][i]._save(fs, "bd_rh0_" + str(i));
            p[lh1][i]._save(fs, "bd_lh1_" + str(i));
            p[rh1][i]._save(fs, "bd_rh1_" + str(i));
            p[lh2][i]._save(fs, "bd_lh2_" + str(i));

            p[lh3][i]._save(fs, "bd_lh3_" + str(i));

            p[rh2][i]._save(fs, "bd_rh2_" + str(i));

            p[rh3][i]._save(fs, "bd_rh3_" + str(i));

            p[lf0][i]._save(fs, "bd_lf0_" + str(i));
            p[rf0][i]._save(fs, "bd_rf0_" + str(i));
            p[lf1][i]._save(fs, "bd_lf1_" + str(i));
            p[rf1][i]._save(fs, "bd_rf1_" + str(i));
            p[lf2][i]._save(fs, "bd_lf2_" + str(i));
            p[rf2][i]._save(fs, "bd_rf2_" + str(i));
        }

        for (int i = 0; i < /*100*/ 32; i++)
        {
            for (int j = 0; j < 4; j++) { fs << "bd_od_" + str(i) + "_" + str(j) << od[i][j]; }
        }

        for (int i = 0; i < /*100*/ 32; i++)
        {
            for (int j = 0; j < 4; j++) { fs << "bd_mm_" + str(i) + "_" + str(j) << mm[i][j]; }
        }

        for (int i = 0; i < /*100*/ 32; i++) { fs << "bd_mt_" + str(i) << mt[i]; }

        for (int i = 0; i < /*100*/ 32; i++) { fs << "bd_mtp_" + str(i) << mtp[i]; }

        w._save(fs, "bd_w");

        fs.release();
    }

    void read(string filename)
    {
        cv::FileStorage fs(filename, cv::FileStorage::READ);
        p[bdo][0]._read(fs, "bd_o");
        p[bdo][1]._read(fs, "bd_eyel");
        p[bdo][2]._read(fs, "bd_eyer");
        for (int i = 0; i < 4 /*10*/; i++)
        {
            p[fac][i]._read(fs, "bd_fac_" + str(i));
            p[hed][i]._read(fs, "bd_hed_" + str(i));
            p[ubd][i]._read(fs, "bd_ubd_" + str(i));
            p[lh0][i]._read(fs, "bd_lh0_" + str(i));
            p[rh0][i]._read(fs, "bd_rh0_" + str(i));
            p[lh1][i]._read(fs, "bd_lh1_" + str(i));
            p[rh1][i]._read(fs, "bd_rh1_" + str(i));
            p[lh2][i]._read(fs, "bd_lh2_" + str(i));
            p[lh3][i]._read(fs, "bd_lh3_" + str(i));
            p[rh2][i]._read(fs, "bd_rh2_" + str(i));
            p[rh3][i]._read(fs, "bd_rh3_" + str(i));
            p[lf0][i]._read(fs, "bd_lf0_" + str(i));
            p[rf0][i]._read(fs, "bd_rf0_" + str(i));
            p[lf1][i]._read(fs, "bd_lf1_" + str(i));
            p[rf1][i]._read(fs, "bd_rf1_" + str(i));
            p[lf2][i]._read(fs, "bd_lf2_" + str(i));
            p[rf2][i]._read(fs, "bd_rf2_" + str(i));
        }

        for (int i = 0; i < /*100*/ 32; i++)
        {
            for (int j = 0; j < 4; j++) { fs["bd_od_" + str(i) + "_" + str(j)] >> od[i][j]; }
        }

        for (int i = 0; i < /*100*/ 32; i++)
        {
            for (int j = 0; j < 4; j++) { fs["bd_mm_" + str(i) + "_" + str(j)] >> mm[i][j]; }
        }

        for (int i = 0; i < /*100*/ 32; i++) { fs["bd_mt_" + str(i)] >> mt[i]; }

        for (int i = 0; i < /*100*/ 32; i++) { fs["bd_mtp_" + str(i)] >> mtp[i]; }

        w._read(fs, "bd_w");
        fs.release();
    }
};

hbody operator*(gm44d &m, hbody a);

struct wdt
{
    double *x = nullptr;
    double v = 0;
    int t = 0, ts = 0; /*唯一动作编号*/
    ulong id = 0;
};

static fyline lc_;

struct traingle
{
    p3d v0, v1, v2, //坐标
        t0, t1, t2, // uvs
        n0, n1, n2; //法向
};

struct fmesh
{
    std::vector<traingle> t;
};

std::vector<traingle> operator*(gm44d m, std::vector<traingle> t);

fmesh operator*(gm44d m, fmesh ms);

struct fmeshb
{
    fmesh m[26], m_[26];
};

class d4
{
  public:
    d4(void);

    ~d4(void);

  private:
    cv::Point rp;
    scalar mea;

  public:
    cv::Mat rsl, rsr, blk2, blk3;
    int blksz = 10;

    int ewd, eht;

    //_Float128 Tan2 (_Float128 a);
    //_Float128 Atan2(_Float128 y,_Float128 x);
    //_Float128 Sin2 (_Float128 a);
    //_Float128 Cos2 (_Float128 a);

    cv::Mat rmat = cv::Mat::zeros(3, 3, CV_64F);

    cv::Mat A = cv::Mat::zeros(3, 3, CV_64F), B = cv::Mat::zeros(3, 3, CV_64F);

    cv::Mat pwmi = cv::Mat::zeros(1, 3, CV_64F);

    cv::Mat pwmo = cv::Mat::zeros(1, 3, CV_64F), pwmo_ = cv::Mat::zeros(1, 3, CV_64F);

    cv::Mat rmata = cv::Mat::zeros(3, 3, CV_64F);

    cv::Mat pwmia = cv::Mat::zeros(1, 3, CV_64F);

    cv::Mat pwmoa = cv::Mat::zeros(1, 3, CV_64F), pwmo_a = cv::Mat::zeros(1, 3, CV_64F);

    void svmat(cv::Mat m, string fn, /**首字母不能是数字*/ string mn);

    void rdmat(cv::Mat &m, string fn, /**首字母不能是数字*/ string mn);

    vr fd(cv::Mat t, cv::Mat &el, cv::Mat &er);

    cv::Point fd2(cv::Mat &t, cv::Mat &er);

    cv::Point fdr(cv::Point lp, cv::Mat &ml);

    p3d b2e128(bipolar b);

    int mai();

    void spimg(const cv::Mat &image, cv::Mat &result);
};

cv::Rect cvRt(int _x, int _y, int _w, int _h);

cv::Rect cvRt(int _x, int _y, int _w);

cv::Rect cvRt(int _x, int _y);

cv::Mat sbmat(cv::Mat &m, cv::Rect rt);
cv::Mat submat(cv::Mat &m, cv::Point p, int d);

/**边缘检测 Roberts算子*/
cv::Mat roberts(cv::Mat srcImage);

/**转为灰度图*/
cv::Mat cvtcolor(cv::Mat m);

/**找出端点靠近的线段*/
void dpsm(std::vector<cv::Vec4f> src, std::vector<fyline> &dst);

/**
 * @brief 找出属于网格的线段 drop small
 * @param src 输入线段
 * @param dst 存放筛选出的线段
 * @param m 用于标记的图像
 * @param d 存放交点位置
 */
void dpsm(const std::vector<cv::Vec4f> &src, std::vector<fyline> &dst, cv::Mat &m,
          std::vector<cv::Point2d> &d);

/**
 * @brief 过滤识别出的棋盘线
 * @param src 线段数组
 * @param dst fyline数组
 * @return 0 正常 1 太少 2 太多
 * @details
 * 检查长度&重复
 * 检查交叉点数量
 */
int BoardLinesFilter(const std::vector<cv::Vec4f> &src, std::vector<fyline> &dst);

/**
 * @brief 按角度分组 line group
 * @param src
 * @param dst
 */
void lgrp(std::vector<fyline> src, std::vector<fyline> dst[2]);

cv::Mat normal(cv::Mat src, cv::Mat dst);

cv::Mat getm33d(cv::Mat m32d);

cv::Mat getm33d(double d00 = 1.0, double d10 = 0.0, double d20 = 64.0, double d01 = 0.0,
                double d11 = 1.0, double d21 = 48.0, double d02 = 0.0, double d12 = 0.0,
                double d22 = 1.0);

cv::Mat getm32d(cv::Mat m33d);

void getRMM2D(cv::Mat &RMM2D, double agl, cv::Point2d o, cv::Point2d m, double cx, double cy);

void getRTM33f(cv::Mat &RMM2D, double agl, cv::Point2d o, cv::Point2d m, double cx, double cy);

/**找到线最多的组*/
ulong lmx(std::vector<fyline> ll2[], ulong r0 = 999, ulong r1 = 999, ulong r2 = 999);

/**
 * @brief 按空间顺序排序
 * @param src 输入
 * @param dst 输出
 * @param lc 与之垂直的参考线
 */
void lst(std::vector<fyline> src, std::vector<fyline> &dst, fyline lc);

/**三级过滤 滤去间隔和角度不对的线段*/
void lL3(std::vector<fyline> src, std::vector<fyline> &dst, /**竖线*/ fyline lc, double dmin = 10.0,
         double dmax = 150.0);

/**获取仿射变换矩阵*/
cv::Mat gAt(float sax, float say, float sbx, float sby, float scx, float scy, float dax, float day,
            float dbx, float dby, float dcx, float dcy);

/**3通道minMaxLoc*/
void minMaxLoc(cv::Mat src, scalar &minVals, scalar &maxVals, cv::Point minLoc[],
               cv::Point maxLoc[]);

void lL4(std::vector<fyline> src[], fydot7070 &dst);

/**
 * @brief 计算点的背景色
 * @param ll2 左图线
 * @param lr2 右图线
 * @param gdv vr点阵，会被交线重新赋值
 * @return
 * todo: 为啥要在这里改gdv
 */
int lL4(std::vector<fyline> ll2[], std::vector<fyline> lr2[], std::vector<std::vector<vr>> &gdv);

void dL5(fydot7070 &gd, ulong s0, ulong s1, cv::Mat frmg, int ewd, int eht);

/**过滤出颜色相近较多的点*/
void dL6(fydot7070 &gd, ulong s0, ulong s1);

/**选取有效点生成逻辑序列*/
void dL7(fydot7070 gd, fygird &dd, double s0, double s1, int ewd, int eht);

/**根据每个点的逻辑坐标和逻辑序列修复网格
              dataview                                           直接传递数组会少一组*/
void rpr(fygird &dd, fygird &dd1, fysize &sz, std::vector<fyline> ll0, std::vector<fyline> ll1);

/**去掉存在过少的行和列*/
int dL8(fygird &ddl1, ulong cols = 9, ulong rows = 9);

int fypair(fygird &ddl, fygird &ddr, double w, double h, double k = 8);

void dataview(std::vector<fygird> dd2);

/**是否近似*/
bool like(double a, double b, double d = 5.0);

/**是否近似*/
bool like(p2d a, p2d b, double d = 5.0);

/**是否近似*/
bool like(p3d a, p3d b, double d = 5.0 * mm);

void svmat(cv::Mat m, string fn, string mn);

void rdmat(cv::Mat &m, string fn, string mn);

bool fsortl(const dface &a, const dface &b);

cv::Mat hsvreg(cv::Mat img, int iLowH = 10, int iHighH = 54, int iLowS = 40, int iHighS = 255,
               int iLowV = 50, int iHighV = 255
#ifdef cvmorphologyExopenclose
               ,
               int szw = 5, int szh = 5, int szwc = 5, int szhc = 5
#endif
);

int GoSubPix(cv::Mat frm, std::vector<nface> &r0, cv::Size sz, string name = "");
cv::Mat imgTranslate(cv::Mat &matSrc, int xOffset, int yOffset, bool bScale);

#endif // D3D_H
