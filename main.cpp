#include "i.cpp"
int kM1 = 0, kM = 0, kH = 0, kN = 0, kI = 0, kT = 1, kJ = 0, kL = 0, kC = 0,
    /** 是否通过云端*/ Dc = 0;

#include "globjld.cpp"
static d2l d3;
static double args[100];
static int glok = 0;
static cv::Mat cr0(2, 3, CV_64FC1), cr1(2, 3, CV_64FC1);
#ifndef FYAIRO_1_0_0_ARM
static cv::Mat mlg;
static int /*msx, msy, msx1, msy1, t, mldrg, mrdrg, */ x_, y_, /*kkey, */
    k0 = 0, k1 = 0, k2 = 0, k3 = 0, k4 = 0, k5 = 0, k6 = 0, k7 = 0, k8 = 0, k9 = 0, kQ = 0, kW = 0,
    kE = 0, kR = 0, kx = 0, kdl = 0, kt = 0, kV = 1, /*ku=1, */ ky = 0, kb = 0, ksp = 0, kA = 0,
    kS = 0, kD = 0, kO = 0, kP = 0, kF = 0, kG = 0, kB = 0 /*, K01=0*/;
static int argn = 0, mw = 1, frmarv = 0, vplock;

static std::deque<obj> tagt, tagtp;
static std::deque<cv::Mat> tagp;
#include <opencv2/freetype.hpp>
static cv::Ptr<cv::freetype::FreeType2> ft2 = cv::freetype::createFreeType2();
std::vector<string> mkvstr(string a = "", string b = "", string c = "", string d = "",
                           string e = "", string f = "", string g = "", string h = "",
                           string i = "", string j = "", string k = "", string l = "")
{
    std::vector<string> rt;
    if (a != "") rt.push_back(a);
    if (b != "") rt.push_back(b);
    if (c != "") rt.push_back(c);
    if (d != "") rt.push_back(d);
    if (e != "") rt.push_back(e);
    if (f != "") rt.push_back(f);
    if (g != "") rt.push_back(g);
    if (h != "") rt.push_back(h);
    if (i != "") rt.push_back(i);
    if (j != "") rt.push_back(j);
    if (k != "") rt.push_back(k);
    if (l != "") rt.push_back(l);
    return rt;
}
/**只能在线程99中运行*/
void puttext(string txt[], p3d e = p3d(-640 * mm - 720, 690, 0), p3d sz = p3d(640, 100, 10),
             int fontsize = 42, scalar fc = scalar(0, 255, 0), scalar bc = scalar(125, 0, 0),
             gm44d m = gset(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1))
{
#if 1
    obj t;
    cv::Mat p(int(sz.y), int(sz.x), CV_8UC3, bc);
    for (ulong i = 0; i < 40; i++)
    {
        if (txt[i] != "")
        {
            ft2->putText(p, txt[i],
                         cv::Point2d(fontsize * 0.5, fontsize * 1.5 + 1.2 * fontsize * i), fontsize,
                         fc, -1, 8, true);
        }
    }
    t.e = me.l[2].worldm * e;
    t.sz = sz;
    t.m = m /*getm(cga(me.pm[2]))*/;
    cv::Mat pt = translucent(p, pt4d(1, 1, 1, 0.62));
    tagp.push_back(pt);
    pt.release();
    tagt.push_back(t);
#endif
}
/**只能在线程99中运行*/
void puttext(std::vector<string> txt, p3d e = p3d(-640 * mm - 720, 690, 0),
             p3d sz = p3d(640, 100, 10), int fontsize = 42, scalar fc = scalar(0, 255, 0),
             scalar bc = scalar(125, 0, 0),
             gm44d m = gset(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1))
{
    string txt_[40];
    for (ulong i = 0; i < txt.size(); i++) { txt_[i] = txt[i]; }
    puttext(txt_, e, sz, fontsize, fc, bc, m);
}

/*************************GL****************************/
struct mss
{
    int x, y, s;
};
struct regp
{
    double d;
    p3d p;
    p3d arg; /**fun*/
    int f;
    int n;
    regp()
    {
        p = p3d(0, 0, 0);
        arg = p3d(0, 0, 0);
        d = DBL_MAX;
        f = -1;
        n = -1;
    }
    regp(p3d _p, p3d _arg = p3d(0, 0, 0), double _d = 0, int /**按钮*/ _f = 0, int /**功能*/ _n = 0)
    {
        p = _p;
        arg = _arg;
        d = _d;
        f = _f;
        n = _n;
    }
};
static mss ms[5];
static p3d ms3;
static regp vp0p, vp0p1, vp0pf1;
static axis ms3l;
static fysize wsz;
static vector<regp> vp;
static scalar downcolor;

struct mousestate
{
    int x, y, btn, state, key, ref;
};
static mousestate mousest;

static GLfloat diffuseMaterial[4] = {0.5, 0.5, 0.5, 1.0};
static p3d desm, G, cuspa;
int sbsa, sbsb;
static std::vector<axis> ax;

