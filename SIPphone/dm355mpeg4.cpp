/* $Id: dm355mpeg4.cpp 5045 2015-04-06 06:13:51Z nanang $ */
/* 
 * Copyright (C)2014 Teluu Inc. (http://www.teluu.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */
#include "dm355mpeg4.h"
#include "dm355mpeg4_packetizer.h"
#include <pjmedia/stream_common.h>
#include <pjmedia/vid_codec_util.h>
#include <pjmedia/errno.h>
#include <pj/log.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <QDebug>
#include <qstandardpaths.h>


#if defined(PJMEDIA_HAS_VIDEO) && (PJMEDIA_HAS_VIDEO != 0)

static int writeVideoBuf(void *data,int size);
static void udpServerThreadFxn(char *buffer,int size);
/* dm355mpeg4: */
//#include <wels/codec_api.h>
//#include <wels/codec_app_def.h>

/*
 * Constants
 */
#define THIS_FILE		"dm355mpeg4.cpp"


#  define DEFAULT_WIDTH		480
#  define DEFAULT_HEIGHT	272

#define DEFAULT_FPS		15
#define DEFAULT_AVG_BITRATE	256000
#define DEFAULT_MAX_BITRATE	256000

#define MAX_RX_WIDTH		1200
#define MAX_RX_HEIGHT		800


FILE * GBLfp;

PJ_DEF(pj_status_t) pjmedia_vid_codec_mpeg4_match_sdp(pj_pool_t *pool,
                                                     pjmedia_sdp_media *offer,
                                                     unsigned o_fmt_idx,
                                                     pjmedia_sdp_media *answer,
                                                     unsigned a_fmt_idx,
                                                     unsigned option);

/*
 * Factory operations.
 */
static pj_status_t dm355mpeg4_test_alloc(pjmedia_vid_codec_factory *factory,
                                    const pjmedia_vid_codec_info *info );
static pj_status_t dm355mpeg4_default_attr(pjmedia_vid_codec_factory *factory,
                                      const pjmedia_vid_codec_info *info,
                                      pjmedia_vid_codec_param *attr );
static pj_status_t dm355mpeg4_enum_info(pjmedia_vid_codec_factory *factory,
                                   unsigned *count,
                                   pjmedia_vid_codec_info codecs[]);
static pj_status_t dm355mpeg4_alloc_codec(pjmedia_vid_codec_factory *factory,
                                     const pjmedia_vid_codec_info *info,
                                     pjmedia_vid_codec **p_codec);
static pj_status_t dm355mpeg4_dealloc_codec(pjmedia_vid_codec_factory *factory,
                                       pjmedia_vid_codec *codec );


/*
 * Codec operations
 */
static pj_status_t dm355mpeg4_codec_init(pjmedia_vid_codec *codec,
                                    pj_pool_t *pool );
static pj_status_t dm355mpeg4_codec_open(pjmedia_vid_codec *codec,
                                    pjmedia_vid_codec_param *param );
static pj_status_t dm355mpeg4_codec_close(pjmedia_vid_codec *codec);
static pj_status_t dm355mpeg4_codec_modify(pjmedia_vid_codec *codec,
                                      const pjmedia_vid_codec_param *param);
static pj_status_t dm355mpeg4_codec_get_param(pjmedia_vid_codec *codec,
                                         pjmedia_vid_codec_param *param);
static pj_status_t dm355mpeg4_codec_encode_begin(pjmedia_vid_codec *codec,
                                            const pjmedia_vid_encode_opt *opt,
                                            const pjmedia_frame *input,
                                            unsigned out_size,
                                            pjmedia_frame *output,
                                            pj_bool_t *has_more);
static pj_status_t dm355mpeg4_codec_encode_more(pjmedia_vid_codec *codec,
                                           unsigned out_size,
                                           pjmedia_frame *output,
                                           pj_bool_t *has_more);
static pj_status_t dm355mpeg4_codec_decode(pjmedia_vid_codec *codec,
                                      pj_size_t count,
                                      pjmedia_frame packets[],
                                      unsigned out_size,
                                      pjmedia_frame *output);

