#include <stdio.h>
#include <stdint.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>

#include <libavutil/pixfmt.h>
#include "mpeg4decoder.h"

#define INBUF_SIZE 4096

#define OUTPUTFORMAT AV_PIX_FMT_RGB24

char streamName[100];

static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
                     char *filename);

AVCodecContext *initialize_codec(uint8_t *inbuf);
int getSample(char *inBuf);
int decode_frame( AVCodecContext *avctx, AVPacket *pkt);
void freeAll(AVCodecContext *c);
void write_frame(AVCodecContext *ctx);
static void video_decode();

static AVPacket avpkt;
static AVFrame *pFrame;
static AVFrame *pFrameRGB;
static AVCodecContext *avctx;
static AVFormatContext *pFormatCtx;

static int sampleSrcInitialized=0;
static FILE *file;
static uint8_t inbuf[INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];

void setVideoDecoderFile(char *name){
    strcpy(streamName,name);
}

static void video_decode(){
    int sampleSize;

    avctx=initialize_codec(inbuf);
    sampleSize=getSample(inbuf);
    while(sampleSize){
        if( decode_frame(avctx,&avpkt)){
            uint8_t *out[3];
            int linesizeOut[1];
            linesizeOut[0]=480*272*4;
            //            out[0]=RGBA_BUFFER;
            //           struct SwsContext *img_convert_ctx = sws_getContext(avctx->width, avctx->height, avctx->pix_fmt, avctx->width, avctx->height, AV_PIX_FMT_RGBA, SWS_BICUBIC, NULL, NULL, NULL);
            //           sws_scale(img_convert_ctx, frame->data, frame->linesize, 0, avctx->height, out, linesizeOut);

            struct SwsContext * img_convert_ctx;
            img_convert_ctx = sws_getCachedContext(NULL,avctx->width, avctx->height, avctx->pix_fmt, avctx->width, avctx->height, OUTPUTFORMAT, SWS_BICUBIC, NULL, NULL,NULL);
            int heightImage = sws_scale(img_convert_ctx, ((AVPicture*)pFrame)->data, ((AVPicture*)pFrame)->linesize, 0, avctx->height, ((AVPicture *)pFrameRGB)->data, ((AVPicture *)pFrameRGB)->linesize);



            write_frame(img_convert_ctx);
        }
        else{
            printf("Waiting for frame\n");
        }
        sampleSize=getSample(inbuf);

    }
    freeAll(avctx);
}

void initializeMpeg4(){

    avctx=initialize_codec(inbuf);
    av_read_frame(pFormatCtx, &avpkt);
    //getSample(inbuf);

}


char *getNextBufferRGBA(){
    int sampleSize;
    //sampleSize=getSample(inbuf);
    //if(sampleSize){
    while(1){
        if( decode_frame(avctx,&avpkt)){
            uint8_t *out[3];
            int linesizeOut[1];
            linesizeOut[0]=480*272*3;
            //            out[0]=RGBA_BUFFER;
            //           struct SwsContext *img_convert_ctx = sws_getContext(avctx->width, avctx->height, avctx->pix_fmt, avctx->width, avctx->height, AV_PIX_FMT_RGBA, SWS_BICUBIC, NULL, NULL, NULL);
            //           sws_scale(img_convert_ctx, frame->data, frame->linesize, 0, avctx->height, out, linesizeOut);

            struct SwsContext * img_convert_ctx;
            img_convert_ctx = sws_getCachedContext(NULL,avctx->width, avctx->height, avctx->pix_fmt, avctx->width, avctx->height, OUTPUTFORMAT, SWS_BICUBIC, NULL, NULL,NULL);
            int heightImage = sws_scale(img_convert_ctx, ((AVPicture*)pFrame)->data, ((AVPicture*)pFrame)->linesize, 0, avctx->height, ((AVPicture *)pFrameRGB)->data, ((AVPicture *)pFrameRGB)->linesize);

            // write_frame(img_convert_ctx);
            return pFrameRGB->data[0];

        }
        else{
            //printf("Waiting for frame\n");
            //return 0;
            //getSample(inbuf);
            av_read_frame(pFormatCtx, &avpkt);
        }
    }

    //     }
    //     else return -1;

}

