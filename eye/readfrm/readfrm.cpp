#include "readfrm.h"

NetCommunicationModule::NetCommunicationModule() = default;

NetCommunicationModule::~NetCommunicationModule() = default;

void NetCommunicationModule::Init(std::string Name,
                                  int ewd_, int eht_,
                                  std::string TcpServerIP_, std::string UdpServerIP_,
                                  std::string TcpClientTargetIP_, std::string UdpClientTargetIP_,
                                  u_short TcpServerPort_, u_short UdpServerPort_,
                                  u_short TcpClientTargetPort_, u_short UdpClientTargetPort_,
                                  int CamId_, int CamIdr_,
                                  char cdc0, char cdc1, char cdc2, char cdc3) {
    TcpServerIP = TcpServerIP_;
    UdpServerIP = UdpServerIP_;
    TcpClientTargetIP = TcpClientTargetIP_;
    UdpClientTargetIP = UdpClientTargetIP_;

    TcpServerPort = TcpServerPort_;
    UdpServerPort = UdpServerPort_;
    TcpClientTargetPort = TcpClientTargetPort_;
    UdpClientTargetPort = UdpClientTargetPort_;

    CamIdl = CamId_;
    CamIdr = CamIdr_;
    ewd = ewd_;
    eht = eht_;
    name = Name;

    iopl = NONE;
    iopr = NONE;
    crt = -1;
    cnt = -1;
    trs = -1;

    rddl = 0;
    UdpServerFd = -1;
    sls = -1;
    scn = -1;
    rcv = -1;
    gnm = -1;

    ssv.ewd = ewd;
    ssv.eht = eht;
    ssv.TcpServerIp = TcpServerIP;
    ssv.UdpServerIp = UdpServerIP;
    ssv.name = name;
    ssv.SetDatak();

    ssc.ewd = ewd;
    ssc.eht = eht;
    ssc.name = name;
    ssc.SetDatak();

    if (CamId_ != CamIdr_) {
        frm0_ = cv::Mat(eht, ewd * 2, CV_8UC3, 0.0);
        std::cout << frm0_.cols << "," << frm0_.rows << " ";
        frml = cv::Mat(frm0_, cv::Rect(0, 0, ewd, eht));
        frmr = cv::Mat(frm0_, cv::Rect(ewd, 0, ewd, eht));
        codec = writer.fourcc(cdc0, cdc1, cdc2, cdc3);
    }
}

int NetCommunicationModule::ReadImageFromFile(cv::Mat &frm0, cv::String filename) {
    frm0 = cv::imread(filename);
    return 0;
}

int NetCommunicationModule::CaptureImage(int chmd) {
    if (chmd == 0) {
        capl.release();
        capr.release();
        iopl = NONE;
        iopr = NONE;
    }
    return 0;
}

int NetCommunicationModule::ClientEncodeImage() {
    return cv::imencode(".jpg", frm0_, ssc.dataj, {1, ssc.jpgb});
}

int NetCommunicationModule::ServerEncodeImage() {
    return cv::imencode(".jpg", frm0_, ssv.datak, {1, ssv.jpgb});
}

