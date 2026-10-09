#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "eye/cvface2/cvface2.h"

#include "eye/readfrm/readfrm.h"
#include "mind/3d/3d.h"
#include <zbar.h>

#define dbg

#include <csignal>
#include <semaphore.h>

static int SIG;
static bool run0 = true, run1 = true;
static std::string MyName;
static cv::Mat frm0, frm10, frm13, frm13b, frm6, frm34, frm35, frm80, frm1l, frm1r,
        frm80l = frm1l, frm80r = frm1r, frm99l, frm99r, frm9l, frm9r, frm27l, frm27r,
        d3ml, d3mr;
static bool ld = true;

static cv::Mat frm1, frm30, frm31, frm32, frm33,
        frm80l0, frm80r0;
static double /*,mr0,*//**传输力*/tspw = 1;

static float mc1;
static float mc0;
static int id0 = 0, id1 = 0, debug = 0, desk = 0, serv = 0, cloud = 0;

static cv::Point mpl, mpr;
static int nmarvl, nmarvr, ewd, eht, Dv = 0, bfm[4], wtj_, warp = 1;
static cv::String filename, sigr = "", sig2 = "", tsmd = "";
static NetCommunicationModule
/**帧传输      */ImageReader,
/**云传输      */ImageSender,
/**指令接收     */OderReceiver,
/**指令发送     */OderSender,

/**云Tcp透传转发*/ServerTcpOderSender,
/**云Tcp透传转发*/ClientTcpOderReceiver;

static string ThreadName[255];
extern int /**网络接入方式*/Dc, kC;

static cv::Mat mapx[2], mapy[2], distCoefficientsl, distCoefficientsr,
        cm0(3, 3, CV_32FC1), cm1(3, 3, CV_32FC1);

static std::string /**上位机IP*/PcIp,/**机器人IP*/RobotIp,/**云端IP*/CloudIp,
/**手机IP*/MbIp, MyIp, LcIP;
static int cmrl, cmrr,/*,flcd*/svrd, wdn = 0, kX = 0, onl1 = 10, sig6 = 0;
extern bool enb[500];
extern sem_t sig[500];
static byte seq[500];
static u_short UdpServerImageReceivePort,
        TcpServerImageReceivePort,
        TcpServerImageSendPort,
        TcpCloudPort,
        UdpCvOderPort,
        UdpFyOderPort;

static u_short ServerTcpReportPort,
        ServerTcpOderSendPort;

static int sfc[20], mode = 2/*,eht_=0*/;
static cv::Rect rect;

/*static pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;*/
static string /**全局消息*/gmsg[40];

void PrintNow() {
    time_t tt = time(nullptr);
    tm *t = localtime(&tt);
    if (wtj_)return;
    std::cout << std::endl;
    printf("%d-%02d-%02d %02d:%02d:%02d>>",
           t->tm_year + 1900,
           t->tm_mon + 1,
           t->tm_mday,
           t->tm_hour,
           t->tm_min,
           t->tm_sec);
    gmsg[7] = str(t->tm_year + 1900) + "-"
              + str(t->tm_mon + 1) + "-"
              + str(t->tm_mday) + " "
              + str(t->tm_hour) + ":"
              + str(t->tm_min) + ":"
              + str(t->tm_sec);
}

static double fps_[100], lastProcessTime[100];

double fps(int n) {
    double nowt = (double) clock() / 2;
    fps_[n] = nowt - lastProcessTime[n];
    lastProcessTime[n] = nowt;
    return 1 / (fps_[n] / 1000 / 1000);
}

struct robot {
    u_short eyeport;
    uint id;
    int threadid;
    int UdpClientSocketFd,
            TcpClientSocketFd;
    sockaddr UdpServerAddr;

    std::string ip,
            name;
    std::vector<std::vector<byte>> oders;
    cv::Mat Frm;
    int FrmArv;
    int FrmCcd;

    robot(u_short eyeport_,
          uint id_,
          std::string ip_,
          std::string name_,
          int UdpClientSocketFd_ = -1,
          int TcpClientSocketFd_ = -1) {
        eyeport = eyeport_;
        id = id_;
        ip = ip_;
        name = name_;
        UdpClientSocketFd = UdpClientSocketFd_;
        TcpClientSocketFd = TcpClientSocketFd_;
        FrmArv = 0;
    }

    int getUdpClientFd() {
        UdpClientSocketFd = ImageReader.ssc.GetUdpClientSocketFd(ip, eyeport, UdpClientSocketFd, UdpServerAddr);
        return UdpClientSocketFd;
    }
};

static std::vector<robot> MyMaster, MyRobotHosts, MyRobots;
static std::vector< /*组号*/
        std::vector< /*一组机器人*/
                robot>> AllRobots;
static uint RsvRobId = 0;
static int FrmArv = 0;

#include "CRC16.cpp"

void sendcmd(bytex data, ushort length = 12,
             byte seq = 1, byte datetype = 1, byte oder = 18, byte suboder = 1,
             std::vector<robot> bots = MyRobots) {
    byte datao[200];
    int offst = 0;
    datao[offst++] = 0x47;   //cout<<"@"<<offst<<","<<int(datao[0])<<","<<int(datao[1]);
    datao[offst++] = (length >> 8) & 0xFF;
    datao[offst++] = length & 0xFF;
    datao[offst++] = seq;
    datao[offst++] = datetype;
    datao[offst++] = oder;
    datao[offst++] = suboder;
    for (int i = 0; i < length - 8; i++) {
        datao[offst++] = data[i];
    }
    ushort crc16 = GetCRC16(datao + 1, length - 2);
    datao[offst++] = (crc16 >> 8) & 0xFF;
    datao[offst++] = crc16 & 0xFF;

    std::cout << std::endl;
    for (uint i = 0; i < AllRobots[bots[0].id].size(); i++) {
        std::cout << std::endl << "[OderSender] 发送 ";
        for (int i = 0; i < offst; i++) {
            printf(" %2.2X", datao[i]);
        }
    }
#if 1
    for (uint i = 0; i < AllRobots[bots[0].id].size(); i++) {
        if (AllRobots[bots[0].id][i].name[0] == '@' && AllRobots[bots[0].id][i].name.length() < 32) {
            for (uint j = 0; j < 168; j++) {
                datao[(167 - j) + 32] = datao[167 - j];
            }
            for (uint j = 0; j < 32; j++) {
                datao[j] = 0;
            }
            for (uint j = 0; j < AllRobots[bots[0].id][i].name.length(); j++) {
                datao[j] = byte(AllRobots[bots[0].id][i].name[j]);
            }
            OderSender.UdpClientSendData(datao, length + 1 + 32, AllRobots[bots[0].id][i].ip,
                                         AllRobots[bots[0].id][i].eyeport,
                                         AllRobots[bots[0].id][i].UdpClientSocketFd,
                                         AllRobots[bots[0].id][i].UdpServerAddr);
        } else {
            OderSender.UdpClientSendData(datao, length + 1, AllRobots[bots[0].id][i].ip,
                                         AllRobots[bots[0].id][i].eyeport,
                                         AllRobots[bots[0].id][i].UdpClientSocketFd,
                                         AllRobots[bots[0].id][i].UdpServerAddr);
        }
    }
#endif
    //Odrrt.msg=233;Odrrt.nmb=233;Odrrt.head=233;Odrrt.crc16=233;
    usleep(30 * 1000);
    /*
    std::cout<<std::endl<<" <<< ";
    sendodrrt.FromO(Odrrt);
    std::cout<<std::endl<<" >>> ";
    */
}

void sendcmd(byte suboder, byte data2, byte oder = 18, byte data3 = 0, byte data4 = 0,
             byte length = 12, byte datetype = 1, byte odernumber = 2,
             std::vector<robot> bots = MyRobots) {
    byte data[4];
    data[0] = data2;
    data[1] = data3;
    data[2] = data4;
    data[3] = 99;
    sendcmd(data, length, odernumber, datetype, oder, suboder, bots);
}

void sendcmd(std::string text,
             byte odernumber = 1, byte datetype = 3, byte oder = 2, byte suboder = 12,
             std::vector<robot> bots = MyRobots) {
    uint length = uint(text.length());
    if (length < 128) {
        byte r[128];
        for (uint i = 0; i < length; i++) {
            r[i] = byte(text[i]);
        }
        sendcmd(r, ushort(length) + 8, odernumber, datetype, oder, suboder, bots);
    }
}


void execute(byte seqn, byte suboder, string threadname, byte st = 2) {
    seq[suboder] =/*r[3]*/seqn;
    enb[suboder] = !enb[suboder];
    if (st == 0)enb[suboder] = 0;
    if (st == 1)enb[suboder] = 1;
    gmsg[0] = MyName + ":" + threadname + soc(enb[suboder]);
    std::cout << std::endl << gmsg[0] << " ";
    sendcmd(gmsg[0], seq[suboder], 3, 3, 12, MyMaster);
}

static byte state[8];