void keyPressed(unsigned char key, int x, int y)
{
    x = y;
    axis tmp(cuspa, me.cus[10].e, 0);
    tmp.ca = scalar(255, 255, 255);
    tmp.cb = scalar(0, 0, 255);
    tmp.linewidth = 2;
    string s;
    if (key == 127)
    {
        kdl++;
        if (kdl > 1) kdl = 0;
        gmsg[0] = "键盘编号:" + str(kdl);
    }
    if (kdl == 0)
    {
        switch (key)
        {
        case 27: run1 = false; break;
        case 'a':
            me.pm[0].x += 100 * mm;
            me.pm[3].x += 100 * mm;
            break;
        case 'd':
            me.pm[0].x -= 100 * mm;
            me.pm[3].x -= 100 * mm;
            break;
        case 'r':
            me.pm[0].y += 100 * mm;
            me.pm[3].y += 100 * mm;
            break;
        case 'f':
            me.pm[0].y -= 100 * mm;
            me.pm[3].y -= 100 * mm;
            break;
        case 'w':
            me.pm[0].z += 100 * mm;
            me.pm[3].z += 100 * mm;
            break;
        case 's':
            me.pm[0].z -= 100 * mm;
            me.pm[3].z -= 100 * mm;
            break;
        case 177:
            me.pm[0].y += 1 * mm;
            me.pm[3].y += 1 * mm;
            break;
        case 176:
            me.pm[0].y -= 1 * mm;
            me.pm[3].y -= 1 * mm;
            break;
        case 'b': kb = kb == 0 ? 1 : 0; break;
        case 'u': sendcmd(1, 1, 17, 255, 252, 12, 2); break;
        case 'i': sendcmd(1, 1, 17, 100, 100, 12, 2); break;
        case 'h': sendcmd(7, 0, 0x11, 3); break;
        case 'j': sendcmd(7, 1, 0x11, 3); break;
        case 'k': me.cus[0].b = t1v[0].vo().b(); break;
        case 'x': kx = !kx; break;
        case 't': kt = 1; break;
        case 'y': ky = 1; break;
        case 'M':
            kM = kM == 1 ? 0 : 1;
            kM1 = kM1 == 1 ? 0 : 1;
            ax.clear();
            gmsg[0] = "触摸目标:" + soc(kM);
            break;
        case '`':
            MyRobots = AllRobots[uint(k0)];
            gmsg[1] = "调试目标:(" + str(k0) + ")" + MyRobots[0].name;
            gmsg[2] = "编号:" + str(MyRobots[0].id);
            gmsg[3] = "IP地址:" + MyRobots[0].ip;
            gmsg[4] = "视觉端口:" + str(MyRobots[0].eyeport);
            if (!enb[99]) cout << "[" << k0 << "]";
            k0++;
            k0 = k0 < int(AllRobots.size()) ? k0 : 0;
            break;
        case '~':
            MyRobots = AllRobots[uint(k0)];
            gmsg[1] = "调试目标:(" + str(k0) + ")" + MyRobots[0].name;
            gmsg[2] = "编号:" + str(MyRobots[0].id);
            gmsg[3] = "IP地址:" + MyRobots[0].ip;
            gmsg[4] = "视觉端口:" + str(MyRobots[0].eyeport);
            if (!enb[99]) cout << "[" << k0 << "]";
            k0--;
            k0 = k0 > -1 ? k0 : int(AllRobots.size()) - 1;
            break;
        case '1': k1 = k1 == 1 ? 0 : 1; break;
        case 32:
            ksp = 1;
            ax.push_back(tmp);
            cuspa = me.cus[10].e;
            break;
        case '2': k2 = k2 == 1 ? 0 : 1; break;
        case '3': k3 = k3 == 1 ? 0 : 1; break;
        case '4': k4 = k4 == 1 ? 0 : 1; break;
        case '5': k5 = k5 == 1 ? 0 : 1; break;
        case '6': k6 = k6 == 1 ? 0 : 1; break;
        case '7': k7 = k7 == 1 ? 0 : 1; break;
        case '8': k8 = k8 == 1 ? 0 : 1; break;
        case '9': k9 = k9 == 1 ? 0 : 1; break;
        case 'Q': kQ = kQ == 1 ? 0 : 1; break;
        case 'W': kW = kW == 1 ? 0 : 1; break;
        case 'E': kE = kE == 1 ? 0 : 1; break;
        case 'R': kR = kR == 1 ? 0 : 1; break;
        case 'A': kA = kA == 1 ? 0 : 1; break;
        case 'S': kS = kS == 1 ? 0 : 1; break;
        case 'D': kD = kD == 1 ? 0 : 1; break;
        case 'F': kF = kF == 1 ? 0 : 1; break;
        case 'N':
            kN = kN == 1 ? 0 : 1;
            gmsg[0] = "检测人脸:" + soc(kN);
            break;
        case 'I':
            kI = kI == 1 ? 0 : 1;
            gmsg[0] = "下围棋:" + soc(kI);
            break;
        case 'O': kO = kO == 1 ? 0 : 1; break;
        case 'P': kP = kP == 1 ? 0 : 1; break;
        case 'L':
            kL = kL < 4 ? kL + 1 : 0;
            if (kL == 0) s = "无";
            if (kL == 1) s = "水平";
            if (kL == 2) s = "垂直";
            if (kL == 3) s = "旋转xz";
            if (kL == 4) s = "旋转xy";
            gmsg[1] = "移动光标方式:" + s;
            break;
        case 'c':
            RsvRobId = MyRobots[0].id;
            gmsg[6] = "请求从云端接收" + MyRobots[0].name + "的视频";
            cout << gmsg[6];
            break;
        case 'Z':
            args1 *= 1.01;
            gmsg[2] = "args1->" + str(args1);
            break;
            /*wdn hi++;hi=hi>4?1:hi;puttext("\n挥手 %d", hi)*/;
        case 'z':
            args1 /= 1.01;
            gmsg[2] = "args1->" + str(args1);
            /*sndcmd(1, 0); sndcmd(2, char(me.l[0].bd[0].od[ 1][ 3]));*/
#ifdef Xdbg
            puttext("{@1%d}", wdn);
#endif
            break;
        case 'C':
            kC = kC == 1 ? 0 : 1;
            gmsg[0] = "相机校正:" + soc(kC);
            break;
        case 'G':
            kG = kG == 1 ? 0 : 1;
            enb[27] = kG;
            dgdo = kG;
            gmsg[0] = "找围棋盘:" + soc(kG);
            break;
        case '+':
            if (argn < 80) argn++;
            gmsg[2] = str(argn);
            break;
        case '-':
            if (argn > 0) argn--;
            gmsg[2] = str(argn);
            break;
        case '*':
            gmsg[2] = str(argn) + str(args[argn]);
            args[argn] = args[argn] * 1.01;
            args[argn + 50]++;
            gmsg[3] = str(args[argn]);
            break;
        case '/':
            gmsg[2] = str(argn) + str(args[argn]);
            args[argn] = args[argn] / 1.01;
            args[argn + 50]--;
            gmsg[3] = str(args[argn]);
            break;
        case 'B':
            kB = !kB;
            if (kB) { scsl = 1; }
            else
            {
                scsl = 0;
            }
            gmsg[0] = "盯着目标:" + soc(scsl);
            break;
        case 'V':
            kV++;
            kV = kV > 6 ? 0 : kV;
            gmsg[2] = "立体模式:" + str(kV);
            for (int v = 0; v < eht; v++)
            {
                for (int u = 0; u < ewd; u++)
                {
                    d3.dots[u][v].x = 0;
                    d3.dots[u][v].y = 0;
                    d3.dots[u][v].z = 0;
                    d3.dots[u][v].r = 0;
                    d3.dots[u][v].g = 0;
                    d3.dots[u][v].b = 0;
                }
            }
            break;
        case 'X': /*wdn=rnd(3)*/;
            kX = !kX;
            if (kX)
            {
                me.l[1].b[0].p[rh3][0].d[0].w(-90, 4);
                me.l[0].b[0].p[rh3][0].d[0].w(-90, 4);
            }
            else
            {
                me.l[1].b[0].p[rh3][0].d[0].w(0, 4);
                me.l[0].b[0].p[rh3][0].d[0].w(0, 4);
            }
            gmsg[3] = str(kX) + str(rnd(3)) + str(rnd(3.0));
            break;
        case '[':
            sbsa = sbsa > 0 ? sbsa - 1 : 0;
            gmsg[2] = "sbsa=" + str(sbsa);
            break;
        case ']':
            sbsa = sbsa < 40 ? sbsa + 1 : 0;
            gmsg[2] = "sbsa=" + str(sbsa);
            break;
        case ',':
            me.l[0].b[0].od[sbsa][3] += 1;
            gmsg[2] =
                "\n me.l[0].bd[0].od[" + str(sbsa) + "][3] -> " + str(me.l[0].b[0].od[sbsa][3]);
            break;
        case '.':
            me.l[0].b[0].od[sbsa][3] -= 1;
            gmsg[2] =
                "\n me.l[0].bd[0].od[" + str(sbsa) + "][3] -> " + str(me.l[0].b[0].od[sbsa][3]);
            break;
        case 'm':
            me.l[0].b[0].save("/home/root/eye/dt/view_bd[0].yml");
            gmsg[2] = "me.l[0].bd[0]>>('/home/root/eye/dt/view_bd[0].yml')";
            break;
        case 'n':
            me.l[1].b[0].read("/home/root/eye/dt/view_bd[0].yml");
            gmsg[2] = "me.l[0].bd[0]<<('/home/root/eye/dt/view_bd[0].yml')";
            break;
        case 'J':
            kJ = !kJ;
            gmsg[2] = "继续[J]";
            me.dot11.clear();
            break;
            /**代主控发送测试命令**/
        case 1: sendcmd(0, 88, 3, 66, 77, 12, 3, 2, MyRobots); break;  // Ctrl+a  视频采集线程
        case 2: sendcmd(6, 88, 3, 66, 77, 12, 3, 2, MyRobots); break;  // Ctrl+b  二维码识别
        case 3: sendcmd(16, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+c  空间感知
        case 4: sendcmd(100, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+d  人脸识别左
        case 5: sendcmd(101, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+e  人脸识别右
        case 6: sendcmd(80, 88, 3, 66, 77, 12, 3, 2, MyRobots); break;  // Ctrl+f  视频发送
        case 7: sendcmd(99, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+g  启动可视化线程
        case 8: sendcmd(91, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+h 视频远程Udp采集线程
        case 9: sendcmd(102, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+i  精确人脸识别左
        case 10: sendcmd(103, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+j  精确人脸识别右
        case 11: sendcmd(161, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+k  云模式
        case 12: sendcmd(161, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+l
        case 13: sendcmd(3, 88, 3, 66, 77, 12, 3, 2, MyRobots); break;   // Ctrl+m  raw视频采集
        case 14: sendcmd(79, 88, 3, 66, 77, 12, 3, 2, MyRobots); break;  // Ctrl+n  raw视频发送
        case 15: sendcmd(30, 88, 3, 66, 77, 12, 3, 2, MyRobots); break;  // Ctrl+o  人脸检测左
        case 16: sendcmd(31, 88, 3, 66, 77, 12, 3, 2, MyRobots); break;  // Ctrl+p  人脸检测右
        case 17: sendcmd(1, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+q  视频编码程序
        case 18: sendcmd(81, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+r  服务器云视频发送
        case 19: sendcmd(83, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+s  PC端云视频接收
        case 20: sendcmd(32, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+t  物体识别左
        case 21: sendcmd(33, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+u  物体识别右
        case 22: sendcmd(34, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+v  棋子识别左
        case 23: sendcmd(35, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+w  棋子识别右
        case 24: sendcmd(82, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+x  视频接收
        case 25: sendcmd(84, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+x  raw视频接收
        case 26:
            sendcmd(86, 88, 3, 66, 77, 12, 3, 2, MyRobots);
            break; // Ctrl+z  服务器端Tcp发送命令
            /*******************/
        }
    }
    if (kdl == 1)
    {
        byte stt[8];
        switch (key)
        {
        case 1: sendcmd(87, 88, 3, 66, 77, 12, 3, 2, MyRobots); break; // Ctrl+a  客户端Tcp获取命令
        case 2: break;                                                 // Ctrl+b
        case 5: sendcmd(27, 65, 3, 66, 67, 12, 3, 2, MyRobots); break; // Ctrl+e  围棋盘识别
        case 6: sendcmd(stt, 16, 3, 3, 2, 1, MyRobots); break;         // Ctrl+f  状态请求

        case 'a': cr0.at<double>(0, 0, 0) += 0.01; break;
        case 's': cr0.at<double>(0, 1, 0) += 0.01; break;
        case 'd': cr0.at<double>(0, 2, 0) += 10; break;
        case 'z': cr0.at<double>(1, 0, 0) += 0.01; break;
        case 'x': cr0.at<double>(1, 1, 0) += 0.01; break;
        case 'c': cr0.at<double>(1, 2, 0) += 10; break;

        case 'A': cr0.at<double>(0, 0, 0) -= 0.01; break;
        case 'S': cr0.at<double>(0, 1, 0) -= 0.01; break;
        case 'D': cr0.at<double>(0, 2, 0) -= 10; break;
        case 'Z': cr0.at<double>(1, 0, 0) -= 0.01; break;
        case 'X': cr0.at<double>(1, 1, 0) -= 0.01; break;
        case 'C': cr0.at<double>(1, 2, 0) -= 10; break;

        case 'g': cr1.at<double>(0, 0, 0) += 0.01; break;
        case 'h': cr1.at<double>(0, 1, 0) += 0.01; break;
        case 'j': cr1.at<double>(0, 2, 0) += 10; break;
        case 'b': cr1.at<double>(1, 0, 0) += 0.01; break;
        case 'n': cr1.at<double>(1, 1, 0) += 0.01; break;
        case 'm': cr1.at<double>(1, 2, 0) += 10; break;

        case 'G': cr1.at<double>(0, 0, 0) -= 0.01; break;
        case 'H': cr1.at<double>(0, 1, 0) -= 0.01; break;
        case 'J': cr1.at<double>(0, 2, 0) -= 10; break;
        case 'B': cr1.at<double>(1, 0, 0) -= 0.01; break;
        case 'N': cr1.at<double>(1, 1, 0) -= 0.01; break;
        case 'M': cr1.at<double>(1, 2, 0) -= 10; break;
        case 19:
            d3.saveEyeMat();
            gmsg[1] = "保存校准矩阵";
            break; // ctrl+s
        case 18:
            d3.readEyeMat();
            gmsg[1] = "读取校准矩阵";
            break; // ctrl+r
        }
    }
    for (int i = 0; i < 100; i++) { d3.args[i] = args[i]; }
    fyml.fl = args[0];
    fyml.fr = args[0];
    fyml.ap = args[3] + 1;
    fyml.bt = args[4] + 1;

    gmsg[7] = "按键:" + str(key) + " " + char(key);
    mousest.key = key;
}

static int bn = 1;
void mouseMove(int x, int y)
{
    // puttext(" (x:%f , y:%f) ", me.pm[2].x, me.pm[2].y);
    if (ms[2].s == 1)
    {
        x_ = (x - ms[2].x);
        y_ = (y - ms[2].y);
        gmsg[1] = "x_:" + str(x_) + " y_:" + str(y_);
        if (!kx)
        {
            me.pm[2].x += x_ * 0.4;
            me.pm[2].y -= y_ * 0.4;
        }
        else
        {
            me.pm[2].z += x_ * 0.4;
        }
    }
    if (ms[1].s == 1)
    {
        x_ = (x - ms[1].x);
        y_ = (y - ms[1].y);
        if (!kx)
        {
            me.pm[0].x += -x_ * args[1] * 10 * mm;
            me.pm[3].x += -x_ * args[1] * 10 * mm;
            me.pm[0].y += -y_ * args[1] * 10 * mm;
            me.pm[3].y += -y_ * args[1] * 10 * mm;
        }
    }
    if (ms[0].s == 1)
    {
        x_ = (x - ms[0].x);
        y_ = (y - ms[0].y);
        if (k1)
        {
            me.l[bn].b[0].p[hed][0].d[0].s += -y_ * 0.05;
            me.l[bn].b[0].p[ubd][0].d[0].s += -x_ * 0.05;
#ifdef dbg
            gmsg[2] = " 抬头:" + str(me.l[bn].b[0].p[hed][0].d[0].s) +
                      " 转头:" + str(me.l[bn].b[0].p[ubd][0].d[0].s);
#endif
        }
        if (k2)
        {
            me.l[bn].b[0].p[ubd][0].d[1].s += -y_ * 0.05;
            me.l[bn].b[0].p[lh0][0].d[0].s += -x_ * 0.05;

#ifdef dbg
            gmsg[2] = " 抬起左上臂:" + str(me.l[bn].b[0].p[ubd][0].d[1].s);
            gmsg[3] = " 张开左上臂:" + str(me.l[bn].b[0].p[lh0][0].d[0].s);
#endif
        }

        if (k6)
        {
            me.l[bn].b[0].p[lh0][0].d[1].s += -y_ * 0.05;
            me.l[bn].b[0].p[lh1][0].d[0].s += -x_ * 0.05;

#ifdef dbg
            gmsg[2] = " 旋转左前臂:" + str(me.l[bn].b[0].p[lh0][0].d[1].s);
            gmsg[3] = " 弯曲左前臂:" + str(me.l[bn].b[0].p[lh1][0].d[0].s);
#endif
        }

        if (kQ)
        {
            me.l[bn].b[0].p[lh2][0].d[0].s += -x_ * 0.05;
            me.l[bn].b[0].p[lh3][0].d[0].s += -y_ * 0.5;
#ifdef dbg
            gmsg[2] = " 旋转左手掌:" + str(me.l[bn].b[0].p[lh2][0].d[0].s);
            gmsg[3] = " 合起左手指:" + str(me.l[bn].b[0].p[lh3][0].d[0].s);
#endif
        }

        if (k3)
        {
            me.l[bn].b[0].p[ubd][0].d[2].s += -y_ * 0.05;
            me.l[bn].b[0].p[rh0][0].d[0].s += x_ * 0.05;
#ifdef dbg
            gmsg[2] = " 抬起右上臂:" + str(me.l[bn].b[0].p[ubd][0].d[2].s);
            gmsg[3] = " 张开右上臂:" + str(me.l[bn].b[0].p[rh0][0].d[0].s);

#endif
        }

        if (k7)
        {
            me.l[bn].b[0].p[rh1][0].d[0].s += -x_ * 0.05;
            me.l[bn].b[0].p[rh0][0].d[1].s += -y_ * 0.05;
#ifdef dbg
            gmsg[2] = " 弯曲右前臂:" + str(me.l[bn].b[0].p[rh1][0].d[0].s);
            gmsg[3] = " 旋转右前臂:" + str(me.l[bn].b[0].p[rh0][0].d[1].s);
#endif
        }

        if (kW)
        {
            me.l[bn].b[0].p[rh1][0].d[1].s += -y_ * 0.05;
            me.l[bn].b[0].p[rh2][0].d[0].s += -x_ * 0.05;

#ifdef dbg
            gmsg[2] = " 旋转右手掌:" + str(me.l[bn].b[0].p[rh1][0].d[1].s);
            gmsg[3] = " 弯曲右手掌:" + str(me.l[bn].b[0].p[rh2][0].d[0].s);
#endif
        }

        if (kS)
        {
            me.l[bn].b[0].p[rh3][0].d[0].s += -y_ * 0.05;
            me.l[bn].b[0].p[rh3][1].d[0].s += -x_ * 0.05;
#ifdef dbg
            gmsg[3] = " 合起右手指1:" + str(me.l[bn].b[0].p[rh3][0].d[0].s);
            gmsg[4] = " 合起右手指2:" + str(me.l[bn].b[0].p[rh3][1].d[0].s);
#endif
        }

        if (kF)
        {
            me.l[bn].b[0].p[rh3][2].d[0].s += -x_ * 0.05;
            me.l[bn].b[0].p[rh3][3].d[0].s += -x_ * 0.05;
#ifdef dbg
            gmsg[3] = " 合起右手指3:" + str(me.l[bn].b[0].p[rh3][2].d[0].s);
            gmsg[3] = " 合起右手指4:" + str(me.l[bn].b[0].p[rh3][3].d[0].s);
#endif
        }

        if (k4)
        {
            me.l[bn].b[0].p[ubd][0].d[3].s += -y_ * 0.05;
            me.l[bn].b[0].p[lf0][0].d[0].s += -x_ * 0.05;

#ifdef dbg
            gmsg[2] = " 侧踢左腿:" + str(me.l[bn].b[0].p[ubd][0].d[3].s);
            gmsg[3] = " 旋转左腿:" + str(me.l[bn].b[0].p[lf0][0].d[0].s);
#endif
        }

        if (k8)
        {
            me.l[bn].b[0].p[lf0][0].d[2].s += -x_ * 0.05;
            me.l[bn].b[0].p[lf0][0].d[1].s += -y_ * 0.05;

#ifdef dbg
            gmsg[2] = " 17.前踢左腿:  " + str(me.l[bn].b[0].p[lf0][0].d[2].s);
            gmsg[3] = " 16.弯左膝盖:" + str(me.l[bn].b[0].p[lf0][0].d[1].s);
#endif
        }

        if (kE)
        {
            me.l[bn].b[0].p[lf1][0].d[0].s += +y_ * 0.05;
            me.l[bn].b[0].p[lf2][0].d[0].s += +x_ * 0.05;

#ifdef dbg
            gmsg[2] = " 侧左脚掌:" + str(me.l[bn].b[0].p[lf2][0].d[0].s);
            gmsg[3] = " 抬左脚掌:" + str(me.l[bn].b[0].p[lf1][0].d[0].s);
#endif
        }

        if (k5)
        {
            me.l[bn].b[0].p[rf0][0].d[0].s += -x_ * 0.05;
            me.l[bn].b[0].p[ubd][0].d[4].s += -y_ * 0.05;
#ifdef dbg
            gmsg[2] = " 侧踢右腿:" + str(me.l[bn].b[0].p[ubd][0].d[4].s);
            gmsg[3] = " 旋转右腿:" + str(me.l[bn].b[0].p[rf0][0].d[0].s);
#endif
        }

        if (k9)
        {
            me.l[bn].b[0].p[rf0][0].d[2].s += -x_ * 0.05;
            me.l[bn].b[0].p[rf0][0].d[1].s += -y_ * 0.05;
#ifdef dbg
            gmsg[2] = " 23.前踢右腿:  " + str(me.l[bn].b[0].p[rf0][0].d[2].s);
            gmsg[3] = " 22.弯右膝盖:" + str(me.l[bn].b[0].p[rf0][0].d[1].s);
#endif
        }

        if (kR)
        {
            me.l[bn].b[0].p[rf1][0].d[0].s += +y_ * 0.05;
            me.l[bn].b[0].p[rf2][0].d[0].s += +x_ * 0.05;

#ifdef dbg
            gmsg[2] = " 侧右脚掌:" + str(me.l[bn].b[0].p[rf2][0].d[0].s);
            gmsg[3] = " 抬右脚掌:" + str(me.l[bn].b[0].p[rf1][0].d[0].s);
#endif
        }

        if (kL == 1)
        {
            me.cus[me.p].e.x += x_ * 0.1 * mm;
            me.cus[me.p].e.z += -y_ * 0.1 * mm;
            me.cus[me.p].e.x += x_ * 0.1 * mm;
            me.cus[me.p].e.z += -y_ * 0.1 * mm;
        }
        if (kL == 2)
        {
            me.cus[me.p].e.x += x_ * 0.1 * mm;
            me.cus[me.p].e.y += -y_ * 0.1 * mm;
            me.cus[me.p].e.x += x_ * 0.1 * mm;
            me.cus[me.p].e.y += -y_ * 0.1 * mm;
        }
        if (kL == 3)
        {
            me.cus[me.p].ag.x += x_ * 0.1;
            me.cus[me.p].ag.z += -y_ * 0.1;
            me.cus[me.p].ag.x += x_ * 0.1;
            me.cus[me.p].ag.z += -y_ * 0.1;
        }
        if (kL == 4)
        {
            me.cus[me.p].ag.x += x_ * 0.1;
            me.cus[me.p].ag.y += -y_ * 0.1;
            me.cus[me.p].ag.x += x_ * 0.1;
            me.cus[me.p].ag.y += -y_ * 0.1;
        }

        if (kG)
        {
            d3.ctr[0] += -x_ * 0.1f;
            d3.ctr[1] += -y_ * 0.1f;
            gmsg[5] = "[" + str(double(d3.ctr[0])) + ", " + str(double(d3.ctr[1])) + "] ";
        }
    }

    ms[0].x = x;
    ms[0].y = y;
    ms[1].x = x;
    ms[1].y = y;
    ms[2].x = x;
    ms[2].y = y;
}

void mouseClick(int btn, int state, int x, int y)
{
    ms[0].x = x;
    ms[0].y = y;
    ms[1].x = x;
    ms[1].y = y;
    ms[2].x = x;
    ms[2].y = y;
    ms[btn].s = state + 1;
    if (btn == 3)
    {
        me.pm[0].z += args[1] * 50 * mm;
        me.pm[3].z += args[1] * 50 * mm;
    }
    if (btn == 4)
    {
        me.pm[0].z -= args[1] * 50 * mm;
        me.pm[3].z -= args[1] * 50 * mm;
    }
    // gmsg[5]="["+str(x)+", "+str(y)+"] ";
    mousest.btn = btn;
    mousest.state = state;
    mousest.ref = 1;
}

void mouseMove2(int x, int y)
{
    ms[3].x = x;
    ms[3].y = y;
    mousest.x = x;
    mousest.y = y;
    mousest.ref = 1;
}
static GLdouble m_AspectRatio = 0.0;
void reshape(GLsizei width, GLsizei height)
{
    wsz.w = width;
    wsz.h = height;
    me.l[0].lcsz = p2d(wsz.w, wsz.h);
    me.l[0].rcsz = p2d(wsz.w, wsz.h);
    me.l[0].sflc = (me.l[0].lcsz.x / 2) / Tan(me.l[0].Pplc / 2);
    me.l[0].sfrc = (me.l[0].rcsz.x / 2) / Tan(me.l[0].Pprc / 2);
    fyml.flc = me.l[0].sflc;
    fyml.frc = me.l[0].sfrc;
    GLdouble aspectRatio = GLdouble(width) / GLdouble(height);

    if (aspectRatio < m_AspectRatio)
    {
        GLint smallHeight = GLint(GLdouble(width) / m_AspectRatio);
        GLint heightBlank = (GLint(height) - smallHeight) / 2;
        glViewport(0, heightBlank, GLint(width), smallHeight);
    }
    else
    {
        GLint smallWidth = GLint(GLdouble(height) * m_AspectRatio);
        GLint widthBlank = (GLint(width) - smallWidth) / 2;
        glViewport(widthBlank, 0, smallWidth, GLint(height));
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(me.l[0].Pplc / 2 * /*修正开度*/ 1.275, m_AspectRatio, 1, 100 * 1000 * 1000 * mm);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
static GLuint diffuseMap, sk2, skr, skg, skb, sky, skc, sk1, texl, texr, texl1, texr1, texl2, texr2,
    texl3, texr3, textdatadftMatSmTable4l, textdatadftMatSmTable4r /*, bodytextr*/;

void display(void)
{
    GLfloat ambient[] = {0.9f, 0.9f, 0.9f, 0.9f};
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除缓冲区颜色
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);          //设置环境光
    glLoadIdentity();                                   //原点放坐标中心
    gluLookAt(me.pm[0].x, me.pm[0].y, me.pm[0].z, me.pm[3].x, me.pm[3].y, me.pm[3].z + 0.001,
              me.pm[5].x, me.pm[5].y + 0.001, me.pm[5].z);

    glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, diffuseMaterial);

    GLfloat earth_mat_ambient1[] = {0.99f, 0.99f, 0.99f, 0.99f};
    GLfloat earth_mat_diffuse1[] = {0.99f, 0.99f, 0.99f, 0.99f};
    glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient1);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse1);

    glPushMatrix();

    p3d rott(me.pm[2]);

    glRotated(rott.y, 1, 0, 0);
    glRotated(rott.x, 0, 1, 0);
    glRotated(rott.z, 0, 0, 1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glRotatef(90, 0.0, 0.0, 1.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    GLUquadricObj *cone;              //定义一个二次对象指针
    cone = gluNewQuadric();           //创建一个二次对象
    gluQuadricTexture(cone, GL_TRUE); //制定二次对象的绘制方式
    gluSphere(cone, 500 * 1000 * mm, 25, 25);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);

    glDepthMask(GL_FALSE);                          //关掉深度测试
    glEnable(GL_BLEND);                                //开混合模式贴图
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 指定混合模式算法

    // dotg(p3d(100, 100, 100), scalar(0, 255, 0));
    for (auto &w : me.dot1)
    {
        glPointSize(GLfloat(w.sz));
        glBegin(GL_POINTS);
        dotg(w);
        glEnd();
    }

    glPointSize(2);
    glBegin(GL_POINTS);
    for (ulong w = 0; w < me.dot2.size(); w++) { dotg(me.dot2[w]); }
    for (int v = 0; v < eht; v++)
    {
        for (int u = 0; u < ewd; u++)
        {
            dotg(d3.dots[u][v].x, d3.dots[u][v].y, d3.dots[u][v].z, d3.dots[u][v].r,
                 d3.dots[u][v].g, d3.dots[u][v].b);
        }
    }
    glEnd();

    // 画线
    for (auto &i : ax) { dline(i); }
    for (auto &i : me.ax1) { dline(i); }
    for (auto &i : me.ax2) { dline(i); }
    for (auto &i : me.ax20) { dline(i); }

    for (int i = 1; i < eht / 20; i++)
    {
        axis l(p3d(-640 * mm - 720 - 320, i * 20 - 560 - 560 + 320, -150 * mm - 6),
               p3d(-640 * mm + 640 - 320, i * 20 - 560 - 560 + 320, -150 * mm - 6));
        l.ca = scalar(0, 0, 255);
        l.cb = scalar(0, 0, 255);
        l.linewidth = 1;
        dline(l);
    }

    for (double i = 0; i < 20; i++)
    {
        axis r0(p3d(10000 * mm, -576.87 * mm, -i * 500 * mm),
                p3d(-10000 * mm, -576.87 * mm, -i * 500 * mm), 0, scalar(60, 80, 60));
        axis r1(p3d(10000 * mm, -576.87 * mm, i * 500 * mm),
                p3d(-10000 * mm, -576.87 * mm, i * 500 * mm), 0, scalar(60, 80, 60));
        axis c0(p3d(-i * 500 * mm, -576.87 * mm, 10000 * mm),
                p3d(-i * 500 * mm, -576.87 * mm, -10000 * mm), 0, scalar(60, 80, 60));
        axis c1(p3d(i * 500 * mm, -576.87 * mm, 10000 * mm),
                p3d(i * 500 * mm, -576.87 * mm, -10000 * mm), 0, scalar(60, 80, 60));
        dline(me.l[2].worldm * r0);
        dline(me.l[2].worldm * r1);
        dline(me.l[2].worldm * c0);
        dline(me.l[2].worldm * c1);
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // glBindTexture(GL_TEXTURE_2D, sk1);/**/
    for (ulong i = 0; i < me.ball1.size(); i++) { drawBall(me.ball1[i]); }

#if 1
    drtg(p3d(-640 * mm - 720, 0 * mm, -150 * mm), p3d(640, 480, 10), texl);
    drtg(p3d(-640 * mm, 0 * mm, -150 * mm), p3d(640, 480, 10), texr);
    drtg(p3d(-640 * mm - 720, -560, -150 * mm), p3d(640, 480, 10), texl1);
    drtg(p3d(-640 * mm, -560, -150 * mm), p3d(640, 480, 10), texr1);
    drtg(p3d(-640 * mm - 720, -560 - 560, -150 * mm), p3d(640, 480, 10), texl2);
    drtg(p3d(-640 * mm, -560 - 560, -150 * mm), p3d(640, 480, 10), texr2);
    drtg(p3d(-640 * mm - 720, -560 * 3, -150 * mm), p3d(640, 480, 10), texl3);
    drtg(p3d(-640 * mm, -560 * 3, -150 * mm), p3d(640, 480, 10), texr3);
    drtg(p3d(1000 * mm, 0, -150 * mm), p3d(500, 1323, 10), textdatadftMatSmTable4l);
    drtg(p3d(1000 * mm + 550, 0, -150 * mm), p3d(500, 1323, 10), textdatadftMatSmTable4r);
    drtg(tagtp);
#endif

    glEnable(GL_TEXTURE_2D);
    GLuint skn = 0;
    if (kL == 0) skn = skr;
    if (kL == 1) skn = skg;
    if (kL == 2) skn = skb;
    if (kL == 3) skn = skc;
    if (kL == 4) skn = sky;
    glBindTexture(GL_TEXTURE_2D, skn);
    drtg1(me.cus[10]);
    glBindTexture(GL_TEXTURE_2D, sk2); /**/
#if 0
    drtg1(me.l[0].b[2].o);
    drtg1(me.l[0].b[2].eyel);
    drtg1(me.l[0].b[2].eyer);
    drtg1(me.l[0].b[2].fac[0]);
    drtg1(me.l[0].b[2].hed[0]);/**/
    drtg1(me.l[0].b[2].ubd[0]);
    drtg1(me.l[0].b[2].lh0[0]);
    drtg1(me.l[0].b[2].lh1[0]);
    drtg1(me.l[0].b[2].lh2[0]);
    drtg1(me.l[0].b[2].lh3[0][0]);

    drtg1(me.l[0].b[2].rh0[0]);
#endif
#if 1
    drtg1(me.l[0].b[2].p[rh1][0]);
    drtg1(me.l[0].b[2].p[rh2][0]);
    drtg1(me.l[0].b[2].p[rh3][0]);
    drtg1(me.l[0].b[2].p[rh3][1]); /**/
#ifdef Mechanical_arm
    drtg1(me.l[0].b[2].p[rh3][2]);
    drtg1(me.l[0].b[2].p[rh3][3]);
#endif
#endif
#if 0
    drtg1(me.l[0].b[2].lf0[0]);
    drtg1(me.l[0].b[2].rf0[0]);
    drtg1(me.l[0].b[2].lf1[0]);
    drtg1(me.l[0].b[2].rf1[0]);
    drtg1(me.l[0].b[2].lf2[0]);
    drtg1(me.l[0].b[2].rf2[0]);
#endif

    glBindTexture(GL_TEXTURE_2D, /*sk2*/ sk1);
    p3d objo(0, 0, 0);
    while (mslock) { usleep(1000); }

    drawmesh = 1;

    DrawObj(me.l[0].ms_.m[hed], objo);
    DrawObj(me.l[0].ms_.m[ubd], objo);

    DrawObj(me.l[0].ms_.m[lh0], objo);
    DrawObj(me.l[0].ms_.m[lh1], objo);
    DrawObj(me.l[0].ms_.m[lh2], objo);
    DrawObj(me.l[0].ms_.m[lh3], objo);
    DrawObj(me.l[0].ms_.m[lh4], objo);
    DrawObj(me.l[0].ms_.m[lh5], objo);
    DrawObj(me.l[0].ms_.m[lh6], objo);
    DrawObj(me.l[0].ms_.m[lh7], objo);

    DrawObj(me.l[0].ms_.m[rh0], objo);
#ifndef Mechanical_arm
    DrawObj(me.l[0].rh1, objo);
    DrawObj(me.l[0].rh2, objo);
    DrawObj(me.l[0].rh300, objo);
    DrawObj(me.l[0].rh301, objo);
    DrawObj(me.l[0].rh302, objo);
    DrawObj(me.l[0].rh303, objo);
    DrawObj(me.l[0].rh304, objo);
#endif
    DrawObj(me.l[0].ms_.m[lf0], objo);
    DrawObj(me.l[0].ms_.m[lf1], objo);
    DrawObj(me.l[0].ms_.m[lf2], objo);

    DrawObj(me.l[0].ms_.m[rf0], objo);
    DrawObj(me.l[0].ms_.m[rf1], objo);
    DrawObj(me.l[0].ms_.m[rf2], objo);

    drawmesh = 0;

    glDisable(GL_TEXTURE_2D);

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glPopMatrix();
    if (!run0) return;
    glutSwapBuffers();
}

#endif
static cv::Mat frmlg, frmrg, fml, fmr;
#ifndef FYAIRO_1_0_0_ARM
static cv::Mat frm99lt, frm99rt, frmlgt, frmrgt, fmlt, fmrt, frm27lt, frm27rt, datadftMatSmTable4l,
    datadftMatSmTable4r;
static double fps1;

void idle()
{

    if (!run0) exit(0);
    glutSetIconTitle("FYAIRO");
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    me.l[0].b[0].p[lf2][1].box(me.l[0].b[0].p[lf2][1].sz, 50 * mm);
#if 0
    printf("<%f, %f, %f>", me.cus[13].e.x/mm, me.cus[13].e.y/mm, me.cus[13].e.z/mm);
#endif
    me.l[0].b[0].p[lf2][2].box(me.l[0].b[0].p[lf2][2].sz, 50 * mm);
    me.l[0].b[0].p[lf2][3].box(me.l[0].b[0].p[lf2][3].sz, 1);
    me.l[0].b[0].p[lf2][4].box(me.l[0].b[0].p[lf2][4].sz, 1);
    me.l[0].wd[1] = me.l[0].b[1].p[lf2][3];
    me.l[0].wd[1] + (-me.l[0].wd[1].e);
    me.l[0].b[2].p[lf2][4] = me.l[0].currentm * me.l[0].b[1].p[lf2][4];

    cv::Point3d sz2 = cv::Point3d(10 * mm, 10 * mm, 10 * mm);
    vr vtst;
    vtst.lp = cv::Point(1, 0);
    vtst.rp = cv::Point(-1, 0);

    me.cus[10].box(me.cus[10].sz, 5.0 * mm);
    me.cus[13].box(me.cus[13].sz, 5.0 * mm);
#if 0
    me.cus[12].box(8*mm);
    me.dbox(me.cus[12], cv::Scalar(255, 0, 0));
#endif
    me.dbox(vtst, sz2, cv::Scalar(0, 122, 225));
    me.dbox(cv::Point3d(0 * mm, -1000 * mm, 1000 * mm), sz2, cv::Scalar(0, 218, 148));
    me.dbox(cv::Point3d(0 * mm, 0 * mm, 2000 * mm), sz2, cv::Scalar(0, 218, 148));
    me.dbox(cv::Point3d(0 * mm, 0 * mm, 3000 * mm), sz2, cv::Scalar(188, 0, 148));
    me.dbox(cv::Point3d(200 * mm, 500 * mm, 1200 * mm), sz2, cv::Scalar(188, 0, 148));
    me.dbox(cv::Point3d(300 * mm, 400 * mm, 1400 * mm), sz2, cv::Scalar(188, 218, 0));
    me.dbox(cv::Point3d(300 * mm, 400 * mm, 1600 * mm), sz2, cv::Scalar(188, 218, 0));
    me.dbox(cv::Point3d(400 * mm, 300 * mm, 1800 * mm), sz2, cv::Scalar(0, 0, 148));
    me.dbox(cv::Point3d(400 * mm, 300 * mm, 2000 * mm), sz2, cv::Scalar(0, 0, 148));
    me.dbox(vr(cv::Point2f(130, 50), cv::Point2f(120, 50)).e(), sz2 * 2, rnds());
    me.dbox(vr(cv::Point2f(130, -50), cv::Point2f(120, -50)).e(), sz2 * 2, rnds());
    me.dbox(vr(cv::Point2f(-120, 50), cv::Point2f(-130, 50)).e(), sz2 * 2, rnds());
    me.dbox(vr(cv::Point2f(-120, -50), cv::Point2f(-130, -50)).e(), sz2 * 2, rnds());

    me.dbox(me.l[0].b[2].p[lf2][4], cv::Scalar(255, 0, 0));
    me.dbox(me.l[0].b[0].p[lf2][1], 50 * mm);
    me.dbox(me.l[0].wd[0], cv::Scalar(255, 0, 0));
    me.dbox(me.l[0].wd[1], cv::Scalar(0, 255, 0));
    me.dbox(me.l[0].b[2].p[lf2][4], cv::Scalar(0, 255, 0));
    me.dbox(me.cus[13], cv::Scalar(0, 255, 0));
    me.dbox(me.l[2].wd[10], fc_cyan);
#if 1
    me.drt(me.l[0].b[2], fc_red);
    me.drt(me.l[1].b[0], fc_yellow);
    //    me.drt(me.l[1].b[2], fc_yellow);

    me.drt(me.l[2].b[0], fc_red);
    me.drt(me.l[2].b[1], fc_yellow);
    me.drt(me.l[2].b[2], fc_blue);

    me.drt(me.l[3].b[2], fc_cyan);

#endif
#if 0
    me.drt(me.l[0].b[0], cv::Scalar(50, 60, 80));
#endif
#if 0
    me.drt(me.l[0].b[1], cv::Scalar(0, 200, 200));
    me.drt(me.l[1].b[0], cv::Scalar(255, 0, 0));
    me.drt(me.l[1].b[1], cv::Scalar(255, 0, 200));
#endif
    me.ax1.clear(); //这里有个冲突
    me.ax1 = me.ax11;
    me.ax11.clear();

    if (sig_activated(20))
    {
        sem_wait(&sig[20]);
        me.ax20.clear();
        me.ax20 = me.ax201;
        me.ax201.clear();
    }
    if (sig_activated(7))
    {
        ThreadName[7] = "Draw Frame";
        texl = gettexture(frm99lt, texl);
        texr = gettexture(frm99rt, texr);
        texl1 = gettexture(frmlgt, texl1);
        texr1 = gettexture(frmrgt, texr1);
        texl2 = gettexture(fmlt, texl2);
        texr2 = gettexture(fmrt, texr2);
        texl3 = gettexture(frm27lt, texl3);
        texr3 = gettexture(frm27rt, texr3);
        textdatadftMatSmTable4l = gettexture(datadftMatSmTable4l, textdatadftMatSmTable4l);
        textdatadftMatSmTable4r = gettexture(datadftMatSmTable4r, textdatadftMatSmTable4r);
        me.dot1 = me.dot11;
        me.dot11.clear();
        me.dot2 = me.dot21;
        me.dot21.clear();
        me.ax2 = me.ax21;
        me.ax21.clear();
        me.ball1 = me.ball11;
        me.ball11.clear();
#if 1
        for (ulong i = 0; i < tagtp.size(); i++) { glDeleteTextures(1, &tagtp[i].l3); }
        tagtp.clear();
        tagtp = tagt;
        tagt.clear();
        for (ulong i = 0; i < tagtp.size(); i++) { tagtp[i].l3 = gettexture(tagp[i]); }
        tagp.clear();
        while (tagp.size()) { usleep(1000); }
#endif
        sem_wait(&sig[7]);
    }
    fps1 = fps(1);
    usleep(10 * 1000);
    glutPostRedisplay();
}

void init()
{
    GLfloat ambient[] = {0.9f, 0.8f, 0.9f, 0.8f}, diffuse[] = {0.9f, 0.8f, 0.7f, 0.8f},
            specular[] = {0.8f, 0.7f, 0.9f, 0.9f},
            position[] = {float(5000 * mm), float(5000 * mm), float(5000 * mm), 0.8f},
            //定义光源的颜色和位置
        lmodel_ambient[] = {0.8f, 0.8f, 0.8f, 1.0f}, //选择光照模型
        local_view[] = {float(5000 * mm), float(5000 * mm), float(5000 * mm)};
    glClearColor(0.0, 0.0, 0.1f, 0.5);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST); // 使用深度测试
    glDepthFunc(GL_LESS);
    glFrontFace(GL_CW); // 顺时针的绘制为正面

    GLfloat mat_specular[] = {0.6f, 0.9f, 0.2f, 0.9f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view); //设置光源位置
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);   //设置环境光
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);   //设置漫射光
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular); //设置镜面光
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // GLfloat mat_diffuse[]={0.8f, 0.8f, 0.8f, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial); //启用双面光照
    desm.z = -600 * mm;

    string msdir = "/home/root/eye/ms";

    diffuseMap = loadtexture(msdir + "/wd.bmp");

    me.l[0].ms.m_[hed] = loadobj(msdir + "/hed.obj");
    me.l[0].ms.m_[ubd] = loadobj(msdir + "/ubd.obj");

    me.l[0].ms.m_[lh0] = loadobj(msdir + "/lh0.obj");
    me.l[0].ms.m_[lh1] = loadobj(msdir + "/lh1.obj");
    me.l[0].ms.m_[lh2] = loadobj(msdir + "/lh2.obj");
    me.l[0].ms.m_[lh3] = loadobj(msdir + "/lh300.obj");
    me.l[0].ms.m_[lh4] = loadobj(msdir + "/lh301.obj");
    me.l[0].ms.m_[lh5] = loadobj(msdir + "/lh302.obj");
    me.l[0].ms.m_[lh6] = loadobj(msdir + "/lh303.obj");
    me.l[0].ms.m_[lh7] = loadobj(msdir + "/lh304.obj");

    me.l[0].ms.m_[rh0] = loadobj(msdir + "/rh0.obj");
    me.l[0].ms.m_[rh1] = loadobj(msdir + "/rh1.obj");
    me.l[0].ms.m_[rh2] = loadobj(msdir + "/rh2.obj");
    me.l[0].ms.m_[rh3] = loadobj(msdir + "/rh300.obj");
    me.l[0].ms.m_[rh4] = loadobj(msdir + "/rh301.obj");
    me.l[0].ms.m_[rh5] = loadobj(msdir + "/rh302.obj");
    me.l[0].ms.m_[rh6] = loadobj(msdir + "/rh303.obj");
    me.l[0].ms.m_[rh7] = loadobj(msdir + "/rh304.obj");

    me.l[0].ms.m_[lf0] = loadobj(msdir + "/lf0.obj");
    me.l[0].ms.m_[lf1] = loadobj(msdir + "/lf1.obj");
    me.l[0].ms.m_[lf2] = loadobj(msdir + "/lf2.obj");
    me.l[0].ms.m_[rf0] = loadobj(msdir + "/rf0.obj");
    me.l[0].ms.m_[rf1] = loadobj(msdir + "/rf1.obj");
    me.l[0].ms.m_[rf2] = loadobj(msdir + "/rf2.obj");

    sk2 = loadtexture(msdir + "/sk2.jpeg", pt4d(1, 1, 1, 0.3));
    skr = loadtexture(msdir + "/sk2.jpeg", pt4d(0.42, 0.42, 1, 0.8));
    skg = loadtexture(msdir + "/sk2.jpeg", pt4d(0.42, 1, 0.42, 0.8));
    skb = loadtexture(msdir + "/sk2.jpeg", pt4d(1, 0.42, 0.42, 0.8));
    skc = loadtexture(msdir + "/sk2.jpeg", pt4d(0.42, 1, 1, 0.8));
    sky = loadtexture(msdir + "/sk2.jpeg", pt4d(1, 1, 0.42, 0.8));
    sk1 = loadtexture(msdir + "/sk2.jpeg", pt4d(0.9, 0.9, 0.9, 0.3));
}

static int windowid;
int Glinit(int argc, char **argv)
{
    glutInit(&argc, argv); //初始化
    glutInitDisplayMode(/*GLUT_SINGLE*/ GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH |
                        GLUT_MULTISAMPLE); //设置显示模式
    glutInitWindowSize(1920, 1080);        //初始化窗口大小
    m_AspectRatio = 1200.0 / 700.0;
    glutInitWindowPosition(0, 0);         //定义左上角窗口位置
    windowid = glutCreateWindow(argv[0]); //创建窗口
    init();                               //初始化
    glutDisplayFunc(display);             //显示函数
    glutReshapeFunc(reshape);             //窗口大小改变时的响应
    glutMouseFunc(mouseClick);            //鼠标点击事件
    glutMotionFunc(mouseMove);            //鼠标按下并移动时调用
    glutPassiveMotionFunc(mouseMove2);    //鼠标移动时调用
    glutKeyboardFunc(keyPressed);         //键盘事件
    glok = 1;
    glwork = 1;
    texl = gettexture(frm99lt, texl);
    texr = gettexture(frm99rt, texr);
    texl1 = gettexture(frmlgt, texl1);
    texr1 = gettexture(frmrgt, texr1);
    texl2 = gettexture(fmlt, texl2);
    texr2 = gettexture(fmrt, texr2);
    texl3 = gettexture(frm27lt, texl3);
    texr3 = gettexture(frm27rt, texr3);
    textdatadftMatSmTable4l = gettexture(datadftMatSmTable4l, textdatadftMatSmTable4l);
    textdatadftMatSmTable4r = gettexture(datadftMatSmTable4r, textdatadftMatSmTable4r);
    glutIdleFunc(idle);
    glutSetIconTitle("/home/root/robot.ico");
    glutSetWindowTitle("FYAIRO");
    glutFullScreen();
    glEnable(GL_MULTISAMPLE);
    glutMainLoop();
    return 0;
}

void *thr_fn15(void *)
{
    if (Ddv != 2) { return (nullptr); }
    while (frm80l.cols == 0 || frm80r.cols == 0)
    {
        usleep(1 * 1000);
    }
    gmsg[0] = "启动GL线程...";
    string s = "FYAIRO";
    char *bb = const_cast<char *>(s.c_str());
    Glinit(1, &bb);
    return (nullptr);
}
bool sortregp(regp a, regp b) { return a.d < b.d; }
void *thr_fn26(void *)
{
    if (Ddv != 2) { return (nullptr); }
    while (frm80l.cols == 0 || frm80r.cols == 0) { usleep(20 * 1000); }
    gmsg[0] = "启动鼠标线程...";
    vector<regp> vpl;
    while (run0)
    {
        while (mousest.ref == 0) { usleep(1000); }
        mousest.ref = 0;
        mousestate mousestt = mousest;
        if (!vplock) { vpl = vp; }

        p3d pm2(me.pm[2].y, me.pm[2].x, me.pm[2].z), pm0(me.pm[0] + me.pm[1]),
            pm1(-pm0.x, pm0.y, pm0.z);

        ms3 =
            getm(pm2) * (p3d(mousestt.x - wsz.w / 2, -mousestt.y + wsz.h / 2, fyml.flc + 1) + pm1);
        p3d p = getm(pm2) * pm1;

        ms3l = axis(p, ms3);
        for (uint i = 0; i < vpl.size(); i++) { vpl[i].d = ms3l.dst(vpl[i].p); }
        sort(vpl.begin(), vpl.end(), sortregp);
        if (vpl.size() && vpl[0].d < 7 * mm)
        {
            vp0p1 = vpl[0];
            downcolor = scalar(0, 255, 255);
            if (mousestt.btn == 0 && mousestt.state == 0)
            {
                if (vpl[0].n) vpl[0].f = 1;
                vp0p = vpl[0];
                gmsg[6] = "抓取:" + str(int(vpl[0].arg.x)) + ", " + str(int(vpl[0].arg.y));
                downcolor = scalar(0, 0, 255);
            }

            if (mousestt.btn == 0 && mousestt.state == 1)
            {
                if (vpl[0].n) vp0p.f = 0;
            }

            if (mousestt.btn == 2 && mousestt.state == 0)
            {
                if (vpl[0].n) vpl[0].f = 3;
                vp0p = vpl[0];
                gmsg[6] = "观察:" + str(int(vpl[0].p.x)) + ", " + str(int(vpl[0].p.y)) + ", " +
                          str(int(vpl[0].p.z));
                downcolor = scalar(0, 0, 255);
            }
            if (mousestt.key == 0) {}
        }
        else
        {
            vp0p1 = p3d(0);
        }

#if 0
        cout<<endl
           <<" me.pm[0]="<<me.pm[0]/mm
           <<" me.pm[1]="<<me.pm[1]/mm
           <<" me.pm[2]="<<me.pm[2]/mm
           <<" me.pm[3]="<<me.pm[3]/mm
           <<" me.pm[4]="<<me.pm[4]/mm
           <<" p="<<ms3/mm
           <<" x="<<x
           <<" y="<<y;
#endif
        usleep(1 * 1000);
    }
    return nullptr;
}
#endif
static gm44d m;

void *thr_fn27(void *)
{
    ThreadName[27] = "棋盘识别";
    while (frm27l.cols == 0 || frm27r.cols == 0) { usleep(20 * 1000); }

    ulong gcols = 9, grows = 9;
    cv::Size2d gdsz(200 * mm, 185 * mm);

    vr gdvy(p2d(0, 0), p2d(0, 0));
    vector<vr> gdvx;
    for (ulong i = 0; i < gcols; i++) { gdvx.push_back(gdvy); }
    for (ulong i = 0; i < grows; i++) { gdv.push_back(gdvx); }

    while (run0)
    {
        sem_wait(&sig[27]);
        mc = d3.FindGridHough(frm27l, frm27r, gdv);
        if (mc)
        {
            if (gdsz.width > 0.0 || gdsz.height > 0.0)
            {
                axis a(m * rpr(gdv[0][0].fvoe()), m * rpr(gdv[0][grows - 1].fvoe())),
                    b(m * rpr(gdv[0][grows - 1].fvoe()), m * rpr(gdv[gcols - 1][grows - 1].fvoe())),
                    c(m * rpr(gdv[gcols - 1][grows - 1].fvoe()), m * rpr(gdv[gcols - 1][0].fvoe())),
                    d(m * rpr(gdv[gcols - 1][0].fvoe()), m * rpr(gdv[0][0].fvoe())),
                    e(m * rpr(gdv[0][0].fvoe()), m * rpr(gdv[gcols - 1][grows - 1].fvoe())),
                    f(m * rpr(gdv[gcols - 1][0].fvoe()), m * rpr(gdv[0][grows - 1].fvoe()));

                double g(sqrt(gdsz.width * gdsz.width + gdsz.height * gdsz.height));
                mc = ((abs(a.d() - gdsz.width) < 15 * mm && abs(c.d() - gdsz.width) < 15 * mm &&
                       abs(b.d() - gdsz.height) < 15 * mm && abs(d.d() - gdsz.height) < 15 * mm) ||
                      (abs(a.d() - gdsz.height) < 15 * mm && abs(c.d() - gdsz.height) < 15 * mm &&
                       abs(b.d() - gdsz.width) < 15 * mm && abs(d.d() - gdsz.width) < 15 * mm)) &&
                     abs(e.d() - g) < 20 * mm && abs(f.d() - g) < 20 * mm;
                mc = 1;
#if 0
                cout<<"@  a-h="<<abs(a.d()-gdsz.height)/mm
                    <<" , b-w="<<abs(b.d()-gdsz. width)/mm
                    <<" , c-h="<<abs(c.d()-gdsz.height)/mm
                    <<" , d-w="<<abs(d.d()-gdsz. width)/mm
                    <<" , e-g="<<(e.d()-g)/mm
                    <<" , f-g="<<(f.d()-g)/mm
                    <<" , g="<<g/mm
                    <<" , mc="<<mc
                    <<"    ";
#endif
            }
        }
#ifndef FYAIRO_1_0_0_ARM
        frm27lt = translucent(frm27l);
        frm27rt = translucent(frm27r);
#endif

        if (mc)
        {
            gdo.clear();
            for (ulong x = 0; x < gdv.size(); x++)
            {
                std::vector<obj> gd0;
                for (ulong y = 0; y < gdv[0].size(); y++)
                {
                    p3d p = gdv[x][y].fvoe(), pa =
#if 1
                                                  m * rpr(p);
#else
                                                  p;
#endif
                    scalar cl = rnds();
                    me.dotf(pa, cl);
                    obj gd;
                    gd.e = pa;
                    gd.cl = cl;
                    gd0.push_back(gd);
                }
                gdo.push_back(gd0);
            }
            gdo1 = gdo;
        }
    }
    return nullptr;
}
static string gmsg2[40], cabmsg[40];

inline bool exists_test(const std::string &name) { return (access(name.c_str(), F_OK) != -1); }

void handle_pipe(int sig)
{
    cout << endl << "收到信号SIGPIPE " << sig << endl;
    cin >> SIG;
    SIG = SIGPIPE;
}
void handle_abrt(int sig)
{
    cout << endl << "收到信号SIGABRT " << sig << endl;
    cin >> SIG;
    SIG = SIGABRT;
}
void handle_segv(int sig)
{
    cout << endl << "收到信号SIGSEGV " << sig << endl;
    cin >> SIG;
    SIG = SIGSEGV;
}

struct j3ms
{
    p3d j3, j2;
    int i, j, k, l;
    j3ms()
    {
        j3 = p3d(0, 0, 0);
        j2 = p3d(0, 0, 0);
        i = 0;
        j = 0;
        k = 0;
        l = 0;
    }
    j3ms(p3d j3_, p3d j2_, int i_, int j_, int k_, int l_ = 0)
    {
        j3 = j3_;
        j2 = j2_;
        i = i_;
        j = j_;
        k = k_;
        l = l_;
    }
};

/*****************************************************/
/*static int vp0pf1x;*/
int main(int argc, char **argv)
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGABRT, handle_abrt);
    signal(SIGSEGV, handle_segv);
    for (auto &i : sig) sem_init(&i, 0, 0); // 初始化信号量
    char wkpath[100];
    std::cout << getcwd(wkpath, 100) << endl;
    string wpath(wkpath);
    std::string flg = "FYAIRO ";
    // cout<<"argc="<<argc;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            cloud = 1;
            flg += "Cloud ";
        }
    }
    if (wpath.find("Debug", 0) < wpath.length())
    {
        debug = 1;
        flg += "Debug ";
    }
    if (exists_test("/home/root/Desktop"))
    {
        desk = 1;
        flg += "Desktop ";
    }
    if (exists_test("/home/root/Server"))
    {
        serv = 1;
        flg += "Server ";
    }
    std::vector<string> ip = getMyIp();
    for (uint i = 0; i < ip.size(); i++)
    {
        cout << "\n" << ip[i];
        if (ip[i].find('.', 0) < ip[i].length() && ip[i] != "127.0.0.1") { MyIp = ip[i]; }
    }
#ifdef CC
    MyIp = "192.168.6.113"; // TODO: 手动设置
#endif

    std::cout << "\nMachine: " << flg << MyIp << "\n";
    cv::FileStorage fs("/home/root/eye/dt/em/eyematrix", cv::FileStorage::READ);
    fs["eyematrix0"] >> d3.cameraMatrix[0];
    fs["eyematrix1"] >> d3.cameraMatrix[1];
    fs["distCoefficients0"] >> d3.distCoefficients[0];
    fs["distCoefficients1"] >> d3.distCoefficients[1];
    fs["Rl"] >> d3.Rl;
    fs["Rr"] >> d3.Rr;
    fs["R0"] >> d3.R0;
    fs["T0"] >> d3.T0;
    fs["E0"] >> d3.E0;
    fs["F0"] >> d3.F0;
    fs["pVE"] >> d3.pVE;
    fs.release();

    distCoefficientsl = d3.distCoefficients[0];
    distCoefficientsr = d3.distCoefficients[1];

    d3.readEyeMat();
    d3ml = d3.ml;
    d3mr = d3.mr;

    for (int i = 0; i < 100; i++)
    {
        d3.args[i] = 1;
        args[i] = 1;
    }
#ifndef FYAIRO_1_0_0_ARM
    ft2->loadFontData("/home/root/eye/方正黑体简体.TTF", 0);
#endif

    run();

    cout << endl << "您好，我是" << MyName;

    for (int i = 1; i < argc; i++)
    {
        int threadi = atoi(argv[i]);
        // cout<<"\nargv"<<i<<"="<<argv[i]<<":开启线程"<<threadi;
        if (threadi <= 500 && threadi >= 0)
        {
            execute(0, byte(threadi), ThreadName[threadi]);
            // cout<<"\n";
        }
    }

    args[0] = fyml.fl;
    args[3] = fyml.ap - 1;
    args[4] = fyml.bt - 1;

    using namespace cv;

    Rect roi1, roi2;
    Mat Q, R1, P1, R2, P2;
    Size sz(ewd, eht);
#ifndef FYAIRO_1_0_0_ARM
    stereoRectify(d3.cameraMatrix[0], d3.distCoefficients[0], d3.cameraMatrix[1],
                  d3.distCoefficients[1], sz, d3.R0, d3.T0, R1, R2, P1, P2, Q,
                  /*CALIB_ZERO_DISPARITY*/ CALIB_FIX_ASPECT_RATIO, 0, sz, &roi1, &roi2);

    cout << endl
         << "d3.R0" << endl
         << d3.R0 << endl
         << endl
         << "d3.T0/mm" << endl
         << d3.T0 / mm << endl
         << endl
         << "R1" << endl
         << R1 << endl
         << endl
         << "R2" << endl
         << R2 << endl
         << endl
         << "P1" << endl
         << P1 << endl
         << endl
         << "P2" << endl
         << P2 << endl
         << endl
         << "Q" << endl
         << Q << endl
         << endl
         << "roi1" << endl
         << roi1 << endl
         << endl
         << "roi2" << endl
         << roi2 << endl
         << endl
         << "d3.cameraMatrix[0]" << endl
         << d3.cameraMatrix[0] << endl
         << endl
         << "d3.cameraMatrix[1]" << endl
         << d3.cameraMatrix[1] << endl
         << endl;
#if 1

    cv::Mat R = cv::Mat::eye(3, 3, CV_32F);

    cm0.at<float>(0, 0) = ewd;
    cm0.at<float>(0, 1) = 0;
    cm0.at<float>(0, 2) = ewd;
    cm0.at<float>(1, 0) = 0;
    cm0.at<float>(1, 1) = eht;
    cm0.at<float>(1, 2) = eht;
    cm0.at<float>(2, 0) = 0;
    cm0.at<float>(2, 1) = 0;
    cm0.at<float>(2, 2) = 1.0f;

    cm1.at<float>(0, 0) = ewd;
    cm1.at<float>(0, 1) = 0;
    cm1.at<float>(0, 2) = ewd;
    cm1.at<float>(1, 0) = 0;
    cm1.at<float>(1, 1) = eht;
    cm1.at<float>(1, 2) = eht;
    cm1.at<float>(2, 0) = 0;
    cm1.at<float>(2, 1) = 0;
    cm1.at<float>(2, 2) = 1.0f;

#endif

    initUndistortRectifyMap(d3.cameraMatrix[0], d3.distCoefficients[0], /*d3.Rl*/ R1,
                            cm0, /*getDefaultNewCameraMatrix(d3.cameraMatrix[0], sz, 1), */
                            sz, CV_32FC1, mapx[0], mapy[0]);

    initUndistortRectifyMap(d3.cameraMatrix[1], d3.distCoefficients[1], /*d3.Rr*/ R2,
                            cm1, /*getDefaultNewCameraMatrix(d3.cameraMatrix[1], sz, 1), */
                            sz, CV_32FC1, mapx[1], mapy[1]);
#endif
    ld = false;

    if (desk)
    {
#ifndef FYAIRO_1_0_0_ARM
        pthread_create(&ntid[15], &attr, thr_fn15, nullptr);
        pthread_create(&ntid[26], &attr, thr_fn26, nullptr);
#endif
    }
    pthread_create(&ntid[27], &attr, thr_fn27, nullptr);

#ifdef OPENCV_CORE_CUDA
    Ptr<cuda::StereoConstantSpaceBP> bm =
        cuda::createStereoConstantSpaceBP(351,     //差异的数量。
                                          8,       //每个级别的BP迭代次数。
                                          4,       //级别数。
                                          4,       //第一级的差异级别数。
                                          CV_16SC1 //支持CV_16SC1和CV_32FC1类型。
        );
    cuda::GpuMat lft, rgt, dsp, dsp2;
#endif
#ifndef FYAIRO_1_0_0_ARM
    Mat disp, frmlg1, frmrg1, frmlg2, frmrg2, /*frmlg3, frmrg3, frmlg4, frmrg4, */
        fmls, fmrs                            /*,
                                    frmll=Mat::eye(3, 3, CV_32F),
                                    frmrl=Mat::eye(3, 3, CV_32F)*/
        ,
        frmlh = Mat::eye(3, 3, CV_32F), frmrh = Mat::eye(3, 3, CV_32F), fml0, fmr0;

    // viz::Viz3d window("window");

    Scalar s(255, 255, 255);

    Mat point_cloud = Mat::zeros(eht, ewd, CV_32FC3);

    Mat frmlcl = Mat(eht, ewd, CV_8UC3, Scalar(0));
#endif
    while (frm80l.cols == 0 || frm80.cols == 0 || frm99l.cols == 0 || frm99r.cols == 0 || !glok)
    {
        usleep(10 * 1000);
    }
    Mat grid = imread("/home/root/eye/img/frm11/grid.jpg");
#ifndef FYAIRO_1_0_0_ARM
    frm27lt = translucent(frm99l);
    frm27rt = translucent(frm99r);
    // sndcmd(1, 0); sndcmd(2, char(me.l[0].bd[0].od[ 1][ 3]));

    long tm0 = 0;
    ThreadName[99] = "启动可视化线程...";

    /*color9 cl, cr;*/

    std::vector<Mat> sbgr0(ulong(frm80l.channels())), cbgr0(ulong(frm80l.channels())),
        sbgr1(ulong(frm80r.channels())), cbgr1(ulong(frm80r.channels()));

    float mvr10 = 0.7f, mvr20 = 0.9f, /*mvr30=0.8f, mvr40 =0.6f, */
        mvr11 = 0.9f, mvr21 = 0.7f /*, mvr31=0.5f, mvr41 =0.4f*/;

    Mat wp1 =
            gAt(0, 0, 1, 0, 0, 1, 0 + mvr10, 0 + mvr11, 1 + mvr10, 0 + mvr11, 0 + mvr10, 1 + mvr11),
        wp2 = gAt(0, 0, 1, 0, 0, 1, 0 + mvr20, 0 + mvr21, 1 + mvr20, 0 + mvr21, 0 + mvr20,
                  1 + mvr21) /*,
wp3=gAt(0, 0, 1, 0, 0, 1,   0+mvr30, 0+mvr31, 1+mvr30, 0+mvr31, 0+mvr30, 1+mvr31),
wp4=gAt(0, 0, 1, 0, 0, 1,   0+mvr40, 0+mvr41, 1+mvr40, 0+mvr41, 0+mvr40, 1+mvr41)*/
        ;

    vector<fygird> ddl2, ddr2;
    static cv::Scalar size1(45, 0.2, 0.1, 0); // (字体大小, 无效的, 字符间距, 无效的 }
    int ewd2 = ewd / 2, eht2 = eht / 2;

    double angl = 0;
    getRMM2D(cr0, angl / agl, Point2d(320, 240), Point2d(0, 0), 1, 1);
    getRMM2D(cr1, angl / agl, Point2d(320, 240), Point2d(0, 0), 1, 1);
#endif

    d3ml = d3.ml;
    d3mr = d3.mr;
    int dway = 0, dway2 = 0, dway3 = 0;
    double argx = 0 /*, argy=0*/;
    p3d j3mp;

    while (run1)
    {
        if (!sig_activated(99) || Dv == 1)
        {
            usleep(50 * 1000);
            continue;
        }
#ifndef FYAIRO_1_0_0_ARM
#if 0

cm0.at<float>(0, 0)=cf+26.0f ;cm0.at<float>(0, 1)=9.85f;    cm0.at<float>(0, 2)=ewd/2+15.8f-6.73f;
cm0.at<float>(1, 0)=        0;cm0.at<float>(1, 1)=cf+17.45f;cm0.at<float>(1, 2)=eht/2+2.85f-25.55f;
cm0.at<float>(2, 0)=0;        cm0.at<float>(2, 1)=0.0520f;  cm0.at<float>(2, 2)=    1.0f;

cm1.at<float>(0, 0)=cf+4.59f ;cm1.at<float>(0, 1)=8.95f;    cm1.at<float>(0, 2)=ewd/2+15.8f-4.59f;
cm1.at<float>(1, 0)=        0;cm1.at<float>(1, 1)=cf  ;     cm1.at<float>(1, 2)=eht/2-25.55f;
cm1.at<float>(2, 0)=0;        cm1.at<float>(2, 1)=0.0520f;  cm1.at<float>(2, 2)=    1.0f;

        initUndistortRectify(d3.cameraMatrix[0], d3.distCoefficients[0],
                                    d3.R0/*R*/, cm0/* P1 d3.cameraMatrix[0]*/, sz,
                                    CV_32FC1, x[0], mapy[0]);

        initUndistortRectifyMap(d3.cameraMatrix[1], d3.distCoefficients[1],
                                    d3.R0/*R*/, cm1/* P2 d3.cameraMatrix[1]*/, sz,
                                    CV_32FC1, mapx[1], mapy[1]);
#endif
#if 0
        frmlg=frm99l/**1.2-30*/;
        frmrg=frm99r/**1.2-30*/;
#endif
        d3ml = d3.ml;
        d3mr = d3.mr;

        // getRMM2D(cr0, (angl+args[5])/agl, Point2d(320, 240), Point2d(0, 0), 1, 1);
        // getRMM2D(cr1, (angl+args[5])/agl, Point2d(320, 240), Point2d(0, 0), 1, 1);
#if 0
        frmlg=frm99l.clone();
        frmrg=frm99r.clone();
#else
        if (kV == 5 || kC) { warp = 0; }
        else
        {
            warp = 1;
        }

        frmlg = frm99l;
        frmrg = frm99r;

        // warpAffine(frm99l, frmlg, cr0, frm99l.size());
        // warpAffine(frm99r, frmrg, cr1, frm99r.size());
#endif
#if 0
        fml=((frmlg-frmlg1)+(frmlg-frmlg2)-16)*8;
        fmr=((frmrg-frmrg1)+(frmrg-frmrg2)-16)*8;
#endif
#if 0
        fml=cvtcolor(roberts(cvtcolor(frmlg))-5.0)*25;
        fmr=cvtcolor(roberts(cvtcolor(frmrg))-5.0)*25;
#endif

#if 0 /*资源检查*/
        if(frmlg.cols>0)imshow("frmlg", frmlg);
        if(frmrg.cols>0)imshow("frmrg", frmrg);
        waitKey(10);
        continue;
#endif

#if 1
        fml = frmlg.clone();
        fmr = frmrg.clone();
#else
        undistort(frmlg, fml, d3.cameraMatrix[0], d3.distCoefficients[0] /*, d3.cameraMatrix[3]*/);
        undistort(frmrg, fmr, d3.cameraMatrix[1], d3.distCoefficients[1] /*, d3.cameraMatrix[3]*/);
#endif

        //       CV_VERSION_MAJOR
        /*CAP_OPENCV_MJPEG

        #if CV_VERSION_MAJOR == 3
        #define CV_CAP_PROP_FRAME_COUNT cv::CAP_PROP_FRAME_COUNT
        #define CV_CAP_PROP_POS_FRAMES cv::CAP_PROP_POS_FRAMES
        #endif
                cv::VideoWriter outputVideo(save_file, CV_FOURCC('D', 'I', 'V', 'X'),
                            30, size, true);
           */
        vr vrP, vrP_, vgird;
        // vgird=me.l[0].d4d.fd(grid, frmlg, frmrg);         //100ms

        // cout<<"["<<vgird.lp.x<<"]";
        /*double girddst=norm(vgird.e());
        if(girddst>10*mm && girddst<10000*mm){*/
#if 0
            me.dgrid(wgird, 25*mm, 25*mm, 1);
        /*}*/
#endif
#ifndef findbug

#ifdef girdisyellow
        imshow("sl", hsvreg(frmlg, 11, 68, 0, 255, 30, 255));
        imshow("sr", hsvreg(frmrg, 11, 68, 0, 255, 30, 255));

        imshow("sl1", hsvreg(frmlg, 0, 255, 0, 255, 0, 10));
        imshow("sr1", hsvreg(frmrg, 0, 255, 0, 255, 0, 20));
#endif
        m = me.l[1].b[1].p[lf2][3] << me.l[1].b[0].p[lf2][3];
        if (kV == 0)
        {
            me.l[0].d4d.blksz = 16;
            double c2 = fyml.c * 2;
            for (int v = me.l[0].d4d.blksz; v < eht - me.l[0].d4d.blksz; v++)
            {
                int v_ = eht - v;
                // me.l[0].d4d.blk3=Mat(frmrg, Rect(0, v_, ewd, me.l[0].d4d.blksz));
                for (int u = 0; u < ewd - me.l[0].d4d.blksz; u++)
                {
                    me.l[0].d4d.blk3 =
                        Mat(frmrg, Rect(0, v_, u + me.l[0].d4d.blksz, me.l[0].d4d.blksz));
                    vrP.lp = Point(u, v);
                    if (!(u % me.l[0].d4d.blksz) && !(v % me.l[0].d4d.blksz))
                    {
                        vrP.rp = me.l[0].d4d.fdr(vrP.lp, frmlg);
                        if (vrP.rp.x > 0 && vrP.rp.x < vrP.lp.x)
                        {
                            for (int bv = 0; bv < me.l[0].d4d.blksz; bv++)
                            {
                                for (int bu = 0; bu < me.l[0].d4d.blksz; bu++)
                                {
                                    Point3d e = m * rpr(vrP.voe()), e1(bu, bv, 0);
                                    d3.dots[u + bu][v + bv] =
                                        e3(e + e1 * e.z / c2, frmlg.at<Vec3b>(v_ - bv, u + bu));
                                }
                            }
                        }
                        else
                        {
                            for (int bv = 0; bv < me.l[0].d4d.blksz; bv++)
                            {
                                for (int bu = 0; bu < me.l[0].d4d.blksz; bu++)
                                {
                                    d3.dots[u + bu][v + bv].dt(Point3d(0));
                                }
                            }
                        }
                    }
                    if (u == ms[0].x && v == eht - ms[0].y) { vrP_ = vrP; }
                }
            }
        }
#ifdef OPENCV_CORE_CUDA
        if (kV == 1)
        {
            lft.upload(frmlg);
            rgt.upload(frmrg);
            if (kF) { d3.dffd(lft, rgt); }
            // copyMakeBorder(frmlg, frmlg, 0, 0, 80, 0, BORDER_REPLICATE);  //防止黑边
            // copyMakeBorder(frmrg, frmrg, 0, 0, 80, 0, BORDER_REPLICATE);

            bm->compute(lft, rgt, dsp);
            dsp.download(disp);
            disp = disp.colRange(81, disp.cols);
            Mat disp8U = Mat(disp.rows, disp.cols, CV_8UC1);
            normalize(disp, disp8U, 0, 255, NORM_MINMAX, CV_8UC1);

            int sc;
            for (int v = 1; v < eht - 1; v++)
            {
                int v_ = eht - v;
                for (int u = 1; u < ewd - 1; u++)
                {
                    vrP.lp = Point(u, v);
                    sc = disp.at<unsigned char>(v_, u);
                    vrP.rp = Point(u - sc, v);
                    if (u == ms[3].x && v == eht - ms[3].y && int(vrP.rp.x) != -1)
                    {
                        vrP_ = vrP;
                    } //标记点对
                    d3.dots[u][v].dt(rpr(vrP.voe()));
                    d3.dots[u][v].dt(frmlg.at<Vec3b>(v_, u));
                }
            }
        }
#endif
        if (kV == 2)
        {
            cv::Mat l16s, r16s;

            frmlg.convertTo(l16s, CV_16SC3);
            frmrg.convertTo(r16s, CV_16SC3);
       /*
            cv::imshow("frmlgmov", frmlgmov);
            cv::imshow("frmlgstep", frmlgstep);
            cv::imshow("frmrgmov", frmrgmov);
            cv::imshow("frmrgstep", frmrgstep);
       */
            for (int v = 20; v < eht - 20; v++)
            {
                int v_ = eht - v;
                int blksz = 32;
                for (int u = 20; u < ewd - 20; u++)
                {
                    cv::Mat blkl = submat(l16s, p2d(u, v), blksz);
                    std::vector<p2d> dfrts;
                    for(int d = 0; d < 180; d++)
                    {
                        if(u - d < 0)break;
                        cv::Mat blkr = submat(r16s, p2d(u - d, v), blksz);
                        cv::Mat dfr = blkl - blkr;
                        cv::Mat dfrp;
                        cv::pow(dfr, 2, dfrp);
                        cv::Scalar sums = cv::sum(dfrp);
                        double dfrt = sums[0] + sums[1] + sums[2];                                               //SSD算法
                        /*if(dfrt < 260000)*/dfrts.push_back(p2d(dfrt, d));
                    }
                    std::sort(dfrts.begin(), dfrts.end(),
                              [](p2d &a, p2d &b)
                    {
                        return a.x < b.x;                                                                 //排序最小差异 胜者为王
                    });
                    int w;
                    if(dfrts.size())
                    {
                        w = u - dfrts[0].y;
                    }
                    else
                    {
                        w = u;
                    }
                    vr vrP;
                    vrP.lp = p2d(u, v_);
                    vrP.rp = p2d(w /*u - 180*/, v_);
                    cv::Vec3s cl00 = frmlg.at<cv::Vec3b>(p2d(u, v));
                    me.dotf(m * rpr(vrP.vo().e2()) - me.l[0].b[0].p[bdo][0].e, cl00);
                }
            }/* */
        }

#if 1

        if (kV == 3)
        {
            vr vrP, vrP_;
            cvtColor(frmlg, frmlh, COLOR_RGB2HSV);
            cvtColor(frmrg, frmrh, COLOR_RGB2HSV);
            color9 cl(frmlg, frmlh, fml, 640, 480), cr(frmrg, frmrh, fmr, 640, 480);
            for (int v = 1; v < eht - 1; v++)
            {
                int v_ = eht - v;
                for (int u = 1; u < ewd - 1; u++)
                {
                    vrP.lp = Point(u, v);
                    vrP.rp = d3.fdr2(vrP.lp, cl, cr);
                    if (int(vrP.rp.x) != -1)
                    {
                        d3.dots[u][v].dt(m * rpr(vrP.voe()));
                        d3.dots[u][v].dt(frmlg.at<Vec3b>(p2d(u, v_)));
                    }
                    else
                    {
                        d3.dots[u][v].dt(Point3d(0, 0, 0));
                        d3.dots[u][v].dt(frmlg.at<Vec3b>(p2d(u, v_)));
                    }
                    if (u == ms[3].x && v == ms[3].y) { vrP_ = vrP; }
                }
            }
        }
#endif


        if (kV == 4)
        {
            vr vrP;
            for (int v_ = 1; v_ < eht - 5; v_ += 5)
            {
                int v = eht - v_;
                for (int u = 1; u < ewd - 5; u += 5)
                {
                    vrP.lp = p2d(u, v_);
                    scalar cl = frmlg.at<Vec3b>(p2d(u, v));
                    for (int k = 1; k < u; k += 5)
                    {
                        vrP.rp = p2d(k, v_);
                        scalar cr = frmrg.at<Vec3b>(p2d(k, v));
                        if (sbs(cl, cr))
                        {
                            // cout<</*p3d(vb.ap, vb.bt, vb.gm)*/ve;
                            me.dotf(m * rpr(vrP.vo().e2()) - me.l[0].b[0].p[bdo][0].e,
                                    (cl + cr) / 2);
                            // me.dotf(rpr(vrP.vo().e()), m)-me.l[0].b[0].o.e, (cl+cr)/2);
                        }
                    }
                }
            }
#if 0
            for(int v=1;v<100;v++){
                Point2d p(20, 480-v);
                frmlg.at<Vec3b>(p)=Vec3b(0, 255, 0);
            }
#endif
#if 0
            p3d test (bipolar(60, 80, 30).e());
            bipolar testb(test);
            vr testv(test);
            gmsg[2]="v:[("+str(testv.lp.x)+", "+str(testv.lp.y)+"), ";
            gmsg[3]="   ("+str(testv.rp.x)+", "+str(testv.rp.y)+")]";
            gmsg[4]="e:["+str(test.x)+", "+str(test.y)+", "+str(test.z)+"]";
            gmsg[5]="b:["+str(testb.ap)+", "+str(testb.bt)+", "+str(testb.gm)+"]";
#endif
        }

        if (kV == 5)
        {
            std::vector<vr> cbc, cbcsrc, cbcdst;

            d3.findChessboard(frm99l, frm99r, cabmsg, cbc);

            p3d scrl(-640 * mm - 720, -560, -150 * mm - 6), scrr(-640 * mm, -560, -150 * mm - 6);

            me.dotf(scrl, scalar(255, 0, 0));
            me.dotf(scrr, scalar(255, 255, 0));
            for (ulong i = 0; i < cbc.size(); i++)
            {
                scalar s(0, 255, 0);
                vr cbcr = cbc[i].f().vo();
                cbcsrc.push_back(cbc[i]);
                me.dotf(m * rpr(cbcr.b().e()) /*-me.l[1].b[2].o.e*/, s);
                me.dotf(p3d(cbcr.lp.x, cbcr.lp.y, 0) + scrl, s);
                me.dotf(p3d(cbcr.rp.x, cbcr.rp.y, 0) + scrr, s);
            }

            std::vector<p3d> ps;
            for (double j = 0; j < 5; j++)
            {
                for (double i = 0; i < 10; i++)
                {
                    p3d p(i * 24 * mm - 4.5 * 24 * mm, j * 24 * mm - 2 * 24 * mm, 500 * mm),
                        rprp(rpr(p));
                    scalar s(255 / 10 * i, 255 / 5 * j, 256);
                    me.dotf(rprp, s / 2);
                    ps.push_back(rprp);
                    vr v(p);
                    cbcdst.push_back(vr(v.lp + p2d(320, 240), v.rp + p2d(320, 240)));
                    me.dotf(p3d(v.lp.x, v.lp.y, 0) + scrl, s);
                    me.dotf(p3d(v.rp.x, v.rp.y, 0) + scrr, s);
                }
            }

            if (cbc.size() == 50)
            {
                Point2f srcl[] = {cbcsrc[40].lp, cbcsrc[49].lp, cbcsrc[0].lp, cbcsrc[9].lp},
                        dstl[] = {cbcdst[9].lp, cbcdst[0].lp, cbcdst[49].lp, cbcdst[40].lp},
                        srcr[] = {cbcsrc[40].rp, cbcsrc[49].rp, cbcsrc[0].rp, cbcsrc[9].rp},
                        dstr[] = {cbcdst[9].rp, cbcdst[0].rp, cbcdst[49].rp, cbcdst[40].rp};

                d3.newml = getPerspectiveTransform(srcl, dstl);
                d3.newmr = getPerspectiveTransform(srcr, dstr);

                warpPerspective(frm99l, fml, d3.newml, Size(ewd, eht));
                warpPerspective(frm99r, fmr, d3.newmr, Size(ewd, eht));
            }

            puttext(maketag(ps[9], wo), ps[9] + p3d(-48, -48, 0), p3d(96, 96, 2), 16,
                    scalar(255, 255, 255), scalar(0, 0, 255));
        }
        static vector<regp> vp_;
        if (kV == 6)
        {
            std::vector<vr> cbc, cbcsrc;
            std::vector<p3d> cbcdst, ps;

            d3.findChessboard(frmlg, frmrg, cabmsg, cbc);

            p3d scrl(-640 * mm - 720, -560, -150 * mm - 6), scrr(-640 * mm, -560, -150 * mm - 6);

            for (ulong i = 0; i < cbc.size(); i++)
            {
                scalar s(0, 255, 0);
                vr cbcr = cbc[i].f().vo();
                p3d cbce = m * rpr(cbcr.b().e()) /*-me.l[1].b[2].o.e*/;
                cbcdst.push_back(cbce);
                me.dotf(cbce, s);
                me.dotf(p3d(cbcr.lp.x, cbcr.lp.y, 0) + scrl, s);
                me.dotf(p3d(cbcr.rp.x, cbcr.rp.y, 0) + scrr, s);
            }

            for (double j = 0; j < 5; j++)
            {
                for (double i = 0; i < 10; i++)
                {
                    p3d p(i * 24 * mm - 4.5 * 24 * mm, j * 24 * mm - 2 * 24 * mm, 500 * mm),
                        rprp(rpr(p));
                    scalar s(255 / 10 * i, 255 / 5 * j, 256);
                    me.dot(rprp, s);
                    ps.push_back(rprp);
                    vp_.push_back(regp(rprp, p3d(i, j, 0), 0, 0));
                    vr v(p);
                    me.dotf(p3d(v.lp.x, v.lp.y, 0) + scrl, s);
                    me.dotf(p3d(v.rp.x, v.rp.y, 0) + scrr, s);
                }
            }

            if (cbcdst.size())
                puttext(maketag(cbcdst[0], wo), cbcdst[0] + p3d(-48, -48, 0), p3d(96, 96, 2), 16,
                        scalar(255, 255, 255));

            puttext(maketag(ps[9], wo), ps[9] + p3d(-48, -48, 0), p3d(96, 96, 2), 16,
                    scalar(255, 255, 255), scalar(0, 0, 255));
        }
        ddot d;

        d.e.z = 1000 * mm;
        d.cl = scalar(255, 0, 0);
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.z = 2000 * mm;
        d.cl = scalar(255, 0, 0);
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.z = 3000 * mm;
        d.cl = scalar(255, 0, 0);
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));

        d.e.z = 500 * mm;
        d.cl = scalar(0, 255, 0);
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.z = 1500 * mm;
        d.cl = scalar(0, 255, 0);
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.z = 2500 * mm;
        d.cl = scalar(0, 255, 0);
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));

        d.e.z = 200 * mm;
        d.e.y = -50 * mm;
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.y = -100 * mm;
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.y = -150 * mm;
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.y = -200 * mm;
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.y = -250 * mm;
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.y = -300 * mm;
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.y = -350 * mm;
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.y = -400 * mm;
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.y = -450 * mm;
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.y = -500 * mm;
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.y = -550 * mm;
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
        d.e.y = -600 * mm;
        me.dot(d.e, d.cl);
        vp_.push_back(regp(d.e));
#if 0
        gm44d m=me.l[1].b[0].lf2[3]*me.l[1].b[1].lf2[3];
        for (int v=0;v<eht;v++) {
            for (int u=0;u<ewd;u++){
               p3d d=d3.dots[u][v].e();
               d3.dots[u][v].dt(d, m)-me.l[0].b[0].o.e);
            }
        }
#endif
        me.cus[10].e2v();
        String s1 = "光标vr:" + str(int(me.cus[10].v.lp.x)) + ", " + str(int(me.cus[10].v.lp.y)) +
                    ", " + str(int(me.cus[10].v.rp.x)) + ", " + str(int(me.cus[10].v.rp.y));

        String s2 = "光标e:" + str(int(me.cus[10].e.x / mm)) + ", " +
                    str(int(me.cus[10].e.y / mm)) + ", " + str(int(me.cus[10].e.z / mm));

        String s3;

        me.dboxf(me.cus[10], scalar(230, 122, 42));

        if (DlibFacel.size())
        {
            s3 = "人脸:";
            if (int(DlibFacel[0].id) < 1000)
            {
                s3 = "name:" + DlibFacel[0].name + " id: " + str(int(DlibFacel[0].id)) + " , " +
                     str(double(DlibFacel[0].mc)) + str(int(Faces.size()));
            }
        }

        for (ulong i = 0; i < GoL.size(); i++)
        {
            circle(fml, GoL[i].p, 4,
                   Scalar(0, 255 * int(GoL[i].id == 1.0f), 255 * int(GoL[i].id != 1.0f)), 2);
        }

        for (ulong i = 0; i < GoR.size(); i++)
        {
            circle(fmr, GoR[i].p, 4,
                   Scalar(0, 255 * int(GoR[i].id == 1.0f), 255 * int(GoR[i].id != 1.0f)), 2);
        }

        Scalar sl, sr;
        if (ms[3].x < ewd && ms[3].y < eht)
        {
            sl = frmlg.at<Vec3b>(ms[3].y, ms[3].x);
            sr = frmrg.at<Vec3b>(ms[3].y, ms[3].x);

            circle(frmlg, Point(ms[3].x, ms[3].y), 3, sl, 5);
            circle(frmrg, Point(ms[3].x, ms[3].y), 3, sr, 5);
            circle(frmlg, Point(ms[3].x, ms[3].y), 5, Scalar(0, 255, 255), 1);
            circle(frmrg, Point(ms[3].x, ms[3].y), 5, Scalar(0, 255, 255), 1);
        }
        String s4 = "(" + str(ms[3].x) + ", " + str(ms[3].y) + "), (" + str(int(sl.val[0])) + ", " +
                    str(int(sl.val[1])) + ", " + str(int(sl.val[2])) + ")(" + str(int(sr.val[0])) +
                    ", " + str(int(sr.val[1])) + ", " + str(int(sr.val[2])) + ")";
#if 0
        printf("%s", (color(sl).name()+", "+color(sr).name()+" ").data());
        printf("\n<%d, %d, %d>", color(sl).h, color(sl).s, color(sl).v);
        printf("  <%d, %d, %d>", color(sr).h, color(sr).s, color(sr).v);
#endif

        if (Dv == 2 || Dv == 3)
        {

            circle(frmlg, Point(ewd2, eht2), 1, rnds());
            circle(frmlg, mdl.mp2, 2, Scalar(255, 0, 0));
            circle(frmlg, pL, 3, Scalar(0, 200, 0));
            circle(frmlg, rL, 3, Scalar(200, 0, 0));
            circle(frmlg, oL, 3, Scalar(0, 0, 255));
            // imshow("frmlg", frmlg);               //<<<<<

            // imshow("fml", fml);

            // imshow("frmlh", frmlh);               //<<<<<
            // imshow("frmll", frmll);
            //<<<<<
            circle(frmrg, Point(ewd2, eht2), 1, rnds());
            circle(frmrg, mdr.mp2, 2, Scalar(255, 0, 0));
            circle(frmrg, pR, 3, Scalar(0, 200, 0));
            circle(frmrg, rR, 3, Scalar(200, 0, 0));
            circle(frmrg, oR, 3, Scalar(0, 0, 255));
            // imshow("frmrg", frmrg);               //<<<<<

            // imshow("fmr", fmr);

            // if(frm80.cols && K01)imshow("frm11", frm80);

            // if(frm11l0.cols)imshow("frm11l0", frm11l0);
            // if(frm11r0.cols)imshow("frm11r0", frm11r0);

            if (!mw)
            {
                mw = 1;
                moveWindow("frmlg", 1 * ewd, 0);
                moveWindow("frmrg", 2 * ewd, 0);

                moveWindow("fml", 1 * ewd, 480 + 60);
                moveWindow("fmr", 2 * ewd, 480 + 60);
            }
        }
        if (kC)
        {
            if (kO)
            {
                Mat cbmatl = frmlg.clone(), cbmatr = frmrg.clone(), cbmatl1 = fml.clone(),
                    cbmatr1 = fmr.clone(); //避免访问冲突
                d3.calib3d(cbmatl, cbmatr, cabmsg, cbmatl1, cbmatr1);
                initUndistortRectifyMap(d3.cameraMatrix[0], d3.distCoefficients[0], R1,
                                        d3.cameraMatrix[0], frm99l.size(), CV_32FC1, mapx[0],
                                        mapy[0]);

                initUndistortRectifyMap(d3.cameraMatrix[1], d3.distCoefficients[1], R1,
                                        d3.cameraMatrix[1], frm99r.size(), CV_32FC1, mapx[1],
                                        mapy[1]);
                kO = 0;
                frmlg = cbmatl;
                frmrg = cbmatr;
                fml = cbmatl1;
                fmr = cbmatr1;
            }
            long tmd = tmn() - tm0;
            if (tmd > 1500)
            {
                tm0 = tmn();
                gmsg[1] = "寻找棋盘格, 按O键开始标定" + str(int(tmd));
                // kO=1;
            }
            puttext(cabmsg, p3d(-640 * mm + 720, 600, -150 * mm), p3d(640, 520, 10));
        }

        gmsg2[0] = "fps:" + str(fps(0)) + ", " + str(fps1);
        gmsg2[1] = s1;
        gmsg2[2] = s2;
        gmsg2[3] = s3;
        gmsg2[4] = s4;
        double memt = double(sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE)),
               memf = double(sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE));
        gmsg2[5] = "已用内存:" + str((memt - memf) / 1024 / 1024) + "兆 " +
                   str((memt - memf) / memt * 100) + "%";
        gmsg2[6] = "全部内存:" + str(memt / 1024 / 1024) + "兆";
        gmsg2[7] = "识别模块:" + str(enb[0]) + str(enb[6]) + str(enb[9]) + str(enb[16]) +
                   str(enb[30]) + " " + str(enb[31]) + str(enb[32]) + str(enb[33]) + str(enb[34]) +
                   str(enb[35]) + " " + str(enb[80]) + str(enb[89]) + str(enb[100]) + str(enb[101]);
        if ((memt - memf) / memt * 100 > 195)
        {
            while (run0)
            {
                cout << "内存使用超过99%=" << (memt - memf) / memt * 100;
                sleep(5);
            }
        }

        tcr.e =
            me.l[1].b[2].p[rh3][2].c.pt[0][0][0][0] -
            (me.l[1].b[2].p[rh3][2].c.pt[0][0][0][0] - me.l[1].b[2].p[rh3][3].c.pt[1][0][1][0]) / 2;

        me.dot(tcr.e, tcr.cl);
        vp_.push_back(regp(tcr.e));
        me.dot(me.cus[10].e, fc_blue, 8);
        vp_.push_back(regp(me.cus[10].e, p3d(0), 0, 0, 1));
        me.dot(me.l[1].b[2].p[rh3][1].e, me.l[1].b[2].p[rh3][1].cl);
        vp_.push_back(regp(me.l[1].b[2].p[rh3][1].e, p3d(0), 0, 0, 1));
        me.dot(me.l[1].b[2].p[rh3][2].e, me.l[1].b[2].p[rh3][2].cl);
        vp_.push_back(regp(me.l[1].b[2].p[rh3][2].e, p3d(0), 0, 0, 1));
        me.dot(me.l[1].b[2].p[rh3][3].e, me.l[1].b[2].p[rh3][3].cl);
        vp_.push_back(regp(me.l[1].b[2].p[rh3][3].e, p3d(0), 0, 0, 1));

        puttext(std::vector<std::string>(msg22), me.cus[10].e + p3d(192, 192, 2), p3d(192, 192, 2),
                20, fc_blue, fc_cyan);

        puttext(gmsg2, p3d(-640 * mm, 600, -150 * mm), p3d(640, 520, 10));

        puttext(gmsg, p3d(-640 * mm - 720, 600, -150 * mm), p3d(640, 520, 10));
#if 0
        puttext(maketag(wo, wo), wo.e+p3d(-48, -48, 0), p3d(96, 96, 2), 16, scalar(255, 255, 255), 0);

        puttext(maketag(me.l[1].b[2].o.e, wo),
                me.l[1].b[2].o.e          +p3d(-48, -48, 0), p3d(96, 96, 2), 16, scalar(255, 255, 255));

        puttext(maketag(me.l[1].b[0].o.e, wo),
                me.l[1].b[0].o.e          +p3d(-48, -48, 0), p3d(96, 96, 2), 16, scalar(255, 255, 255));


        puttext(maketag(me.l[1].b[2].rf0[0].d[1].O(), wo),
                me.l[1].b[2].rf0[0].d[1].O()+p3d(-48, -48, 0), p3d(96, 96, 2), 16, scalar(255, 255, 255));

        puttext(maketag(me.l[0].b[2].rf1[0].d[0].O(), wo),
                me.l[0].b[2].rf1[0].d[0].O()+p3d(-48, -48, 0), p3d(96, 96, 2), 16, scalar(255, 255, 255));

        puttext(maketag(me.l[0].b[2].rf0[0].d[0].O(), wo),
                me.l[0].b[2].rf0[0].d[0].O()+p3d(-48, -48, 0), p3d(96, 96, 2), 16, scalar(255, 255, 255));

        puttext(maketag(me.l[0].b[2].rh1[0].d[0].O(), wo),
                me.l[0].b[2].rh1[0].d[0].O()+p3d(-48, -48, 0), p3d(96, 96, 2), 16, scalar(255, 255, 255));

        puttext(maketag(me.l[0].b[2].rh0[0].d[0].O(), wo),
                me.l[0].b[2].rh0[0].d[0].O()+p3d(-48, -48, 0), p3d(96, 96, 2), 16, scalar(255, 255, 255));

        puttext(maketag(me.l[0].b[2].ubd[0].d[2].O(), wo),
                me.l[0].b[2].ubd[0].d[2].O()+p3d(-48, -48, 0), p3d(96, 96, 2), 16, scalar(255, 255, 255));

        puttext(maketag(me.l[0].b[2].hed[0].d[0].O(), wo),
                me.l[0].b[2].hed[0].d[0].O()+p3d(-48, -48, 0), p3d(96, 96, 2), 16, scalar(255, 255, 255));
#endif
#ifdef Mechanical_arm
#if 0
        puttext(maketag(me.l[0].b[2].rh1[0].d[1].O(), wo),
                me.l[0].b[2].rh1[0].d[1].O()+p3d(-48, -48, 0), p3d(96, 96, 2), 16, scalar(255, 255, 255));
        puttext(maketag(me.l[0].b[2].rh3[0][0].d[0].O(), wo),
                me.l[0].b[2].rh3[0][0].d[0].O()+p3d(-48, -48, 0), p3d(96, 96, 2), 16, scalar(255, 255, 255));
#endif
        if (vp0p.f == 1) vp0pf1 = vp0p;

#ifdef Mechanical_arm_data
        p3d gdoo(me.l[1].b[2].ubd[0].d[2].O());
#else
        p3d gdoo(wo.e);
#endif
#if 1 //画个棋盘
        for (ulong x = 0; x < 9; x++)
        {
            std::vector<obj> gd0;
            for (ulong y = 0; y < 9; y++)
            {
                obj gd(
                    gdoo +
#ifdef Mechanical_arm_data
                        p3d(-88 * mm, -(128 + 13) * mm - y * 23 * mm, -4 * 25 * mm + x * 25 * mm),
#else
                        p3d(-92 * mm + y * 25 * mm, -180 * mm, 4 * 25 * mm + x * 23 * mm),
#endif
                    p3d(0), p3d(0), fc_yellow);
                gd0.push_back(gd);
            }
            gdo.push_back(gd0);
            gd0.clear();
        }

        gdo1 = gdo;
        gdo.clear();
        mc = 1;
#endif

        vp_.push_back(regp(p3d(-160 * mm, 0, 0), p3d(0), 0, 0, 10));
        vp_.push_back(regp(p3d(-200 * mm, 0, 0), p3d(0), 0, 0, 11));
        me.dot(p3d(-160 * mm, 0, 0), fc_red);
        me.dot(p3d(-200 * mm, 0, 0), fc_red);

        vp_.push_back(regp(me.cus[18].e, p3d(0), 0, 0, 5));
        vp_.push_back(regp(me.cus[10].e + me.cus[10].vy() * 66 * mm, p3d(0), 0, 0, 5));
        me.dot(me.cus[10].e + me.cus[10].vy() * 66 * mm, fc_cyan, 4);

        me.dboxf(me.cus[18], fc_black);

#if 0 //测试包含碰撞
        me.dlinef(me.l[1].b[2].ubd[0].ls().linesa(cl[7]));
        me.dlinef(me.l[1].b[2].ubd[0].rs().linesa(cl[7]));
        me.dlinef(me.l[1].b[2].ubd[0].ds().linesa(cl[7]));
        me.dlinef(me.l[1].b[2].ubd[0].us().linesa(cl[7]));
        me.dlinef(me.l[1].b[2].ubd[0].bs().linesa(cl[7]));
        me.dlinef(me.l[1].b[2].ubd[0].fs().linesa(cl[7]));
        string msgt=" ls:"+str(me.l[1].b[2].ubd[0].ls().dist(me.cus[10].e)/mm)+
                    " rs:"+str(me.l[1].b[2].ubd[0].rs().dist(me.cus[10].e)/mm);
        me.dlinef(me.l[1].b[2].ubd[0].c.lines(fc_red), 4);

        vector<axis> cusmm= me.l[1].b[2].ubd[0].ds().in()*
                            me.l[1].b[2].ubd[0].c.lines();
        me.dlinef(cusmm, 4);
        me.dlinef(me.l[1].b[2].ubd[0].ds().out()*axis(cusmm[0]), 8);
#endif
        if (me.l[1].b[2].p[ubd][0] & me.cus[10] || me.l[1].b[2].p[ubd][0] & me.cus[10].e /**/)
        {
            me.dot(me.cus[10].e + p3d(0, 15 * mm, 0), fc_yellow, 12);
        }

#else
        puttext(maketag(me.l[0].b[2].rh2[0].d[1].O(), wo),
                me.l[0].b[2].rh2[0].d[1].O() + p3d(-48, -48, 0), p3d(96, 96, 2), 16,
                scalar(255, 255, 255));
#endif
        if (dgdo)
        {
            if (gdo1.size() == 9)
            {

                puttext(maketag(gdo1[0][0], wo), gdo1[0][0].e + p3d(-48, -48, 0), p3d(96, 96, 2),
                        16, scalar(255, 255, 255));

                puttext(maketag(gdo1[0][8], wo), gdo1[0][8].e + p3d(-48, -48, 0), p3d(96, 96, 2),
                        16, scalar(255, 255, 255));

                puttext(maketag(gdo1[8][0], wo), gdo1[8][0].e + p3d(-48, -48, 0), p3d(96, 96, 2),
                        16, scalar(255, 255, 255));

                puttext(maketag(gdo1[8][8], wo), gdo1[8][8].e + p3d(-48, -48, 0), p3d(96, 96, 2),
                        16, scalar(255, 255, 255));

                me.l[1].wd[10] = obj(gdo1[4][4].e, p3d(11 * 25 * mm, 10 * mm, 11 * 23 * mm));
            }

            for (uint i = 0; i < chesso.size(); i++)
            {
                int cl = 255 * int(chesso[i].id != 1.0f);
                cv::Scalar scl(cl, cl, cl);
                chesso[i].box(chesso[i].sz, 5.0 * mm);
                me.dbox(chesso[i], scl);
            }
            for (ulong x = 0; x < gdo1.size(); x++)
            {
                for (ulong y = 1; y < gdo1[x].size(); y++)
                {
                    me.dlinef(gdo1[x][y].e, gdo1[x][y - 1].e, gdo1[x][y].cl, 1, l2);
                }
            }
            for (ulong y = 0; y < gdo1.size(); y++)
            {
                me.ball(me.l[2].worldm * gdo1[0][y].e, gdo1[0][y].cl, 8);
                vp_.push_back(regp(me.l[2].worldm * gdo1[0][y].e, p3d(0), 0, 0, 5));
                for (ulong x = 1; x < gdo1.size(); x++)
                {
                    me.dlinef(gdo1[x][y].e, gdo1[x - 1][y].e, gdo1[x][y].cl, 1, l2);
                    vp_.push_back(regp(me.l[2].worldm * gdo1[x][y].e, p3d(0), 0, 0, 5));
                    me.ball(me.l[2].worldm * gdo1[x][y].e, gdo1[x][y].cl, 8);
                }
            }
        }

        p3d DrawWavePoint(1000 * mm, 0, -500 * mm);
        me.dotf(DrawWavePoint, fc_red);
        vp_.push_back(regp(DrawWavePoint, p3d(0), 0, 0, 21));

        mc = 0;
        putText(frmlg, s2, Point(10, 20), 1, 0.8, Scalar(0, 0, 255), 1); /**/

        waitKey(10);                            //<<<<<没这句imshow无效

        while (sig_activated(16)) { usleep(10 * 1000); }
        obj a, b;
        for (uint i = 0; i < DetectedFaceObj.size(); i++)
        {
            DetectedFaceObj[i].box(p3d(180 * mm, 180 * mm, 180 * mm), 50 * mm);
            me.dboxf(DetectedFaceObj[i], cv::Scalar(125, 200, 0));
            puttext(maketag(DetectedFaceObj[i], wo), DetectedFaceObj[i].e + p3d(-48, -48, 0),
                    p3d(96, 96, 96), 16, scalar(255, 255, 255));
        }
        cv::Point3d sz = cv::Point3d(180 * mm, 180 * mm, 180 * mm);
        for (uint i = 0; i < Face.size(); i++)
        {
            a.e = Face[i].e;
            a.box(sz, 50 * mm);
            b = me.l[0].currentm * a;
            me.dboxf(b, cv::Scalar(125, 200, 0));
            // me.dot(b.e, cv::Scalar(0, 255, 0));
            Face[i].e = b.e;
        }

        if (vp0p.p != p3d(0))
            puttext(maketag(vp0p.p, wo), vp0p.p + p3d(-66, -66, 0), p3d(128, 96, 2), 16,
                    scalar(255, 0, 255), scalar(0, 255, 255));
        if (vp0p.f == 1)
        {
            if (vp0p.n == 1)
            {
                keyPressed('L', 0, 0);
                vp0p.f = 0;
            }
            if (vp0p.n == 2)
            {
                dway = !dway;
                vp0p.f = 0;
            }
            if (vp0p.n == 3)
            {
                if (int(argx) == int(vp0p.arg.x))
                    dway2 = !dway2;
                else
                    dway2 = 1;
                argx = vp0p.arg.x;
                vp0p.f = 0;
            }
            if (vp0p.n == 4)
            {
                if (j3mp == vp0p.p) dway3 = !dway3;
                j3mp = vp0p.p;
                vp0p.f = 0;
            }
            if (vp0p.n == 5)
            {
                me.cus[10].e = vp0p.p;
                vp0p.f = 0;
                dg1 = 0;
                dg2 = 0;
            }
            if (vp0p.n == 6)
            {
                if (vct1_ == 0.0)
                    vct1_ = 1;
                else if (vct1_ == 1.0)
                    vct1_ = -1;
                else if (vct1_ < 0)
                    vct1_ = 0;
            }
            if (vp0p.n == 7)
            {
                if (vct2_ == 0.0)
                    vct2_ = 1;
                else if (vct2_ == 1.0)
                    vct2_ = -1;
                else if (vct2_ < 0)
                    vct2_ = 0;
            }
            if (vp0p.n == 8) { gmrh1b += vct1_; }
            if (vp0p.n == 9)
            {
                if (bn < 3)
                    ++bn;
                else
                    bn = 0;
                gmsg[2] = "身体" + str(bn);
            }
            if (vp0p.n == 10)
            {
                me.reftcr(lf2);
                // me.l[1].b[0].read("/home/root/eye/dt/view_bd[0]l.yml");
                vp0p.n = -1;
            }
            if (vp0p.n == 11)
            {
                me.reftcr(rf2);
                // me.l[1].b[0].read("/home/root/eye/dt/view_bd[0]r.yml");
                vp0p.n = -1;
            }
            if (vp0p.n == 21)
            {
                std::cout << "\033[34m 21 \033[0m" << std::endl;
                viewMat = true;
            }
        }
        if (vp0p.f == 3)
        {
            vp0p.f = 0;
            printf("\n{ %4.1f, %4.1f, %4.1f, %4.1f, %4.1f, %4.1f, %4.1f, %4.1f } , \r\n",
                   me.l[1].b[0].p[rh0][0].d[0].s, me.l[1].b[0].p[ubd][0].d[2].s,
                   me.l[1].b[0].p[rh1][0].d[0].s, me.l[1].b[0].p[rh0][0].d[1].s,
                   me.l[1].b[0].p[rh2][0].d[0].s, me.l[1].b[0].p[rh1][0].d[1].s,
                   me.l[1].b[0].p[rh3][1].d[0].s, me.l[1].b[0].p[rh3][0].d[0].s);
        }
        me.dlinef(ms3l);
        me.dot(ms3, scalar(0, 255, 255));
        me.dotf(vp0p1.p, downcolor);
        me.dboxf(vp0p1.p, p3d(10 * mm, 10 * mm, 10 * mm), downcolor);
        me.dotf(vp0p.p, scalar(0, 0, 255));
        me.dboxf(vp0p.p, p3d(10 * mm, 10 * mm, 10 * mm), scalar(0, 0, 255));

        frm99lt = translucent(frm99l, pt4d(1, 1, 1, 1));
        frm99rt = translucent(frm99r, pt4d(1, 1, 1, 1));
        frmlgt = translucent(frmlg, pt4d(1, 1, 1, 0.8));
        frmrgt = translucent(frmrg, pt4d(1, 1, 1, 0.8));
        fmlt = translucent(fml, pt4d(1, 1, 1, 0.7));
        fmrt = translucent(fmr, pt4d(1, 1, 1, 0.7));
        frm27lt = translucent(frm27l, pt4d(1, 1, 1, 1));
        frm27rt = translucent(frm27r, pt4d(1, 1, 1, 1));

        datadftMatSmTable4l = translucent(datadftMatSmTableL.t(), pt4d(0.76, 1.0, 0.8, 0.89));
        datadftMatSmTable4r = translucent(datadftMatSmTableR.t(), pt4d(0.76, 1.0, 0.8, 0.89));
        frmarv = 1;
        sem_post(&sig[7]);
        vplock = 1;
        vp = vp_;
        vplock = 0;
        vp_.clear();
        // cout<<" $"<<vp.size();
        while (sig_activated(7)) usleep(20 * 1000); // 等待7线程
#endif
#endif
        sem_post(&sig[0]); // 重启0线程
                           // std::cout<<"["<<me.nt<<"]";
    }
#ifdef OPENCV_CORE_CUDA
    lft.release();
    rgt.release();
    dsp.release();
    dsp2.release();
    bm.release();
#endif

    svface(Faces, "/home/root/eye/dt/face.yml");
    printf("Bye\n");
    run0 = false;
    usleep(6 * 1000 * 1000);
    return (0);
}
