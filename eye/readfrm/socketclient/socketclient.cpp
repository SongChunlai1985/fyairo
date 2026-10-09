#include "socketclient.h"

SocketClient::SocketClient(void){
}

SocketClient::~SocketClient(void){

}
                                                                    //设置非阻塞
void SocketClient::SetNonblocking(int sockfd) {
    int flag=fcntl(sockfd,F_GETFL,0);
    if (flag > 0) {
        //std::cout<<std::endl<<name<<(" fcntl F_GETFL OK ");
    }
    if (fcntl(sockfd,F_SETFL,flag | O_NONBLOCK) > 0) {
        //std::cout<<std::endl<<name<<(" fcntl F_SETFL OK ");
    }
}

int SocketClient::SetDatak(){
    datak=std::vector<uchar>(ulong(3*ewd*eht));
    return 0;
}

int SocketClient::TcpCreateSocket(int &TcpClientSocketFd){
    if(TcpClientSocketFd!=-1)return TcpClientSocketFd;
    TcpClientSocketFd=socket(AF_INET,SOCK_STREAM,0);
    std::cout<<std::endl<<name<<" 创建TCP套接字! FD:"<<TcpClientSocketFd;
    return TcpClientSocketFd;
}

int SocketClient::TcpConnectSocket(std::string IP,u_short PORT,int &TcpClientSocketFd,sockaddr &TcpServerAddr){
    sockaddr_in servaddr;
    servaddr.sin_family       =AF_INET;
    servaddr.sin_port         =ushort(htons(PORT));
    servaddr.sin_addr.s_addr  =inet_addr(IP.data());
    TcpServerAddr=*sockaddrx (&servaddr);
    if(connect(TcpClientSocketFd,&TcpServerAddr,sizeof(servaddr))==-1){
        if(debug){
            std::cout<<std::endl<<name<<" 正在连接TCP地址:"<<IP<<":"<<PORT<<" "
                    <<errno<<":"<<strerror(errno);
            sleep(5);
        }
        return -1;
    }
    std::cout<<std::endl<<name<<" 连接TCP地址:"<<IP<<":"<<PORT<<"成功"<<std::endl;
    SetNonblocking(TcpClientSocketFd);
    TcpIP=IP;
    TcpPort=PORT;
    return 0;
}

int SocketClient::TcpSendData(int TcpClientSocketFd){
    if(dataj.size()==0)return -1;
    byte a[4]={'J','P','E','G'};
    ssize_t len0=0;
    while(len0!=4){
        len0=send(TcpClientSocketFd,a,4,0);
        if (len0<1 && errno!=11){if(errno==104){return -104;}return -1;}
    }
    int len1,len2;
    len2=int(dataj.size());len1=-len2;
    len0=0;
    while(len0!=4){
        len0=send(TcpClientSocketFd,&len2,sizeof(len2),0);
        if (len0<1 && errno!=11){if(errno==104){return -104;}return -1;}
    }

    len0=0;
    while(len0!=4){
        len0=send(TcpClientSocketFd,&len1,sizeof(len1),0);
        if (len0<1 && errno!=11){if(errno==104){return -104;}return -1;}
    }

    if(debug)printf("\n%s 即将发送: %zu字节",name.c_str(),dataj.size());

    int sendt=0;
    while(sendt<len2){
        len0=send(TcpClientSocketFd,&dataj[ulong(sendt)],dataj.size()-ulong(sendt),0);
        if(len0!=-1)sendt+=len0;
        if(debug)printf("数据发送: %s , %d/%d",name.c_str(),sendt,len2);
        if (len0<1 && errno!=11){
            printf("\n%s数据发送错误: %s , %d/%d",name.c_str(),strerror(errno),
                   sendt,len2);
            if(debug)sleep(2);
            if(errno==104){
                return -104;
            }
            return -1;
        }
        if (len0<1 && errno==11){       //11:资源暂不可用
            usleep(1*1000);
        }
    }

    if(debug)printf("\n%s 发送数据: %d/%d ",name.c_str(),sendt,len2);
    return 0;
}