void sendstate(byte seq) {
    sendcmd(state, 16, seq, 3, 2, 2, MyMaster);
}

void *thr_fn2(void *) {
    ThreadName[2] = "信息打印";
    long t = 0;
    while (run0) {
        //if(frm0.empty() && frm11.empty()){usleep(10*1000);continue;}
#ifndef FYAIRO_1_0_0_ARM
        if (Dv == 2) {
            std::cout << "\033[1A";
            PrintNow();
            std::cout << enb[0] << enb[6] << enb[9] << enb[16] << enb[30] << " "
                      << enb[31] << enb[32] << enb[33] << enb[34] << enb[35] << " "
                      << enb[80] << enb[100] << enb[101];
        } else {
#endif
            std::cout << "\033[1A";
            PrintNow();
            std::cout << enb[0] << enb[6] << enb[9] << enb[16] << enb[30] << " "
                      << enb[31] << enb[32] << enb[33] << enb[34] << enb[35] << " "
                      << enb[80] << enb[100] << enb[101];

#ifndef FYAIRO_1_0_0_ARM
        }
#endif
        printf(" [%5.5ld]", t);
        t++;
        usleep(1000 * 1000);
    }
    return (nullptr);
}

void *thr_fn25(void *) {
    ThreadName[25] = "执行模块";
    byte r[4096];
    std::vector<byte> d;
    int run1 = 1,
            run2 = 1;
    int UdpServerFd = -1;
    sockaddr ClientAddr;
    while (run0) {
        int len = OderReceiver.UdpServerReceiveMessage(r, UdpServerFd, ClientAddr);
        if (run2) {
            run2 = 0;
            sendcmd(MyName + ":" + "视觉模块已准备好", 0, 3, 3, 12, MyMaster);
            setbit(state[0], 0, 1);
            sendstate(r[3]);
        }
        if (run1 && ImageReader.iop()) {
            run1 = 0;
            sendcmd(MyName + ":" + "摄像头开始工作", 0, 3, 3, 12, MyMaster);
            setbit(state[0], 1, 1);
            sendstate(r[3]);
        }
        if (len < 1) {
            usleep(1 * 1000);
            continue;
        }
        std::cout << std::endl;
        for (int i = 0; i < len; i++) {
            printf(" %2.2X", r[i]);
        }
        std::cout << std::endl;
        string name;
        int forward = 0;
        if (r[0] == '@' && len > 33) {
            char nm[32];
            for (uint i = 1; i < 32; i++) {
                nm[i - 1] = char(r[i]);
            }
            name = nm;
            std::cout << std::endl << "转发给" << name << ": ";
            forward = 1;
            for (uint i = 0; i < uint(len - 32); i++) {
                r[i] = r[i + 32];
            }
            len = len - 32;
        }
        if (!ConfirmOder(r, len))continue;
        printf("\n发送序号:%d 数据类型:%d 命令字:%d 子命令字:%d ",
               int(r[3]), int(r[4]), int(r[5]), int(r[6]));
        printf("\n数据: ");
        d.clear();
        for (int i = 6; i < len - 2; i++) {
            d.push_back(r[i]);
        }
        string s(d.begin() + 1, d.end());
        std::cout << s;
        if (forward) {
            for (uint i = 0; i < AllRobots.size(); i++) {
                if (name == AllRobots[i][0].name) {
                    std::vector<byte> oder;
                    for (int j = 0; j < len; j++) {
                        oder.push_back(r[j]);
                    }
                    AllRobots[i][0].oders.push_back(oder);
                    break;
                }
            }
            continue;
        }

        if (r[4] == 1 ||
            r[4] == 2 ||
            (r[4] == 3 && r[5] == 3 && r[6] == 12) ||
            (r[4] == 3 && r[5] == 2 && r[6] == 2)
                )
        {
            for (uint i = 0; i < MyMaster.size(); i++)
            {
                OderSender.UdpClientSendData(r, size_t(len), MyMaster[i].ip,
                                             MyMaster[i].eyeport, MyMaster[i].UdpClientSocketFd,
                                             MyMaster[i].UdpServerAddr);
            }
            continue;
        }
        if (r[4] == 3 && r[5] == 3) {

            execute(r[3], r[6], ThreadName[r[6]], r[7]);

            if (r[6] == 161) {
                int enb91 = enb[91];
                enb[91] = 0;
                int enb80 = enb[80];
                enb[80] = 0;
                if (enb[161]) {
                    MyRobotHosts = AllRobots[2];
                } else {
                    MyRobotHosts = AllRobots[0];
                }
                enb[80] = enb80;
                enb[91] = enb91;
            }
        }

        if (r[4] == 3 && r[5] == 2) {
            if (r[6] == 1) {
                sendstate(r[3]);
            }
        }
        setzero(r, len);
    }
    return nullptr;
}

// 判断线程是否被激活
bool sig_activated(uint id) {
    int val;
    sem_getvalue(&sig[id], &val);
    if (val > 0) return true;
    else return false;
}

static uint JobId;

