#ifndef __PJMEDIA_DM355MPEG4_PACKETIZER_H__
#define __PJMEDIA_DM355MPEG4_PACKETIZER_H__

#include <pj/types.h>

PJ_BEGIN_DECL

typedef struct pjmedia_dm355mpeg4_packetizer pjmedia_dm355mpeg4_packetizer;


PJ_DECL(pj_status_t) pjmedia_dm355mpeg4_packetizer_create(pj_pool_t *pool);


/**
 * Generate an RTP payload from a dm355 mpeg4 picture bitstream. Note that this
 * function will apply in-place processing, so the bitstream may be modified
 * during the packetization.
 *
 * @param pktz		The packetizer.
 * @param bits		The picture bitstream to be packetized.
 * @param bits_len	The length of the bitstream.
 * @param bits_pos	The bitstream offset to be packetized.
 * @param payload	The output payload.
 * @param payload_len	The output payload length.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjmedia_dm355mpeg4_packetize(void *pktz,
					    pj_uint8_t *bits,
                                            pj_size_t bits_len,
                                            unsigned *bits_pos,
                                            const pj_uint8_t **payload,
                                            pj_size_t *payload_len);


/**
 * Append an RTP payload to an dm355 mpeg4 picture bitstream. Note that in case of
 * noticing packet lost, application should keep calling this function with
 * payload pointer set to NULL, as the packetizer need to update its internal
 * state.
 *
 * @param pktz		The packetizer.
 * @param payload	The payload to be unpacketized.
 * @param payload_len	The payload length.
 * @param bits		The bitstream buffer.
 * @param bits_size	The bitstream buffer size.
 * @param bits_pos	The bitstream offset to put the unpacketized payload
 *			in the bitstream, upon return, this will be updated
 *			to the latest offset as a result of the unpacketized
 *			payload.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjmedia_dm355mpeg4_unpacketize(void *pktz,
					      const pj_uint8_t *payload,
                                              pj_size_t   payload_len,
                                              pj_uint8_t *bits,
                                              pj_size_t   bits_len,
					      unsigned   *bits_pos);


PJ_END_DECL

#endif	/* __PJMEDIA_DM355MPEG4_PACKETIZER_H__ */
