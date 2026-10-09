#include "mind/3d/3d.h"
#include "fycrc/fycrc.h"
typedef unsigned char byte;
typedef byte* bytex;
#define MSG_DATA_SYNC_FALG  (0x47)
/*
unsigned short GetCRC16(bytex pData,int len)
{
    byte CRCHi=0x00,CRCLo=0x00;
    int checklen;
    byte BD;
    unsigned short i;
    bool sCF,lCF,hCF;
    byte CRCGXHi=0x10;
    byte CRCGXLo=0x21;
    unsigned short CRC;

    for(checklen=0;checklen<len;checklen++)
    {
        BD=pData[checklen];
        sCF=false;
        lCF=false;
        hCF=false;
        for(i=0;i<8;i++)
        {
            if((BD&0x80)==0x80)  sCF=true;
            if((CRCHi&0x80)==0x80)  hCF=true;
            if((CRCLo&0x80)==0x80)  lCF=true;
            CRCLo=CRCLo<<1;
            CRCHi=CRCHi<<1;
            if(lCF)CRCHi=CRCHi|0x01;
            if(sCF!=hCF)
            {
                CRCHi=CRCHi^CRCGXHi;
                CRCLo=CRCLo^CRCGXLo;
            }
            BD=BD<<1;
            sCF=false;
            lCF=false;
            hCF=false;
        }
    }

    CRC = CRCHi;
    CRC = CRC << 8;
    CRC |=  CRCLo;

    return CRC;
}
*/

unsigned short GetCRC16(bytex pData,int len){
    return getCRC16(pData,len);
}

int ConfirmCRC(bytex r,int len){
    ushort crc16=GetCRC16(r+1,len-2);
    byte a[2];
    ushort2byte(crc16,a);
    printf("crc16:%2.2X%2.2X ",a[1],a[0]);
    return a[1]==r[len+1-2]&&a[0]==r[len+1-1];
}

int ConfirmOder(bytex r,int len){
    printf(" %2.2X ",r[0]);
    if(r[0]!='G'){
        setzero(r,len);
        return 0;
    }
    printf ("G_OK ");
    byte a[2]={r[2],r[1]};
    ushort c=byte2ushort(a);
    printf("len: %2.2X%2.2X %d ",a[1],a[0],c);
    if(c!=len-1){
        setzero(r,len);
        return 0;
    }
    printf("Len_OK ");
    if(!ConfirmCRC(r,c)){
        setzero(r,len);
        return 0;
    }
    printf("CRC_OK ");
    return 1;
}

/*
int response(byte seq, byte result, sockaddr_in &addr)
{
#define RESPONSE_DATA_CRC_LEN    (2)
#define RESPONSE_DATA_LEN    (5)

    byte data[16];
    int offset = 0;

    data[offset++] = MSG_DATA_SYNC_FALG;
    data[offset++] = seq;
    data[offset++] = result;

    unsigned short crc = GetCRC16(data+1, RESPONSE_DATA_CRC_LEN);

    data[offset++] = (crc >> 8) & 0xFF;
    data[offset++] = crc & 0xFF;


    addr.sin_family = AF_INET;
    //addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(CLIENT_PORT);

#if 0
    sendto(sendSock, data, RESPONSE_DATA_LEN, 0, (struct sockaddr *)&addr, sizeof(addr));

    LogDebug("send response.");
#endif

    return 0;
}
*/