void *thr_fn0(void *) {
    ThreadName[0] = "视频采集线程";
    ThreadName[1] = "视频编码程序";
    ThreadName[3] = "raw视频采集";
    ThreadName[79] = "raw视频发送";
    ThreadName[80] = "视频发送";
    ThreadName[91] = "视频远程Udp采集线程";
    ThreadName[102] = "精确人脸识别左";
    ThreadName[103] = "精确人脸识别右";
    ThreadName[161] = "云模式";
    nmarvl = false;
    if (Dv == 2 || Dv == 1 || Dv == 0) {
        while (ld && frm80.cols) {
            usleep(50 * 1000);
        }
        int msg[50];
//        std::cout<<"@"<<ImageReader.CaptureImage(frm0,enb[1]);
        int TcpClientSocketFd = -1,
                UdpServerSocketFd = -1;
        sockaddr TcpServerAddr,
                ClientAddr;

        int cnt80 = -1;
        int trs;
        ImageReader.clientname = MyName;
        while (run0) {
            if (enb[0]) {
                ImageReader.CaptureImage(frm0, enb[1]);
            }
            if (enb[3]) {
                ImageReader.CaptureRawImage();
            }
            //std::cout<<int(onl);

//            快速视频发送
            if (enb[79]) {
                trs = ImageReader.TcpClientSendPacket(msg, 0, TcpClientSocketFd, TcpServerAddr,
                                                      MyRobotHosts[0].ip, TcpServerImageReceivePort, cnt80);
            }
//            视频发送
            else if (enb[80]) {
                if (ImageReader.rddl || ImageReader.rddr) {
                    trs = ImageReader.TcpClientSendImage(msg, 0, TcpClientSocketFd, TcpServerAddr,
                                                         MyRobotHosts[0].ip, TcpServerImageReceivePort, cnt80);
                    if (trs == -104) {
                        cnt80 = -1;
                        TcpClientSocketFd = -1;
                        ImageReader.gnm = -1;
                        std::cout << std::endl << ImageReader.name << " 连接重置";
                    }
                }
            }
//            关闭tcp连接
            else {
                ImageReader.gnm = -1; // 重置 get name
                if (cnt80 == 0) {
                    ImageReader.ssc.TcpDisconnectSocket(TcpClientSocketFd);
                    cnt80 = -1;
                    TcpClientSocketFd = -1;
                    ImageReader.gnm = -1;
                }
            }


            if (enb[82]) {
                sem_post(&sig[82]);
                const uint CHECK_INT = 30; //查询间隙， 毫秒单位
                int tm = 0;
                while (sig_activated(82) && enb[82] && FrmArv == 0) {                //取消等待会图像闪烁异常
                    tm+=CHECK_INT;
                    gmsg[8] = "获取LAN图像中..." + str(tm) + "ms";
                    if (enb[99])sem_post(&sig[99]); // 等待时刷新屏幕
                    usleep(CHECK_INT*1000);
                }
                frm80 = AllRobots[RsvRobId][0].Frm.clone();
                FrmArv = 0;
            }

            if (enb[84]) {
                sem_post(&sig[84]);
                const uint CHECK_INT = 30; //查询间隙， 毫秒单位
                int tm = 0;
                while (sig_activated(84) && enb[84] && FrmArv == 0) {
                    tm+=CHECK_INT;
                    gmsg[8] = "获取LAN图像中..." + str(tm) + "ms";
                    if (enb[99])sem_post(&sig[99]); // 等待时刷新屏幕
                    usleep(CHECK_INT*1000);
                }
                frm80 = AllRobots[RsvRobId][0].Frm.clone();
                FrmArv = 0;
            }

            if (enb[81]) {
                sem_post(&sig[81]);
                /*
                while(sig_activated(81) && enb[81]){
                     usleep(10*1000);
                }
                */
            }
            if (enb[83]) {
                sem_post(&sig[83]);
                int tm = 0;
                // 等待视频信号
                while (sig_activated(83) && enb[83] && FrmArv == 0) {
                    tm++;
                    gmsg[8] = "获取WAN图像中..." + str(tm * 10) + "ms";
                    if (enb[99])sem_post(&sig[99]);
                    usleep(30 * 1000);
                }
                frm80 = AllRobots[RsvRobId][0].Frm.clone();
                FrmArv = 0;
            }

            if (!enb[0] && !enb[82] && !enb[83]) {
                usleep(20 * 1000);
            }

            JobId = RsvRobId;
            if (serv) {
                string name = AllRobots[RsvRobId][0].name;
                int mc = 0;
                for (uint i = 0; i < AllRobots.size(); i++) {
                    if ("@" + name == AllRobots[i][0].name) {
                        JobId = AllRobots[i][0].id;
                        mc = 1;
                    }
                }
                if (mc)MyRobots = AllRobots[JobId];
            }

            if (enb[91]) {
                int UdpClientSocketFd = -1;
                sockaddr UdpServerAddr;
                ImageReader.UdpClientSendImage(UdpClientSocketFd, UdpServerAddr);
            }

#ifndef FYAIRO_1_0_0_ARM
            if (enb[91]) {
                ImageReader.UdpServerReceiveImage(frm0, UdpServerSocketFd, ClientAddr);
            }
            if (enb[0]) {
                frm80l0 = cv::Mat(frm0, cv::Rect(0, 0, ewd, eht));
                frm80r0 = cv::Mat(frm0, cv::Rect(ewd, 0, ewd, eht));
            } else if ((enb[82] || enb[83] || enb[84]) && (ewd * 2 == frm80.cols && eht == frm80.rows)) {
                frm80l0 = cv::Mat(frm80, cv::Rect(0, 0, ewd, eht));
                frm80r0 = cv::Mat(frm80, cv::Rect(ewd, 0, ewd, eht));
            } else {

            }

            //cv::remap(frm80l0, frm80l, mapx[0], mapy[0], cv::INTER_LINEAR);
            //cv::remap(frm80r0, frm80r, mapx[1], mapy[1], cv::INTER_LINEAR);

            //undistort(frm80l0, frm80l,cm0,distCoefficientsl);
            //undistort(frm80r0, frm80r,cm0,distCoefficientsr);

            if (warp) {
                if (frm80l0.data!= nullptr && frm80r0.data!= nullptr)
                {
                    if(frm80l.data == nullptr)frm80l = frm80l0.clone();
                    if(frm80r.data == nullptr)frm80r = frm80r0.clone();
                    //warpPerspective(frm80l0, frm80l, d3ml, cv::Size(ewd, eht));
                    //warpPerspective(frm80r0, frm80r, d3mr, cv::Size(ewd, eht));
                }
            } else {
                // 单位矩阵,相当于复制
                // frm80l, frm80r data指针不能更改
                warpPerspective(frm80l0, frm80l, cv::Mat::eye(3,3,CV_32F),cv::Size(ewd, eht));
                warpPerspective(frm80r0, frm80r, cv::Mat::eye(3,3,CV_32F), cv::Size(ewd, eht));
            }

#else
            frm80l=ImageReader.frml;
            frm80r=ImageReader.frmr;
#endif
            //共12f
            if (!sig_activated(100) && enb[100]) {                        //-3f
                frm1l = frm80l;
                sem_post(&sig[100]);
            }
            if (!sig_activated(101) && enb[101]) {                        //-3f
                frm1r = frm80r;
                sem_post(&sig[101]);
            }

            if (!sig_activated(27) && enb[27]) {                          //-0f
                frm27l = frm80l;
                frm27r = frm80r;
                sem_post(&sig[27]);
            }

            if (!sig_activated(16) && enb[16]) {                          //-0f
                sem_post(&sig[16]);
            }

            if (!sig_activated(6) && enb[6]) {                            //-0f
                sig6 = !sig6;
                if (sig6)frm6 = frm80l;
                else frm6 = frm80r;
                sem_post(&sig[6]);
            }

            if (!sig_activated(8) && enb[8]) {                              //-0f
                sem_post(&sig[8]);
            }

            if (!sig_activated(9) && enb[9]) {
                frm9l = frm80l.clone();
                frm9r = frm80r.clone();
                sem_post(&sig[9]);
            }

            /// TODO: 重复项
//            if(!sig_activated(27) && enb[27]){                          //-0f
//                frm27l=frm80l->clone();
//                frm27r=frm80r->clone();
//                sem_post(&sig[27]);
//            }

            if (!sig_activated(30) && enb[30]) {
                frm30 = frm80l.clone();
                sem_post(&sig[30]);
            }

            if (!sig_activated(31) && enb[31]) {
                frm31 = frm80r.clone();
                sem_post(&sig[31]);
            }

            if (!sig_activated(32) && enb[32]) {
                frm32 = frm80l.clone();
                sem_post(&sig[32]);
            }

            if (!sig_activated(33) && enb[33]) {
                frm33 = frm80r.clone();
                sem_post(&sig[33]);
            }

            if (!sig_activated(34) && enb[34]) {
                frm34 = frm80l;
                sem_post(&sig[34]);
            }

            if (!sig_activated(35) && enb[35]) {
                frm35 = frm80r;
                sem_post(&sig[35]);
            }
//            TODO: 不存在
//            if(!sig_activated(90) && enb[90]){
//                sem_post(&sig[90]);
//            }

            if (sig_activated(100) && enb[100] && !nmarvl) {
                frm1l = frm80l.clone();
                sem_post(&sig[100]);
            }

            if (sig_activated(101) && enb[101] && !nmarvl) {
                frm1r = frm80r.clone();
                sem_post(&sig[101]);
            }

            if (!sig_activated(99) && enb[99]) {
                frm99l = frm80l;
                frm99r = frm80r;
                sem_post(&sig[99]);
                sem_wait(&sig[0]); // 等待99线程激活
            }
        }
    }

#ifndef FYAIRO_1_0_0_ARM
    if (Dv == 3) {
        while (run0) {
            ImageReader.ReadImageFromFile(frm0, filename);
            if (frm0.empty()) {
                usleep(100 * 1000);
                std::cout << "frm0 empty";
                continue;
            }
            cv::cvtColor(frm0, frm1, cv::COLOR_RGB2BGR);
            cv::resize(frm0, frm99l,
                       cv::Size(int(720 * (double(frm0.cols) / double(frm0.rows))), 720));
            sem_post(&sig[99]);
            usleep(1000 * 1000);
        }
    }

#endif
    return (nullptr);
}

#include "mou/say.cpp"

#include "eye/io.cpp"

#ifndef FYAIRO_1_0_0_ARM

void nsleep(long sleepnanoseconds) {
    std::this_thread::sleep_for(std::chrono::nanoseconds(sleepnanoseconds));
}

#endif

static std::vector<nface> ResNetFaceL, ResNetFaceR, ObjL, ObjR, GoL, GoR;
static std::vector<dface> Faces;
extern std::vector<dface> DlibFacel, DlibFacer;
#ifndef FYAIRO_1_0_0_ARM
#endif

#include "eye/facedection/facedection.h"

static ResNetDetector FaceDetectorL, FaceDetectorR, ObjDetectorL, ObjDetectorR,
        GoDetectorL, GoDetectorR;


static std::string classNames[21] = {"background",
                                     "aeroplane", "bicycle", "bird", "boat",
                                     "bottle", "bus", "car", "cat", "chair",
                                     "cow", "diningtable", "dog", "horse",
                                     "motorbike", "person", "pottedplant",
                                     "sheep", "sofa", "train", "tvmonitor"};
static std::string classNamesc[3] = {"background", "黑棋", "白棋"};
static int sz500 = 0, sz300 = 0;

void *thr_fn30(void *) {
    ThreadName[30] = "人脸检测左";//
    while (frm30.cols == 0 || !enb[30]) {
        usleep(50 * 1000);
    }
    FaceDetectorL.netinit(0);
    usleep(10 * 1000);
    std::vector<nface> Rl0;
    while (run0) {
        sem_wait(&sig[30]);
        Rl0.clear();
        FaceDetectorL.Detection(frm30, Rl0, cv::Size(sz300, sz300));
        ResNetFaceL = Rl0;
    }
    return (nullptr);
}

void *thr_fn31(void *) {
    ThreadName[31] = "人脸检测右";//
    while (frm31.cols == 0 || !enb[31]) {
        usleep(50 * 1000);
    }
    FaceDetectorR.netinit(0);
    usleep(10 * 1000);
    std::vector<nface> Rr0;
    while (run0) {
        sem_wait(&sig[31]);
        Rr0.clear();
        FaceDetectorR.Detection(frm31, Rr0, cv::Size(sz300, sz300));
        ResNetFaceR = Rr0;
    }
    return (nullptr);
}

