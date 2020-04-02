//
// Created by 小老虎 on 2020/4/2.
//
#include "ffmpeg_analyzer.h"

FFmpegAnalyzer::FFmpegAnalyzer() {
}

FFmpegAnalyzer::~FFmpegAnalyzer() {
}

int FFmpegAnalyzer::open_264_media(const char *p_media_path) {
//    av_register_all();
    this->media_video_codec = avcodec_find_decoder(AV_CODEC_ID_H264);

    if (!this->media_video_codec) {
        printf("avcodec_find_decoder 初始化解码器失败\n");
        return -1;
    }

    const char *codec_name  = this->media_video_codec->name;
    this->media_video_ctx   = avcodec_alloc_context3(this->media_video_codec);

    if (!this->media_video_ctx) {
        return -2;
    }

    AVDictionary *options = NULL;
    av_dict_set(&options, "flags2", "+export_mvs", 0);

    if (avcodec_open2(this->media_video_ctx, this->media_video_codec, &options) < 0) {
        return -3;
    }

    // open analyzer
    this->media_video_ctx->cyl_analyze_open = 1;
    this->media_video_ctx->cyl_test(5);
//    this->media_video_ctx->cyl_analyzer_state_switch(this->media_video_ctx, 1);

    this->v_frame = av_frame_alloc();
    if (!this->v_frame) {
        return -4;
    }

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
            this->v_start_time = (this->read_packet->dts < this->read_packet->pts) ? this->read_packet->dts : this->read_packet->pts;

        }

        this->read_packet->dts -= this->v_start_time;
        this->read_packet->pts -= this->v_start_time;
        this->read_packet->pos = -1;

        int rec_re = avcodec_send_packet(this->media_video_ctx, this->read_packet);

        while (avcodec_receive_frame(this->media_video_ctx, this->v_frame) == 0) {
            if (!this->fctx || !this->v_frame || this->v_frame->linesize[0] <= 0) {
                return -1;
            }

//            // DEBUG
            CYLCodecAnalyzerLinkListNode *ptr = this->v_frame->cyl_analyzer_head;

            while (ptr != NULL) {
                if (ptr->mb_index >= 0) {
                    printf("%d-%hhu-%s | ", ptr->mb_index, ptr->qp_item, ptr->mb_desc);

//                    cout << "ptr->mb_desc == CYL_ANALYZE_MB_TYPE_IS_INTER? "  << ptr->mb_desc << "," << CYL_ANALYZE_MB_TYPE_IS_INTER << "====>" << (ptr->mb_desc == CYL_ANALYZE_MB_TYPE_IS_INTER) << endl;
//                    if (strcmp(ptr->mb_desc , CYL_ANALYZE_MB_TYPE_IS_INTER))
//                        printf("is CYL_ANALYZE_MB_TYPE_IS_INTER ");
//                        if (IS_INTER(ptr->mb_item_type))
//                            printf("is CYL_ANALYZE_MB_TYPE_IS_INTER ");
                }

                if (ptr->next == NULL) {
//                    printf("break\n");
//                    break;
                }
                ptr = ptr->next;
            } // end CYLCodecAnalyzerLinkListNode
            ptr = NULL;

            /**
             * @begin 运动矢量
             */
            int i;
            AVFrameSideData *sd;

            if (this->v_frame->key_frame != 1 && this->v_frame->pict_type != AV_PICTURE_TYPE_I) {
                printf("----------------\n");
                sd = av_frame_get_side_data(this->v_frame, AV_FRAME_DATA_MOTION_VECTORS);
                if (sd) {
                    printf("=================\n");
                    const AVMotionVector *mvs = (const AVMotionVector *)sd->data;
                    for (i = 0; i < sd->size / sizeof(*mvs); i++) {
                        const AVMotionVector *mv = &mvs[i];

                        if (mv->src_x != mv->dst_x || mv->src_y != mv->dst_y) {
                             printf("[MV:]%4d,%4d,%4d,%4d" PRIx64"\n",
                                 mv->src_x, mv->src_y,
                                 mv->dst_x, mv->dst_y);
                        }
                    }
                } // end sd
            } // end mv
        } // end while recv
    } else {
        av_packet_unref(this->read_packet);
        return -2;
    } // end read_res

    av_packet_unref(this->read_packet);
    return 0;
}

int FFmpegAnalyzer::close_media() {
    if (this->v_frame != nullptr) {
        av_frame_free(&this->v_frame);
        this->v_frame = nullptr;
    }

    if (nullptr != this->media_video_ctx) {
        avcodec_close(this->media_video_ctx);
        this->media_video_ctx = nullptr;
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

    this->media_video_codec = nullptr;
    this->v_start_time      = -1;


    return 0;
}







