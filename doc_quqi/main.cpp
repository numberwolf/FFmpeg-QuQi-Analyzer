//
// Created by 小老虎 on 2020/4/2.
//

#include <iostream>
#include "ffmpeg_analyzer.h"

int main(int argc,char * argv[]) {
    if (argc < 2) {
        return -1;
    }

    FFmpegAnalyzer *analyzer = new FFmpegAnalyzer();

    int open_ret    = analyzer->open_264_media(argv[1]);
    std::cout << open_ret << std::endl;

    while (analyzer->get_frame() >= 0) {
        // todo
    }

    int close_ret   = analyzer->close_media();

    return 0;
}