void *thr_fn32(void *) {
    ThreadName[32] = "物体识别左";//
    while (frm32.cols == 0 || !enb[32]) {
        usleep(50 * 1000);
    }
    ObjDetectorL.netinit(1);
    usleep(10 * 1000);
    std::vector<nface> Rl0;
    while (run0) {
        sem_wait(&sig[32]);
        Rl0.clear();
        ObjDetectorL.Detection(frm32, Rl0, cv::Size(sz300, sz300));
        ObjL = Rl0;
#if 0
        printf(" >%lu< ",Rl1.size());
#endif
    }
    return (nullptr);
}

void *thr_fn33(void *) {
    ThreadName[33] = "物体识别右";//
    while (frm33.cols == 0 || !enb[33]) {
        usleep(50 * 1000);
    }
    ObjDetectorR.netinit(1);
    usleep(10 * 1000);
    std::vector<nface> Rr0;
    while (run0) {
        sem_wait(&sig[33]);
        Rr0.clear();
        ObjDetectorR.Detection(frm33, Rr0, cv::Size(sz300, sz300));
        ObjR = Rr0;
#if 0
        printf(" >%lu< ",Rr1.size());
#endif
    }
    return (nullptr);
}

static double args1 = 1.0;

void *thr_fn34(void *) {
    ThreadName[34] = "棋子识别左";//
    while (frm34.cols == 0 || !enb[34]) {
        usleep(50 * 1000);
    }
    GoDetectorL.netinit(2);
    usleep(10 * 1000);
#ifdef FYAIRO_1_0_0_ARM
    cv::Size sz(120,120);
#else
    cv::Size sz(820/*+sbsa*/, 820/*+sbsa*/);
#endif
    std::vector<nface> Rl0;
    while (run0) {
        sem_wait(&sig[34]);
        Rl0.clear();
        GoDetectorL.inScaleFactor = 1.0 / 256 * 2 * args1;
        GoDetectorL.Detection(frm34, Rl0, sz);
        GoSubPix(frm34, Rl0, cv::Size(70, 70), "L");
        GoL = Rl0;
#if 0
        printf("%lu",Rl2.size());
#endif
    }
    return (nullptr);
}

void *thr_fn35(void *) {
    ThreadName[35] = "棋子识别右";//
    while (frm35.cols == 0 || !enb[35]) {
        usleep(50 * 1000);
    }
    GoDetectorR.netinit(2);
    usleep(10 * 1000);
    std::vector<nface> Rr0;
#ifdef FYAIRO_1_0_0_ARM
    cv::Size sz(120,120);
#else
    cv::Size sz(820/*+sbsa*/, 820/*+sbsa*/);
#endif
    while (run0) {
        sem_wait(&sig[35]);
        Rr0.clear();
        GoDetectorR.Detection(frm35, Rr0, sz);
        GoSubPix(frm35, Rr0, cv::Size(70, 70), "R");
        GoR = Rr0;
#if 0
        printf("%lu ",Rr2.size());
#endif
    }
    return (nullptr);
}

static DlibFaceRecog DlibFaceRegL, DlibFaceRegR;
static cvfacereg cvfrg0;

static std::string words[1000];
static int eginit = 0;

void *thr_fn100(void *) {
    ThreadName[100] = "人脸识别左";
    std::vector<dface> facel;
    while (frm1l.cols == 0 || frm1r.cols == 0 || !enb[100]) {
        usleep(50 * 1000);
    }
    if (!eginit) {
        eginit = 1;
        eginit = cvfrg0.eigenfaceinit(Dv);                        //16s
    }
    usleep(300 * 1000);
    while (run0) {
        sem_wait(&sig[100]);
        facel.clear();

        if (!enb[102]) {
            std::vector<cv::Mat> fcs;
            std::vector<int> lb;
            std::vector<cv::Point2f> pos;
            std::vector<std::vector<cv::Point2f>> landmarks;
            std::vector<cv::Rect> frt;
            cvfrg0.eigenfacereg(frm1l, 0, fcs, lb, pos, landmarks, frt);
            for (uint i = 0; i < fcs.size(); i++) {
                dface f;
                f.id = lb[i];
                f.p = pos[i];
                for (uint j = 0; j < landmarks[i].size(); j++) {
                    f.d68[j] = landmarks[i][j];
                }
                facel.push_back(f);
            }
        } else {
            DlibFaceRegL.DlibRecognition(frm1l, facel, 0.999f);
        }

        if (facel.size()) {
            id0 = int(facel[0].id);
            mc0 = facel[0].mc;
#if 1
            if (id0 <= 10) {
                char s[128];
                sprintf(s, "\n 左眼看见%s [%f,%f] ", Faces[uint(id0)].name.c_str(),
                        facel[0].p.x, facel[0].p.y);
                printf("%s", s);
                sendcmd(s, seq[100], 3, 3);
            }
#endif
        } else {
            id0 = 0;
            mc0 = 0;
        }
        DlibFacel = facel;

#ifdef Xdbg
        printf("\n T1:  id0   id1    fl    fr   fcl   fcr ");
        printf("\n     %4.1d %4.1d %4.1d %4.1d %4.1d %4.1d ",
               id0,id1,int(facel.size()),int(facer.size()),int(fcl.size()),int(fcr.size()));
#endif

        nmarvl = 1;
    }
    return (nullptr);
}

void *thr_fn101(void *) {
    ThreadName[101] = "人脸识别右";
    std::vector<dface> facer;
    while (frm1l.cols == 0 || frm1r.cols == 0 || !enb[101]) {
        usleep(50 * 1000);
    }
    if (!eginit) {
        eginit = 1;
        eginit = cvfrg0.eigenfaceinit(Dv);                        //16s
    }
    while (run0) {
        sem_wait(&sig[101]);
        facer.clear();
        std::cout << 1;
        if (!enb[103]) {
            std::vector<cv::Mat> fcs;
            std::vector<int> lb;
            std::vector<cv::Point2f> pos;
            std::vector<std::vector<cv::Point2f>> landmarks;
            std::vector<cv::Rect> frt;
            cvfrg0.eigenfacereg(frm1r, 1, fcs, lb, pos, landmarks, frt);
            for (uint i = 0; i < fcs.size(); i++) {
                dface f;
                f.id = lb[i];
                f.p = pos[i];
                for (uint j = 0; j < landmarks[i].size(); j++) {
                    f.d68[j] = landmarks[i][j];
                }
                facer.push_back(f);
            }
        } else {
            DlibFaceRegR.DlibRecognition(frm1r, facer, 0.999f);
        }

        if (facer.size()) {
            id1 = int(facer[0].id);
            mc1 = facer[0].mc;
#if 1
            if (id1 <= 10) {
                char s[128];
                sprintf(s, "\n 右眼看见%s [%f,%f] ", Faces[uint(id1)].name.c_str(),
                        facer[0].p.x, facer[0].p.y);
                printf("%s", s);
                sendcmd(s, seq[101], 3, 3);
            }
#endif
        } else {
            id1 = 0;
            mc1 = 0;
        }
        DlibFacer = facer;
#ifdef Xdbg
        printf("\n T1:  id0   id1    fl    fr   fcl   fcr ");
        printf("\n     %4.1d %4.1d %4.1d %4.1d %4.1d %4.1d ",
               id0,id1,int(facel.size()),int(facer.size()),int(fcl.size()),int(fcr.size()));
#endif
        nmarvr = 1;
    }
    return (nullptr);
}

#ifndef FYAIRO_1_0_0_ARM

void *thr_fn4(void *) {
    ThreadName[4] = "人脸数据文件读写";
    cv::Mat Rects(5, 20, CV_32S, cv::Scalar(1));
    while (frm80.cols == 0 && frm0.cols == 0) {
        usleep(500 * 1000);
    }
    usleep(180 * 1000 * 1000);
    if (Dv < 2) { return (nullptr); }
    while (run0) {
        usleep(5 * 1000 * 1000);
        DlibFaceRegL.svfaces(Faces);
        svface(Faces, "/home/root/eye/dt/face.yml");
        usleep(55 * 1000 * 1000);
        readface(Faces, "/home/root/eye/dt/face.yml");
    }
    return (nullptr);
}

#endif

void *thr_fn5(void *) {
    ThreadName[5] = "定时器";
    while (run0) {
        std::cout << std::endl << "-------------------------------";
        usleep(60000 * 1000);
    }
    return (nullptr);
}

void *thr_fn6(void *) {
    ThreadName[6] = "二维码识别";
    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
    cv::Mat frm6g = cv::Mat(eht, ewd, CV_8UC1, 0.0);
    zbar::Image image(uint(ewd), uint(eht), "Y800", frm6g.data, ulong(ewd * eht));
    zbar::Image::SymbolIterator symbol;
    while (run0) {
        sem_wait(&sig[6]);
        if (frm6.empty())continue;
        cv::cvtColor(frm6, frm6g, cv::COLOR_BGR2GRAY);
        image.set_data(frm6g.data, ulong(ewd * eht));
        if (scanner.scan(image)) {
            symbol = image.symbol_begin();
            int n = 0;
            for (; symbol != image.symbol_end(); ++symbol) {
                std::cout << std::endl << "Num " << n << " decoded " << symbol->get_type_name()
                          << " symbol \"" << symbol->get_data() << '"';
                n++;
                std::string qrc = symbol->get_data();
                sendcmd(qrc, seq[6], 3, 3);
            }
        }
        usleep(10 * 1000);
    }
    return (nullptr);
}

