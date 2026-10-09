#include "ear.h"

ear::ear()
{

}

int ear::init(std::string device, _snd_pcm_format sndFormat, int channel, uint rate, double grabTime)
{
    GrabTime = grabTime;
    Rate = rate;
    Channel = channel;
    snd_pcm_open (&capturer, device.c_str(), SND_PCM_STREAM_CAPTURE, 0);
    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(capturer, params);
    snd_pcm_hw_params_set_access(capturer, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(capturer, params, sndFormat);
    snd_pcm_hw_params_set_rate_near(capturer, params, &rate, 0);
    snd_pcm_hw_params_set_channels(capturer, params, channel);
    snd_pcm_hw_params(capturer, params);
    snd_pcm_hw_params_free(params);
    snd_pcm_prepare(capturer);
    int bytePerFrame = snd_pcm_format_width(sndFormat) / 8;
    GrabSamples = GrabTime * Rate * channel;
    bufferSize = GrabSamples * bytePerFrame;
    buff = (char*)malloc(bufferSize);
    return 0;
}

int ear::stop()
{
    snd_pcm_close(capturer);
    return 0;
}

cv::Mat ear::readbuf()
{
    snd_pcm_readi(capturer, buff, GrabSamples / Channel);
    cv::Mat dataMat(2, bufferSize / 2 / Channel, CV_16SC1, buff);
    return dataMat;
}
