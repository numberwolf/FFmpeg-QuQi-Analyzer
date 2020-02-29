# FFmpeg-QuQi-Analyzer

<img src="doc/cylanalyzer.png" width="500" height="400" >

--------------------------------

LICENSE: <a href="LICENSE.md">"Anti 996" License Version 1.0 (Draft)</a>

---------------------------------
### 1. Get QP:
`AVCodecContext *avctx;`
```c++
// analyzer open
avctx->cyl_analyzer_state_switch(avctx,1);

...etc

// decode
int ret = avcodec_send_packet(avctx, avpkt);

... etc

CYLCodecAnalyzerLinkListNode *ptr = avctx->cyl_analyzer_head;

while (ptr != NULL) {
    if (ptr->mb_index >= 0) {
        // Debug print Macro Index And QP
        printf("%d-%hhu | ",ptr->mb_index,ptr->qp_item);
    }

    if (ptr->next == NULL) {
        printf("break\n");
        break;
    }
    ptr = ptr->next;
}
ptr = NULL;
printf("\n");

```

2. Get Macro Block Type
```c++
// analyzer open
avctx->cyl_analyzer_state_switch(avctx,1);

...etc

// decode
int ret = avcodec_send_packet(avctx, avpkt);

... etc

CYLCodecAnalyzerLinkListNode *ptr = avctx->cyl_analyzer_head;

while (ptr != NULL) {
    if (ptr->mb_index >= 0) {
        // Debug print Macro Index And Macro Block Type
        printf("%d-%hhu-%s | ",ptr->mb_index,ptr->mb_item_type,ptr->mb_desc);
    }

    if (ptr->next == NULL) {
        printf("break\n");
        break;
    }
    ptr = ptr->next;
}
ptr = NULL;
printf("\n");

```

-----------------------------------
porschegt23@foxmail.com