static std::string wd[20];

void *thr_fn8(void *) {
    ThreadName[8] = "说话";
    //return (nullptr);                    /**************/

    words[0] = "";
    words[1] = "girl_cn_nihao";
    words[2] = "girl_cn_zaijian";
    words[3] = "girl_cn_zaijian";

    if (Dv != 1) {
        return (nullptr);
    }
    while (run0) {
        if (wdn > 0) {
            int n = wdn;
            wdn = 0;
            sayt(words[n]);
        }
        if (!sig_activated(8)) {
            usleep(10 * 1000);
            continue;
        }
        for (int i = 0; i < 20; i++) {
            wd[i] = "";
        }
        for (int i = 0; i < 20; i++) {
            if (sfc[i] < 1 || ulong(sfc[i]) > Faces.size()) {
                break;
            }
            wd[i] = Faces[uint(sfc[i])].name;
        }
        if (sfc[0] > 0 && wd[0] != "") {
            std::cout << "<" << wd[0] << "," << sfc[0] << ">";
            say(wd);
            sayt("girl_cn_nihao");
            sem_wait(&sig[8]); // 取消八号线程响应
        }
        for (int & i : sfc) {
            if (i < 1 || ulong(i) > Faces.size()) {
                break;
            }
            i = 0;
        }
        usleep(10 * 1000);
    }
    return (nullptr);
}

#include "eye/mov/mov.h"

#ifndef FYAIRO_1_0_0_ARM
static mov mdl, mdr;                                  //占用300+M内存
void *thr_fn9(void *) {
    if (Dv != 2) { return (nullptr); }
    ThreadName[9] = "移动侦测";

    while (frm9l.cols == 0 || frm9r.cols == 0) {
        usleep(50 * 1000);
    }
    while (run0) {
        sem_wait(&sig[9]);
//        mdl.movrt(frm9l);
//        mdr.movrt(frm9r);
    }
    return (nullptr);
}

#endif

#ifndef FYAIRO_1_0_0_ARM

void *TcpServerRawImageReceive(void *TcpCilientFdx) {
    typedef int *intx;

    int TcpCilientFd = *intx(TcpCilientFdx);
    byte cname[32];
    ImageReader.ssv.TcpReceiveMessage(cname, TcpCilientFd, 32);
    char cnamec[32];
    for (uint i = 0; i < 32; i++) {
        cnamec[i] = char(cname[i]);
    }
    std::string clientname = std::string(cnamec);
    int id = -1;
    std::cout << std::endl << ImageReader.name << " "
              << clientname
              << " 欢迎访问";
    for (uint i = 0; i < AllRobots.size(); i++) {
        if (AllRobots[i][0].name == clientname) {
            id = int(i);
            std::cout << std::endl << "你的ID是" << i << " 文件描述符:" << TcpCilientFd;
        }
    }
    if (id == -1)return nullptr;
    int cnt = 1;
    uint Id = uint(id);
    AllRobots[Id][0].Frm = cv::Mat(eht, ewd*2, CV_8UC3, cv::Scalar(0, 100, 0));  // 初始化

    //    创建引用
    cv::Mat frame_left = cv::Mat(AllRobots[Id][0].Frm, cv::Rect(0, 0, ewd, eht));
    cv::Mat frame_right = cv::Mat(AllRobots[Id][0].Frm, cv::Rect(ewd, 0, ewd, eht));
    while (enb[84] && cnt) {
        int mysig = SIG;
        int rcv = 0;
        //cout<<endl<<"ImageReader从"<<clientname<<"接收图像";
        rcv = ImageReader.ssv.TcpReceiveRawImage(frame_left,frame_right, TcpCilientFd);
        if (rcv == 0 && ewd * 2 == AllRobots[Id][0].Frm.cols && eht == AllRobots[Id][0].Frm.rows) {
            std::cout << "v";
            onl1 = 10;
            RsvRobId = Id;
            FrmArv = 1;
        }
        if (rcv == -1) {
            if (SIG == SIGPIPE && mysig != SIG)cnt = 0;
            continue;
        }
        if (rcv == -104) {
            cnt = 0;
        }
    }
    byte a[32];
    a[0] = 66;
    ImageReader.ssv.TcpSendMessage(a, TcpCilientFd, 32);
    std::cout << std::endl << cnamec << "视频接收断开了";
    ImageReader.ssv.TcpDisconnectSocket(TcpCilientFd);
    SIG = 0;
    return nullptr;
}

void *TcpServerImageReceive(void *TcpCilientFdx) {
    typedef int *intx;
    int TcpCilientFd = *intx(TcpCilientFdx);

    byte cname[32];
    ImageReader.ssv.TcpReceiveMessage(cname, TcpCilientFd, 32);
    char cnamec[32];
    for (uint i = 0; i < 32; i++) {
        cnamec[i] = char(cname[i]);
    }
    std::string clientname = cnamec;
    int id = -1;
    std::cout << std::endl << ImageReader.name << " "
              << clientname
              << " 欢迎访问";
    for (uint i = 0; i < AllRobots.size(); i++) {
        if (AllRobots[i][0].name == clientname) {
            id = int(i);
            std::cout << std::endl << "你的ID是" << i << " 文件描述符:" << TcpCilientFd;
        }
    }
    if (id == -1)return nullptr;
    int cnt = 1;
    while (enb[82] && cnt) {
        uint Id = uint(id);
        int mysig = SIG;
        int rcv = 0;
        //cout<<endl<<"ImageReader从"<<clientname<<"接收图像";
        rcv = ImageReader.ssv.TcpReceiveImage(AllRobots[Id][0].Frm, TcpCilientFd);
        if (rcv == 0 && ewd * 2 == AllRobots[Id][0].Frm.cols && eht == AllRobots[Id][0].Frm.rows) {
            std::cout << "v";
            onl1 = 10;
            RsvRobId = Id;
            FrmArv = 1;
        }
        if (rcv == -1) {
            if (SIG == SIGPIPE && mysig != SIG)cnt = 0;
            continue;
        }
        if (rcv == -104) {
            cnt = 0;
        }
    }
    byte a[32];
    a[0] = 66;
    ImageReader.ssv.TcpSendMessage(a, TcpCilientFd, 32);
    std::cout << std::endl << cnamec << "视频接收断开了";
    ImageReader.ssv.TcpDisconnectSocket(TcpCilientFd);
    SIG = 0;
    return nullptr;
}

void *thr_fn82(void *) {
    ThreadName[82] = "视频接收";

    int TcpServerFd = -1,
            UdpServerSocketFd = -1;

    sockaddr ClientAddr;
    while (run0) {
        sem_wait(&sig[82]);
        if (tsmd == "tcp") {
            pthread_t id;
            int TcpClientFd = -1;
            sockaddr TcpClientAddr;
            TcpClientFd = ImageReader.TcpServerGetOneClient(TcpServerFd, TcpClientAddr, TcpClientFd);
            if (TcpClientFd == -1)continue;
            pthread_create(&id, nullptr, TcpServerImageReceive, &TcpClientFd);
        }
        if (tsmd == "udp") {
            if (ImageReader.UdpServerReceiveImage(frm80, UdpServerSocketFd, ClientAddr) == 0) {
                if (ewd * 2 == frm80.cols && eht == frm80.rows) {
                    std::cout << "v";
                    onl1 = 10;
                }
            }
        }
        usleep(10 * 1000);
    }
    return (nullptr);
}

void *thr_fn84(void *) {
    ThreadName[84] = "raw视频接收";

    int TcpServerFd = -1,
            UdpServerSocketFd = -1;

    sockaddr ClientAddr{};
    while (run0) {
        sem_wait(&sig[84]);
        if (tsmd == "tcp") {
            pthread_t id; // pthread_join的时候才有用，这里用不着就直接丢了
            int TcpClientFd = -1;
            sockaddr TcpClientAddr{};
            TcpClientFd = ImageReader.TcpServerGetOneClient(TcpServerFd, TcpClientAddr, TcpClientFd);
            if (TcpClientFd == -1)continue;
            pthread_create(&id, nullptr, TcpServerRawImageReceive, &TcpClientFd);
        }
        usleep(10 * 1000);
    }
    return (nullptr);
}