AVCodecContext *initialize_codec(uint8_t *inbuf){

    AVCodec *codec;
    AVCodecContext *pCodecCtx= NULL;
    //extern AVCodec ff_mpeg4_decoder;
    //avcodec_register(&ff_mpeg4_decoder);
    AVDictionary *d = NULL;
    av_dict_set(&d, "protocol_whitelist", "file,udp,rtp", 0);
    //ret = avformat_open_input(&inFormatCtx, filename, NULL, &d);
    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    /// Open video file
    /// file:///home/james/Programs/SIPphone/vlc.sdp
    //if(avformat_open_input(&pFormatCtx, "/home/james/Programs/EDoorWatch/test.m4v", 0, NULL) != 0)
    if(avformat_open_input(&pFormatCtx, streamName, 0, &d) != 0)
    {
        printf("avformat_open_input failed: Couldn't open file\n");
        return -1; // Couldn't open file
    }


    av_init_packet(&avpkt);
    /* set end of buffer to 0 (this ensures that no overreading happens for damaged mpeg streams) */
    memset(inbuf + INBUF_SIZE, 0, FF_INPUT_BUFFER_PADDING_SIZE);
    //printf("Decode video file %s to %s\n", filename, outfilename);
    /* find the mpeg4 video decoder */
    codec = avcodec_find_decoder(AV_CODEC_ID_MPEG4);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }
    pCodecCtx = avcodec_alloc_context3(codec);
    if (!pCodecCtx) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }
    if(codec->capabilities&CODEC_CAP_TRUNCATED)
        pCodecCtx->flags|= CODEC_FLAG_TRUNCATED; /* we do not send complete frames */

    pCodecCtx->width = 480;
    pCodecCtx->height = 272;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    /* For some codecs, such as msmpeg4 and mpeg4, width and height
       MUST be initialized there because this information is not
       available in the bitstream. */
    /* open it */
    if (avcodec_open2(pCodecCtx, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    pFrame = av_frame_alloc();
    if (!pFrame) {
        fprintf(stderr, "Could not allocate video pFrame\n");
        exit(1);
    }
    pFrame->width  = pCodecCtx->width;
    pFrame->height = pCodecCtx->height;
    pFrame->linesize[0] = pFrame->width*2;



    pFrameRGB = av_frame_alloc();

    uint8_t *buffer;
    int numBytes;

    int  pFormat = OUTPUTFORMAT;
    numBytes = avpicture_get_size(pFormat,pCodecCtx->width,pCodecCtx->height) ; //OUTPUTFORMAT
    buffer = (uint8_t *) av_malloc(numBytes*sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB,buffer,pFormat,pCodecCtx->width,pCodecCtx->height);


    return pCodecCtx;
}


int getSample(char *inBuf){
    if(!sampleSrcInitialized){
        initSampleSrc();
        sampleSrcInitialized = 1;
    }
    avpkt.size = fread(inBuf, 1, INBUF_SIZE, file);
    if(avpkt.size==0)
        return 0;

    avpkt.data = inBuf;


    return avpkt.size;
}
void initSampleSrc(){
    file = fopen("/home/james/Programs/EDoorWatch/test.m4v", "rb");
    if (!file) {
        fprintf(stderr, "Could not open file\n");
        exit(1);
    }


}



void freeAll(AVCodecContext *c){
    avpkt.data = NULL;
    avpkt.size = 0;
    //decode_write_frame(outfilename, c, frame, &frame_count, &avpkt, 1); //decode flush last frame
    fclose(file);
    avcodec_close(c);
    av_free(c);
    av_frame_free(&pFrame);

    printf("\n");
}



int decode_frame( AVCodecContext *avctx, AVPacket *pkt){
    int len, got_frame;

    len = avcodec_decode_video2(avctx, pFrame, &got_frame, pkt);
    if (len < 0) {
        fprintf(stderr, "Error while decoding frame \n");
        return len;
    }

    if (pkt->data) {
        pkt->size -= len;
        pkt->data += len;
    }

    return got_frame;

}

void write_frame(AVCodecContext *ctx){
    char buf[1024];
    char outfilename[]= "decodeRaw.rgb";
    printf("Saving frame \n");
    fflush(stdout);
    snprintf(buf, sizeof(buf), outfilename, 1);
    pgm_save(pFrameRGB->data[0], pFrameRGB->linesize[0], 480*4, 272, buf);


}



static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
                     char *filename)
{
    FILE *f;
    int i;
    f=fopen(filename,"w");
    //fprintf(f,"P5\n%d %d\n%d\n",xsize,ysize,255);
    for(i=0;i<ysize;i++)
        fwrite(buf + i * wrap,1,xsize,f);
    fclose(f);
}