int SocketClient::TcpSendData(int msg[],std::string dn,int TcpClientSocketFd){
    ssize_t len0=0;
    int m[50];
    len0=recv(TcpClientSocketFd,m,sizeof(m),0);
    for (int i=0;i<50;i++) {
        msg[i]=m[i];
    }
    if ( len0< 1 && errno!=11){       //11:资源暂不可用
        printf("%s 信息接收错误: %s%s(errno: %d)\n",name.c_str(),strerror(errno),dn.c_str(),errno);
        return -1;
    }
    if (debug)printf("%s recv mesg: %zd \n",name.c_str(),len0);
    len0=send(TcpClientSocketFd,head,sizeof(head),0);
    if (len0 < 1){
        printf("%s send head error: %s(errno: %d)\n",name.c_str(),strerror(errno),errno);
        return -1;
    }
    if (debug)printf("%s send head: %zd size:%d\n",name.c_str(),len0,head[1]);
    return 0;
}

int SocketClient::TcpSendText(std::string text,int TcpClientSocketFd){
    char p[50];
        text.copy(p,text.length(),0);
        *(p+text.length())='\0';
    send(TcpClientSocketFd,&p,50,0); //&?
    return 0;
}

int SocketClient::TcpSafeSend(int TcpClientSocketFd, uint8_t * data, uint32_t size) {
    uint64_t sendt=0;
    int64_t len0;
    while(sendt<size){
        len0=send(TcpClientSocketFd,data+sendt,size-sendt,0);
        if(len0 > 0)sendt+=len0;
        if (len0<1 && errno!=11){
            if(errno==104){
                return -104;
            }
            return -1;
        }
        if (len0<1 && errno==11){       //11:资源暂不可用
            usleep(1*1000);
        }
        std::cout<<sendt<<std::endl;
    }
    return sendt;
}

int SocketClient::TcpSendPacket(uint8_t *datal ,uint32_t sizel,uint8_t* datar,uint32_t sizer, int TcpClientSocketFd){
    assert(dataj.empty());
    //  左边图像
    byte name_l[] = "FRAME_LEFT";
    byte name_r[] = "FRAME_RIGHT";
    byte bkspace = '\n'; // 暂时没用到
    TcpSafeSend(TcpClientSocketFd,name_l,sizeof(name_l)/sizeof(byte));
    send(TcpClientSocketFd,&sizel,sizeof(sizel),0);
    // 图像数据
    TcpSafeSend(TcpClientSocketFd,datal,sizel);

//    右边图像
    TcpSafeSend(TcpClientSocketFd,name_r,sizeof(name_r)/sizeof(byte));
    send(TcpClientSocketFd,&sizer,sizeof(sizer),0);
    // 图像数据
    TcpSafeSend(TcpClientSocketFd,datar,sizer);
    std::cout<<sizer<<std::endl;
    return 0;
}

void SocketClient::TcpDisconnectSocket(int &TcpClientSocketFd){
    std::cout<<std::endl<<name<<"断开连接"<<TcpIP<<":"<<TcpPort;
    close(TcpClientSocketFd);
    TcpClientSocketFd=-1;
}

int SocketClient::TcpReceiveImage(cv::Mat &frm0,int TcpClientSocketFd){
    //needRecv[slid] = sizeof(data);
    ssize_t len0 = 0;
    len0=recv(TcpClientSocketFd,head,80,0);
    while(!(len0==80 && head[5]==head[1]+31 && head[17]==head[1]+111)){
#if 0
        printf("\n%s 获取文件长度出错: len0=%zd head[1]=%d "
               "head[5]=%d head[17]=%d",
               name.c_str(),len0,head[1],head[5],head[17]);
#endif
        len0=recv(TcpClientSocketFd,&datak[0],512*1024,0);          //清空缓存
        if(errno==-104)return -104;
        return -1;
    }

    if(debug)printf("\n%s 即将接收 % 4.1d字节 ",name.c_str(),head[1]);

    unsigned long rcv=0;
    while (rcv<ulong(head[1])){
        len0=recv(TcpClientSocketFd,&datak[rcv],size_t(ulong(head[1])-rcv),0);
        if(debug)printf("\n%s 接收图片数据 %zd %lu/%d",
                          name.c_str(),len0,rcv,head[1]);
        if(debug)printf(" %zd,",len0);
        if(len0>0)rcv+= ulong(len0);
        if(len0<0){
            if(errno!=11){       //11:资源暂不可用
                printf("\n%s 没收到图片数据, %s errno:%d ",name.c_str(),
                          strerror(errno),errno);
            }
            if(errno==-104)return -104;
        }
    }

    if(debug)printf("\n%s 收到数据 %lu/%d,%zd",name.c_str(),rcv,head[1],len0);

    if(rcv!=ulong(head[1]))printf(".");

    frm0=imdecode(cv::Mat(datak),1);
    if(frm0.cols!=ewd*2||frm0.rows!=eht){
        printf("错误的图像尺寸: % 4.0d , % 4.0d, \n\n",frm0.cols,frm0.rows);
        return -1;
    }
    return 0;
}

