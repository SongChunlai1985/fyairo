#ifndef  SOCKETCLIENT_H
#define  SOCKETCLIENT_H

#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstdlib>
#include "../../../mind/3d/3d.h"

class SocketClient {
public:
    SocketClient(void);

    ~SocketClient(void);

private:
    sockaddr TcpServerAddr;
    sockaddr UdpServerAddr;
    sockaddr_in dest_addr;

    void UdpSetting(int sockfd);

public:
    typedef unsigned char byte;
    typedef sockaddr *sockaddrx;
    typedef socklen_t *socklen_tx;
    typedef const struct sockaddr *const_struct_sockaddrx;
    typedef char *charx;
    int UdpClientSocketFd;
    int head[20];
    int debug, ewd, eht;
    int jpgb = 66;
    std::string name, TcpIP;
    ushort TcpPort;
    std::vector<byte> dataj, datak;

    void SetNonblocking(int sockfd);

    int SetDatak();

    int TcpCreateSocket(int &TcpClientSocketFd);

    int TcpConnectSocket(const std::string IP, u_short PORT, int &TcpClientSocketFd, sockaddr &TcpServerAddr);

    int TcpSendData(int TcpClientSocketFd);

    int TcpSendData(int msg[], std::string dn, int TcpClientSocketFd);

    /**
     * @brief 安全发送数据，一直发送直到全部完成
     * @param TcpClientSocketFd tcp端口
     * @param data 数据
     * @param size 数据长度
     * @return 发送的数据长度/错误码
     */
    static int TcpSafeSend(int TcpClientSocketFd,uint8_t * data,uint32_t size);
    int TcpSendPacket(uint8_t *datal,uint32_t sizel,uint8_t * datar ,uint32_t sizer, int TcpClientSocketFd);  // 传输图片raw数据

    int TcpSendText(std::string text, int TcpClientSocketFd);

    void TcpDisconnectSocket(int &TcpClientSocketFd);

    int TcpReceiveImage(cv::Mat &frm0, int TcpClientSocketFd);

    int TcpReceiveImage(int msg[], int TcpClientSocketFd);

    int TcpReceiveJpegData(cv::Mat &frm0, int TcpClientSocketFd);

    int TcpReceiveData(char oder[], int TcpClientSocketFd);

    int GetUdpClientSocketFd(const std::string IP, u_short PORT,
                             int &UdpClientSocketFd, sockaddr &UdpServerAddr);

    int UdpSendImage(int UdpClientSocketFd, sockaddr UdpServerAddr);

    int UdpSendData(byte pData[], size_t len1, int UdpClientSocketFd, sockaddr UdpServerAddr);

    int UdpSendData(std::vector<uchar> Ro, int UdpClientSocketFd, sockaddr UdpServerAddr);
};

#endif//SOCKETCLIENT_H