/* Definition for dm355mpeg4 codecs operations. */
static pjmedia_vid_codec_op dm355mpeg4_codec_op =
{
    &dm355mpeg4_codec_init,
    &dm355mpeg4_codec_open,
    &dm355mpeg4_codec_close,
    &dm355mpeg4_codec_modify,
    &dm355mpeg4_codec_get_param,
    &dm355mpeg4_codec_encode_begin,
    &dm355mpeg4_codec_encode_more,
    &dm355mpeg4_codec_decode,
    NULL
};

/* Definition for dm355mpeg4 codecs factory operations. */
static pjmedia_vid_codec_factory_op dm355mpeg4_factory_op =
{
    &dm355mpeg4_test_alloc,
    &dm355mpeg4_default_attr,
    &dm355mpeg4_enum_info,
    &dm355mpeg4_alloc_codec,
    &dm355mpeg4_dealloc_codec
};


static struct dm355mpeg4_factory
{
    pjmedia_vid_codec_factory    base;
    pjmedia_vid_codec_mgr	*mgr;
    pj_pool_factory             *pf;
    pj_pool_t		        *pool;
} dm355mpeg4_factory;


typedef struct dm355mpeg4_codec_data
{
    pj_pool_t			*pool;
    pjmedia_vid_codec_param	*prm;
    pj_bool_t			 whole;

} dm355mpeg4_codec_data;




PJ_DEF(pj_status_t) pjmedia_codec_dm355mpeg4_vid_init(pjmedia_vid_codec_mgr *mgr,
                                                    pj_pool_factory *pf)
{
    const pj_str_t mpeg4_name = { (char*)"MP4V-ES", 4};
    pj_status_t status;


    PJ_LOG(4,(THIS_FILE, "dm355mpeg4 codec initializing"));

    if (dm355mpeg4_factory.pool != NULL) {
	/* Already initialized. */
	return PJ_SUCCESS;
    }

    if (!mgr) mgr = pjmedia_vid_codec_mgr_instance();
    PJ_ASSERT_RETURN(mgr, PJ_EINVAL);

    /* Create dm355mpeg4 codec factory. */
    dm355mpeg4_factory.base.op = &dm355mpeg4_factory_op;
    dm355mpeg4_factory.base.factory_data = NULL;
    dm355mpeg4_factory.mgr = mgr;
    dm355mpeg4_factory.pf = pf;
    dm355mpeg4_factory.pool = pj_pool_create(pf, "dm355mpeg4factory", 256, 256, NULL);
    if (!dm355mpeg4_factory.pool)
	return PJ_ENOMEM;

    /* Registering format match for SDP negotiation */
    status = pjmedia_sdp_neg_register_fmt_match_cb(
                                        &mpeg4_name,
                                        &pjmedia_vid_codec_mpeg4_match_sdp);
    if (status != PJ_SUCCESS)
	goto on_error;

    /* Register codec factory to codec manager. */
    status = pjmedia_vid_codec_mgr_register_factory(mgr,
                                                    &dm355mpeg4_factory.base);
    if (status != PJ_SUCCESS)
	goto on_error;

    //GBLfp = fopen (MyNamedPipe, "a");

        PJ_LOG(4,(THIS_FILE, "dm355mpeg4 codec initialized "));

    /* Done. */
    return PJ_SUCCESS;

on_error:
    pj_pool_release(dm355mpeg4_factory.pool);
    dm355mpeg4_factory.pool = NULL;
    return status;
}



