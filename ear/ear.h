#ifndef EAR_H
#define EAR_H
#include <alsa/asoundlib.h>
#include <opencv4/opencv2/core.hpp>
#include <string>
class ear
{
public:
    ear();
    int init(std::string device = "hw:2",
            _snd_pcm_format sndFormat = SND_PCM_FORMAT_S16_LE,
             int channel = 2,
             uint rate = 44100,
             double grabTime = 0.03/*s*/);
    int stop();
    snd_pcm_t *capturer;
    snd_pcm_hw_params_t *params;
    double GrabTime;
    int Rate;
    int GrabSamples;
    int Channel;
    char* buff;
    int bufferSize;
    cv::Mat readbuf();
};

#endif // EAR_H
