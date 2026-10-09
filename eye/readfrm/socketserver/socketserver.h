#ifndef SOCKETSEVER_H
#define SOCKETSEVER_H

#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "../Camera/Camera.h"

typedef unsigned char byte;

class SocketServer{
public:
        SocketServer(void);
        ~SocketServer(void);
private:
        int TcpCilientFd;
        int count;
        int TcpServerFd,
            UdpServerFd;
        Decoder decoder = Decoder(AV_CODEC_ID_RAWVIDEO);
        sockaddr client_add;
        int rcv;
        int slid_;
public:
        typedef sockaddr*  sockaddrx;
        typedef sockaddr_in* sockaddr_inx;
        typedef const struct sockaddr* const_struct_sockaddrx;
        typedef char* charx;

        int head[20];
        int jpgb=66;
        std::vector<byte> datak;
        int debug,ewd,eht;
        int ebd,eap,tot;

        std::string TcpServerIp;
        std::string UdpServerIp;
        std::string name;
        std::string clientname;

        char* sock_ntop(const struct sockaddr *sa);
        void SetNonblocking(int sockfd,int tv_sec,int tv_usec);
        int SetDatak();

        void TcpSetting(int sockfd);
        int TcpBind(u_short PORT, int &TcpServerFd);
        int TcpListen(int TcpServerFd);
        int TcpSocketAccept(int TcpServerFd, sockaddr &client_add);
        int TcpReceiveImage(cv::Mat& frm0, int TcpClientFd);
        int TcpReceiveImage(int msg[], int TcpClientFd);

        /**
         * @brief 收取数据直到收取到或者出错
         * @param TcpClientfd
         * @param buf
         * @param n
         * @return n 成功
         *         -104 reset by peer
         *         -1 别的失败
         */
        static int TcpSafeRecv(int TcpClientFd, uint8_t* buf, size_t n);
        /**
         * @brief 从指定连接寻找匹配的字符串
         * @param pattern 需要匹配的字符串
         * @param TcpClientFd 连接的fd
         * @reutrn size of pettern 成功
         *         负数 失败, 参考 TcpSafeRecv
         */
        static int TcpMatch(std::string pattern,int TcpClientFd);
        /**
         * @brief 收取raw send发出的图像并解码存入frm0中
         * @param frm0 存放图像的mat
         * @return
         */
        int TcpReceiveRawImage(cv::Mat &frame_left, cv::Mat &frame_right, int TcpClientFd);
        int TcpReceiveMessage(uchar Rc[], int TcpCilientFd,size_t len1);

        void TcpDisconnectSocket(int &TcpCilientFd);
        int TcpSendImage(int TcpCilientFd);
        int TcpSendData(int TcpCilientFd);
        int TcpSendImage(int msg[], std::string dn, int TcpCilientFd);
        int TcpSendMessage(byte Rc[], int TcpCilientFd, size_t len1);

        int UdpBind(u_short UdpPort, int &UdpServerFd);
        int UdpReceiveImage(cv::Mat& frm0, int &UdpServerFd, sockaddr &ClientAddr);
        int UdpReceiveMessage(uchar Rc[], int &UdpServerFd, sockaddr &ClientAddr);

};

#endif //SOCKETSEVER_H