int SocketClient::TcpReceiveJpegData(cv::Mat &frm0,int TcpClientSocketFd){
    //needRecv[slid] = sizeof(data);
    char a=1;
    int start=0;
    int len2 = 0;
    while(!start){
        int j=0;
        long len=0;
        while(a!='J'){
            len=recv(TcpClientSocketFd,&a,1,0);
            j++;
            if (len<1 && errno!=11){if(errno==104){return -104;}return -1;}
            if(j%1024*10==0)printf(" 丢掉10k");
        }
        len=0;
        while (len==0) {
            len=recv(TcpClientSocketFd,&a,1,0);
            if (len<1 && errno!=11){if(errno==104){return -104;}return -1;}
        }
        if (a=='P'){
            len=0;
            while (len==0) {
               len= recv(TcpClientSocketFd,&a,1,0);
               if (len<1 && errno!=11){if(errno==104){return -104;}return -1;}
            }
            if(a=='E'){
                len=0;
                while (len==0) {
                    len=recv(TcpClientSocketFd,&a,1,0);
                    if (len<1 && errno!=11){if(errno==104){return -104;}return -1;}
                }
                if(a=='G'){
                    int len1=0;
                    len=0;
                    while (len==0) {
                        len=recv(TcpClientSocketFd,&len2,4,0);
                        if (len<1 && errno!=11){if(errno==104){return -104;}return -1;}
                    }
                    len=0;
                    while (len==0) {
                        len=recv(TcpClientSocketFd,&len1,4,0);
                        if (len<1 && errno!=11){if(errno==104){return -104;}return -1;}
                    }
                    if(len1==-len2){
                        start=1;
                        //printf("JPEG");
                    }
                }
            }
        }
    }

    long len0=0;
    if(debug)printf("\n%s 即将接收 % 4.1d字节 ",name.c_str(),len2);

    unsigned long rcv=0;
    while (rcv<ulong(len2)){
        len0=recv(TcpClientSocketFd,&datak[rcv],size_t(ulong(len2)-rcv),0);
        //printf("\n%s 接收图片数据 %zd %lu/%d",name.c_str(),len0,rcv,head[1]);
        //printf(" %zd,",len0);
        if(len0>0)rcv+= ulong(len0);
        if(len0<0 && errno!=11){       //11:资源暂不可用
            printf("%s ,%d:%s",name.c_str(),errno,strerror(errno));
            if(errno==104){
                return -104;
            }
            return -1;
        }
    }

   if(debug)printf(" \n%s 收到数据 %lu/%d,%ld",name.c_str(),rcv,len2,len0);

    if(rcv!=ulong(len2))printf(".");
    frm0=imdecode(cv::Mat(datak),1);
    if(frm0.cols!=ewd*2||frm0.rows!=eht){
        printf("错误的图像尺寸: % 4.0d , % 4.0d, \n\n",frm0.cols,frm0.rows);
        frm0=cv::Mat( eht, ewd*2, CV_8UC3, cv::Scalar(0,100,0));
        return -1;
    }
    return 0;
}

int SocketClient::TcpReceiveImage(int msg[],int TcpClientSocketFd){
    ssize_t len0 = 0;
    int m[50];
    for (int i=0;i<50;i++) {
        m[i]=msg[i];
    }
    len0 = send(TcpClientSocketFd, m , sizeof(m), 0);
    if(debug) printf("\n%s 发送消息 %zd, ",name.c_str(),len0);
    if(len0<1){
         printf("\n%s 发送消息失败 ",name.c_str());
         return -1;
    }
    len0 = recv(TcpClientSocketFd, head , sizeof(head), 0);
    if(len0<1){
        printf("\n 接收消息出错 ");
        return -1;
    }
    if(debug) printf("\n%s 接收信息 %zd, ",name.c_str(),len0);
    if(len0<1){
        printf("\n%s 接收信息出错 ",name.c_str());
        return -1;
    }
    return 0;
}
int SocketClient::TcpReceiveData(char oder[],int TcpClientSocketFd){
    ssize_t len0 = recv(TcpClientSocketFd,&oder[0],4096,0);
    if(len0==-1 && errno!=11){       //11:资源暂不可用
        if(!debug) printf("\n%s 接收信息出错 %d:%s ",name.c_str(),errno,strerror(errno));
        if(errno==104)return -104;
        return -1;
    }
    if(len0==-1)return -1;
    return int(len0);                //断点位置
}