/* H264 fmtp parser */
PJ_DEF(pj_status_t) pjmedia_vid_codec_mpeg4_parse_fmtp(
                                    const pjmedia_codec_fmtp *fmtp,
                                    pjmedia_vid_codec_dm355mpeg4_fmtp *mpeg4_fmtp)
{
    const pj_str_t PROFILE_LEVEL_ID	= {"profile-level-id",1};
    const pj_str_t MAX_MBPS		= {"max-mbps", 8};
    const pj_str_t MAX_FS		= {"max-fs", 6};
    const pj_str_t MAX_CPB		= {"max-cpb", 7};
    const pj_str_t MAX_DPB	    	= {"max-dpb", 7};
    const pj_str_t MAX_BR		= {"max-br", 6};
    const pj_str_t PACKETIZATION_MODE	= {"packetization-mode", 18};

    unsigned i;
    pj_status_t status;

    pj_bzero(mpeg4_fmtp, sizeof(*mpeg4_fmtp));

    for (i=0; i<fmtp->cnt; ++i) {
        unsigned tmp;
        if (pj_stricmp(&fmtp->param[i].name, &PROFILE_LEVEL_ID)==0) {

        } else if (pj_stricmp(&fmtp->param[i].name, &PACKETIZATION_MODE)==0) {

        } else if (pj_stricmp(&fmtp->param[i].name, &MAX_MBPS)==0) {

        } else if (pj_stricmp(&fmtp->param[i].name, &MAX_FS)==0) {

        } else if (pj_stricmp(&fmtp->param[i].name, &MAX_CPB)==0) {

        } else if (pj_stricmp(&fmtp->param[i].name, &MAX_DPB)==0) {

        } else if (pj_stricmp(&fmtp->param[i].name, &MAX_BR)==0) {

        }
    }


    return PJ_SUCCESS;
}


PJ_DEF(pj_status_t) pjmedia_vid_codec_mpeg4_match_sdp(pj_pool_t *pool,
                                                     pjmedia_sdp_media *offer,
                                                     unsigned o_fmt_idx,
                                                     pjmedia_sdp_media *answer,
                                                     unsigned a_fmt_idx,
                                                     unsigned option)
{
    const pj_str_t PROFILE_LEVEL_ID	= {"profile-level-id", 1};
    const pj_str_t PACKETIZATION_MODE	= {"packetization-mode", 18};
    pjmedia_codec_fmtp o_fmtp_raw, a_fmtp_raw;
    pjmedia_vid_codec_dm355mpeg4_fmtp o_fmtp, a_fmtp;
    pj_status_t status;

    PJ_UNUSED_ARG(pool);

    /* Parse offer */
    status = pjmedia_stream_info_parse_fmtp(
                                    NULL, offer,
                                    pj_strtoul(&offer->desc.fmt[o_fmt_idx]),
                                    &o_fmtp_raw);
    if (status != PJ_SUCCESS)
        return status;

//    status = pjmedia_vid_codec_h264_parse_fmtp(&o_fmtp_raw, &o_fmtp);
//    if (status != PJ_SUCCESS)
//        return status;

    /* Parse answer */
    status = pjmedia_stream_info_parse_fmtp(
                                    NULL, answer,
                                    pj_strtoul(&answer->desc.fmt[a_fmt_idx]),
                                    &a_fmtp_raw);
    if (status != PJ_SUCCESS)
        return status;

    status = pjmedia_vid_codec_mpeg4_parse_fmtp(&a_fmtp_raw, &a_fmtp);
    if (status != PJ_SUCCESS)
        return status;

    if (a_fmtp.profile_idc != o_fmtp.profile_idc)
    {
        return PJMEDIA_SDP_EFORMATNOTEQUAL;
    }

    /* Update the answer */
//    for (i = 0; i < a_fmtp_raw.cnt; ++i) {
//        if (pj_stricmp(&a_fmtp_raw.param[i].name, &PROFILE_LEVEL_ID) == 0)
//        {
//            char *p = a_fmtp_raw.param[i].val.ptr;
//            pj_val_to_hex_digit(a_fmtp.profile_idc, p);
//            p += 2;
//            pj_val_to_hex_digit(a_fmtp.profile_iop, p);
//            p += 2;
//            pj_val_to_hex_digit(a_fmtp.level, p);
//        }
//        else if (pj_stricmp(&a_fmtp_raw.param[i].name, &PACKETIZATION_MODE) == 0)
//        {
//            char *p = a_fmtp_raw.param[i].val.ptr;
//            *p = '0' + a_fmtp.packetization_mode;
//        }
//    }


    return PJ_SUCCESS;
}

