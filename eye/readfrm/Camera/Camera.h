//
// Created by cc on 2020/12/28.
// 利用ffmpeg库调用摄像头
// 以及编码解码
//

#ifndef VIDEO_INPUT_STREAM_H
#define VIDEO_INPUT_STREAM_H

#include <cassert>
#include <string>
#include <opencv2/core.hpp>

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#ifdef __cplusplus
}
#endif

class Camera {
private:
    AVInputFormat *ifmt{};
    AVFormatContext *pFormatCtx{};
    int videoindex = -1;
    AVCodecContext *pCodecCtx{};
    AVCodec *pCodec{};
    AVPacket *packet{};
public:
    Camera();

    /**
     * @brief 打开摄像头
     * @param index 摄像头的参数 如0->/dev/video0
     * @return 0 成功
     */
    int open(uint index);

    AVPacket *read_frame(); // 读取一帧
    void packet_unref(); // 取消对包的链接
};

class Decoder {
private:
    const AVCodec *codec;
    AVCodecContext *codecCtx = nullptr;
    AVPacket *pkt;
    AVFrame *frame;

public:
    explicit Decoder(AVCodecID codec_id);
    ~Decoder();

    /**
     * @brief 根据传入的数据和长度，将图像解码并放入cv::Mat中
     * @param data 传入数据
     * @param size 传入数据大小
     * @param cvMat 将要放入的结构体
     * @return 0 成功
     *        <0 失败或者别的问题，参看 decode()
     */
    int decode_raw(uint8_t *data, uint32_t size, cv::Mat& cvMat);

    /**
     * @brief 根据传入的解码参数解码
     * @param dec_ctx
     * @param frame ffmpeg存储图像的结构
     * @param pkt 包数据
     * @return 0:                 success, a frame was returned
 *      AVERROR(EAGAIN):   output is not available in this state - user must try
 *                         to send new input
 *      AVERROR_EOF:       the decoder has been fully flushed, and there will be
 *                         no more output frames
 *      AVERROR(EINVAL):   codec not opened, or it is an encoder
 *      AVERROR_INPUT_CHANGED:   current decoded frame has changed parameters
 *                               with respect to first decoded frame. Applicable
 *                               when flag AV_CODEC_FLAG_DROPCHANGED is set.
 *      other negative values: legitimate decoding errors
     */
    static int decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt);

    /**
     * @brief 将帧数据存入视频文件中， 没用过不保证能用
     */
    static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize, char *filename);

    /**
     * @brief 将avframe转换为cv::Mat
     * @param frame 导入的avFrame结构体
     * @param image 导出的cv::Mat
     * https://zhuanlan.zhihu.com/p/80350418
     */
    static void avframe2Cvmat(const AVFrame *frame,cv::Mat& image);
};

#endif
