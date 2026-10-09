#include "d2/d2.h"
#include "mind/mind.h"
#include "thread000.cpp"
#include <pthread.h>

#ifndef FYAIRO_1_0_0_ARM
static cv::Mat prtm(200, 500, CV_8UC3, 0.0);
#endif
static struct timeval tv;
long tmn()
{
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static mind me;
static obj wo(/* 0, -576.87 * mm, -64.5 * mm */ 0, 0, 0);
static int Ddv /* kv, tm = 0,  */, /** 盯住视频获得的目标 */ scs = 0, /** 看目标 */ scsl = 0,
                                                             mc = 0;
static cv::Point3d pv3, pw3;

static struct vr v0[200], v1, t1v[200], v3[200][68], vrect;
static int goout = 0, hi, /* 人脸刷新 */ fsrf = 0, zr = 1, mrf = 0;
extern int kH, kM, kM1, kN, kI, Dc, kT, kJ, kL;
bool enb[500];
sem_t sig[500];
std::vector<dface> DlibFacel, DlibFacer;
static std::vector<dface> Face, DetectedFace, DetectedObject, DetectedGoChess;
static std::vector<obj> FaceObj, DetectedFaceObj;

static double fpw = 160 /* 16 */;
inline void w(/** 角度变量 */ axis *a, /** 目标值 */ double v,
              /*   */ double t = 2520, /*   */ double ts = 30)
{
    a->w(v, fpw * 2520 / t, 30 / ts * 0.1);
    a->w(v, fpw * 2520 / t, 30 / ts * 0.1);
}

p3d rpr(p3d p)
{
#if 0
    return p;
#else
    if (me.l[0].b[0].p[bdo][0].d[0].s != 0.0) { return me.l[0].b[0].p[bdo][0].d[0].m * p; }
    else
    {
        return p;
    }
#endif
}
/** 方向逼近 */
int dnear2(double *u, obj *a, obj *b)
{
    double ud = 0.1;
    double r0 = dnorm(a->d[1].n(), b->d[1].n());
    if (r0 < 0.3) ud = 0.03;
    if (r0 < 0.1) ud = 0.003;
    if (r0 < 0.075 && r0 > 0.0)
    {
        // printf("E");
        return 2;
    }

    *u += ud;
    me.l[1].m();
    gm44d m = me.l[1].b[1].p[lf2][0] << me.l[1].b[0].p[lf2][0];
    me.l[1].b1_b2(m);
    double rl = dnorm(a->d[1].n(), b->d[1].n());
    if (rl < r0) { return 1; }

    *u -= 2 * ud;
    me.l[1].m();
    m = me.l[1].b[1].p[lf2][0] << me.l[1].b[0].p[lf2][0];
    me.l[1].b1_b2(m);
    double rr = dnorm(a->d[1].n(), b->d[1].n());
    if (rr < r0) { return 1; }

    *u += ud;
#if 0
    printf("\n i.cpp@{%f}", r0);
#endif
    return 0;
}

/**
 * @brief 使关节调整至特定方向
 * @param a 节点a
 * @param b 节点b
 * @param sta 偏角电机
 * @param gma 仰角电机
 * @param m 绑定关系矩阵
 * @param msg 调试信息
 * @param dst 偏角调整
 * @param dgm 仰角调整
 * @param h 头
 */
void fmove(p3d a, p3d b, axis *sta, axis *gma, gm44d m, std::vector<std::string> &msg,
           double dst = 0, double dgm = 0, int h = 0)
{
#ifdef Mechanical_arm
    // me.dot(me.cus[15].e, scalar(0, 0, 255));
    // me.dot(c2, scalar(0, 255, 0));
    axis ct0(b, a, 0, scalar(100, 100, 255)), ct2 = m * ct0;
    p3d x(gma->n()), y(sta->n()), vz(x.cross(y)), z(vz / dnorm(vz)), o(p3d(0, 0, 0));
    gm44d m1(fdot(x, y, z, o) >> fdot(p3d(1, 0, 0), p3d(0, 1, 0), p3d(0, 0, 1), p3d(0, 0, 0)));
    p3d vct = m1 * ct2.v();
    pola c2p(vct);
    double st, gm;
    if (h)
    {
        st = -(c2p.st + dst);
        gm = -(-c2p.gm + dgm);
    }
    else
    {
        st = c2p.st + dst;
        gm = -c2p.gm + dgm;
    }
    if (!isnan(c2p.st)) sta->s = st;
    if (!isnan(c2p.gm)) gma->s = gm;

#if 0
    me.dline(ct0, 4);
    me.dline(m1 * vct, fc_green, 4);
    me.dline(m1 * (x * 120 * mm), fc_red, 3);
    me.dline(m1 * (y * 120 * mm), fc_yellow, 3);
    me.dline(m1 * (z * 120 * mm), fc_blue, 3);

    msg.push_back("θ:" + str(c2p.st));
    msg.push_back("γ:" + str(c2p.gm));
    msg.push_back("rh0.θ:" + str(st));
    msg.push_back("rh0.γ:" + str(gm));
    msg.push_back("sn:" + str(int(sta->n().x)) + ", " + str(int(sta->n().y)) + ", " +
                  str(int(sta->n().z)));
#endif
#else
    gm44d m11lf2ubd = me.l[1].b[1].lf2[0] * me.l[1].b[1].ubd[0];
    // me.dot(me.cus[15].e, scalar(0, 0, 255));
    // me.dot(c2, scalar(0, 255, 0));
    axis ct0, ct2;
    ct0.a = me.l[1].b[2].ubd[0].d[2].O();
    ct0.b = a; //退出到b[1]进入新b[2]
    ct2 = ct0 * m11lf2ubd;
    p3d vct = ct2.b - ct2.a;
    double tmp;
    tmp = vct.x;
    vct.x = vct.y;
    vct.y = tmp; /* theta0向前 gamma0向前 */
    pola c2p(vct);
    if (!isnan(c2p.st)) w(&me.l[1].b[0].ubd[0].d[2], c2p.st + 90);
    if (!isnan(c2p.gm)) w(&me.l[1].b[0].rh0[0].d[0], c2p.gm);
#endif
}

/** 等待J按下 */
void wtj()
{
    int j = kJ;
    gmsg[5] = "按J键继续...";
    wtj_ = 1;
    while (j /*  !=  */ == kJ) { usleep(1 * 1000); }
    gmsg[5] = "";
    wtj_ = 0;
}
static std::vector<std::vector<vr>> gdv;

static int /** draw gird object */ dgdo = 1;
static std::vector<obj> chesso;
static std::vector<std::vector<obj>> gdo, gdo1;
static dface df0;
static obj /** I_he_obj */ iho, tcr;

std::vector<string> maketag(obj t, obj /** 相对物 */ wo = obj(0, 0, 0))
{
    std::vector<string> txt;
    txt.push_back(t.name + "坐标:");
    txt.push_back("x:" + str((t.e.x - wo.e.x) / mm));
    txt.push_back("y:" + str((t.e.y - wo.e.y) / mm));
    txt.push_back("z:" + str((t.e.z - wo.e.z) / mm));
    return txt;
}

std::vector<string> maketag(p3d t, obj /** 相对物 */ wo = obj(0, 0, 0))
{
    std::vector<string> txt;
    txt.push_back("坐标:");
    txt.push_back("x:" + str((t.x - wo.e.x) / mm));
    txt.push_back("y:" + str((t.y - wo.e.y) / mm));
    txt.push_back("z:" + str((t.z - wo.e.z) / mm));
    return txt;
}

volatile int meshmk0 = 1, meshmk1 = 1, meshmk2 = 1;
void *thr_fn24(void *)
{
    ThreadName[24] = "控制线程";
#ifdef FYAIRO_1_0_0_ARM
    return nullptr;
#endif
    while (run0)
    {
        if (meshmk0 == 1)
        {
            me.l[0].ms.m[rh0] = me.l[0].b[2].p[rh0][0].m * me.l[0].ms.m_[rh0];
            me.l[0].ms.m[rh1] = me.l[0].b[2].p[rh1][0].m * me.l[0].ms.m_[rh1];
            me.l[0].ms.m[rh2] = me.l[0].b[2].p[rh2][0].m * me.l[0].ms.m_[rh2];
            me.l[0].ms.m[rh3] = me.l[0].b[2].p[rh3][0].m * me.l[0].ms.m_[rh3];
            me.l[0].ms.m[rh4] = me.l[0].b[2].p[rh3][0].m * me.l[0].ms.m_[rh4];
            me.l[0].ms.m[rh5] = me.l[0].b[2].p[rh3][0].m * me.l[0].ms.m_[rh5];
            me.l[0].ms.m[rh6] = me.l[0].b[2].p[rh3][0].m * me.l[0].ms.m_[rh6];
            me.l[0].ms.m[rh7] = me.l[0].b[2].p[rh3][0].m * me.l[0].ms.m_[rh7];

            meshmk0 = 0;
        }
        else
        {
            usleep(20 * 1000);
        }
    }
    return nullptr;
}

void *thr_fn29(void *)
{
    ThreadName[29] = "控制线程";
#ifdef FYAIRO_1_0_0_ARM
    return nullptr;
#endif
    while (run0)
    {
        if (meshmk1 == 1)
        {
            me.l[0].ms.m[hed] = me.l[0].b[2].p[hed][0].m * me.l[0].ms.m_[hed];
            me.l[0].ms.m[ubd] = me.l[0].b[2].p[ubd][0].m * me.l[0].ms.m_[ubd];

            me.l[0].ms.m[lf0] = me.l[0].b[2].p[lf0][0].m * me.l[0].ms.m_[lf0];
            me.l[0].ms.m[lf1] = me.l[0].b[2].p[lf1][0].m * me.l[0].ms.m_[lf1];
            me.l[0].ms.m[lf2] = me.l[0].b[2].p[lf2][0].m * me.l[0].ms.m_[lf2];

            me.l[0].ms.m[rf0] = me.l[0].b[2].p[rf0][0].m * me.l[0].ms.m_[rf0];
            me.l[0].ms.m[rf1] = me.l[0].b[2].p[rf1][0].m * me.l[0].ms.m_[rf1];
            me.l[0].ms.m[rf2] = me.l[0].b[2].p[rf2][0].m * me.l[0].ms.m_[rf2];

            meshmk1 = 0;
        }
        else
        {
            usleep(20 * 1000);
        }
    }
    return nullptr;
}

void *thr_fn28(void *)
{
    ThreadName[28] = "控制线程";
#ifdef FYAIRO_1_0_0_ARM
    return nullptr;
#endif
    while (run0)
    {
        if (meshmk2 == 1)
        {

            me.l[0].ms.m[lh0] = me.l[0].b[2].p[lh0][0].m * me.l[0].ms.m_[lh0];
            me.l[0].ms.m[lh1] = me.l[0].b[2].p[lh1][0].m * me.l[0].ms.m_[lh1];
            me.l[0].ms.m[lh2] = me.l[0].b[2].p[lh2][0].m * me.l[0].ms.m_[lh2];
            me.l[0].ms.m[lh3] = me.l[0].b[2].p[lh3][0].m * me.l[0].ms.m_[lh3];
            me.l[0].ms.m[lh4] = me.l[0].b[2].p[lh3][0].m * me.l[0].ms.m_[lh4];
            me.l[0].ms.m[lh5] = me.l[0].b[2].p[lh3][0].m * me.l[0].ms.m_[lh5];
            me.l[0].ms.m[lh6] = me.l[0].b[2].p[lh3][0].m * me.l[0].ms.m_[lh6];
            me.l[0].ms.m[lh7] = me.l[0].b[2].p[lh3][0].m * me.l[0].ms.m_[lh7];

            meshmk2 = 0;
        }
        else
        {
            usleep(20 * 1000);
        }
    }
    return nullptr;
}

void *thr_fn16(void *)
{
    ThreadName[16] = "空间感知";
    std::vector<dface> Dfl, Dfr;
#ifndef FYAIRO_1_0_0_ARM
    while (frm99l.cols == 0 || frm99r.cols == 0) { usleep(50 * 1000); }
#endif

    while (run0)
    {
        sem_wait(&sig[16]);
        if (nmarvl == 1 && nmarvr == 1)
        {
            nmarvl = 0;
            nmarvr = 0;
        }

        Face.clear(); //匹配人脸
        Dfl.clear();
        Dfr.clear();
        Dfl = DlibFacel;
        Dfr = DlibFacer;

#ifdef xdbg
        printf("{%2.0lu, %2.0lu, %2.0lu, %2.0lu}", Rl.size(), Rr.size(), fl.size(), fr.size());
#endif
        if (Dfl.size() && Dfr.size())
        {
            for (uint i = 0; i < Dfl.size(); i++)
            {
                for (uint j = 0; j < Dfr.size(); j++)
                {
                    if (like(Dfl[j].d68[28].y, Dfr[j].d68[28].y,
#ifdef FYAIRO_1_0_0_ARM
                             50
#else
                             10
#endif
                             ) &&
                        Dfl[j].id == Dfr[i].id)
                    {
                        dface face_;
                        face_.v.lp = Dfl[i].d68[28] /* .p */;
                        face_.v.rp = Dfr[j].d68[28] /* .p */;
                        face_.id = Dfl[j].id;
                        face_.name = Dfl[j].name;
                        face_.Sz = vr(Dfl[j].rb, Dfr[j].rb).f().vo().e() -
                                   vr(Dfl[j].lt, Dfr[j].lt).f().vo().e();
                        face_.v2e();
                        face_.e = rpr(face_.e);
                        Face.push_back(face_);
                    }
                }
            }
        }
        //二次匹配
        if (Face.size())
        {
            std::vector<uint> erl, err;
            for (uint i = 0; i < Face.size(); i++)
            {
                for (uint j = 0; j < ResNetFaceL.size(); j++)
                {
                    if (like(Face[i].p, ResNetFaceL[j].p, 50))
                    {
                        Face[i].mcl = ResNetFaceL[j].mc;
                        Face[i].rv.lp = ResNetFaceL[j].p;
                        erl.push_back(j);
                    }
                }
                for (uint j = 0; j < ResNetFaceR.size(); j++)
                {
                    if (like(Face[i].p, ResNetFaceR[j].p, 50))
                    {
                        Face[i].mcr = ResNetFaceR[j].mc;
                        Face[i].rv.rp = ResNetFaceR[j].p;
                        err.push_back(j);
                    }
                }
            }
#ifdef xdbg
            printf("[%2.0lu, %2.0lu]", erl.size(), err.size());
#endif
            for (uint i = 0; i < erl.size(); i++)
            {
                ResNetFaceL.erase(ResNetFaceL.begin() + long(erl[i]));
            }
            for (uint i = 0; i < erl.size(); i++)
            {
                ResNetFaceR.erase(ResNetFaceR.begin() + long(err[i]));
            }
        }

        DetectedFace.clear();
        for (uint i = 0; i < ResNetFaceL.size(); i++)
        {
            for (uint j = 0; j < ResNetFaceR.size(); j++)
            {
                if (like(ResNetFaceL[i].p.y, ResNetFaceR[j].p.y, 10) &&
                    ResNetFaceL[i].p.x > ResNetFaceR[j].p.x)
                {
                    dface f0;
                    f0.id = 0;
                    f0.v.lp = ResNetFaceL[i].p;
                    f0.v.rp = ResNetFaceR[j].p;
                    f0.mcl = ResNetFaceL[i].mc;
                    f0.mcr = ResNetFaceR[i].mc;
                    f0.v2e();
                    f0.e = rpr(f0.e);

                    DetectedFace.push_back(f0);
                }
            }
        }

        DetectedObject.clear();
        for (uint i = 0; i < ObjL.size(); i++)
        {
            for (uint j = 0; j < ObjR.size(); j++)
            {
                if (like(ObjL[i].p.y, ObjR[j].p.y, 10) && ObjL[i].p.x > ObjR[j].p.x &&
                    int(ObjL[i].id) == int(ObjR[j].id))
                {
                    dface f0;
                    f0.id = 0;
                    f0.v.lp = ObjL[i].p;
                    f0.v.rp = ObjR[j].p;
                    f0.mcl = ObjL[i].mc;
                    f0.mcr = ObjR[i].mc;
                    f0.name = classNames[int(ObjL[i].id)];
                    f0.v2e();
                    f0.e = rpr(f0.e);
                    DetectedObject.push_back(f0);
                }
            }
        }

        DetectedGoChess.clear();
        for (uint i = 0; i < GoL.size(); i++)
        {
            for (uint j = 0; j < GoR.size(); j++)
            {
                if (like(GoL[i].p.y, GoR[j].p.y, 7) && GoL[i].p.x > GoR[j].p.x &&
                    int(GoL[i].id) == int(GoR[j].id))
                {
                    dface f0;
                    f0.id = int(GoL[i].id);
                    f0.v.lp = GoL[i].p;
                    f0.v.rp = GoR[j].p;
                    f0.mcl = GoL[i].mc;
                    f0.mcr = GoR[i].mc;
                    f0.name = classNamesc[int(GoL[i].id)];
                    f0.v2e();
                    f0.e = rpr(f0.e);
                    DetectedGoChess.push_back(f0);
                }
            }
        }

        GoL.clear();
        GoR.clear();

#if 0
        for(int i = 0;i<0;i++){
            printf("for(int i = 0;i<0;i++)");
        }
#endif
        std::sort(Face.begin(), Face.end(), fsortl);
        std::sort(DetectedFace.begin(), DetectedFace.end(), fsortl);

        if (Face.size())
        {
            pL = Face[0].v.lp;
            pR = Face[0].v.rp;
            me.l[0].wd[20].v = Face[0].v;
            me.l[0].wd[20].e = Face[0].e;
            // std::vector<dface> Face2 = Face;
            for (uint i = 0; i < Face.size(); i++)
            {
                gmsg[2] = MyName + ":" + str(int(i)) + " 我看见 " +
                          Faces[uint(Face[i].id)].name.c_str() + " 在我的[" +
                          str(Face[i].e.x / mm) + "mm " + str(Face[i].e.y / mm) + "mm " +
                          str(Face[i].e.z / mm) + "mm]位置";
                std::cout << std::endl << gmsg[2];
                sendcmd(gmsg[2], seq[6], 3, 3);
                gmsg[3] = str(Faces[uint(Face[i].id)].hi) + " " + str(Faces[uint(Face[i].id)].ht) +
                          "/100";
                if (!Faces[uint(Face[i].id)].hi && !enb[80])
                {
                    sfc[i] = Face[0].id;
                    Faces[uint(Face[i].id)].hi = 1;
#if 0
                    printf("@%d %u", sfc[i], i);
#endif
                }
            }
        }
        for (uint i = 0; i < Faces.size(); i++)
        {
            if (Faces[i].ht > 100)
            {
                Faces[i].hi = 0;
                Faces[i].ht = 0;
            }
            if (Faces[i].hi == 1) Faces[i].ht++;
        }

        if (DetectedFace.size())
        {
            rL = DetectedFace[0].v.lp;
            rR = DetectedFace[0].v.rp;
            me.l[0].wd[30].v = DetectedFace[0].v;
            me.l[0].wd[30].e = DetectedFace[0].e;
        }

        if (DetectedObject.size())
        {
            oL = DetectedObject[0].v.lp;
            oR = DetectedObject[0].v.rp;
            me.l[0].wd[40].v = DetectedObject[0].v;
            me.l[0].wd[40].e = DetectedObject[0].e;
        }

#if 0
        printf("\n"
               "\n[%9.4f %9.4f %9.4f] "
               "\n[%9.4f %9.4f %9.4f] "
               "\n[%9.4f %9.4f %9.4f] "
               "\n[%9.4f %9.4f %9.4f] ",
               me.l[0].b[0].lf2[0].o.x, me.l[0].b[0].lf2[0].o.y, me.l[0].b[0].lf2[0].o.z,
                me.l[0].b[0].lf2[0].x.x, me.l[0].b[0].lf2[0].x.y, me.l[0].b[0].lf2[0].x.z,
                me.l[0].b[0].lf2[0].y.x, me.l[0].b[0].lf2[0].y.y, me.l[0].b[0].lf2[0].y.z,
                me.l[0].b[0].lf2[0].z.x, me.l[0].b[0].lf2[0].z.y, me.l[0].b[0].lf2[0].z.z);
#endif
        cv::Point3d sz = cv::Point3d(180 * mm, 180 * mm, 180 * mm);
        obj a, b;

        for (uint i = 0; i < DetectedFace.size(); i++)
        {
            a.e = DetectedFace[i].e;
            a.box(sz, 50 * mm);
            b = me.l[0].currentm * a;
            DetectedFace[i].e = b.e;
#if 0
            std::cout<<face0[i].e/mm<<face0[i].eb/mm;
#endif
        }

#if 0
        printf("<%2.0lu, %2.0lu>", face0.size(), face.size());
#endif

#if 0
        if(debug)std::cout<<"D"<<sfc[0];
        if(face.size())std::cout<<std::endl<<" s = "<<face.size()
                               <<", "<<face[0].name;
        if(face0.size())std::cout<<std::endl<<"s0 = "<<face0.size()
                                <<", "<<face0[0].name;
#endif

        FaceObj.clear();
        for (uint i = 0; i < Face.size(); i++)
        {
            obj tmp(Face[i].e);
            tmp.name = Face[i].name;
            FaceObj.push_back(tmp);
        }

        DetectedFaceObj.clear();
        for (uint i = 0; i < DetectedFace.size(); i++)
        {
            obj tmp(DetectedFace[i].e);
            tmp.name = DetectedFace[i].name;
            DetectedFaceObj.push_back(tmp);
        }

        fsrf = 1;
#ifndef FYAIRO_1_0_0_ARM
        v1.lp = mdl.mp2;
        v1.rp = mdr.mp2;
#endif

        if (Dfl.size() && Dfr.size())
        {
            vrect.lp = Dfl[0].p;
            vrect.rp = Dfr[0].p;
        }

        vr vv, v;
        v.lp = cv::Point(0, 1);
        v.rp = cv::Point(-19, 1);
        vv = vr(me.cus[12].b);

#if 0
        me.drt(me.l[0].bd[0].fotl[2], me.l[0].bd[1].fotl[2], cv::Scalar(200, 0, 0  ), 50 * mm);
        me.drt(me.l[0].bd[0].fotl[3], me.l[0].bd[1].fotl[3], cv::Scalar(  0, 0, 255), 1);
        me.drt(me.l[0].bd[0].fotl[4], me.l[0].bd[1].fotl[4], cv::Scalar(  0, 0, 255), 1);
#endif

        chesso.clear();
        for (uint i = 0; i < DetectedGoChess.size(); i++)
        {
            obj chesst;
            chesst.e = me.l[0].currentm * DetectedGoChess[i].e;
            chesst.sz = cv::Point3d(22.83 * mm, 6.66 * mm, 22.83 * mm);
            chesst.id = DetectedGoChess[i].id;
            chesst.d[1].a.y = 1;
            chesso.push_back(chesst);
            gmsg[2] = MyName + ":" + str(int(i)) + " 我看见 " + classNamesc[uint(chesso[i].id)] +
                      " 在我的[" + str(chesso[i].e.x / mm) + "mm " + str(chesso[i].e.y / mm) +
                      "mm " + str(chesso[i].e.z / mm) + "mm]位置";
            std::cout << std::endl << gmsg[2];
            sendcmd(gmsg[2], seq[6], 3, 3);
        }
#if 0
        if(!enb[80]){                                                  /* 假设棋子 */
                    obj c = obj(110 * mm, -330 * mm, 140 * mm, 22.83 * mm, 6.66 * mm, 22.83 * mm);
            c.d[1].a.y = 1;
            c.id = 0;
            chesso.push_back(c);
            c = obj(135 * mm, -330 * mm, 140 * mm, 22.83 * mm, 6.66 * mm, 22.83 * mm);
            c.d[1].a.y = 1;
            c.id = 1;
            chesso.push_back(c);
        }
#endif

        gm44d m01lf2_ = me.l[0].b[1].p[lf2][0] << me.l[0].b[0].p[lf2][0];
        obj cusp0 = *me.cusp[0];
        me.cus[13] = m01lf2_ * cusp0;
        me.cus[12] = m01lf2_ * cusp0; /** 0逆变换给12 */

        mrf = 1;

        if (DetectedFaceObj.size())
        {
            me.cus[0].e = DetectedFaceObj[0].e;
            me.cusp[0] = &me.cus[0];
        }

        iho.d[1] = axis(me.cusp[0]->e, me.l[1].b[2].p[hed][0].e, 0);
        iho.d[1].ca = 0;
        iho.d[1].cb = scalar(255);
        me.dlinef(iho.d[1]);

        me.nt = me.n;
        me.n = 0;
        me.dnt = me.dn;
        me.dn = 0;
        usleep(10 * 1000
#ifdef FYAIRO_1_0_0_ARM
               * 10
#endif
        );
    }
    return (nullptr);
}

#ifndef FYAIRO_1_0_0_ARM
void *thr_fn17(void *)
{
    ThreadName[17] = "通讯线程";
    if (Ddv != 2) { return (nullptr); }
    while (frm99l.cols == 0 || frm99r.cols == 0) { usleep(50 * 1000); } //等待网络初始化
    int i = 0;
    uchar j = 0;
    usleep(3200 * 1000);
    while (run0)
    {
        i = i < 40 ? i + 1 : 0;
        if (me.l[0].b[0].mt[i] != me.l[0].b[0].mtp[i])
        {
            me.l[0].b[0].mtp[i] = me.l[0].b[0].mt[i];
            byte a[2];
            short2byte(me.l[0].b[0].mt[i], a);
            if (i < 2)
                sendcmd(byte(me.l[0].b[0].od[i][1]), a[1], byte(me.l[0].b[0].od[i][0]), a[0], 0, 12,
                        2, j);
            else
                sendcmd(byte(me.l[0].b[0].od[i][1]), byte(me.l[0].b[0].od[i][2]),
                        byte(me.l[0].b[0].od[i][0]), a[1], a[0], 12, 2, j);
#if 1
            j = j == 255 ? 0 : j + 1;
#endif
            usleep(/* 30 */ 30 * ms_); /* 110ms */
        }
        else
        {
            usleep(1 * ms_);
        }
    }
    return (nullptr);
}

void *thr_fn18(void *)
{
    if (Ddv != 2) { return (nullptr); }
    while (frm99l.cols == 0 || frm99r.cols == 0) { usleep(50 * 1000); } //等待网络初始化
    ThreadName[18] = "识别人并打招呼";
    usleep(5000 * 1000);
    while (!kN)
    { // w()改变d.pw
        usleep(50 * 1000);
    }
    // 初始化动作
    w(&me.l[1].b[0].p[ubd][0].d[0], 1);
    w(&me.l[1].b[0].p[hed][0].d[0], 1);
    w(&me.l[1].b[0].p[ubd][0].d[0], 0);
    w(&me.l[1].b[0].p[hed][0].d[0], 0);
    usleep(1000 * 1000);
    w(&me.l[1].b[0].p[lh2][0].d[0], 1);
    w(&me.l[1].b[0].p[lh2][0].d[0], 1);
    w(&me.l[1].b[0].p[lh2][0].d[0], 0);
    w(&me.l[1].b[0].p[lh2][0].d[0], 0);
    usleep(1000 * 1000);
    w(&me.l[1].b[0].p[lh1][0].d[0], 1);
    //    w(&me.l[1].b[0].p[rh1][0].d[0], 1);
    w(&me.l[1].b[0].p[lh0][0].d[1], 1);
    //    w(&me.l[1].b[0].p[rh0][0].d[1], 1);
    w(&me.l[1].b[0].p[lh1][0].d[0], 0);
    //    w(&me.l[1].b[0].p[rh1][0].d[0], 0);
    w(&me.l[1].b[0].p[lh0][0].d[1], 0);
    //    w(&me.l[1].b[0].p[rh0][0].d[1], 0);
    usleep(1000 * 1000);
    w(&me.l[1].b[0].p[lh0][0].d[0], 1);
    //    w(&me.l[1].b[0].p[rh0][0].d[0], 1);
    w(&me.l[1].b[0].p[ubd][0].d[1], 1);
    w(&me.l[1].b[0].p[ubd][0].d[2], 1);
    w(&me.l[1].b[0].p[lh0][0].d[0], 0);
    w(&me.l[1].b[0].p[rh0][0].d[0], 0);
    w(&me.l[1].b[0].p[ubd][0].d[1], 0);
    w(&me.l[1].b[0].p[ubd][0].d[2], 0);

    while (run0)
    {
        goout = 0;
        int tm2 = 0;
        if (!kN)
        {
            usleep(20 * 1000);
            continue;
        }
        if (DetectedFace.size())
        {
            me.cusp[0]->e = DetectedFace[0].e;
            scs = 1;
        }

        if (Face.size()) { printf("我面前有%d个人", int(Face.size())); }
        else
        {
            usleep(10 * 1000);
            continue;
        }

        me.cusp[0]->e = Face[0].e;
        me.cusp[0]->id = Face[0].id;

        printf("id0 = %d ", Face[0].id);

        std::cout << std::endl << "开始跟随";

        for (uint i = 0; i < Face.size(); i++)
        {
            if (Face[i].mc < 0.42f) { sfc[i] = int(Face[i].id); }
        }
#ifdef Xdbg
        printf("{%d}", sfc[0]);
#endif
        goout = 0;
        scs = 1; // 设置跟随
        hi = 1  ; // 第hi个打招呼的姿势
//        sendcmd(7, 0, 17, 0, 2);
        sendcmd(8, Face[0].id, 3, 66, 77, 12, 3, 2, MyRobots); // 发送打招呼指令
        while (tm2 < 60 * 1000 * 1000)
        {
            if (!fsrf)
            {
                usleep(1000 * 1000);
                continue;
            }
            fsrf = 0;

            if (DetectedFace.size()) { me.cusp[0]->e = DetectedFace[0].e; }

            if (Face.size())
            {
                for (uint i = 0; i < Face.size(); i++)
                {
                    if (Face[i].id == me.cusp[0]->id) { me.cusp[0]->e = Face[0].e; }
                }
            }

            if (DetectedFace.size() || Face.size()) scs = 1;

#ifdef xdbg
            std::cout << me.cs[0]->eb / mm;
            printf("<%2.0lu, %2.0lu, %2.0lu, %2.0lu>", Rl.size(), Rr.size(), face0.size(),
                   face.size());
#endif

            if (!DetectedFace.size()) { goout++; }
            else
            {
                goout = 0;
            }
#ifdef dbg
            printf("<%d>", goout);
#endif
            if (goout > 20)
            {
                printf("\n人离开, 提前结束。");
                break;
            }

#ifdef Xdbg
            printf(" <%d> ", goout);
#endif
            usleep(100 * 1000);
            tm2 += 100 * 1000;
        }

        usleep(6000 * 1000);
        wdn = 2;
        hi = 2;

        usleep(10 * 1000 * 1000);

        me.cusp[0]->e = cv::Point3d(0, 0, 1000 * mm);

        scs = 1;
        while (scs == 0) usleep(1000);

        sleep(2);

        std::cout << std::endl << "工作完成...sleep 12s";

        sleep(10);
    }
    return (nullptr);
}

std::vector<std::string> msg22;
void *thr_fn22(void *)
{
    ThreadName[22] = "目标跟随";
    usleep(4000 * 1000);

    while (run0)
    {
        if (scs == 1 || scsl)
        {
            scs = 0;
            usleep(3000 * 1000);
            // wtj();

            /** 是控制整个身体及世界来对准 而不是头 */
            me.dlinef(me.cusp[0]->e, me.l[1].b[2].p[ubd][0].d[0].O(), fc_blue);
            msg22.clear(); //不能超过四十条 注意清空
            fmove(me.cusp[0]->e, me.l[1].b[2].p[ubd][0].d[0].O(), &me.l[1].b[0].p[ubd][0].d[0],
                  &me.l[1].b[0].p[hed][0].d[0],
                  me.l[1].b[1].p[ubd][0] << me.l[1].b[1].p[me.l[1].tp][0], msg22, 0, 0, 1);
            me.l[1].m();
#if 0
            usleep(15 * 1000 * 1000);
#endif
        }
        else
        {
            usleep(50 * 1000);
        }
    }
    return (nullptr);
}
volatile int mslock = 0, drawmesh = 0, glwork = 0;

void *thr_fn23(void *)
{
    ThreadName[23] = "动作控制";
    me.cus[10] = obj(91 * mm, -160 * mm, 260 * mm, 22.83 * mm, 6.66 * mm, 22.83 * mm);
    me.cus[10].d[1].a.y = 1;
    me.cus[11] = obj(0 * mm, 0 * mm, 1000 * mm, 5 * mm, 5 * mm, 5 * mm, 0, 0, 0, 255, 1 * mm);
    me.cus[13] = obj(0 * mm, -130 * mm, 240 * mm, 22.83 * mm, 6.66 * mm, 22.83 * mm);
    me.cus[18] = obj(200 * mm, -180 * mm, 160 * mm, 22.83 * mm, 6.66 * mm, 22.83 * mm);
    me.cus[20] = obj(0 * mm, 0 * mm, 1000 * mm, 8 * mm, 8 * mm, 8 * mm);
    me.cusp[0] = &me.cus[0];

    me.l[1].b[0].setpower(fpw);         /* 设定动作速度 *  */
    me.l[0].b[0].p[bdo][0].d[0].getM(); /* 获取眼睛初始仰角矩阵 */
    me.reftcr(lf2);                     /* 确定初始接触面 */
    usleep(1000 * 1000);
    while (run0)
    {
#if 0
        printf("@ %f %f",  me.l[0].bd[0].hed[0].d[0].pw, me.l[0].bd[0].hed[0].d[0].ppw);
#endif
        me.l[1].m();

        me.l[0].b[0].p[hed][0].d[0] >> me.l[1].b[0].p[hed][0].d[0];
        me.l[0].b[0].p[ubd][0].d[0] >> me.l[1].b[0].p[ubd][0].d[0];

        me.l[0].b[0].p[ubd][0].d[1] >> me.l[1].b[0].p[ubd][0].d[1];
        me.l[0].b[0].p[lh0][0].d[0] >> me.l[1].b[0].p[lh0][0].d[0];
        me.l[0].b[0].p[lh0][0].d[1] >> me.l[1].b[0].p[lh0][0].d[1];
        me.l[0].b[0].p[lh1][0].d[0] >> me.l[1].b[0].p[lh1][0].d[0];
        me.l[0].b[0].p[lh2][0].d[0] >> me.l[1].b[0].p[lh2][0].d[0];
        me.l[0].b[0].p[lh3][0].d[0] >> me.l[1].b[0].p[lh3][0].d[0];

        me.l[0].b[0].p[ubd][0].d[2] >> me.l[1].b[0].p[ubd][0].d[2];
        me.l[0].b[0].p[rh0][0].d[0] >> me.l[1].b[0].p[rh0][0].d[0];
        me.l[0].b[0].p[rh0][0].d[1] >> me.l[1].b[0].p[rh0][0].d[1];
        me.l[0].b[0].p[rh1][0].d[0] >> me.l[1].b[0].p[rh1][0].d[0];
        me.l[0].b[0].p[rh1][0].d[1] >> me.l[1].b[0].p[rh1][0].d[1];
        me.l[0].b[0].p[rh2][0].d[0] >> me.l[1].b[0].p[rh2][0].d[0];
        me.l[0].b[0].p[rh3][0].d[0] >> me.l[1].b[0].p[rh3][0].d[0];
        me.l[0].b[0].p[rh3][1].d[0] >> me.l[1].b[0].p[rh3][1].d[0];
        me.l[0].b[0].p[rh3][2].d[0] >> me.l[1].b[0].p[rh3][2].d[0];
        me.l[0].b[0].p[rh3][3].d[0] >> me.l[1].b[0].p[rh3][3].d[0];

        me.l[0].b[0].p[ubd][0].d[3] >> me.l[1].b[0].p[ubd][0].d[3];
        me.l[0].b[0].p[lf0][0].d[0] >> me.l[1].b[0].p[lf0][0].d[0];
        me.l[0].b[0].p[lf0][0].d[1] >> me.l[1].b[0].p[lf0][0].d[1];
        me.l[0].b[0].p[lf0][0].d[2] >> me.l[1].b[0].p[lf0][0].d[2];
        me.l[0].b[0].p[lf1][0].d[0] >> me.l[1].b[0].p[lf1][0].d[0];
        me.l[0].b[0].p[lf2][0].d[0] >> me.l[1].b[0].p[lf2][0].d[0];

        me.l[0].b[0].p[ubd][0].d[4] >> me.l[1].b[0].p[ubd][0].d[4];
        me.l[0].b[0].p[rf0][0].d[0] >> me.l[1].b[0].p[rf0][0].d[0];
        me.l[0].b[0].p[rf0][0].d[1] >> me.l[1].b[0].p[rf0][0].d[1];
        me.l[0].b[0].p[rf0][0].d[2] >> me.l[1].b[0].p[rf0][0].d[2];
        me.l[0].b[0].p[rf1][0].d[0] >> me.l[1].b[0].p[rf1][0].d[0];
        me.l[0].b[0].p[rf2][0].d[0] >> me.l[1].b[0].p[rf2][0].d[0];

        if (me.l[0].m() || glwork)
        {
            meshmk0 = meshmk1 = meshmk2 = 1;
            while (meshmk0 || meshmk1 || meshmk2) { usleep(1000); }
            mslock = 1;
            while (drawmesh) { usleep(1000); }
            me.l[0].refmesh();
            mslock = 0;
            glwork = 0;
        }
        usleep(20 * 1000);
    }
    return (nullptr);
}

void *thr_fn19(void *)
{
    if (Ddv != 2) { return (nullptr); }
    ThreadName[19] = "打招呼与告别";
    while (run0)
    {
        if (hi == 1)
        {
            hi = 0;
            zr = 0;
            std::cout << std::endl << "挥手招呼";

            w(&me.l[1].b[0].p[lh0][0].d[0], 82, 3200);
            w(&me.l[1].b[0].p[ubd][0].d[1], 138, 4000);
            usleep(3200);
            w(&me.l[1].b[0].p[lh0][0].d[0], 27.2, 1200, 30);

            w(&me.l[1].b[0].p[lh2][0].d[0], -90, 1200, 30);

            w(&me.l[1].b[0].p[lh1][0].d[0], 22.6, 1200, 30);
            w(&me.l[1].b[0].p[lh0][0].d[1], 28.6, 1200, 30);
            usleep(1200);

            w(&me.l[1].b[0].p[lh1][0].d[0], 45.8, 1200, 30);
            w(&me.l[1].b[0].p[lh0][0].d[1], 36, 1200, 30);
            usleep(1200);

            w(&me.l[1].b[0].p[lh2][0].d[0], 0, 1500, 30);
            w(&me.l[1].b[0].p[lh0][0].d[1], 0, 1500, 30);
            w(&me.l[1].b[0].p[lh1][0].d[0], 0, 1500, 30);
            w(&me.l[1].b[0].p[lh0][0].d[0], 0, 3500, 30);
            w(&me.l[1].b[0].p[ubd][0].d[1], 0, 5000, 30);
            usleep(1500);
            zr = 1;
        }

        if (hi == 2)
        {
            hi = 0;
            zr = 0;
            std::cout << std::endl << "挥手告别";

            w(&me.l[1].b[0].p[ubd][0].d[1], 150, 3000, 30);
            w(&me.l[1].b[0].p[lh0][0].d[0], 82, 2400, 30);
            usleep(3000 * 1000);
            w(&me.l[1].b[0].p[lh2][0].d[0], -90, 1500, 30);
            w(&me.l[1].b[0].p[lh0][0].d[0], 1.2, 4500, 30);
            usleep(4500 * 1000);

            w(&me.l[1].b[0].p[lh0][0].d[0], 60, 1600, 30);
            usleep(1600 * 1000);

            w(&me.l[1].b[0].p[lh0][0].d[0], 1.2, 1600, 30);
            usleep(1600 * 1000);

            w(&me.l[1].b[0].p[lh0][0].d[0], 60, 1600, 30);
            usleep(1600 * 1000);

            w(&me.l[1].b[0].p[lh0][0].d[0], 1.2, 1600, 30);
            usleep(1600 * 1000);

            w(&me.l[1].b[0].p[lh0][0].d[0], 40, 1600, 30);
            usleep(1600 * 1000);

            w(&me.l[1].b[0].p[lh2][0].d[0], 0);

            w(&me.l[1].b[0].p[lh0][0].d[0], 0, 3000, 30);
            w(&me.l[1].b[0].p[ubd][0].d[1], 0, 4800, 30);

            usleep(4800 * 1000);
            zr = 1;
        }

        if (hi == 3)
        {
            hi = 0;
            zr = 0;
            std::cout << std::endl << "挥手招呼";

            w(&me.l[1].b[0].p[rh0][0].d[0], 82, 3600, 30);
            w(&me.l[1].b[0].p[ubd][0].d[2], 138, 4000, 30);
            usleep(4000 * 1000);
            w(&me.l[1].b[0].p[rh0][0].d[0], 27.2, 1600, 30);

            w(&me.l[1].b[0].p[rh2][0].d[0], -90, 1600, 30);

            w(&me.l[1].b[0].p[rh1][0].d[0], 22.6, 1600, 30);
            w(&me.l[1].b[0].p[rh0][0].d[1], 28.6, 1600, 30);
            usleep(1600 * 1000);

            w(&me.l[1].b[0].p[rh1][0].d[0], 58.8, 1600, 30);
            w(&me.l[1].b[0].p[rh0][0].d[1], 36, 1600, 30);
            usleep(1600 * 1000);

            w(&me.l[1].b[0].p[rh0][0].d[1], 0, 1600, 30);

            w(&me.l[1].b[0].p[rh2][0].d[1], 0, 1600, 30);
            w(&me.l[1].b[0].p[rh0][0].d[1], 0, 1600, 30);
            w(&me.l[1].b[0].p[rh1][0].d[0], 0, 1600, 30);
            w(&me.l[1].b[0].p[rh0][0].d[0], 0, 3600, 30);
            w(&me.l[1].b[0].p[ubd][0].d[2], 0, 5200, 30);
            usleep(5200 * 1000);
            zr = 1;
        }

        if (hi == 4)
        {
            hi = 0;
            zr = 0;
            std::cout << std::endl << "挥手告别";

            w(&me.l[1].b[0].p[ubd][0].d[2], 150, 3000, 30);
            w(&me.l[1].b[0].p[rh0][0].d[0], 82, 4200, 30);

            w(&me.l[1].b[0].p[rh2][0].d[1], -90, 1600, 30);
            usleep(4200 * 1000);

            w(&me.l[1].b[0].p[rh0][0].d[0], 1.2, 1600, 30);
            usleep(1600 * 1000);

            w(&me.l[1].b[0].p[rh0][0].d[0], 60, 1600, 30);
            usleep(1600 * 1000);

            w(&me.l[1].b[0].p[rh0][0].d[0], 1.2, 1600, 30);
            usleep(1600 * 1000);

            w(&me.l[1].b[0].p[rh0][0].d[0], 60, 1600, 30);
            usleep(1300 * 1000);

            w(&me.l[1].b[0].p[rh0][0].d[0], 1.2, 1600, 30);
            usleep(1600 * 1000);

            w(&me.l[1].b[0].p[rh0][0].d[0], 40, 1500, 30);
            usleep(1500 * 1000);

            w(&me.l[1].b[0].p[rh2][0].d[1], 0);

            w(&me.l[1].b[0].p[rh0][0].d[0], 0, 3000, 30);
            w(&me.l[1].b[0].p[ubd][0].d[2], 0, 4800, 30);

            usleep(2800 * 1000);
            zr = 1;
        }

        usleep(20 * 1000);
    }
    return (nullptr);
}

bool crash(int ln = 2)
{
    return me.l[ln].b[2].p[hed][0] & me.l[ln].b[2].p[rh0][0] ||
           me.l[ln].b[2].p[hed][0] & me.l[ln].b[2].p[rh1][0] ||
           me.l[ln].b[2].p[hed][0] & me.l[ln].b[2].p[rh2][0] ||
           me.l[ln].b[2].p[hed][0] & me.l[ln].b[2].p[rh3][1] ||

           me.l[ln].b[2].p[ubd][0] & me.l[ln].b[2].p[rh1][0] ||
           me.l[ln].b[2].p[ubd][0] & me.l[ln].b[2].p[rh2][0] ||
           me.l[ln].b[2].p[ubd][0] & me.l[ln].b[2].p[rh3][1] ||

           me.l[ln].b[2].p[lh0][0] & me.l[ln].b[2].p[rh0][0] ||
           me.l[ln].b[2].p[lh0][0] & me.l[ln].b[2].p[rh1][0] ||
           me.l[ln].b[2].p[lh0][0] & me.l[ln].b[2].p[rh2][0] ||
           me.l[ln].b[2].p[lh0][0] & me.l[ln].b[2].p[rh3][1] ||

           me.l[ln].b[2].p[lh2][0] & me.l[ln].b[2].p[rh0][0] ||
           me.l[ln].b[2].p[lh2][0] & me.l[ln].b[2].p[rh1][0] ||
           me.l[ln].b[2].p[lh2][0] & me.l[ln].b[2].p[rh2][0] ||
           me.l[ln].b[2].p[lh2][0] & me.l[ln].b[2].p[rh3][1] ||

           me.l[ln].b[2].p[lf0][0] & me.l[ln].b[2].p[rh0][0] ||
           me.l[ln].b[2].p[lf0][0] & me.l[ln].b[2].p[rh1][0] ||
           me.l[ln].b[2].p[lf0][0] & me.l[ln].b[2].p[rh2][0] ||
           me.l[ln].b[2].p[lf0][0] & me.l[ln].b[2].p[rh3][1] ||

           me.l[ln].b[2].p[lf1][0] & me.l[ln].b[2].p[rh0][0] ||
           me.l[ln].b[2].p[lf1][0] & me.l[ln].b[2].p[rh1][0] ||
           me.l[ln].b[2].p[lf1][0] & me.l[ln].b[2].p[rh2][0] ||
           me.l[ln].b[2].p[lf1][0] & me.l[ln].b[2].p[rh3][1] ||

           me.l[ln].b[2].p[lf2][0] & me.l[ln].b[2].p[rh0][0] ||
           me.l[ln].b[2].p[lf2][0] & me.l[ln].b[2].p[rh1][0] ||
           me.l[ln].b[2].p[lf2][0] & me.l[ln].b[2].p[rh2][0] ||
           me.l[ln].b[2].p[lf2][0] & me.l[ln].b[2].p[rh3][1] ||

           me.l[ln].b[2].p[rf0][0] & me.l[ln].b[2].p[rh0][0] ||
           me.l[ln].b[2].p[rf0][0] & me.l[ln].b[2].p[rh1][0] ||
           me.l[ln].b[2].p[rf0][0] & me.l[ln].b[2].p[rh2][0] ||
           me.l[ln].b[2].p[rf0][0] & me.l[ln].b[2].p[rh3][1] ||

           me.l[ln].b[2].p[rf1][0] & me.l[ln].b[2].p[rh0][0] ||
           me.l[ln].b[2].p[rf1][0] & me.l[ln].b[2].p[rh1][0] ||
           me.l[ln].b[2].p[rf1][0] & me.l[ln].b[2].p[rh2][0] ||
           me.l[ln].b[2].p[rf1][0] & me.l[ln].b[2].p[rh3][1] ||

           me.l[ln].b[2].p[rf2][0] & me.l[ln].b[2].p[rh0][0] ||
           me.l[ln].b[2].p[rf2][0] & me.l[ln].b[2].p[rh1][0] ||
           me.l[ln].b[2].p[rf2][0] & me.l[ln].b[2].p[rh2][0] ||
           me.l[ln].b[2].p[rf2][0] & me.l[ln].b[2].p[rh3][1] ||

#if 0 //棋盘碰撞
            me.l[2].wd[10] & me.l[ln].b[2].p[rh0][0]    ||
            me.l[2].wd[10] & me.l[ln].b[2].p[rh1][0]    ||
            me.l[2].wd[10] & me.l[ln].b[2].p[rh2][0]    ||
            me.l[2].wd[10] & me.l[ln].b[2].p[rh3][1]    ||
#endif
           0;
}

static double dg2 = 0, dg1 = 0, gmrh1b = 0, vct2_ = 0, vct1_ = 0;
#include "InverseDynamics.cpp"
void *thr_fn20(void *)
{
    if (Ddv != 2) { return (nullptr); }
    ThreadName[20] = "抓取工作";
    /* double nhu  =  0.0, nht  =  0.0, nhh  =  0.0; */

    while (run0)
    {
        while (!kM) { usleep(20 * 1000); }
        me.l[1].b[0].p[rh0][0].d[0].s = -90;
        usleep(uint(90 / fpw) * 1000 * 1000);
        me.l[1].b[0].p[ubd][0].d[2].s = 100;
        usleep(uint(90 / fpw) * 1000 * 1000);
        tspw = 1;
        me.l[2] = me.l[1];

        while (kM)
        {
            int Catch = 0, tout = 0;
            bool Crash = false;
            while (!Catch)
            {
//#define Mechanical_arm_data /* Mechanical_arm_data */
#ifdef Mechanical_arm_data
                obj j0;
                j0.e = me.cus[10].e + p3d(66 * mm, 0, 0);
                obj j1;
                j1.e = me.l[2].b[2].ubd[0].d[2].O();
#else
                p3d j4 = me.cus[10].e;                    // 要抓取的目标
                p3d j3 = j4 + me.cus[10].vy() * 66 * mm;  // 目标上方关节
                p3d j0 = me.l[2].b[2].p[ubd][0].d[2].O(); // 肩膀关节
#endif
#ifdef Mechanical_arm_data
                fcircle crh3(p3d(0, 0, 0), 140 * mm, p3d(0, 1, 0)),
                    crh345(crh3 * getm(0, 90, 0) * getm(-45, 0, 0) /**/ + j0.e);

#else
                // fcircle c3(/* me.cus[10].out() *  */getm(0, 45, 0) * getm(90, 0, 0) *
                // (fcircle(p3d(0, 0, 0), 140 * mm, p3d(0, 1, 0)))+j3);
                fcircle c3(me.cus[10].out() * (getm(0, 45, 0) * getm(90, 0, 0) *
                                               (fcircle(p3d(0, 0, 0), 140 * mm, p3d(0, 1, 0)))) +
                           j3); // 立着的45度偏转圆圈
#endif
                fball b0b(j0, (140 + 140) * mm, me.l[2].b[2].p[ubd][0].d[2].n()); // 一二两关节的和
                fplane p3(j3, j3 - j4); // 爪子的法平面
                // 圆环的允许区域
                // 一二两关节能到达的区域 & 棋子的上半部分区域
                farea a2(shave(b0b, c3) & shave(p3, c3) /**/);
                // 关节2
                p3d j2(c3.out() * pola(0, a2.mid() + dg2, c3.r).e());
                fball b2(j2, 140 * mm, j2 - j3); // 关节2活动范围
                fball b0(me.l[2].b[2].p[ubd][0].m *
                         fball(me.l[2].b[0].p[ubd][0].d[2].O(), 140 * mm,
                               me.l[2].b[0].p[ubd][0].d[2].n())); // 关节0活动范围
                fcircle c1(b2 & b0);                              // b0 b2交集
                // 限位平面
                fplane p2(j2, j2 - j3), p0(j0, -me.l[2].b[2].p[ubd][0].d[2].n()),
                    p0u(me.l[2].b[2].p[ubd][0].m *
                        fplane(me.l[2].b[0].p[ubd][0].d[2].O(),
                               getm(0, 90, 180 - 160) * me.l[2].b[0].p[ubd][0].d[2].n())),
                    p0d(me.l[2].b[2].p[ubd][0].m *
                        fplane(me.l[2].b[0].p[ubd][0].d[2].O(),
                               getm(0, 90, -(180 - 160) - 180) * me.l[2].b[0].p[ubd][0].d[2].n()));
                farea a1 = shave(p2, c1) & shave(p0, c1) & (shave(p0u, c1) | shave(p0d, c1));
                p3d j1 = c1.out() * pola(0, a1.mid(0) + dg1, c1.r).e(); // 关节1

                fplane p1(j1, j1 - j0);
                farea a2p = (a2 & shave(p1, c3));

#if 0                                                                  //绘制辅助线
                //me.dline20(b0b.linesa(fc_green, farea(-90, 90), farea(-180, 180)));
                //me.dline20(b0.linesa(fc_blue, farea(-90, 0), farea(-160, 160)), 2);
                //me.dline20(b2.linesa(fc_blue, farea(-90, 90), farea(-180, 180)), 2);
                me.dline20(p0u.linesa(fc_green), 1);
                me.dline20(p0d.linesa(fc_blue), 1);
                me.dot(c3.out() * pola(0, a2.min(0), c3.r).e(), fc_white, 10);
                me.dot(c3.out() * pola(0, a2.max(0), c3.r).e(), fc_black, 10);
                me.dot(c3.out() * pola(0, a2.min(1), c3.r).e(), fc_yellow, 10);
                me.dot(c3.out() * pola(0, a2.max(1), c3.r).e(), fc_blue, 10);
                me.dline20(p1.linesa(fc_white));
                me.dline20(p0.linesa(fc_cyan));
                me.dline20(c3.linesa(fc_green));

                me.dot(j1, fc_red, 8);
                me.dot(j2, fc_red, 10);
                me.dot(j3, scalar(255, 100, 0));

                me.dline20(c3.lines(fc_cyan, a2, +10), 3);
                me.dline20(c3.lines(fc_yellow, a2 & a2p, +5), 6);
                me.dline20(c3.lines(fc_blue, shave(b0b, c3), -5), 3);
                me.dline20(c3.lines(fc_red, shave(p1, c3), -10), 3);
                me.dline20(c3.lines(fc_green, shave(p3, c3), -15), 3);

                me.dline20(c1.linesa(fc_yellow));
                me.dline20(c1.lines(fc_red, shave(p2, c1), +5), 3);
                me.dline20(c1.lines(fc_purple, shave(p0, c1), -5), 3);
                me.dline20(c1.lines(fc_yellow, a1, -10), 6);
                me.dline20(c1.lines(fc_green, shave(p0u, c1), -15), 3);
                me.dline20(c1.lines(fc_cyan, shave(p0d, c1), -20), 3);
                me.dline20(c1.lines(fc_white, shave(p0u, c1) | shave(p0d, c1), -25), 3);
                me.dline20(c1.plane() & p2, 1);
                sem_post(&sig[20]);
#endif
                std::vector<std::string> msg;

                fmove(j0, /* c2 */ j1, &me.l[2].b[0].p[ubd][0].d[2], &me.l[2].b[0].p[rh0][0].d[0],
                      me.l[2].b[1].p[ubd][0] << me.l[2].b[1].p[me.l[2].tp][0], msg, 0, 0);
                me.l[2].m();

                fmove(j1, /* c2 */ j2, &me.l[2].b[0].p[rh0][0].d[1], &me.l[2].b[0].p[rh1][0].d[0],
                      me.l[2].b[1].p[rh0][0] << me.l[2].b[1].p[me.l[2].tp][0], msg, 0, 90);
                me.l[2].m();

                fmove(j2, /* c2 */ j3, &me.l[2].b[0].p[rh1][0].d[1], &me.l[2].b[0].p[rh2][0].d[0],
                      me.l[2].b[1].p[rh1][0] << me.l[2].b[1].p[me.l[2].tp][0], msg, 0, 90);
                me.l[2].m();

                fmove(j3, /* c2 */ me.cus[10].e, &me.l[2].b[0].p[rh3][0].d[0],
                      &me.l[2].b[0].p[rh3][1].d[0],
                      me.l[2].b[1].p[rh2][0] << me.l[2].b[1].p[me.l[2].tp][0], msg, 0, 90);
                me.l[2].m();

                tcr.e = me.l[2].b[2].p[rh3][2].c.pt[0][0][0][0] -
                        (me.l[2].b[2].p[rh3][2].c.pt[0][0][0][0] -
                         me.l[2].b[2].p[rh3][3].c.pt[1][0][1][0]) /
                            2;

#if 0
                me.dline20(me.l[2].b[2].rh3[0][2].o, (me.l[2].b[2].rh3[0][2].x-me.l[2].b[2].rh3[0][2].o) * 100 * mm+me.l[2].b[2].rh3[0][2].o, fc_red, 12);
                me.dline20(me.l[2].b[2].rh3[0][2].o, (me.l[2].b[2].rh3[0][2].y-me.l[2].b[2].rh3[0][2].o) * 100 * mm+me.l[2].b[2].rh3[0][2].o, fc_green, 12);
                me.dline20(me.l[2].b[2].rh3[0][2].o, (me.l[2].b[2].rh3[0][2].z-me.l[2].b[2].rh3[0][2].o) * 100 * mm+me.l[2].b[2].rh3[0][2].o, fc_blue, 12);
#endif

                if (dnorm(tcr.e - me.cus[10].e) < /** 精确度 */ 3)
                {
                    Catch = 1;
                    me.dot(j4 + 20 * mm * me.cus[10].vy(), fc_green, 12);
                    break;
                }
                else
                {
                    dg2 -= 3 /** 搜索步长 */;
                    me.dot(j4 + 10 * mm * me.cus[10].vy(), rnds(), 12);
                    tout++;
                }
                if (tout > 15)
                {
                    tout = 0;
                    break;
                }
            }
            Crash = crash(2);

            if (!Catch)
            {
                if (dnorm(me.cus[10].e - me.l[2].b[2].p[ubd][0].d[2].O()) < 260 * mm)
                {
                    me.l[2].b[0].p[lf0][0].d[2].s -= 3;
                    me.l[2].b[0].p[rf0][0].d[2].s -= 3;
                }

                if (dnorm(me.cus[10].e - me.l[2].b[2].p[ubd][0].d[2].O()) >= 260 * mm)
                {
                    me.l[2].b[0].p[lf0][0].d[2].s += 3;
                    me.l[2].b[0].p[rf0][0].d[2].s += 3;
                }
            }

            if (Catch && Crash)
            {
                me.l[2].b[0].p[lf0][0].d[2].s -= 1;
                me.l[2].b[0].p[rf0][0].d[2].s -= 1;
            }
            int Carash2 = 0, notInPlace = 1;
            if (Catch && !Crash)
            {
#if 1
                me.l[3] = me.l[1];
                me.l[2].b[0].setpower(192);
                while (notInPlace)
                {
                    notInPlace = (me.l[3].b[0].p[ubd][0].d[2] >> me.l[2].b[0].p[ubd][0].d[2]) +
                                 (me.l[3].b[0].p[rh0][0].d[0] >> me.l[2].b[0].p[rh0][0].d[0]) +
                                 (me.l[3].b[0].p[rh0][0].d[1] >> me.l[2].b[0].p[rh0][0].d[1]) +
                                 (me.l[3].b[0].p[rh1][0].d[0] >> me.l[2].b[0].p[rh1][0].d[0]) +
                                 (me.l[3].b[0].p[rh1][0].d[1] >> me.l[2].b[0].p[rh1][0].d[1]) +
                                 (me.l[3].b[0].p[rh2][0].d[0] >> me.l[2].b[0].p[rh2][0].d[0]) +
                                 (me.l[3].b[0].p[rh3][0].d[0] >> me.l[2].b[0].p[rh3][0].d[0]) +
                                 (me.l[3].b[0].p[rh3][1].d[0] >> me.l[2].b[0].p[rh3][1].d[0]) +
                                 (me.l[3].b[0].p[rh3][2].d[0] >> me.l[2].b[0].p[rh3][2].d[0]) +
                                 (me.l[3].b[0].p[rh3][3].d[0] >> me.l[2].b[0].p[rh3][3].d[0]);
                    me.l[3].m();
                    Carash2 = Carash2 || crash(3);
#if 0
                    usleep(50 * 1000); //停下以显示碰撞位置
#endif
                }
#endif
                if (Carash2)
                {
                    /*  ... ... */
                    int b = 1;
                    b++;
                }
                else
                {
                    me.l[1] = me.l[2];
                    me.l[1].b[0].setpower(fpw);
                    me.l[1].b[0].p[ubd][0].d[0].s_ -= 1; //动一下
                }
            }
            gmsg[3] = str(dnorm(me.cus[10].e - me.l[2].b[2].p[ubd][0].d[2].O()) / mm);
            dg2 = 0;
            usleep(2000);
        }
        me.l[1].b[0].p[ubd][0].d[2].s = 120;
        me.l[1].b[0].p[rh0][0].d[0].s = -90;
        usleep(uint(90 / fpw) * 1000 * 1000);

        me.l[1].b[0].p[ubd][0].d[2].s = 0;
        usleep(uint(90 / fpw) * 1000 * 1000);
        me.l[1].b[0].p[rh0][0].d[0].s = 0;
        me.l[1].b[0].p[rh0][0].d[1].s = 0;
        me.l[1].b[0].p[rh1][0].d[0].s = 0;
        me.l[1].b[0].p[rh1][0].d[1].s = 0;
        me.l[1].b[0].p[rh2][0].d[0].s = 0;
        me.l[1].b[0].p[rh3][0].d[0].s = 0;
        me.l[1].b[0].p[rh3][1].d[0].s = 0;
        me.l[1].b[0].p[rh3][2].d[0].s = 0;
        me.l[1].b[0].p[rh3][3].d[0].s = 0;
    }
    return (nullptr);
}

void *thr_fn21(void *)
{
    if (Ddv != 2) { return (nullptr); }
    while (frm99l.cols == 0 || frm99r.cols == 0) { usleep(50 * 1000); } //等待网络初始化
    ThreadName[21] = "下围棋";
    usleep(4000 * 1000);
    while (run0)
    {
        if (!kI)
        {
            usleep(100 * 1000);
            continue;
        }
        enb[16] = 1; //预加载线程16，防止大臂复位
        goout = 0;
        kN = 0;
        usleep(1000 * 1000);
        enb[16] = 0;
#if 0
        sndcmd(11, 0, 19, 3);
        usleep(10 * 1000 * 1000);
#endif
        me.l[1].b[0].p[hed][0].d[0].w(-28);
        me.l[1].b[0].p[rh0][0].d[0].w(85);
        usleep(5000 * 1000);
        me.l[1].b[0].p[ubd][0].d[2].w(95);
        usleep(5000 * 1000);
        // me.l[1].b[0].rh0[0].d[0].w(15);
        me.l[1].b[0].p[rh1][0].d[0].w(75);
        usleep(5000 * 1000);
        dgdo = 1;
        enb[27] = 1;
        enb[16] = 1;
        int outt = 0;
        gmsg[1] = "棋盘识别...";
        int l = 0;
        while (!(gdo1.size() == 9))
        {
            usleep(200 * 1000);
            outt++;
            if (outt > 20)
            {
                me.cus[10] = obj(p3d((l - 0.5) * 2 * 50 * mm, -150 * mm, 240 * mm),
                                 p3d(10 * mm, 10 * mm, 10 * mm), p3d(0, 0, 0), scalar(156, 42, 83));
                l = !l;
                me.cusp[0] = &me.cus[10];
                scs = 1;
                outt = 0;
            }
        }

        gmsg[4] = "棋盘识别成功!按J继续";
        wtj();

        enb[27] = 0;
        enb[34] = 1;
        enb[35] = 1;
        while (!chesso.size())
        {
            gmsg[1] = "棋子识别..." + str(outt) + "数量:" + str(int(chesso.size()));
            outt++;
            usleep(200 * 1000);
            if (outt > 20)
            {
                me.cus[10] = obj(50 * mm, -530 * mm, 240 * mm);
                me.cusp[0] = &me.cus[10];
                scs = 1;
                outt = 0;
            }
        }
        gmsg[4] = "棋子数量" + str(int(chesso.size()));
        int t = 0, csfd = 0;
        obj chesstmp;
        while (t < 10 || !csfd)
        {
            gmsg[1] = "计算棋子坐标..." + str(t);
            for (uint i = 0; i < chesso.size(); i++)
            {
                axis l(chesso[i].e, me.l[1].b[2].p[ubd][0].d[2].b, 0);
                me.dlinef(l);
                printf(" %f ", l.d() / mm);
                if (l.d() < 350 * mm && chesso[i].id == 2)
                { // id 1:白棋 2:黑棋
                    chesstmp = chesso[i];
                    me.cusp[0] = &chesso[i];
#if 1
                    gmsg[4] = "坐标(" + str(me.cusp[0]->e.x / mm) + ", " +
                              str(me.cusp[0]->e.y / mm) + ", " + str(me.cusp[0]->e.z / mm) + ")";
#endif
                    csfd++;
                }
            }
            usleep(200 * 1000);
            t++;
        }

        gmsg[1] = "确定目标坐标位置!按J继续";
        wtj();
        enb[34] = 0;
        enb[35] = 0;
        enb[16] = 0;

        me.cus[10] = chesstmp;

        kM = 1;
        kM1 = 1;

        me.cus[10].e.y += 50 * mm;

        outt = 0;
        usleep(2000 * 1000);

        double er = +0 * mm;
        gmsg[1] = "已移动到棋子上方 按J继续";
        usleep(2000 * 1000);
        wtj();
        me.cusp[0] = &me.l[1].b[2].p[rh3][0];
        me.cus[10].e.y -= 50 * mm + er;
        usleep(2000 * 1000);
        gmsg[1] = "已移动到棋子位置 按J继续";
        wtj();
        sendcmd(1, 1, 17, 100, 100, 12, 2);
        usleep(2000 * 1000);
        gmsg[1] = "抓住了棋子  按J继续";
        wtj();
        me.cus[10].e.y += 50 * mm + er;
        gmsg[1] = "提起了棋子  按J继续";
        wtj();
        usleep(2000 * 1000);
        uint ccs = 0, crs = 0;
        me.cus[10].e = gdo1[ccs][crs].e;
        me.cus[10].e.y += 50 * mm + er;
        me.cusp[0] = &me.cus[10];
        usleep(3 * 1000 * 1000);

        outt = 0;
        gmsg[1] = "已移动到落子坐标上方  按J继续";
        wtj();
        me.cus[10].e.y -= 50 * mm + er;
        me.cusp[0] = &me.cus[10];
        usleep(3000 * 1000);
        gmsg[1] = "已移动到落子位置 按J继续";
        wtj();
        sendcmd(1, 1, 17, 255, 252, 12, 2);
        gmsg[1] = "松开了棋子 按J结束";
        wtj();
        me.cusp[0] = &me.l[1].b[2].p[rh3][0];
        kM1 = 0;
        usleep(400 * 1000);
        me.l[1].b[0].p[ubd][0].d[2].s += 20;
        me.l[1].b[0].p[rh0][0].d[0].w(85);
        usleep(3000 * 1000);
        scsl = 1;
        me.cus[10] = gdo1[5][5];
        me.cusp[0] = &me.cus[10];

        me.l[1].b[0].p[rh1][0].d[0].w(0);
        me.l[1].b[0].p[rh0][0].d[1].w(0);
        // std::cout<<std::endl<< "棋子翻面..." ;
        usleep(1000 * 1000);
        // std::cout<<std::endl<< "拿起棋子..." ;
        usleep(1000 * 1000);
        // std::cout<<std::endl<< "落子..." ;
        usleep(1000 * 1000);
        // std::cout<<std::endl<< "提子..." ;
        usleep(1000 * 1000);
        // std::cout<<std::endl<< "放下棋子..." ;
        /* ...... */
        me.l[1].b[0].p[rh0][0].d[0].w(85);
        usleep(5000 * 1000);
        me.l[1].b[0].p[ubd][0].d[2].w(0);
        me.l[1].b[0].p[hed][0].d[0].w(0);
        usleep(8000 * 1000);
        me.l[1].b[0].p[rh0][0].d[0].w(0);
        gmsg[1] = "结束...";
        for (uint x = 0; x < gdv.size(); x++)
        {
            for (uint y = 0; y < gdv[0].size(); y++) { gdv[x][y].e() = cv::Point3d(0); }
        }
        dgdo = 0;
        me.cusp[0] = &me.cus[20];
        if (DetectedFaceObj.size())
        {
            me.cus[10] = DetectedFaceObj[0];
            me.cusp[0] = &me.cus[10];
        }
        if (FaceObj.size())
        {
            me.cus[10] = FaceObj[0];
            me.cusp[0] = &me.cus[10];
        }

        enb[34] = 0;
        enb[35] = 0;
        scsl = 0;
        kM = 0;
        // kI = 0;
        gmsg[0] = "下围棋:" + soc(kI);
        gdo1.clear();
    }
    return (nullptr);
}
#endif
int run()
{
    fystart();
    Ddv = Dv;

    me.l[0].init(ewd, eht);
    me.l[1].init(ewd, eht);
    me.l[2].init(ewd, eht);
    me.l[3].init(ewd, eht);

    fyml.c = me.l[0].c;
    fyml.c2 = me.l[0].c2;
    fyml.fl = me.l[0].sfl;
    fyml.fr = me.l[0].sfr;
    fyml.ap = me.l[0].ap;
    fyml.bt = me.l[0].bt;
    fyml.l_vc_lp = me.l[0].vc.lp;
    fyml.l_vc_rp = me.l[0].vc.rp;
    fyml.l_vch_lp = me.l[0].vch.lp;
    fyml.l_vch_rp = me.l[0].vch.rp;
    fyml.l_lcsz = me.l[0].lcsz;
    fyml.l_rcsz = me.l[0].rcsz;
    fyml.ewd = ewd;
    fyml.eht = eht;
    fyml.rpx = me.l[0].rpx;

    me.drt(me.l[0].b[0]);
    me.drt(me.l[1].b[0]);

    // me.l[0].bd[0].mtp[0] = -1;                                      //头归位

    me.l[0].b[0].mt[1] = short(me.l[0].b[0].od[1][3]);
    pthread_create(&ntid[16], &attr, thr_fn16, nullptr);
    pthread_create(&ntid[24], &attr, thr_fn24, nullptr);
    pthread_create(&ntid[29], &attr, thr_fn29, nullptr);
    pthread_create(&ntid[28], &attr, thr_fn28, nullptr);

#ifndef FYAIRO_1_0_0_ARM
    pthread_create(&ntid[17], &attr, thr_fn17, nullptr);
    pthread_create(&ntid[18], &attr, thr_fn18, nullptr);
    pthread_create(&ntid[19], &attr, thr_fn19, nullptr);
    pthread_create(&ntid[20], &attr, thr_fn20, nullptr);
    pthread_create(&ntid[21], &attr, thr_fn21, nullptr);
    pthread_create(&ntid[22], &attr, thr_fn22, nullptr);
#if 1
    pthread_create(&ntid[23], &attr, thr_fn23, nullptr);
#endif
#endif

    return 1;
}
