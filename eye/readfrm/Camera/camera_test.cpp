//
// Created by cc on 2020/12/29.
//

#include "Camera.h"
#include <iostream>

int main()
{
    Camera cap;
    cap.open(0);
    AVPacket* frm = cap.read_frame();
    for (uint i=0;i<frm->size;i++)
    {
        std::cout<<frm->data[i];
    }
    std::cout<<std::endl;
}