void *TcpServerImageSend(void *TcpCilientFdx) {
    typedef int *intx;
    int TcpCilientFd = *intx(TcpCilientFdx);

    byte cname[32];
    ImageSender.ssv.TcpReceiveMessage(cname, TcpCilientFd, 32);
    char cnamec[32];
    for (uint i = 0; i < 32; i++) {
        cnamec[i] = char(cname[i]);
    }
    std::string clientname = cnamec;
    int id = -1;
    std::cout << std::endl << ImageSender.name << " "
              << clientname
              << " 欢迎访问";
    for (uint i = 0; i < AllRobots.size(); i++) {
        if (AllRobots[i][0].name == clientname) {
            id = int(i);
            std::cout << std::endl << "您请求接收" << clientname << "的视频 文件描述符:" << TcpCilientFd;
        }
    }
    if (id == -1)return nullptr;
    int cnt = 1;

    int msg[50];
    while (enb[81] && cnt) {
        uint Id = uint(id);
        int mysig = SIG;
        int trs = 0;
        if (debug)std::cout << std::endl << "ImageReader从" << clientname << "发送图像";

        if (AllRobots[Id][0].Frm.cols) {
            trs = ImageSender.TcpServerSendImage(AllRobots[Id][0].Frm, msg, TcpCilientFd);
            std::cout << "v";
            onl1 = 10;
        }
        if (trs == -1) {
            if (SIG == SIGPIPE && mysig != SIG)cnt = 0;
            continue;
        }
        if (trs == -104) {
            cnt = 0;
        }
        usleep(1000);
    }
    byte a[32];
    a[0] = 66;
    std::cout << std::endl << cnamec << "视频发送断开了";
    ImageSender.ssv.TcpDisconnectSocket(TcpCilientFd);
    SIG = 0;
    return nullptr;
}

void *thr_fn81(void *) {
    if (Dv != 2) { return nullptr; }
    ThreadName[81] = "服务器云视频发送";
    int TcpServerFd = -1;
    while (run0) {
        sem_wait(&sig[81]);

        pthread_t id;
        int TcpClientFd = -1;
        sockaddr TcpClientAddr;
        TcpClientFd = ImageSender.TcpServerGetOneClient(TcpServerFd, TcpClientAddr, TcpClientFd);
        if (TcpClientFd == -1)continue;
        pthread_create(&id, nullptr, TcpServerImageSend, &TcpClientFd);

        usleep(10 * 1000);
    }
    return (nullptr);
}

void *thr_fn83(void *) {
    if (Dv != 2) { return (nullptr); }
    ThreadName[83] = "PC端云视频接收";
    int msg[50];
    int TcpClientSocketFd = -1;
    sockaddr TcpServerAddr;
    robot rob = AllRobots[2][0];
    int cnt = -1;
    while (run0) {
        // 关闭连接
        if (!enb[83] && TcpClientSocketFd != -1) {
            ImageSender.ssc.TcpDisconnectSocket(TcpClientSocketFd);
            ImageSender.gnm = -1;
            cnt = -1;
        }
        sem_wait(&sig[83]);

        ImageSender.clientname = AllRobots[RsvRobId][0].name;
        int sg = SIG;
        int rcv = ImageSender.TcpClientReceiveImage(AllRobots[RsvRobId][0].Frm, msg, 0, TcpClientSocketFd,
                                                    TcpServerAddr, rob.ip, TcpServerImageSendPort, cnt);
        if (rcv == 0) {
            if (ewd * 2 == AllRobots[RsvRobId][0].Frm.cols &&
                eht == AllRobots[RsvRobId][0].Frm.rows) {
                std::cout << "v";
                onl1 = 10;
                FrmArv = 1;
                /*while(FrmArv==1){
                     usleep(1000);
                 }*/
            }
        }
        if (rcv == -1 || rcv == -104) {
            if ((SIG == SIGPIPE && sg != SIG) || rcv == -104) {
                ImageSender.ssc.TcpDisconnectSocket(TcpClientSocketFd);
                ImageSender.gnm = -1;
                cnt = -1;
            }
        }

        usleep(10 * 1000);
    }
    return (nullptr);
}

void *TcpServerSendOder(void *TcpCilientFdx) {
    typedef int *intx;
    int TcpCilientFd = *intx(TcpCilientFdx);

    byte cname[32];
    ServerTcpOderSender.ssv.TcpReceiveMessage(cname, TcpCilientFd, 32);
    char cnamec[32];
    for (uint i = 0; i < 32; i++) {
        cnamec[i] = char(cname[i]);
    }
    std::string clientname = cnamec;
    int id = -1;
    std::cout << std::endl << ServerTcpOderSender.name << " "
              << clientname
              << " 欢迎访问!!! ";
    for (uint i = 0; i < AllRobots.size(); i++) {
        if (AllRobots[i][0].name == clientname) {
            id = int(i);
            std::cout << std::endl << "你的ID是" << i << " 文件描述符:" << TcpCilientFd;
        }
    }
    if (id == -1)return nullptr;
    int cnt = 1;
    byte r[4096];
    while (enb[86] && cnt) {
        uint Id = uint(id);
        if (!AllRobots[Id][0].oders.size()) {
            usleep(1000);
        }
        int mysig = SIG;
        int snd = 0;

        for (uint i = 0; i < AllRobots[Id][0].oders.size(); i++) {
            std::vector<byte> oder = AllRobots[Id][0].oders[i];
            ulong OderSz = oder.size();
            std::cout << std::endl << "ServerTcpOderSender发送到" << clientname << ":";
            for (uint j = 0; j < OderSz; j++) {
                r[j] = oder[j];
                printf(" %2.2X", r[j]);
            }
            std::cout << std::endl;
            snd = ServerTcpOderSender.ssv.TcpSendMessage(r, TcpCilientFd, OderSz);
        }
        if (snd == -1) {
            if (SIG == SIGPIPE && mysig != SIG)cnt = 0;
            continue;
        }
        if (snd == -104) {
            cnt = 0;
        }
        AllRobots[Id][0].oders.clear();
    }
    byte a[32];
    a[0] = 66;
    ServerTcpOderSender.ssv.TcpSendMessage(a, TcpCilientFd, 32);
    std::cout << std::endl << cnamec << "被断开了";
    ServerTcpOderSender.ssv.TcpDisconnectSocket(TcpCilientFd);
    SIG = 0;
    return nullptr;
}

void *thr_fn86(void *) {
    if (Dv != 2) { return (nullptr); }
    ThreadName[86] = "服务器端Tcp发送命令";
    int TcpServerFd = -1;
    while (run0) {
        if (!enb[86]) {
            usleep(20 * 1000);
            continue;
        }
        pthread_t id;
        int TcpClientFd = -1;
        sockaddr TcpClientAddr;
        TcpClientFd = ServerTcpOderSender.TcpServerGetOneClient(TcpServerFd, TcpClientAddr, TcpClientFd);
        if (TcpClientFd == -1)continue;
        pthread_create(&id, nullptr, TcpServerSendOder, &TcpClientFd);
        usleep(10 * 1000);
    }
    return (nullptr);
}

void *thr_fn87(void *) {
    ThreadName[87] = "客户端Tcp获取命令";
    char Rcoder[4096];
    int TcpClientSocketFd = -1,
            UdpClientSocketFd = -1;
    sockaddr TcpServerAddr;
    sockaddr UdpServerAddr;
    robot rob = AllRobots[2][0];
    int cnt = -1;
    while (run0) {
        if (!enb[87]) {
            if (!enb[87] && TcpClientSocketFd != -1) {
                ClientTcpOderReceiver.ssc.TcpDisconnectSocket(TcpClientSocketFd);
                TcpClientSocketFd = -1;
                ClientTcpOderReceiver.gnm = -1;
                cnt = -1;
            }
            usleep(20 * 1000);
            continue;
        }
        ClientTcpOderReceiver.clientname = MyName;
        int len = ClientTcpOderReceiver.TcpClientReceiveData(
                Rcoder, TcpClientSocketFd, TcpServerAddr, rob.ip, ServerTcpOderSendPort, cnt);
        if (Rcoder[0] == 66 || len == -104) {
            Rcoder[0] = 0;
            if (Rcoder[0] == 66)std::cout << std::endl << "收到信号66退出";
            if (len == -104)std::cout << std::endl << "连接意外中断退出";
            ClientTcpOderReceiver.ssc.TcpDisconnectSocket(TcpClientSocketFd);
            TcpClientSocketFd = -1;
            ClientTcpOderReceiver.gnm = -1;
            cnt = -1;
        }
        if (len < 1)continue;
        byte Rc1[4096];
        std::cout << std::endl << "ClientTcpOderReceiver 收到" << len << "字节 ";
        for (int i = 0; i < len; i++) {
            Rc1[i] = byte(Rcoder[i]);
            printf(" %2.2X", Rc1[i]);
        }
        OderSender.UdpClientSendData(Rc1, size_t(len), LcIP, UdpCvOderPort,
                                     UdpClientSocketFd, UdpServerAddr);
        usleep(1 * 1000);
    }
    return (nullptr);
}

#endif

#include "ear/ear.h"

cv::Mat datadftMatSmTableL, datadftMatSmTableR,
        energyFilter, hanmingFilter;
volatile int viewMat = 0;

