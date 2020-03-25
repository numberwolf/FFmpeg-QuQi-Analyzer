#!/bin/bash
gcc -o extract_mvs extract_mvs.c -I/usr/local/include -L/usr/local/lib -pthread -lavdevice -lavformat -lavfilter -lavcodec -ldl -lswscale -lavutil -lm