int NetCommunicationModule::CaptureImage(cv::Mat &frm0, int OnLine) {
    // 检查冲突
    if (iopl == RAW || iopr == RAW) {
        std::cout << std::endl << "冲突：快速模式已开启";
        return -1;
    }
    // 启动左摄像头
    if (iopl == NONE) {
        if (ler > 100) {
            frm0_.setTo(10);
            iopl = capl.open(CamIdl, cv::CAP_V4L2) ? NORMAL : NONE; // 开启则为normal
            ler = 0;
        }
        if (iopl == NORMAL) {
            capl.set(cv::CAP_PROP_FRAME_WIDTH, ewd);
            capl.set(cv::CAP_PROP_FRAME_HEIGHT, eht);
            capl.set(cv::CAP_PROP_FOURCC, codec);
            capl.set(cv::CAP_PROP_CONVERT_RGB, 1);
            capl.set(cv::CAP_PROP_BRIGHTNESS, 15);
            capl.set(cv::CAP_PROP_CONTRAST, 15);
            cv::Mat frm;
            rddl = capl.read(frm);
            std::cout << std::endl << name << "摄像头连接成功!"
                      << iopl << "[" << frm.cols << "," << frm.rows << "]" << rtl;
            if (frm.cols != ewd || frm.rows != eht) {
                rtl = "但不标准!!!";
                if (frm.cols == 0 || frm.rows == 0) iopl = NONE;
            } else {
                rtl = "";
            }
            ler = 0;
            sleep(1);                           /**等电流峰值过后启动另外一颗*/
        } else {
            ler++;
            cv::putText(frm0_, name + " No Camera " + str(ler), cv::Point(rnd(640), rnd(480)), 1,
                        rnd(5),
                        cv::Scalar(rnd(255), rnd(255), rnd(255)), rnd(5));
            std::cout << ".";
            usleep(10 * 1000);
        }
    }
    // 启动右摄像头
    if (iopr == NONE) {
        if (rer > 100) {
            frm0_.setTo(10);
            iopr = capr.open(CamIdr, cv::CAP_V4L2) ? NORMAL : NONE;
            rer = 0;
        }
        if (iopr == NORMAL) {
            capr.set(cv::CAP_PROP_FRAME_WIDTH, ewd);
            capr.set(cv::CAP_PROP_FRAME_HEIGHT, eht);
            capr.set(cv::CAP_PROP_FOURCC, codec);
            capr.set(cv::CAP_PROP_CONVERT_RGB, 1);
            capr.set(cv::CAP_PROP_BRIGHTNESS, 15);
            capr.set(cv::CAP_PROP_CONTRAST, 15);
            cv::Mat frm;
            rddr = capr.read(frm);
            std::cout << std::endl << name << "摄像头连接成功!"
                      << iopl << "[" << frm.cols << "," << frm.rows << "]" << rtr;
            if (frm.cols != ewd || frm.rows != eht) {
                rtr = "但不标准!!!";
                if (frm.cols == 0 || frm.rows == 0)iopr = NONE;
            } else {
                rtr = "";
            }
            rer = 0;
            sleep(1);                           /**等电流峰值过后启动另外一颗*/
        } else {
            rer++;
            cv::putText(frm0_, "No Camera " + str(rer), cv::Point(rnd(640) + 640, rnd(480)), 1,
                        rnd(5),
                        cv::Scalar(rnd(255), rnd(255), rnd(255)), rnd(5));
            std::cout << ".";
            usleep(10 * 1000);
        }
    }
#if 0
    grb=grb==4?0:grb+1;
#endif
    // 从摄像头读取一帧
    if (iopl == NORMAL) {
        /*if(grb==4)capl.grab();  */                 //丢帧
        if (rtl.empty()) {
            rddl = capl.read(frml);
        } else {
            rddl = capl.read(frml1);
            cv::resize(frml1, frml, cv::Size(ewd, eht));
        }                                            //rdd 成功=1
        iopl = rddl ? NORMAL : NONE;
    }
    if (iopr == NORMAL) {
        /*if(grb==4)capr.grab(); */
        if (rtl.empty()) {
            rddr = capr.read(frmr);
        } else {
            rddr = capr.read(frmr1);
            cv::resize(frmr1, frmr, cv::Size(ewd, eht));
        }
        iopr = rddr ? NORMAL : NONE;
    }
#if 0
    cv::imshow("frml",frml);
    cv::imshow("frmr",frmr);
#endif
    if (!OnLine) {
        frm0 = frm0_.clone();
        if (rddl || rddr)std::cout << "V";
        return 0;
    }
    if (rddl || rddr) {
#if 0
        printf(" frml<%d,%d,%d>",frml.cols,frml.rows,frml.channels());
#endif
        ClientEncodeImage();
        std::cout << "v";
        return 0;
    } else {
        ClientEncodeImage();
        std::cout << ". ";
        return -1;
    }
}

