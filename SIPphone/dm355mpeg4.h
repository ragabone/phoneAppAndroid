#ifndef __PJMEDIA_CODEC_DM355MPEG4_H__
#define __PJMEDIA_CODEC_DM355MPEG4_H__

#include <pjmedia-codec/types.h>
#include <pjmedia/vid_codec.h>

PJ_BEGIN_DECL


PJ_DECL(pj_status_t) pjmedia_codec_dm355mpeg4_vid_init(pjmedia_vid_codec_mgr *mgr,
                                                     pj_pool_factory *pf);

PJ_DECL(pj_status_t) pjmedia_codec_dm355mpeg4_vid_deinit(void);


/**
 * Definition of dm355mpeg4 parameters.
 */
typedef struct pjmedia_vid_codec_dm355mpeg4_fmtp
{
    /* profile-level-id */
    pj_uint8_t	    profile_idc;    /**< Profile ID			    */
    pj_uint8_t	    profile_iop;    /**< Profile constraints bits	    */
    pj_uint8_t	    level;	    /**< Level				    */

    /* packetization-mode */
    pj_uint8_t	    packetization_mode;	/**< Packetization mode		    */

    /* max-mbps, max-fs, max-cpb, max-dpb, and max-br */
    unsigned	    max_mbps;	    /**< Max macroblock processing rate	    */
    unsigned	    max_fs;	    /**< Max frame size (in macroblocks)    */
    unsigned	    max_cpb;	    /**< Max coded picture buffer size	    */
    unsigned	    max_dpb;	    /**< Max decoded picture buffer size    */
    unsigned	    max_br;	    /**< Max video bit rate		    */

    /* sprop-parameter-sets, in NAL units */
    pj_size_t	    sprop_param_sets_len;   /**< Parameter set length	    */
    pj_uint8_t	    sprop_param_sets[256];  /**< Parameter set (SPS & PPS),
                                                 in NAL unit bitstream	    */

} pjmedia_vid_codec_dm355mpeg4_fmtp;


PJ_END_DECL

#endif	/* __PJMEDIA_CODEC_OPENH264_H__ */
