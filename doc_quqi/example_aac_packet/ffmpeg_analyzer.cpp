//
// Created by 小老虎 on 2020/4/2.
//
#include "ffmpeg_analyzer.h"

FFmpegAnalyzer::FFmpegAnalyzer() {
}

FFmpegAnalyzer::~FFmpegAnalyzer() {
}

int FFmpegAnalyzer::open_aac_media(const char *p_media_path) {
//    av_register_all();
    this->media_codec = avcodec_find_decoder(AV_CODEC_ID_AAC);

    if (!this->media_codec) {
        printf("avcodec_find_decoder 初始化解码器失败\n");
        return -1;
    }

    const char *codec_name  = this->media_codec->name;
    this->media_ctx   = avcodec_alloc_context3(this->media_codec);

    if (!this->media_ctx) {
        return -2;
    }

    AVDictionary *options = NULL;
    av_dict_set(&options, "flags2", "+export_mvs", 0);

    if (avcodec_open2(this->media_ctx, this->media_codec, &options) < 0) {
        return -3;
    }

    // open analyzer
    this->media_ctx->cyl_analyze_open = 1;
    this->media_ctx->cyl_test(5);
//    this->media_video_ctx->cyl_analyzer_state_switch(this->media_video_ctx, 1);

//    this->v_frame = av_frame_alloc();
//    if (!this->v_frame) {
//        return -4;
//    }

    this->read_packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    av_init_packet(this->read_packet);

    int ret = avformat_open_input(&this->fctx, p_media_path, NULL, NULL); // &options

    if(ret != 0) {
        return -5;
    }

//    fctx->probesize = 4096;

    return 0;
}

int FFmpegAnalyzer::get_frame() {
    // loop frame
    int read_res = av_read_frame(this->fctx, this->read_packet);

    if(read_res >= 0) {
        if (this->v_start_time < 0) {
            this->v_start_time = (this->read_packet->dts < this->read_packet->pts) ? this->read_packet->dts
                                                                                   : this->read_packet->pts;

        }

        this->read_packet->dts -= this->v_start_time;
        this->read_packet->pts -= this->v_start_time;
        this->read_packet->pos = -1;

        std::cout << this->read_packet->data << std::endl;
    }

    av_packet_unref(this->read_packet);
    return 0;
}

int FFmpegAnalyzer::close_media() {
//    if (this->v_frame != nullptr) {
//        av_frame_free(&this->v_frame);
//        this->v_frame = nullptr;
//    }

    if (nullptr != this->media_ctx) {
        avcodec_close(this->media_ctx);
        this->media_ctx = nullptr;
    }

    if (nullptr != read_packet) {
        av_packet_unref(this->read_packet);
        av_freep(this->read_packet);
//        this->read_packet->data = NULL;
//        this->read_packet->size = 0;
        this->read_packet = nullptr;
    }

    if (nullptr != this->fctx) {
        avformat_close_input(&this->fctx);
        // avformat_free_context(fctx);
        this->fctx = nullptr;
    }

    this->media_codec = nullptr;
    this->v_start_time      = -1;


    return 0;
}