void SocketClient::UdpSetting(int sockfd) {
    int yes=1;
    std::cout<<std::endl<<name<<" 启用UDP地址复用 "<<
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));

    int on=1;
    std::cout<<std::endl<<name<<" 启用UDP端口复用 "<<
    setsockopt(sockfd,SOL_SOCKET,15,&on,sizeof(on)) ;//SO_REUSEPORT=15

    struct ip_mreq  mreq;
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    mreq.imr_multiaddr.s_addr=inet_addr(/*"224.0.1.71"*/"127.0.0.1");
                                           //targetAddr.sin_addr.s_addr;
    std::cout<<std::endl<<name<<" 加入组播 "<<
    setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,
                charx (&mreq),sizeof(mreq)) ;

    int n=1;  //  0表示关闭属性，非0表示打开属性
    setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&n,sizeof(n));
}

int SocketClient::GetUdpClientSocketFd(std::string IP,u_short PORT,int &UdpClientSocketFd,sockaddr &UdpServerAddr){
    if(UdpClientSocketFd==-1)UdpClientSocketFd=socket(AF_INET,SOCK_DGRAM,0);
    if(UdpClientSocketFd==-1)return -1;
    SetNonblocking(UdpClientSocketFd);
    sockaddr_in servaddr;
    servaddr.sin_family       =AF_INET;
    servaddr.sin_port         =ushort(htons(PORT));
    servaddr.sin_addr.s_addr  =inet_addr(IP.data());
    UdpServerAddr=*const_struct_sockaddrx(&servaddr);
    if(debug)std::cout<<std::endl<<name<<" UDP发送到: "<<IP<<" 端口: "<<PORT<<" 文件描述符:"<<UdpClientSocketFd;
    return 0;
}

int SocketClient::UdpSendImage(int UdpClientSocketFd,sockaddr UdpServerAddr){
    ssize_t len0=-2;
    size_t jsz=dataj.size();
    if(debug)printf("%s 将发送数据:%zu\n",name.c_str(),jsz);
    socklen_t servddr_len=sizeof(UdpServerAddr);
    len0=sendto(UdpClientSocketFd,dataj.data(),jsz,0,&UdpServerAddr,servddr_len);
    if(debug)std::cout<<">"<<len0;
    if (len0 < 1){
        printf("%s 发送数据错误: %s\n",name.c_str(),strerror(errno));
        return -1;
     }
    if (debug)printf("%s 发送数据: %zd\n\n",name.c_str(),len0);
    return 0;
}

int SocketClient::UdpSendData(byte pData[],size_t len1,int UdpClientSocketFd,sockaddr UdpServerAddr){
    socklen_t servddr_len=sizeof(UdpServerAddr);
    ssize_t len0=sendto(UdpClientSocketFd,pData,len1,0,&UdpServerAddr,servddr_len);
    if(debug)std::cout<<">"<<len0;
    if (len0<1){
        printf("\n%s 发送数据失败: %d %s \n",name.c_str(),errno,strerror(errno));
        if(errno==9)return -9;
        return -1;
     }
    if (debug)printf("%s 发送数据: %zd \n",name.c_str(),len0);
    return int(len0);
}

int SocketClient::UdpSendData(std::vector<uchar> Ro,int UdpClientSocketFd,sockaddr UdpServerAddr){
    socklen_t servddr_len=sizeof(UdpServerAddr);
    ssize_t len0=sendto(UdpClientSocketFd,&Ro,Ro.size(),0,&UdpServerAddr,servddr_len);
    if(debug)std::cout<<">"<<len0;
    if (len0 < 1){
        printf("%s 未能发送数据: %s \n",name.c_str(),strerror(errno));
        return -1;
    }
    if (debug)printf("%s 已发送数据: %zd \n",name.c_str(),len0);
    return 0;
}

