/*
 * Unregister dm355mpeg4 codecs factory from pjmedia endpoint.
 */
PJ_DEF(pj_status_t) pjmedia_codec_dm355mpeg4_vid_deinit(void)
{
    pj_status_t status = PJ_SUCCESS;

    if (dm355mpeg4_factory.pool == NULL) {
	/* Already deinitialized */
	return PJ_SUCCESS;
    }

    /* Unregister dm355mpeg4 codecs factory. */
    status = pjmedia_vid_codec_mgr_unregister_factory(dm355mpeg4_factory.mgr,
                                                      &dm355mpeg4_factory.base);

    /* Destroy pool. */
    pj_pool_release(dm355mpeg4_factory.pool);
    dm355mpeg4_factory.pool = NULL;

    return status;
}

static pj_status_t dm355mpeg4_test_alloc(pjmedia_vid_codec_factory *factory,
                                    const pjmedia_vid_codec_info *info )
{
    PJ_ASSERT_RETURN(factory == &dm355mpeg4_factory.base, PJ_EINVAL);

    PJ_LOG(5,(THIS_FILE, "dm355mpeg4_test_alloc MPEG4 codec.."));

    return PJ_SUCCESS;
}

static pj_status_t dm355mpeg4_default_attr(pjmedia_vid_codec_factory *factory,
                                      const pjmedia_vid_codec_info *info,
                                      pjmedia_vid_codec_param *attr )
{
    PJ_ASSERT_RETURN(factory == &dm355mpeg4_factory.base, PJ_EINVAL);
    PJ_ASSERT_RETURN(info && attr, PJ_EINVAL);

    pj_bzero(attr, sizeof(pjmedia_vid_codec_param));

    attr->dir = PJMEDIA_DIR_ENCODING_DECODING;
    attr->packing = PJMEDIA_VID_PACKING_PACKETS;

    PJ_LOG(5,(THIS_FILE, "dm355mpeg4_default_attr MPEG4 codec.."));

    /* Encoded format */
    pjmedia_format_init_video(&attr->enc_fmt, PJMEDIA_FORMAT_MPEG4,
                              DEFAULT_WIDTH, DEFAULT_HEIGHT,
			      DEFAULT_FPS, 1);

    /* Decoded format */
    pjmedia_format_init_video(&attr->dec_fmt, PJMEDIA_FORMAT_I420,
                              DEFAULT_WIDTH, DEFAULT_HEIGHT,
			      DEFAULT_FPS, 1);

    /* Decoding fmtp */
//    attr->dec_fmtp.cnt = 2;
//    attr->dec_fmtp.param[0].name = pj_str((char*)"profile-level-id");
//    attr->dec_fmtp.param[0].val = pj_str((char*)"42e01e");
//    attr->dec_fmtp.param[1].name = pj_str((char*)" packetization-mode");
//    attr->dec_fmtp.param[1].val = pj_str((char*)"1");



    /* Bitrate */
    attr->enc_fmt.det.vid.avg_bps = DEFAULT_AVG_BITRATE;
    attr->enc_fmt.det.vid.max_bps = DEFAULT_MAX_BITRATE;



    /* Encoding MTU */
    attr->enc_mtu = PJMEDIA_MAX_VID_PAYLOAD_SIZE;

    return PJ_SUCCESS;
}

static pj_status_t dm355mpeg4_enum_info(pjmedia_vid_codec_factory *factory,
                                   unsigned *count,
                                   pjmedia_vid_codec_info info[])
{
    PJ_ASSERT_RETURN(info && *count > 0, PJ_EINVAL);
    PJ_ASSERT_RETURN(factory == &dm355mpeg4_factory.base, PJ_EINVAL);

    PJ_LOG(5,(THIS_FILE, "dm355mpeg4_enum_info MPEG4 codec.."));
    *count = 1;
    info->fmt_id = PJMEDIA_FORMAT_MPEG4;
    info->pt = 0;

    info->encoding_name = pj_str((char*)"MP4V-ES");
    info->encoding_desc = pj_str((char*)"dm355mpeg4 codec");
    info->clock_rate = 90000;
    info->dir = PJMEDIA_DIR_ENCODING_DECODING;
    info->dec_fmt_id_cnt = 1;
    info->dec_fmt_id[0] = PJMEDIA_FORMAT_I420;
    info->packings = PJMEDIA_VID_PACKING_PACKETS |
		     PJMEDIA_VID_PACKING_WHOLE;
    info->fps_cnt = 3;
    info->fps[0].num = 15;
    info->fps[0].denum = 1;
    info->fps[1].num = 25;
    info->fps[1].denum = 1;
    info->fps[2].num = 30;
    info->fps[2].denum = 1;

    return PJ_SUCCESS;

}

