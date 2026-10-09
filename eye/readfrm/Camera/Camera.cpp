//
// Created by cc on 2020/12/28.
//

#include "Camera.h"
#include <iostream>

Camera::Camera() {
//    av_register_all(); // 高级版本不用手动注册了
    avformat_network_init();
    //Register Device
    avdevice_register_all();
}

int Camera::open(uint index) {
    pFormatCtx = avformat_alloc_context();
    std::string dev_name = "/dev/video";
    dev_name.append(std::to_string(index));

    ifmt = av_find_input_format("video4linux2");
    // Couldn't open input stream.
    int flag = avformat_open_input(&pFormatCtx, dev_name.c_str(), ifmt, nullptr);
    if (flag != 0) return 0;
#ifdef FYAIRO_1_0_0_ARM
        assert(flag == 0); // 机器人平台要求摄像头全部开启
#endif
    //    Couldn't find stream information.
    assert(avformat_find_stream_info(pFormatCtx, nullptr) >= 0);
    // find video index
    for (uint i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            break;
        }
    }

//    Couldn't find a video stream.
//    assert(videoindex != -1);
    pCodecCtx = pFormatCtx->streams[videoindex]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
//    Codec not found.
//    assert(pCodec != nullptr);

//    Could not open codec.
//    assert(avcodec_open2(pCodecCtx, pCodec, NULL) == 0);
    packet = (AVPacket *) av_malloc(sizeof(AVPacket));

#if 0
    std::cout<<"<CamInfo>\n";
    std::cout<<"width:"<<pCodecCtx->width<<'\n';
    std::cout<<"height:"<<pCodecCtx->height<<'\n';
    std::cout<<"bit_rate:"<<pCodecCtx->bit_rate<<'\n';
    std::cout<<"pix_fmt:"<<pCodecCtx->pix_fmt<<'\n';
    std::cout<<"framerate:"<<pCodecCtx->framerate.den<<','<<pCodecCtx->framerate.num<<std::endl;
#endif
    return !(bool) flag;
}

AVPacket *Camera::read_frame() {
    av_read_frame(pFormatCtx, packet);
#if 0
    for (uint i=0;i<10;i++)
    {
        std::cout<< packet->data[i];
    }
#endif
    return packet;
}

void Camera::packet_unref() {
    av_packet_unref(packet);
}


Decoder::Decoder(AVCodecID codec_id) {
#ifdef FYAIRO_1_0_0_ARM
    return;
#endif
    pkt = av_packet_alloc();
    assert(pkt != nullptr);

    // 设置解码格式
    // raw: AV_CODEC_ID_RAWVIDEO
    codec = avcodec_find_decoder(codec_id);
    //assert(codec); // Codec not found

    codecCtx = avcodec_alloc_context3(codec);
    assert(codecCtx); // Could not allocate video codec context

    if (codec_id == AV_CODEC_ID_RAWVIDEO)
    {
        // 手动设置参数
        codecCtx->width = 640;
        codecCtx->height = 480;
        codecCtx->bit_rate = 147456000;
        codecCtx->pix_fmt = AV_PIX_FMT_YUYV422;
        codecCtx->framerate = (AVRational) {1, 30}; // 不知道干啥用
    }

    /*assert*/(avcodec_open2(codecCtx, codec, nullptr) == 0); // Could not open codec

    frame = av_frame_alloc();
    assert(frame); // Could not allocate video frame
}

Decoder::~Decoder() {
    avcodec_free_context(&codecCtx);
    av_frame_free(&frame);
    av_packet_free(&pkt);
}

int Decoder::decode_raw(uint8_t *data, uint32_t size, cv::Mat& cvMat) {
    pkt->buf = nullptr;
    pkt->data = data+1;
    pkt->size = size;
    int ret = decode(codecCtx, frame, pkt);
    avframe2Cvmat(frame,cvMat);
    return ret;
}

int Decoder::decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt) {
    int ret;

    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
    }

    ret = avcodec_receive_frame(dec_ctx, frame);
    return ret;
}

void Decoder::pgm_save(unsigned char *buf, int wrap, int xsize, int ysize, char *filename) {
    FILE *f;
    int i;

    f = fopen(filename, "wb");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}

void Decoder::avframe2Cvmat(const AVFrame *frame, cv::Mat &image) {
    int width = frame->width;
    int height = frame->height;
    image.cols = width;
    image.rows = height;
    int cvLinesizes[1];
    cvLinesizes[0] = image.step1();
    SwsContext *conversion = sws_getContext(width, height, (AVPixelFormat) frame->format, width, height,
                                            AVPixelFormat::AV_PIX_FMT_BGR24, SWS_FAST_BILINEAR, nullptr, nullptr,
                                            nullptr);
    sws_scale(conversion, frame->data, frame->linesize, 0, height, &image.data, cvLinesizes);
    sws_freeContext(conversion);
}