int NetCommunicationModule::CaptureRawImage() {
    if (iopl == NORMAL || iopr == NORMAL) {
        std::cout << std::endl << "冲突：标准模式已开启";
        return -1;
    }
    // 启动左摄像头
    if (iopl == NONE)
    {
        iopl = raw_capl.open(CamIdl)?RAW:NONE;
        sleep(1);                           /**等电流峰值过后启动另外一颗*/
    }
    // 启动右摄像头
    if (iopr == NONE)
    {
        iopr = raw_capr.open(CamIdr)?RAW:NONE;
    }
    // 获取图像
    if (iopl == RAW)
    {
        raw_packl = raw_capl.read_frame();
    }
    if (iopr == RAW)
    {
        raw_packr = raw_capr.read_frame();
    }
    return 0;
}

int NetCommunicationModule::GrabImage(cv::Mat &frm13) {
    capl.open(CamIdl);
    capl.set(3, 1280);
    capl.set(4, 720);
    codec = writer.fourcc('M', 'J', 'P', 'G');
    capl.set(6, codec);
    capl.read(frm13);
    capl.release();
    return 0;
}

int NetCommunicationModule::TcpClientConnect(int &TcpClientSocketFd, sockaddr &TcpServerAddr,
                                             std::string TcpClientTargetIP, ushort TcpClientTargetPort, int &cnt) {
    if (TcpClientSocketFd == -1) {
        ssc.TcpCreateSocket(TcpClientSocketFd);
        std::cout << std::endl << name << " 正在连接TCP地址:" << TcpClientTargetIP << ":" << TcpClientTargetPort << " ";
    }
    if (TcpClientSocketFd == -1)return -1;
    if (cnt == -1)cnt = ssc.TcpConnectSocket(TcpClientTargetIP, TcpClientTargetPort, TcpClientSocketFd, TcpServerAddr);
    if (cnt == -1)return -1;
    return cnt;
}

int NetCommunicationModule::TcpClientSendImage(int msg[], int tp, int &TcpClientSocketFd,
                                               sockaddr &TcpServerAddr, std::string TcpClientTargetIP,
                                               ushort TcpClientTargetPort, int &cnt) {
    if (TcpClientConnect(TcpClientSocketFd, TcpServerAddr, TcpClientTargetIP, TcpClientTargetPort, cnt) == -1)return -1;
    if (gnm == -1) {
        ssc.TcpSendText(clientname, TcpClientSocketFd);
        gnm = 0;
    }
    if (tp == 0)trs = ssc.TcpSendData(TcpClientSocketFd);
    if (tp == 1)trs = ssc.TcpSendData(msg, "A", TcpClientSocketFd);
    return trs;
}

int NetCommunicationModule::TcpClientReceiveImage(cv::Mat &frm0, int msg[], int tp,
                                                  int &TcpClientSocketFd, sockaddr &TcpServerAddr,
                                                  std::string TcpClientTargetIP,
                                                  ushort TcpClientTargetPort, int &cnt) {
    if (TcpClientConnect(TcpClientSocketFd, TcpServerAddr, TcpClientTargetIP, TcpClientTargetPort, cnt) == -1)return -1;
    if (gnm == -1) {
        ssc.TcpSendText(clientname, TcpClientSocketFd);
        gnm = 0;
    }
    if (tp == 0)rcv = ssc.TcpReceiveJpegData(frm0, TcpClientSocketFd);
    if (tp == 1)rcv = ssc.TcpReceiveImage(msg, TcpClientSocketFd);
    return rcv;
}

int NetCommunicationModule::TcpClientReceiveData(char oder[], int &TcpClientSocketFd,
                                                 sockaddr &TcpServerAddr, std::string TcpClientTargetIP,
                                                 ushort TcpClientTargetPort, int &cnt) {
    if (TcpClientConnect(TcpClientSocketFd, TcpServerAddr, TcpClientTargetIP, TcpClientTargetPort, cnt) == -1)return -1;
    if (gnm == -1) {
        ssc.TcpSendText(clientname, TcpClientSocketFd);
        gnm = 0;
    }
    rcv = ssc.TcpReceiveData(oder, TcpClientSocketFd);
    return rcv;
}

