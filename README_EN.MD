# FFmpeg-QuQi-Analyzer

<img src="doc_quqi/cylanalyzer@400x400.png" width="500" height="400" >

<br><br>

### Example：
```bash
cd doc_quqi
bash build.sh
cd build
./main ../veilside.h264
```

--------------------------------

LICENSE: 
- <a href="LICENSE.md">LICENSE(1) "Anti 996" License Version 1.0 (Draft)</a>
- <a href="FFMPEG_LICENSE.md">LICENSE(2)</a>
---------------------------------

## Pre
`AVCodecContext *avctx = ... ;`

### 1. Get QP:

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
<br>

### 2. Get Macro Block Type
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
<br>

### 3. Check MB Type   
#### include : `libavcodec/cyl2analyzer.h `  
(1) Func
> @Param    CYLCodecAnalyzerLinkListNode->mb_item_type            
> @return   bool     
>         
> IS_INTRA4x4(a)      
> IS_INTRA16x16(a)    
> IS_PCM(a)    
> IS_INTRA(a)    
> IS_INTER(a)    
> IS_SKIP(a)    
> IS_INTRA_PCM(a)    
> IS_INTERLACED(a)    
> IS_DIRECT(a)    
> IS_GMC(a)    
> IS_16X16(a)    
> IS_16X8(a)    
> IS_8X16(a)    
> IS_8X8(a)    
> IS_SUB_8X8(a)    
> IS_SUB_8X4(a)    
> IS_SUB_4X8(a)    
> IS_SUB_4X4(a)    
> IS_ACPRED(a)    
> IS_QUANT(a)    

(2) OR strcmp
> CYL_ANALYZE_MB_TYPE_IS_INTRA4x4     
> CYL_ANALYZE_MB_TYPE_IS_INTRA16x16   
> CYL_ANALYZE_MB_TYPE_IS_PCM          
> CYL_ANALYZE_MB_TYPE_IS_INTRA       
> CYL_ANALYZE_MB_TYPE_IS_INTER      
> CYL_ANALYZE_MB_TYPE_IS_SKIP       
> CYL_ANALYZE_MB_TYPE_IS_INTRA_PCM  
> CYL_ANALYZE_MB_TYPE_IS_INTERLACED
> CYL_ANALYZE_MB_TYPE_IS_DIRECT    
> CYL_ANALYZE_MB_TYPE_IS_GMC       
> CYL_ANALYZE_MB_TYPE_IS_16X16     
> CYL_ANALYZE_MB_TYPE_IS_16X8       
> CYL_ANALYZE_MB_TYPE_IS_8X16       
> CYL_ANALYZE_MB_TYPE_IS_8X8        
> CYL_ANALYZE_MB_TYPE_IS_SUB_8X8     
> CYL_ANALYZE_MB_TYPE_IS_SUB_8X4     
> CYL_ANALYZE_MB_TYPE_IS_SUB_4X8    
> CYL_ANALYZE_MB_TYPE_IS_SUB_4X4   
> CYL_ANALYZE_MB_TYPE_IS_ACPRED      
> CYL_ANALYZE_MB_TYPE_IS_QUANT       
> CYL_ANALYZE_MB_TYPE_UnKnow         
>

<br>

- #### Code
```c++
#include <libavcodec/cyl2analyzer.h>

...etc

// (1)
if (IS_INTER(ptr->mb_item_type)) {
    etc...
}

// (2)
if (strcmp(ptr->mb_desc , CYL_ANALYZE_MB_TYPE_IS_INTER)) {
    etc...
}

```

### 4. Get Motion Vector
```c++
AVDictionary *options = NULL;
av_dict_set(&options, "flags2", "+export_mvs", 0);

avcodec_open2(rtspVideoCodecContext, rtspVideoCodec, &options);
... etc

while (avcodec_receive_frame(rtspVideoCodecContext, v_frame) == 0) {
    ...etc

    AVFrameSideData *sd;
sd = av_frame_get_side_data(v_frame, AV_FRAME_DATA_MOTION_VECTORS);
    ...etc

    if (sd) {
        clear_mv_data();
        const AVMotionVector *mvs = (const AVMotionVector *)sd->data;
        for (i = 0; i < sd->size / sizeof(*mvs); i++) {
            const AVMotionVector *mv = &mvs[i];
            /*
                 mv->src_x, mv->src_y,
                 mv->dst_x, mv->dst_y
             */
        }
    }
   
}


```

-----------------------------------
porschegt23@foxmail.com
