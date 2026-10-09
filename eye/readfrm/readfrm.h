#ifndef READFRM_H
#define READFRM_H

#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <unistd.h>
#include <ifaddrs.h>
#include "../../mind/3d/3d.h"
#include <iostream>
#include "socketclient/socketclient.h"
#include "socketserver/socketserver.h"
#include "Camera/Camera.h"

typedef sockaddr_in *sockaddr_inx;
typedef sockaddr_in6 *sockaddr_in6x;

std::vector<string> getMyIp();
/**
 * @表示摄像头状态
 * NONE: 摄像头未开启
 * NORMAL: 由VideoCapture开启
 * RAW: 由VideoInputStream开启
 *
 * sudo rmmod uvcvideo
 * sudo modprobe uvcvideo quirks=128           强制带宽
 */
enum cam_status{NONE, NORMAL, RAW};


class NetCommunicationModule {
public:
    NetCommunicationModule();

    ~NetCommunicationModule();

private:
    int CamIdl{}, CamIdr{};
    cv::VideoWriter writer;
    cv::VideoCapture capl, capr;
    Camera raw_capl, raw_capr; // 不解码
    int codec{};
    cam_status iopl{}, iopr{}; // 左右摄像头开启状态
public:
    int rddl{}, rddr{}, crt{}, trs{};
    int UdpServerFd{}, TcpClientFd{}, rcv{}, sls{}, grb{}, ler = 500, rer = 500;
    cv::Mat frm0_;
    cv::Mat frml, frmr, frml1, frmr1;
    AVPacket * raw_packl, *raw_packr; // raw格式frame
    std::string rtl,rtr;

    void Init(std::string Name, int ewd = 640, int eht = 480,
              std::string TcpServerIP_ = "192.168.0.0",
              std::string UdpServerIP_ = "192.168.0.1",
              std::string TcpClientTargetIP_ = "192.168.0.2",
              std::string UdpClientTargetIP_ = "192.168.0.3",
              u_short TcpServerPort_ = 8000,
              u_short UdpServerPort_ = 8001,
              u_short TcpClientTargetPort_ = 8002,
              u_short UdpClientTargetPort_ = 8003,
              int CamIdl = 0,
              int CamIdr = 0,
              char cdc0 = 'Y',
              char cdc1 = 'U',
              char cdc2 = 'Y',
              char cdc3 = 'V'
    );

    int sgl{}, cnt{}, scn{}, gnm{};
    typedef unsigned char byte;

    SocketClient ssc;
    SocketServer ssv;

    int CaptureImage(cv::Mat &frm0, int OnLine = 0);

    /**
     *
     * 获取摄像头raw数据
     * @return 0 if success
     *        -1 if failed
     */
    int CaptureRawImage();

    int ClientEncodeImage();

    int ServerEncodeImage();

    int CaptureImage(int chmd);

    int GrabImage(cv::Mat &frm0);

    int ReadImageFromFile(cv::Mat &frm0,
                          cv::String filename = "/home/root/eye/img/001.jpg");

    int TcpClientConnect(int &TcpClientSocketFd, sockaddr &TcpServerAddr,
                         std::string TcpClientTargetIP, ushort TcpClientTargetPort, int &cnt);

    int TcpClientSendImage(int msg[], int tp, int &TcpClientSocketFd, sockaddr &TcpServerAddr,
                           std::string TcpClientTargetIP, ushort TcpClientTargetPort, int &cnt);

    int TcpClientSendText(std::string text, int &TcpClientSocketFd, sockaddr &TcpServerAddr,
                          std::string TcpClientTargetIP, ushort TcpClientTargetPort, int &cnt);

    int TcpClientSendPacket(int msg[], int tp, int &TcpClientSocketFd, sockaddr &TcpServerAddr,
                            std::string tcpClientTargetIp, ushort tcpClientTargetPort, int &i);

    int TcpClientReceiveImage(cv::Mat &frm0, int msg[], int tp, int &TcpClientSocketFd,
                              sockaddr &TcpServerAddr, std::string TcpClientTargetIP,
                              ushort TcpClientTargetPort, int &cnt);

    int TcpClientReceiveData(char oder[], int &TcpClientSocketFd, sockaddr &TcpServerAddr,
                             std::string TcpClientTargetIP, ushort TcpClientTargetPort, int &cnt);

    int TcpServerReceiveImage(cv::Mat &frm0, int msg[], int &TcpServerFd, sockaddr &TcpServerAddr, int &TcpCilientFd,
                              int tp = 0);

    int TcpServerSendImage(cv::Mat &frm0, int msg[], int &TcpCilientFd, int tp = 0);

    int TcpServerGetOneClient(int &TcpServerFd, sockaddr &TcpClientAddr, int &TcpCilientFd);

    int UdpClientSendImage(int &UdpClientSocketFd, sockaddr &UdpServerAddr);

    int UdpClientSendData(byte pData[], size_t len1, int &UdpClientSocketFd);

    int UdpClientSendData(byte pData[], size_t len1,
                          std::string UdpClientTargetIP_, u_short UdpClientTargetPort_,
                          int &UdpClientSocketFd, sockaddr &UdpServerAddr);

    int UdpServerReceiveImage(cv::Mat &frm0, int &UdpServerFd, sockaddr &ClientAddr);

    int UdpServerReceiveMessage(uchar Rc[], int &UdpServerFd, sockaddr &ClientAddr);
    /**
     * @brief 有一个摄像头准备好了
     * @return true if iopl || iopr
     */
    bool iop() const;
    int debug = 0;
    int ewd{}, eht{};
    u_short TcpServerPort{}, UdpServerPort{}, TcpClientTargetPort{}, UdpClientTargetPort{};
    std::string TcpServerIP, UdpServerIP, TcpClientTargetIP, UdpClientTargetIP;
    std::string name, clientname;
};

#endif //READFRM_H