int NetCommunicationModule::TcpClientSendText(std::string text, int &TcpClientSocketFd,
                                              sockaddr &TcpServerAddr, std::string TcpClientTargetIP,
                                              ushort TcpClientTargetPort, int &cnt) {
    if (TcpClientConnect(TcpClientSocketFd, TcpServerAddr, TcpClientTargetIP, TcpClientTargetPort, cnt) == -1)return -1;
    if (TcpClientSocketFd == -1)return -1;
    return ssc.TcpSendText(text, TcpClientSocketFd);
}

int NetCommunicationModule::TcpClientSendPacket(int msg[], int tp, int &TcpClientSocketFd, sockaddr &TcpServerAddr,
                                                std::string tcpClientTargetIp, ushort tcpClientTargetPort, int &i) {
    if (TcpClientConnect(TcpClientSocketFd, TcpServerAddr, std::move(tcpClientTargetIp), tcpClientTargetPort, i) == -1)return -1;
    if (TcpClientSocketFd == -1)return -1;
    if (gnm == -1) {
        ssc.TcpSendText(clientname, TcpClientSocketFd);
        gnm = 0;
    }
    if (tp == 1)trs = ssc.TcpSendData(msg, "A", TcpClientSocketFd);
    else if (tp == 0)
    {
        trs = ssc.TcpSendPacket(raw_packl->data,raw_packl->size,raw_packr->data,raw_packr->size, TcpClientSocketFd);
        // 使得下一帧能够被读取
        raw_capl.packet_unref();
        raw_capr.packet_unref();
    }
    return trs;
}

int NetCommunicationModule::TcpServerGetOneClient(int &TcpServerFd, sockaddr &TcpClientAddr, int &TcpCilientFd) {
    if (TcpServerFd == -1)TcpServerFd = ssv.TcpBind(TcpServerPort, TcpServerFd);
    if (TcpServerFd == -1)return -1;
    if (ssv.TcpListen(TcpServerFd) == -1)return -1;
    TcpCilientFd = ssv.TcpSocketAccept(TcpServerFd, TcpClientAddr);
    return TcpCilientFd;   //返回TcpCilientFd；X
}

int NetCommunicationModule::TcpServerReceiveImage(cv::Mat &frm0, int msg[], int &TcpServerFd,
                                                  sockaddr &TcpClientAddr, int &TcpCilientFd, int tp) {
    if (TcpServerFd == -1)TcpServerGetOneClient(TcpServerFd, TcpClientAddr, TcpCilientFd);
    if (TcpCilientFd == -1)return -1;
    if (tp == 0)return ssv.TcpReceiveImage(frm0, TcpCilientFd);
    if (tp == 1)return ssv.TcpReceiveImage(msg, TcpCilientFd);
    return -1;
}

int NetCommunicationModule::TcpServerSendImage(cv::Mat &frm0, int msg[], int &TcpCilientFd, int tp) {
    frm0_ = frm0;
    ServerEncodeImage();
    if (tp == 0)trs = ssv.TcpSendData(TcpCilientFd);
    if (tp == 1)trs = ssv.TcpSendImage(msg, "A", TcpCilientFd);
    return trs;
}

int NetCommunicationModule::UdpClientSendImage(int &UdpClientSocketFd, sockaddr &UdpServerAddr) {
    if (UdpClientSocketFd == -1)
        ssc.GetUdpClientSocketFd(UdpClientTargetIP, UdpClientTargetPort, UdpClientSocketFd, UdpServerAddr);
    trs = ssc.UdpSendImage(UdpClientSocketFd, UdpServerAddr);
    if (debug)std::cout << std::endl << ">";
    return trs;
}

