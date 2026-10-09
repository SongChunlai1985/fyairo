#ifndef FYCRC_H
#define FYCRC_H

unsigned short getCRC16(unsigned char * pData, int len);
unsigned short getCRC16(unsigned short crc, unsigned char * pData, int len); // crc为上次计算的值

unsigned int getCRC32(unsigned char* pData, int len);
unsigned int getCRC32(unsigned int crc, unsigned char* pData, int len); // crc为上次计算的值

#endif // FYCRC_H
