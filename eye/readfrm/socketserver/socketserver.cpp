#include "socketserver.h"

SocketServer::SocketServer(void){
}

SocketServer::~SocketServer(void){
    TcpDisconnectSocket(slid_);
}

char* SocketServer::sock_ntop(const struct sockaddr *sa){
    char portstr[8];
    static char str[128];
    struct sockaddr_in* s_in = sockaddr_inx(sa);
                                                  //只处理AF_INET: 先将4字节的数字地址转为字符串
    if(inet_ntop(AF_INET,&s_in->sin_addr,str,sizeof(str))==nullptr)return nullptr;
                                                  //再将2字节的数字端口号用snprintf转为字符串
    if (ntohs(s_in->sin_port) != 0) {
        snprintf(portstr, sizeof(portstr), ":%d", ntohs(s_in->sin_port));
        strcat(str, portstr);
    }
    return(str);
}

int SocketServer::SetDatak(){
    datak=std::vector<uchar>(ulong(3*ewd*eht));
    return 1;
}

//设置非阻塞

void SocketServer::SetNonblocking(int sockfd,int tv_sec,int tv_usec) {
    int flag = fcntl(sockfd, F_GETFL, 0);
    if (flag < 0) {
        if(debug) std::cout<<name<<("fcntl F_GETFL fail");
    return;
    }
    if (fcntl(sockfd, F_SETFL, flag | O_NONBLOCK) < 0) {
        if(debug) std::cout<<name<<("fcntl F_SETFL fail");
    }
    struct timeval timeout;
    timeout.tv_sec  = tv_sec;//秒
    timeout.tv_usec = tv_usec;//微秒
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        std::cout<<name<< ("setsockopt failed:");
    }
}

void SocketServer::TcpSetting(int sockfd) {
int yes=1;
if(debug) std::cout<<name<<" 启用TcP地址复用"<<
setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));

int on = 1;
if(debug) std::cout<<name<<" 启用TcP端口复用"<<
setsockopt(sockfd,SOL_SOCKET, 15, &on, sizeof(on)) ;//SO_REUSEPORT=15

/*struct ip_mreq  mreq;
mreq.imr_interface.s_addr = htonl(INADDR_ANY);
mreq.imr_multiaddr.s_addr = inet_addr(UdpServerIp.data());//targetAddr.sin_addr.s_addr;

if(debug) std::cout<<name<<" 加入组播"<<
setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
charx (&mreq), sizeof(mreq)) ;*/
}


int SocketServer::TcpBind(u_short PORT,int &TcpServerFd){
    if (TcpServerFd==-1)TcpServerFd=socket(AF_INET,SOCK_STREAM,0);
    if (TcpServerFd==-1){
        std::cout<<std::endl<<name<<" 获取FD"<<PORT<<"失败";
        if(!debug)sleep(2);
        return -1;
    }
    TcpSetting(TcpServerFd);
    sockaddr TcpServerAddr;
    sockaddr_in server_sockaddr;
    server_sockaddr.sin_family      = AF_INET;
    server_sockaddr.sin_port        = ushort(htons(PORT));
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    TcpServerAddr=*sockaddrx(&server_sockaddr);

    if(bind(TcpServerFd,&TcpServerAddr,sizeof(server_sockaddr))==-1){
        std::cout<<std::endl<<name<<" 绑定TCP端口0.0.0.0:"<<PORT<<"失败";
        if(!debug)sleep(2);
        return -1;
    }
    std::cout<<std::endl<<name<<" 绑定TCP端口0.0.0.0:"<<PORT<<"成功! TcpServerFd:"<<TcpServerFd;
    return TcpServerFd;
}

int SocketServer::TcpListen(int TcpServerFd){
    return listen(TcpServerFd,50);
}

void SocketServer::TcpDisconnectSocket(int &TcpCilientFd){
    close(TcpCilientFd);
    TcpCilientFd=-1;
    printf("\n%s 断开连接 %d",name.c_str(),TcpCilientFd);
}

