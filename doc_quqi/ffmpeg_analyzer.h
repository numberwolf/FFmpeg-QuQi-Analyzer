//
// Created by 小老虎 on 2020/4/2.
//

#ifndef FFMPEG_N4_1_3_FFMPEG_ANALYZER_H
#define FFMPEG_N4_1_3_FFMPEG_ANALYZER_H
#include<iostream>
#ifdef __cplusplus
extern "C"
{
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
//#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
//#include <libavutil/imgutils.h>
//#include <libavutil/opt.h>
//#include <libavutil/mathematics.h>
//#include <libavutil/samplefmt.h>
#include <libavutil/motion_vector.h>
#include <libavfilter/avfilter.h>
#include <libavcodec/cyl2analyzer.h>

//typedef struct {
//    uint32_t width;
//    uint32_t height;
//    uint8_t *dataY;
//    uint8_t *dataChromaB;
//    uint8_t *dataChromaR;
//} ImageData;

#ifdef __cplusplus
}
#endif

#define DEL(p) { delete p; p = NULL; }
#define DELX(p) { delete[] p; p = NULL; }

class FFmpegAnalyzer;

class FFmpegAnalyzer {
private:
    long long v_start_time  = -1;

    AVFormatContext *fctx   = nullptr;
    AVPacket *read_packet   = nullptr;

    // Video
    AVFrame *v_frame                = nullptr;
    AVCodecContext *media_video_ctx = nullptr;
    AVCodec *media_video_codec      = nullptr;

public:
    explicit FFmpegAnalyzer();
    ~FFmpegAnalyzer();

    int open_264_media(const char *p_media_path);
    int get_frame();
    int close_media();
};

#endif //FFMPEG_N4_1_3_FFMPEG_ANALYZER_H