static pj_status_t dm355mpeg4_alloc_codec(pjmedia_vid_codec_factory *factory,
                                     const pjmedia_vid_codec_info *info,
                                     pjmedia_vid_codec **p_codec)
{
    pj_pool_t *pool;
    pjmedia_vid_codec *codec;
    int rc;

    PJ_ASSERT_RETURN(factory == &dm355mpeg4_factory.base && info && p_codec,
                     PJ_EINVAL);

    *p_codec = NULL;
    PJ_LOG(5,(THIS_FILE, "dm355mpeg4_alloc_codec MP4V codec.."));
    pool = pj_pool_create(dm355mpeg4_factory.pf, "dm355mpeg4%p", 512, 512, NULL);
    if (!pool)
	return PJ_ENOMEM;

    /* codec instance */
    codec = PJ_POOL_ZALLOC_T(pool, pjmedia_vid_codec);
    codec->factory = factory;
    codec->op = &dm355mpeg4_codec_op;

    /* codec data */
//    dm355mpeg4_data = PJ_POOL_ZALLOC_T(pool, dm355mpeg4_codec_data);
//    dm355mpeg4_data->pool = pool;
//    codec->codec_data = dm355mpeg4_data;

    /* encoder allocation */
//    rc = WelsCreateSVCEncoder(&dm355mpeg4_data->enc);
//    if (rc != 0)
//	goto on_error;

//    dm355mpeg4_data->esrc_pic = PJ_POOL_ZALLOC_T(pool, SSourcePicture);

//    /* decoder allocation */
//    rc = WelsCreateDecoder(&dm355mpeg4_data->dec);
//    if (rc != 0)
//	goto on_error;

    *p_codec = codec;
    return PJ_SUCCESS;

on_error:
    dm355mpeg4_dealloc_codec(factory, codec);
    return PJMEDIA_CODEC_EFAILED;
}

static pj_status_t dm355mpeg4_dealloc_codec(pjmedia_vid_codec_factory *factory,
                                       pjmedia_vid_codec *codec )
{

    PJ_ASSERT_RETURN(codec, PJ_EINVAL);

    PJ_UNUSED_ARG(factory);
    PJ_LOG(5,(THIS_FILE, "dm355mpeg4_dealloc_codec MPEG4 codec.."));

    return PJ_SUCCESS;
}

static pj_status_t dm355mpeg4_codec_init(pjmedia_vid_codec *codec,
                                    pj_pool_t *pool )
{
    PJ_ASSERT_RETURN(codec && pool, PJ_EINVAL);
    PJ_UNUSED_ARG(codec);
    PJ_UNUSED_ARG(pool);
    PJ_LOG(5,(THIS_FILE, "dm355mpeg4_codec_init MPEG4 codec.."));
    return PJ_SUCCESS;
}

static pj_status_t dm355mpeg4_codec_open(pjmedia_vid_codec *codec,
                                    pjmedia_vid_codec_param *codec_param )
{
    int 		 rc;
    pj_status_t		 status;

    PJ_ASSERT_RETURN(codec && codec_param, PJ_EINVAL);

    PJ_LOG(5,(THIS_FILE, "Opening MPEG4 codec.."));


    return PJ_SUCCESS;
}

