//
// Created by 小老虎 on 2020/4/2.
//
#include "ffmpeg_analyzer.h"

FFmpegAnalyzer::FFmpegAnalyzer() {
}

FFmpegAnalyzer::~FFmpegAnalyzer() {
}

int FFmpegAnalyzer::open_aac_media(const char *p_media_path) {
//    this->fctx = avformat_alloc_context();
    int ret = avformat_open_input(&this->fctx, p_media_path, NULL, NULL); // &options
    this->fctx->probesize = 4096;

    if(ret != 0) {
        return -5;
    }

    ret = avformat_find_stream_info(this->fctx,NULL);
    if(ret < 0) {
        printf("can't find stream info ");
        return -1;
    }

    for(int i=0; i<this->fctx->nb_streams; i++) {
        if (this->fctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            std::cout << "v," << i << std::endl;
        } else if (this->fctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            this->audio_stream = i;
            std::cout << "a:" << this->audio_stream << std::endl;
        }
    }

    this->media_ctx     = this->fctx->streams[this->audio_stream]->codec;
    this->media_codec   = avcodec_find_decoder(this->media_ctx->codec_id);

    std::cout << "acodec name:" << this->media_codec->name << std::endl;
    std::cout << "acodec id:" << this->media_ctx->codec_id << std::endl;
    std::cout << "aac id:" << AV_CODEC_ID_AAC << std::endl;



//    av_register_all();
//    this->media_codec = avcodec_find_decoder(AV_CODEC_ID_AAC);
//
//    if (!this->media_codec) {
//        printf("avcodec_find_decoder 初始化解码器失败\n");
//        return -1;
//    }
//
//    const char *codec_name  = this->media_codec->name;
//    this->media_ctx   = avcodec_alloc_context3(this->media_codec);
//
//    if (!this->media_ctx) {
//        return -2;
//    }

//    AVDictionary *options = NULL;
//    av_dict_set(&options, "flags2", "+export_mvs", 0);

    if (avcodec_open2(this->media_ctx, this->media_codec, NULL) < 0) {
        return -3;
    }

    // open analyzer
//    this->media_ctx->cyl_analyze_open = 1;
//    this->media_ctx->cyl_test(5);

    this->a_frame = av_frame_alloc();
    if (!this->a_frame) {
        return -4;
    }

    this->read_packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    av_init_packet(this->read_packet);


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

//        std::cout << this->read_packet->data << std::endl;
//        std::cout << this->read_packet->size << "," << this->read_packet->duration << std::endl;

        int rec_re = avcodec_send_packet(this->media_ctx, this->read_packet);

        while (avcodec_receive_frame(this->media_ctx, this->a_frame) == 0) {
            if (!this->fctx || !this->a_frame) {
                return -1;
            }
            std::cout << "loop" << std::endl;
        }

        std::cout << this->media_ctx->sample_rate << "," << this->read_packet->duration << std::endl;
        std::cout << this->media_ctx->frame_size << "," << this->media_ctx->frame_number << std::endl;
        // For audio, only linesize[0] may be set. For planar audio, each channel
        std::cout << this->read_packet->data << std::endl;
    } else {
        av_packet_unref(this->read_packet);
        return -2;
    }

    av_packet_unref(this->read_packet);
    return 0;
}

int FFmpegAnalyzer::close_media() {
    if (this->a_frame != nullptr) {
        av_frame_free(&this->a_frame);
        this->a_frame = nullptr;
    }

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