int NetCommunicationModule::UdpClientSendData(byte pData[], size_t len1, int &UdpClientSocketFd) {
    sockaddr UdpServerAddr;
    if (UdpClientSocketFd == -1)
        ssc.GetUdpClientSocketFd(UdpClientTargetIP, UdpClientTargetPort, UdpClientSocketFd, UdpServerAddr);
    trs = ssc.UdpSendData(pData, len1, UdpClientSocketFd, UdpServerAddr);
    if (trs == -9)UdpClientSocketFd = -1;        //发送缓冲区满
    if (!debug)
        std::cout << std::endl << name << " UDP发送" << trs << "/" << len1 << ","
                  << crt << "," << cnt << "," << UdpClientTargetIP << ":" << UdpClientTargetPort;
    return trs;
}

int NetCommunicationModule::UdpClientSendData(byte pData[], size_t len1,
                                              std::string UdpClientTargetIP_, u_short UdpClientTargetPort_,
                                              int &UdpClientSocketFd, sockaddr &UdpServerAddr) {
    if (UdpClientSocketFd == -1)
        ssc.GetUdpClientSocketFd(UdpClientTargetIP_, UdpClientTargetPort_, UdpClientSocketFd, UdpServerAddr);
    int trs = ssc.UdpSendData(pData, len1, UdpClientSocketFd, UdpServerAddr);
    if (!debug)
        std::cout << std::endl << name << " UDP发送 "
                  << UdpClientTargetIP_ << ":" << UdpClientTargetPort_ << "  " << trs << "/" << len1;
    if (trs == -9)UdpClientSocketFd = -1;        //发送缓冲区满
    if (trs > 0)return trs;
    return 0;
}

int NetCommunicationModule::UdpServerReceiveImage(cv::Mat &frm0, int &UdpServerFd, sockaddr &ClientAddr) {
    if (UdpServerFd == -1)UdpServerFd = ssv.UdpBind(UdpServerPort, UdpServerFd);
    rcv = ssv.UdpReceiveImage(frm0, UdpServerFd, ClientAddr);
    if (rcv > 0) return rcv;
    return -1;
}

int NetCommunicationModule::UdpServerReceiveMessage(uchar Rc[], int &UdpServerFd,
                                                    sockaddr &ClientAddr) {
    if (UdpServerFd == -1)ssv.UdpBind(UdpServerPort, UdpServerFd);
    return ssv.UdpReceiveMessage(Rc, UdpServerFd, ClientAddr);
}

bool NetCommunicationModule::iop() const {
    return (bool) iopl || (bool) iopr;
}

#if 0   //<android 24
std::vector<std::string> getMyIp(){
    std::vector<std::string> ips;
    int fd, intrface;
    ifreq buf[INET_ADDRSTRLEN];
    ifconf ifc;
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0){
        ifc.ifc_len = sizeof(buf);
        // caddr_t,linux内核源码里定义的：typedef void *caddr_t；
        ifc.ifc_buf = (caddr_t)buf;
        if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc)){
            intrface = ifc.ifc_len/sizeof(struct ifreq);
            while (intrface-- > 0){
                if (!(ioctl(fd, SIOCGIFADDR, (char *)&buf[intrface]))){
                    std::string ip=(inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
                    ips.push_back(ip);
                }
            }
        }
    }
    close(fd);
    return ips;
}
#endif

std::vector<string> getMyIp() {
    struct ifaddrs *ifAddrStruct = nullptr;
    struct ifaddrs *ifa = nullptr;
    void *tmpAddrPtr = nullptr;
    std::vector<string> ip;
    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) {
            tmpAddrPtr = &sockaddr_inx(ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            ip.push_back(addressBuffer);
            //printf("IP4: %s %s\n", ifa->ifa_name, addressBuffer);
        } else if (ifa->ifa_addr->sa_family == AF_INET6) {
            tmpAddrPtr = &sockaddr_in6x(ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            ip.push_back(addressBuffer);
            //printf("IP6: %s %s\n", ifa->ifa_name, addressBuffer);
        }
    }
    if (ifAddrStruct != nullptr) freeifaddrs(ifAddrStruct);
    return ip;
}