cv::Mat MFCC(cv::Mat datadftMat32fc1E) {
    cv::Mat mfcc = datadftMat32fc1E.clone();
    for (int i = 0; i < datadftMat32fc1E.cols - 1; ++i) {
        mfcc.at<float>(2595.0f * log(1.0f + i / 700.f), 0) = datadftMat32fc1E.at<float>(i, 0);
    }
    return mfcc;
}

cv::Mat getFeqMap(cv::Mat earbuff0) {
#if 1
    cv::Mat datadftMat32fc1;
    if (earbuff0.cols % 2)cv::resize(earbuff0, earbuff0, cv::Size(earbuff0.cols, earbuff0.rows + 1));
    cv::dct(earbuff0, datadftMat32fc1);
    //datadftMat32fc1 = earbuff0.clone();
    cv::Mat datadftMat;
    cv::Mat EnergyFilter = energyFilter.mul(hanmingFilter);
    if (viewMat) {
        std::cout << hanmingFilter;
        viewMat = 0;
    }
    cv::Mat datadftMat32fc1E = MFCC(EnergyFilter.mul(datadftMat32fc1));
    datadftMat32fc1E.convertTo(datadftMat, CV_8UC1);
#else
    cv::Mat datadftMat;
    buff.convertTo(datadftMat, CV_8UC1);
#endif
    cv::Mat datadftMatSmFiltered;
    int dropOut = 220;
    datadftMatSmFiltered =
            datadftMat - dropOut;                                                                       //丢弃不重要的信息
    datadftMat = datadftMatSmFiltered * (256 / (256 - dropOut));
    if (earbuff0.cols % 2)cv::resize(datadftMat, datadftMat, cv::Size(datadftMat.cols, datadftMat.rows - 1));
    return datadftMat;
}

void *thr_fn88(void *) {
    ThreadName[88] = "听觉";
    ear Ear;
    cv::Mat earbuff;
#ifdef FYAIRO_1_0_0_ARM
    Ear.init("hw:0");
#else
//    Ear.init();
    Ear.init("hw:0");
#endif

    int total;
    int imageWidth = Ear.GrabSamples / Ear.Channel;
    int imageHeight = 500;
    datadftMatSmTableL = cv::Mat(imageHeight, imageWidth, CV_8UC1);
    datadftMatSmTableR = cv::Mat(imageHeight, imageWidth, CV_8UC1);
    if (imageWidth % 2) {
        energyFilter = cv::Mat(imageWidth + 1, 1, CV_32F, 0.0f);
    } else {
        energyFilter = cv::Mat(imageWidth, 1, CV_32F, 0.0f);
    }
    hanmingFilter = energyFilter.clone();
    float alpha = 0.46;
    for (int i = 0; i < energyFilter.rows; ++i) {
        energyFilter.at<float>(0, i) =
                0.25f + float(i) / 800.0f;                                                             //平均能量分布
        hanmingFilter.at<float>(0, i) =
                (1.0f - alpha) - alpha * cos(2.0f * M_PI * i / (energyFilter.rows - 1.0f));      //加窗，减少泄漏
    }
    return nullptr;
    while (run0) {
        earbuff = Ear.readbuf().t();
        total++;
        if (total > 33) {
            std::cout << std::endl << "buff Size: " << earbuff.cols << " , " << earbuff.rows << " , C "
                      << earbuff.channels()
                      << " total: " << total << " time:" << (clock() - lastProcessTime[88]) / 2000 << " ms"
                      << std::endl;
            lastProcessTime[88] = clock();
            total = 0;
        }

        cv::Mat dataMat32fc1;
        earbuff.convertTo(dataMat32fc1, CV_32F);

        cv::Mat col0 = dataMat32fc1.col(0);
        cv::Mat datadftMat = getFeqMap(col0);
        cv::Mat datadftMatSmTablep(imageWidth, 0, CV_8UC1);

        cv::flip(datadftMat, datadftMat, 0);


        for (int i = 1; i < imageHeight; ++i) {
            cv::Mat rowi = datadftMatSmTableL.row(i);
            datadftMatSmTablep.push_back(rowi);
        }
        datadftMatSmTablep.push_back(datadftMat.t());
        datadftMatSmTableL = datadftMatSmTablep.clone();

        cv::Mat col1 = dataMat32fc1.col(1);
        datadftMat = getFeqMap(col1);
        datadftMatSmTablep = cv::Mat(imageWidth, 0, CV_8UC1);

        cv::flip(datadftMat, datadftMat, 0);

        for (int i = 1; i < imageHeight; ++i) {
            cv::Mat rowi = datadftMatSmTableR.row(i);
            datadftMatSmTablep.push_back(rowi);
        }
        datadftMatSmTablep.push_back(datadftMat.t());
        datadftMatSmTableR = datadftMatSmTablep.clone();

        if (total == 0)
            std::cout << __FUNCTION__
                      << "cols:" << datadftMat.cols << "rows:" << datadftMat.rows
                      << "tableCols:" << datadftMatSmTablep.cols << "tableRows:" << datadftMatSmTablep.rows
                      << std::endl;
    }
    return (nullptr);
}

static cv::Mat blk0, blk1;
static cv::Point pL, pR, rL, rR, oL, oR;
static pthread_t ntid[255];

static pthread_attr_t attr;