//static cv::Mat img(IMG_HEIGH, IMG_WIDT, CV_8UC3, scalar(0));
int SocketServer::TcpReceiveImage(cv::Mat &frm0,int TcpClientFd){
    //needRecv[slid] = sizeof(data);
    char a=1;
    int start=0;
    int len2 = 0;
    while(!start){
        int j=0;
        long len=0;
        while(a!='J'){
            len=recv(TcpClientFd, &a, 1, 0);
            j++;
            if (len<1 && errno!=11){if(errno==104){return -104;}return -1;}
            if(j%1024*10==0)printf(" 丢掉10k");
        }
        len=0;
        while (len==0) {
            len=recv(TcpClientFd, &a, 1, 0);
            if (len<1 && errno!=11){if(errno==104){return -104;}return -1;}
        }
        if (a=='P'){
            len=0;
            while (len==0) {
               len= recv(TcpClientFd, &a, 1, 0);
               if (len<1 && errno!=11){if(errno==104){return -104;}return -1;}
            }
            if(a=='E'){
                len=0;
                while (len==0) {
                    len=recv(TcpClientFd, &a, 1, 0);
                    if (len<1 && errno!=11){if(errno==104){return -104;}return -1;}
                }
                if(a=='G'){
                    int len1=0;
                    len=0;
                    while (len==0) {
                        len=recv(TcpClientFd, &len2, 4, 0);
                        if (len<1 && errno!=11){if(errno==104){return -104;}return -1;}
                    }
                    len=0;
                    while (len==0) {
                        len=recv(TcpClientFd, &len1, 4, 0);
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
        len0=recv(TcpClientFd, &datak[rcv], size_t(ulong(len2) - rcv), 0);
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

int SocketServer::TcpReceiveImage(int msg[],int TcpClientFd){
    int m[50];
    for (int i=0;i<50;i++) {
        m[i]=msg[i];
    }
    ssize_t len0 = send(TcpClientFd, m , sizeof(m), 0);
    if(debug) printf("\n%s 发送消息 %zd, ",name.c_str(),len0);
    if(len0<1){
         printf("\n%s 发送消息失败 ",name.c_str());
         return -1;
    }
    len0 = recv(TcpClientFd, head , sizeof(head), 0);

    if(debug) printf("\n%s 接收信息 %zd, ",name.c_str(),len0);
    if(len0>0)return 0;

    printf("\n%s 接收信息出错 ",name.c_str());
    return -1;
}

int SocketServer::TcpSafeRecv(int TcpClientFd, uint8_t* buf, size_t n){
    /**
    @param flags   0：常规操作，与read()相同
    MSG_DONTWAIT:将单个I／O操作设置为非阻塞模式
    MSG_OOB:指明发送的是带外信息
    MSG_PEEK:可以查看可读的信息，在接收数据后不会将这些数据丢失
    MSG_WAITALL:通知内核直到读到请求的数据字节数时，才返回。
    */
    uint32_t index(0);
    int32_t val(0);
    while (index<n)
    {
        val = recv(TcpClientFd, buf+index, n-index, 0); // wait all请求到固定字节数时才返回
        if (val > 0) index += val; // 添加index
        else if (val<1 && errno!=11){if(errno==104){return -104;}return -1;}
        std::cout<<index<<std::endl;
    }
    return index;
}

int SocketServer::TcpMatch(std::string pattern,int TcpClientFd){
    uint index = 0;
    byte tmp; // 收到的字符
    int val; // recv返回值
    uint size = pattern.size();
    uint64_t count = 0; // 记录一共读了多少
    while (true)
    {
        val = TcpSafeRecv(TcpClientFd,&tmp,1);
        count++;
        if (count%1024*10==0)printf(" 丢掉10k");
        if (val != 1) return val;        // check val
        if (tmp == pattern.at(index)) index++;
        else index = 0;
        if (index == size) return size;
    }
}


int SocketServer::TcpReceiveRawImage(cv::Mat &frame_left, cv::Mat &frame_right, int TcpClientFd){
    //needRecv[slid] = sizeof(data);
    char a = 1;
    int len = 0;
    byte tmp;

    len = TcpMatch("FRAME_LEFT", TcpClientFd); // 左图标识符
    if (len < 1 && errno != 11) return len;
    uint32_t data_length;
    recv(TcpClientFd, &data_length, sizeof(data_length),0); // 接受数据长度
    if (debug)printf("\n%s 即将接收左图 %u字节 ", name.c_str(), data_length);
    data_length = 614400;
    datak.resize(data_length); // 声明足够多的空间
    len = TcpSafeRecv(TcpClientFd, &datak[0], data_length);
    if (len < 1 && errno != 11) return len;
    decoder.decode_raw(&datak[0],data_length,frame_left);
    len = TcpMatch("FRAME_RIGHT", TcpClientFd); // 右图标识符
    if (len < 1 && errno != 11) return len;
    recv(TcpClientFd, &data_length, sizeof(data_length),0); // 接受数据长度
    data_length = 614400;
    if (debug)printf("\n%s 即将接收右图 %u字节 ", name.c_str(), data_length);
    datak.resize(data_length); // 声明足够多的空间
    len = TcpSafeRecv(TcpClientFd, &datak[0], data_length);
    if (len < 1 && errno != 11) return len;
    decoder.decode_raw(&datak[0],data_length,frame_right);

    if (frame_left.cols != ewd || frame_left.rows != eht || frame_right.cols != ewd || frame_right.rows!= eht) {
        printf("错误的图像尺寸! \n\n");
        frame_left = cv::Mat(eht, ewd, CV_8UC3, cv::Scalar(0, 100, 0));
        frame_right = cv::Mat(eht, ewd, CV_8UC3, cv::Scalar(0, 100, 0));
        return -1;
    }
    return 0;
}

int SocketServer::TcpReceiveMessage(uchar Rc[], int TcpCilientFd,size_t len1){
    ssize_t len0=recv(TcpCilientFd,&Rc[0],len1,0);
    if(!debug) printf("\n%s 接收信息 %zd, ",name.c_str(),len0);
    if(len0>0) return 0;
    printf("\n%s 接收信息出错 ",name.c_str());
    return -1;
}

int SocketServer::TcpSendMessage(byte Rc[],int TcpCilientFd,size_t len1){
    ssize_t len0 =send(TcpCilientFd,&Rc[0],len1,0);
    if(!debug) printf("\n%s 发送消息 %zd, ",name.c_str(),len0);
    if(len0>0)return int(len0);
    return -1;
}

int SocketServer::TcpSendImage(int TcpCilientFd){
    head[1]=int(datak.size());head[5]=head[1]+31;head[17]=head[1]+111;
    ssize_t len0=send(TcpCilientFd,head,sizeof(head),0);
    if(len0<0 && errno!=11){       //11:资源暂不可用
        printf("\n%s 数据头发送错误: %s(errno: %d)",name.c_str(),strerror(errno),errno);
        if(errno==104)return -104;
        return -1;
    }
    if(debug)printf("\n%s 数据头发送: %zd size:%zu",name.c_str(),len0,datak.size());
    len0=send(TcpCilientFd,datak.data(),datak.size(),0);
    if ((len0 < 1 || len0!=ssize_t(head[1])) && errno!=11){       //11:资源暂不可用
        printf("\n%s数据发送错误: %s ,len0:%zd ,head[1]:%d",name.c_str(),strerror(errno),
               len0,head[1]);
        if(errno==104)return -104;
        return -1;
    }
    if (debug)printf("\n%s 发送数据: %zd\n ",name.c_str(),len0);
    return int(len0);
}

int SocketServer::TcpSendData(int TcpCilientFd){
    if(datak.size()==0)return -1;
    byte a[4]={'J','P','E','G'};
    ssize_t len0=0;
    while(len0!=4){
        len0=send(TcpCilientFd,a,4,0);
        if (len0<1 && errno!=11){if(errno==104){return -104;}return -1;}
    }
    int len1,len2;
    len2=int(datak.size());len1=-len2;
    len0=0;
    while(len0!=4){
        len0=send(TcpCilientFd,&len2,sizeof(len2),0);
        if (len0<1 && errno!=11){if(errno==104){return -104;}return -1;}
    }

    len0=0;
    while(len0!=4){
        len0=send(TcpCilientFd,&len1,sizeof(len1),0);
        if (len0<1 && errno!=11){if(errno==104){return -104;}return -1;}
    }

    if(debug)printf("\n%s 即将发送: %zu字节",name.c_str(),datak.size());

    int sendt=0;
    while(sendt<len2){
        len0=send(TcpCilientFd,&datak[ulong(sendt)],datak.size()-ulong(sendt),0);
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


int SocketServer::TcpSendImage(int msg[],std::string dn,int TcpCilientFd){
    ssize_t len0=0;
    int m[50];
    len0=recv(TcpCilientFd,m,sizeof(m),0);
    for (int i=0;i<50;i++) {
        msg[i]=m[i];
    }
    if ( len0< 1 && errno!=11){       //11:资源暂不可用
        printf("%s 信息接收错误: %s%s(errno: %d)\n",name.c_str(),strerror(errno),dn.c_str(),errno);
        if(errno==104)return -104;
        return -1;
    }
    if (debug)printf("%s recv mesg: %zd \n",name.c_str(),len0);
    len0=send(TcpCilientFd,head,sizeof(head),0);
    if (len0 < 1 && errno!=11){       //11:资源暂不可用
        printf("%s send head error: %s(errno: %d)\n",name.c_str(),strerror(errno),errno);
        if(errno==104)return -104;
        return -1;
    }
    if (debug)printf("%s send head: %zd size:%d\n",name.c_str(),len0,head[1]);
    return 0;
}

int SocketServer::TcpSocketAccept(int TcpServerFd,sockaddr &TcpClientAddr){
    sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    TcpClientAddr=*sockaddrx(&client_addr);
    int TcpCilientFd;
    char cli_ip[INET_ADDRSTRLEN];
    if(!debug) std::cout<<std::endl<<name<<" 等待TCP连接 ... ";
    TcpCilientFd=accept(TcpServerFd, &TcpClientAddr, &length);
    if(TcpCilientFd==-1)return -1;
    inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
    std::cout<<"\n欢迎"<<sock_ntop(&TcpClientAddr)<<" ";
    return TcpCilientFd;
}

int SocketServer::UdpBind(u_short UdpPort,int &UdpServerFd){
    while (UdpServerFd==-1) {
        UdpServerFd = socket(AF_INET,SOCK_DGRAM,0);
        if(debug) std::cout<<std::endl<<name<<" 创建UDP服务器端 ";
        usleep(1000*1000);
    }
    std::cout<<std::endl<<name<<" 创建UDP服务器端成功!编号:"<<UdpServerFd;
    SetNonblocking(UdpServerFd,0,500*1000);                                       //阻塞影响线程25工作

    sockaddr_in server_sockaddr;
         server_sockaddr.sin_family      = AF_INET;
         server_sockaddr.sin_port        = ushort(htons(UdpPort));
         server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY)/*inet_addr(UdpServerIp.data())*/;
    sockaddr ServerAddr=*sockaddrx(&server_sockaddr);
    ebd=-1;
    while (ebd) {
        ebd=bind(UdpServerFd,&ServerAddr,sizeof(ServerAddr));
        if(!debug) printf("\n%s 绑定UDP服务器端端口 %s %5.0d (%d)",
                           name.c_str(),UdpServerIp.c_str(),UdpPort,ebd);
        usleep(2000*1000);
    }
    std::cout<<std::endl<<name<<UdpServerFd<<" 成功绑定UDP服务器端端口:"
            <<"0.0.0.0:"<<UdpPort;
    //client_add=*sockaddrx(&client_addr);
    return 1;
}

int SocketServer::UdpReceiveImage(cv::Mat &frm0,int &UdpServerFd,sockaddr &ClientAddr){
     socklen_t cliaddr_len = sizeof(ClientAddr);
     bzero(&client_add, cliaddr_len);
     ssize_t len2=recvfrom(UdpServerFd,datak.data(),1024*1024,0,&ClientAddr,&cliaddr_len);
     if(debug)std::cout<<len2<<" 字节 ";
     if(debug)printf("\n%s 接收图像数据 %zd,",name.c_str(),len2);
     if (len2>0) frm0 =imdecode(cv::Mat(datak),1);
     return len2>0?0:-1;
}

int SocketServer::UdpReceiveMessage(uchar Rc[],int &UdpServerFd,sockaddr &ClientAddr){
    socklen_t cliaddr_len = sizeof(ClientAddr);
    ssize_t len0=recvfrom(UdpServerFd,Rc,1024*1024,0,&ClientAddr,&cliaddr_len);
    if(!debug && len0!=-1)std::cout<<std::endl<<name<<" 收到"<<len0<<"字节来自"<<" "
                                   <<sock_ntop(&ClientAddr)<<" ";
    if(debug)printf("%s 接收指令 %zd, \n",name.c_str(),len0);
    return int(len0);
}

















