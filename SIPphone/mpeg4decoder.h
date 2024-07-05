#ifndef MPEG4DECODER
#define MPEG4DECODER

#ifdef __cplusplus
extern "C"{
void initializeMpeg4();
char *getNextBufferRGBA();
void setVideoDecoderFile(char *);
}
#endif

#endif // MPEG4DECODER