void fystart() {
    std::string R,
            C,
            codec;
    int jpgb;
#define fyloadconfigfromyml

    // /**手动添加被识别人*/filename="/home/root/eye/img/WuZong.jpg";
    //dlibface.p2fid=2; dlibface.p2f=7;  dv=3; //p2fid:1=add 2=change p2f:num
    // /**输出更多调试信息*/debug=-1;
    //rdfm.debug=-1;
    //readfm.socketMatsvr.debug=-1;
    //readfm.socketMat.debug=-1;
    //sendodr.debug=1;
    //sendodrrt.debug=1;
    //sendodr.socketMat.debug=1;
    //sendodrrt.socketMatsvr.debug=1;
    //readodr.debug=1;
    //readodr.socketMatsvr.debug=1;
    //readodr.socketMat.debug=1;
#if 1                                     //无下位机运行
#define FYNOROBT
    ImageReader.frm0_ = cv::imread
            ("/home/root/eye/img/frm11/013.png");
#endif

#ifndef fyloadconfigfromyml
    Dv=dv;
    Dc=0;
    tsmd="tcp";            //tsmd="udp";
    jpgb=50;               //压缩质量高数据量大

    ewd=320;eht=240;
    ewd=352;eht=288;
    ewd=640;eht=480;

/*ewd=800;eht=600;
ewd=1280;eht=720;*/

// tip="192.168.1.10";
     MbIp="192.168.8.110";
 if(Dc==1){
     CloudIp="106.75.217.253";
     PcIp="192.168.8.110";
     RobotIp="192.168.8.11";
     TcpCloudPort=30006;
 }

 if(Dc==0){
     PcIp= "192.168.8.110" /*"192.168.8.110""116.232.9.122""101.80.10.73""192.168.0.74"*/;
     RobotIp= "192.168.8.11" /*"192.168.8.111""192.168.0.11"*/;
 }

 TcpServerImageReceivePort =15555;UdpServerImageReceivePort   =16666;
 UdpCvOderPort=10687;
 UdpFyOderPort=10688;             UdpPcOderPort=10689;

 cmrl=1;
 cmrr=0;

#ifndef FYAIRO_1_0_0_ARM
 cmrr=2;
#endif

 R="R*FY2019RA001";
 C="C*FY2019CA001*FY2019RA001";
 codec="YUYV";

 cv::FileStorage fsw("/home/root/eye_config.yml",cv::FileStorage::WRITE);
 fsw<<"MyName"<<MyName
    <<"Dv"   <<Dv
    <<"Dc"   <<Dc
    <<"tsmd" <<tsmd
    <<"jpgb" <<jpgb

    <<"cmrl" <<cmrl
    <<"cmrr" <<cmrr
    <<"ewd"  <<ewd
    <<"eht"  <<eht
    <<"codec"<<codec

    <<"PcIp"   <<PcIp
    <<"RobotIp"<<RobotIp
    <<"CloudIp"<<CloudIp
    <<"MbIp"   <<MbIp

    <<"TcpCloudPort"             <<TcpCloudPort
    <<"TcpServerImageReceivePort"<<TcpServerImageReceivePort
    <<"TcpServerImageSendPort"   <<TcpServerImageSendPort
    <<"UdpServerImageReceivePort"<<UdpServerImageReceivePort

    <<"UdpCvOderPort"<<UdpCvOderPort
    <<"UdpFyOderPort"<<UdpFyOderPort

    <<"ServerUdpReportReceiverPort"      <<ServerUdpReportReceiverPort
    <<"ServerTcpReporterPort"            <<ServerTcpReporterPort
    <<"ServerTcpOderSenderPort"          <<ServerTcpOderSenderPort

     ;
 fsw.release();
#endif
#ifdef fyloadconfigfromyml
//#else
    cv::FileStorage fs("/home/root/eye_config.yml", cv::FileStorage::READ);
    fs["MyName"] >> MyName;
    fs["Dv"] >> Dv;
    fs["Dc"] >> Dc;
    fs["tsmd"] >> tsmd;
    fs["jpgb"] >> jpgb;

    fs["cmrl"] >> cmrl;
    fs["cmrr"] >> cmrr;
    fs["ewd"] >> ewd;
    fs["eht"] >> eht;
    fs["codec"] >> codec;

    fs["PcIp"] >> PcIp;
    fs["RobotIp"] >> RobotIp;
    fs["CloudIp"] >> CloudIp;
    fs["MbIp"] >> MbIp;

    fs["TcpCloudPort"] >> TcpCloudPort;
    fs["TcpServerImageReceivePort"] >> TcpServerImageReceivePort;
    fs["TcpServerImageSendPort"] >> TcpServerImageSendPort;
    fs["UdpServerImageReceivePort"] >> UdpServerImageReceivePort;

    fs["UdpCvOderPort"] >> UdpCvOderPort;
    fs["UdpFyOderPort"] >> UdpFyOderPort;

    fs["ServerTcpReportPort"] >> ServerTcpReportPort;
    fs["ServerTcpOderSendPort"] >> ServerTcpOderSendPort;

    fs.release();
#endif
    LcIP = "127.0.0.1";   /**IP 不能为空 否则控制局域网所有设备*/

    static std::vector<robot> PcRobots,
            Robxiaofei,
            ServerRobots,
            RobWxiaofei,
            RobRobotsLc,
            Roblingxi,
            RobWlingxi,
            WPcRobots,
            Master;

    PcRobots.push_back(robot(UdpCvOderPort, 0, PcIp, "电脑端"));
    Robxiaofei.push_back(robot(UdpCvOderPort, 1, RobotIp, "小飞"));
    ServerRobots.push_back(robot(UdpCvOderPort, 2, CloudIp, "服务器端"));
    RobWxiaofei.push_back(robot(UdpCvOderPort, 3, CloudIp, "@小飞"));
    RobRobotsLc.push_back(robot(UdpCvOderPort, 4, LcIP, "自己"));
    Roblingxi.push_back(robot(UdpCvOderPort, 5, "192.168.8.108", "灵犀"));
    RobWlingxi.push_back(robot(UdpCvOderPort, 6, CloudIp, "@灵犀"));
    WPcRobots.push_back(robot(UdpCvOderPort, 7, CloudIp, "@电脑端"));
    Master.push_back(robot(UdpFyOderPort, 8, LcIP, "主控端"));

    AllRobots.push_back(PcRobots);                //0
    AllRobots.push_back(Robxiaofei);              //1
    AllRobots.push_back(ServerRobots);            //2
    AllRobots.push_back(RobWxiaofei);             //3
    AllRobots.push_back(RobRobotsLc);             //4
    AllRobots.push_back(Roblingxi);               //5
    AllRobots.push_back(RobWlingxi);              //6
    AllRobots.push_back(WPcRobots);               //7
    AllRobots.push_back(Master);                  //8

    RsvRobId = 4;

    std::cout << "Mode :" << Dv << std::endl;
    ImageReader.Init("ImageReader",
                     ewd, eht, PcIp, PcIp, PcIp, MbIp,
                     TcpServerImageReceivePort,
                     UdpServerImageReceivePort,
                     TcpServerImageReceivePort,
                     UdpServerImageReceivePort, cmrl, cmrr,
            /*'M','J','P','G' */              /*MJPG用于2BUS高清图像采集 YUYV快*/
                     codec[0], codec[1], codec[2], codec[3]);
    std::cout << "Codec :" << codec << std::endl;
    /*yuyv@352*288@uImage_2USB_Host OK*/
    ImageSender.Init("ImageSender",
                     ewd, eht, CloudIp, "", CloudIp, "",
                     TcpServerImageSendPort, 8009, TcpServerImageSendPort);

    readface(Faces, "/home/root/eye/dt/face.yml");
    ImageReader.ssc.jpgb = jpgb;
    ImageSender.ssv.jpgb = jpgb;

    frm13 = cv::Mat(1, 1, CV_8UC3, cv::Scalar(0));
    frm13b = cv::Mat(1, 1, CV_8UC3, cv::Scalar(0));
    frm0 = cv::Mat(eht, ewd * 2, CV_8UC3, cv::Scalar(0, 100, 0));
    frm80l0 = cv::Mat(eht, ewd, CV_8UC3, cv::Scalar(0, 100, 0));
    frm80r0 = cv::Mat(eht, ewd, CV_8UC3, cv::Scalar(0, 100, 0));
    OderSender.name = "OderSender";
    OderReceiver.Init("OderReceiver",
                      ewd, eht, "", "", "", "", 0, UdpCvOderPort);
    ClientTcpOderReceiver.Init("ClientTcpOderReceiver",
                               ewd, eht, "", "", CloudIp, "", 0, 0, ServerTcpOderSendPort, 0);
    ServerTcpOderSender.Init("ServerTcpOderSender",
                             ewd, eht, MyIp, "", "", "", ServerTcpOderSendPort, 0, 0, 0);
    MyMaster = AllRobots[8];
    if (Dv == 1) {
        MyRobots = AllRobots[4];
        MyRobotHosts = AllRobots[0]; //if(Dc==1)CloudOderSender.TcpClientSendText(R);
        if (cloud) {
            MyRobots = AllRobots[4];
            MyRobotHosts = AllRobots[2];
            ImageReader.TcpClientTargetIP = CloudIp;
            ImageReader.TcpClientTargetPort = TcpServerImageReceivePort;
        }
    }
    if (Dv == 2) {
        if (serv)MyRobots = AllRobots[2];
        if (desk)MyRobots = AllRobots[0];//if(Dc==1)CloudOderSender.TcpClientSendText(C);
#ifndef FYAIRO_1_0_0_ARM
        frm99l = cv::Mat(eht, ewd, CV_8UC3, cv::Scalar(0, 100, 0));
        frm99r = cv::Mat(eht, ewd, CV_8UC3, cv::Scalar(0, 100, 0));
        frm80 = cv::Mat(eht, ewd * 2, CV_8UC3, cv::Scalar(0, 100, 0));

        mapx[0] = cv::Mat(frm99l.size(), CV_32FC1);
        mapx[1] = cv::Mat(frm99r.size(), CV_32FC1);
        mapy[0] = cv::Mat(frm99l.size(), CV_32FC1);
        mapy[1] = cv::Mat(frm99r.size(), CV_32FC1);

        DlibFaceRegL.DlibRgin(Faces);
        DlibFaceRegR.DlibRgin(Faces);

        if (Dv == 0) {
            sz500 = 50;
            sz300 = 25;
        }
        if (Dv == 1) {
            sz500 = 100;
            sz300 = 50;
        }
        if (Dv == 2) {
            sz500 = 500;
            sz300 = 300;
        }
#endif
    }

    ImageReader.ewd = ewd;
    ImageReader.eht = eht;

    struct sched_param param;
    int newprio = 50;

    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    pthread_attr_getschedparam(&attr, &param);
    param.sched_priority = newprio;
    pthread_attr_setschedparam(&attr, &param);

    pthread_attr_destroy(&attr);

    pthread_create(&ntid[25], &attr, thr_fn25, nullptr);
    pthread_create(&ntid[0], &attr, thr_fn0, nullptr);
    pthread_create(&ntid[2], &attr, thr_fn2, nullptr);
    pthread_create(&ntid[5], &attr, thr_fn5, nullptr);
    pthread_create(&ntid[6], &attr, thr_fn6, nullptr);
    pthread_create(&ntid[8], &attr, thr_fn8, nullptr);

    pthread_create(&ntid[100], &attr, thr_fn100, nullptr);
    pthread_create(&ntid[101], &attr, thr_fn101, nullptr);

    pthread_create(&ntid[34], &attr, thr_fn34, nullptr);
    pthread_create(&ntid[35], &attr, thr_fn35, nullptr);

#ifndef FYAIRO_1_0_0_ARM
    pthread_create(&ntid[30], &attr, thr_fn30, nullptr);
    pthread_create(&ntid[31], &attr, thr_fn31, nullptr);

    pthread_create(&ntid[32], &attr, thr_fn32, nullptr);
    pthread_create(&ntid[33], &attr, thr_fn33, nullptr);


    pthread_create(&ntid[4], &attr, thr_fn4, nullptr);
    pthread_create(&ntid[9], &attr, thr_fn9, nullptr);
    pthread_create(&ntid[82], &attr, thr_fn82, nullptr);
    pthread_create(&ntid[81], &attr, thr_fn81, nullptr);
    pthread_create(&ntid[83], &attr, thr_fn83, nullptr);
    pthread_create(&ntid[84], &attr, thr_fn84, nullptr);
    pthread_create(&ntid[86], &attr, thr_fn86, nullptr);
    pthread_create(&ntid[87], &attr, thr_fn87, nullptr);
#endif

    pthread_create(&ntid[88], &attr, thr_fn88, nullptr);
}

