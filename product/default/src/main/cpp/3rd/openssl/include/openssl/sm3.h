
#ifndef XYSSL_SM3_H
#define XYSSL_SM3_H

#include <openssl/ossl_typ.h>
#include <openssl/gmssl.h>
#include <openssl/swsds.h>
#include <sys/types.h>
#include <stdint.h>
#define SM3_DIGEST_LENGTH   32
#define SM3_BLOCK_SIZE      64

typedef struct SM3_ss_state_st {
	SGD_HANDLE hSessionHandle;
	DEVICEINFO stDeviceInfo;
	uint32_t digest[8];
	int nblocks;
	unsigned char block[64];
	int num;
	GMSSL_ALG_ST gm_alg_info;
}SM3_SS_CTX;

/**
 * \brief          SM3 context structure
 */
typedef struct
{
    unsigned int state[8];     /*!< intermediate digest state  */
    unsigned int total[2];     /*!< number of bytes processed  */
    unsigned char buffer[64];   /*!< data block being processed */

    unsigned char ipad[64];     /*!< HMAC: inner padding        */
    unsigned char opad[64];     /*!< HMAC: outer padding        */

	SM3_SS_CTX gm_alg_info;

}
SM3_CTX;

#ifdef __cplusplus
extern "C" {
#endif

int  SM3_ss_Init(SM3_CTX *ctx);
int  SM3_ss_Update(SM3_CTX *ctx, void *data, size_t len);
int  SM3_ss_Final(unsigned char *md, SM3_CTX *ctx);
int  SM3_ss_Cleanup(SM3_CTX *ctx);
void SM3_ss_compress(uint32_t digest[8], const unsigned char block[SM3_BLOCK_SIZE]);


/**
 * \brief          SM3 context setup
 *
 * \param ctx      context to be initialized
 */
void sm3_starts( SM3_CTX *ctx, GMSSL_ALG_ST *gm_alg_info );
void sm3_cy_starts( SM3_CTX *ctx );

/**
 * \brief          SM3 process buffer
 *
 * \param ctx      SM3 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void sm3_update( SM3_CTX *ctx, unsigned char *input, int ilen );
void sm3_cy_update( SM3_CTX *ctx, unsigned char *input, int ilen );

/**
 * \brief          SM3 final digest
 *
 * \param ctx      SM3 context
 */
void sm3_finish( SM3_CTX *ctx, unsigned char *output);
void sm3_cy_finish( SM3_CTX *ctx, unsigned char *output);

/**
 * \brief          Output = SM3( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   SM3 checksum result
 */
void sm3( unsigned char *input, int ilen,
           unsigned char output[32]);

/**
 * \brief          Output = SM3( file contents )
 *
 * \param path     input file name
 * \param output   SM3 checksum result
 *
 * \return         0 if successful, 1 if fopen failed,
 *                 or 2 if fread failed
 */
int sm3_file( char *path, unsigned char output[32] );

/**
 * \brief          SM3 HMAC context setup
 *
 * \param ctx      HMAC context to be initialized
 * \param key      HMAC secret key
 * \param keylen   length of the HMAC key
 */

//void sm3_hmac_starts( SM3_CTX *ctx, unsigned char *key, int keylen);

/**
 * \brief          SM3 HMAC process buffer
 *
 * \param ctx      HMAC context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
//void sm3_hmac_update( SM3_CTX *ctx, unsigned char *input, int ilen );

/**
 * \brief          SM3 HMAC final digest
 *
 * \param ctx      HMAC context
 * \param output   SM3 HMAC checksum result
 */
//void sm3_hmac_finish( SM3_CTX *ctx, unsigned char output[32] );

/**
 * \brief          Output = HMAC-SM3( hmac key, input buffer )
 *
 * \param key      HMAC secret key
 * \param keylen   length of the HMAC key
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   HMAC-SM3 result
 */
//void sm3_hmac( unsigned char *key, int keylen,
 //               unsigned char *input, int ilen,
   //             unsigned char output[32] );



#ifdef __cplusplus
}
#endif

#endif /* sm3.h */