static pj_status_t dm355mpeg4_codec_close(pjmedia_vid_codec *codec)
{
    PJ_ASSERT_RETURN(codec, PJ_EINVAL);
    PJ_UNUSED_ARG(codec);
    PJ_LOG(5,(THIS_FILE, "dm355mpeg4_codec_close MPEG4 codec.."));
    return PJ_SUCCESS;
}

static pj_status_t dm355mpeg4_codec_modify(pjmedia_vid_codec *codec,
                                      const pjmedia_vid_codec_param *param)
{
    PJ_ASSERT_RETURN(codec && param, PJ_EINVAL);
    PJ_UNUSED_ARG(codec);
    PJ_UNUSED_ARG(param);
    PJ_LOG(5,(THIS_FILE, "Modify MPEG4 codec.."));
    return PJ_EINVALIDOP;
}

static pj_status_t dm355mpeg4_codec_get_param(pjmedia_vid_codec *codec,
                                         pjmedia_vid_codec_param *param)
{
    struct dm355mpeg4_codec_data *dm355mpeg4_data;

    PJ_ASSERT_RETURN(codec && param, PJ_EINVAL);

    PJ_LOG(5,(THIS_FILE, "Get Param MPEG4 codec.."));
    return PJ_SUCCESS;
}

static pj_status_t dm355mpeg4_codec_encode_begin(pjmedia_vid_codec *codec,
                                            const pjmedia_vid_encode_opt *opt,
                                            const pjmedia_frame *input,
                                            unsigned out_size,
                                            pjmedia_frame *output,
                                            pj_bool_t *has_more)
{
    int rc;

    PJ_LOG(5,(THIS_FILE, "Encode Begin MPEG4 codec.."));

    return PJ_SUCCESS;



}


static pj_status_t dm355mpeg4_codec_encode_more(pjmedia_vid_codec *codec,
                                           unsigned out_size,
                                           pjmedia_frame *output,
                                           pj_bool_t *has_more)
{
    PJ_LOG(5,(THIS_FILE, "dm355mpeg4_codec_encode_more MPEG4 codec.."));
    return PJ_SUCCESS;
}



static pj_status_t dm355mpeg4_codec_decode(pjmedia_vid_codec *codec,
                                      pj_size_t count,
                                      pjmedia_frame packets[],
                                      unsigned out_size,
                                      pjmedia_frame *output)
{

    pj_status_t status = PJ_SUCCESS;

    PJ_ASSERT_RETURN(codec && count && packets && out_size && output,
                     PJ_EINVAL);
    PJ_ASSERT_RETURN(output->buf, PJ_EINVAL);

     PJ_LOG(5,(THIS_FILE, "dm355mpeg4_codec_encode_more MPEG4 codec.."));

    if(count){

        for(int i=0;i<count;i++)
            writeVideoBuf(packets[i].buf,packets[i].size);

    }
    return status;
}
//static int writeVideoBuf(void *data,int size){


//    PJ_LOG(5,(THIS_FILE, "writeVideoBuf %d",size));

//   udpServerThreadFxn((char*)data,size);


//}
static int fdFifo;
extern char *fifoPath;
static int writeVideoBuf(void *data,int size){

    int retSize;
    //PJ_LOG(5,(THIS_FILE, "writeVideoBuf %d",size));

    fdFifo = open(fifoPath, O_RDWR);

    retSize=write(fdFifo,data,size);
    //fwrite(data , 1 , size , GBLfp );
    //fclose(fp);
    //close(fdFifo);


}

static int Videofd = NULL;

void udpServerThreadFxn(char *buffer,int size)
{
    const int port = 7327;
    struct sockaddr_in addr;
    int bytesSent;
    int s = socket(AF_INET, SOCK_DGRAM, 0);

    //Videofd = open("/home/dm355/test2.m4v",O_RDONLY);
    //printf("Videofd is %d\n", Videofd);


    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port        = htons(port);

    bytesSent = sendto(s, buffer, size, 0L, (struct sockaddr*)&addr, sizeof(addr));
    //printf("Sent a UDP packet... send() returned %i\n", bytesSent);
   //fflush(stdout);
    close(s);
}

#endif	/* PJMEDIA_HAS_dm355mpeg4_CODEC */
