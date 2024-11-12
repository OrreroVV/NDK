
#ifndef HEADER_GM_SM1_H
#define HEADER_GM_SM1_H

#include <openssl/e_os2.h>
#include <stddef.h>

#ifndef OPENSSL_NO_SANSEC_SM1
#include <openssl/gmssl.h>
#include <openssl/ossl_typ.h>
#include <openssl/swsds.h>
#endif

#define SM1_ENCRYPT     1
#define SM1_DECRYPT     0

#define SM1_KEY_LENGTH     16
#define SM1_BLOCK_SIZE     16
#define SM1_IV_LENGTH     16

#ifdef __cplusplus
extern "c" {
#endif

#ifndef OPENSSL_NO_SANSEC_SM1

typedef struct sm1_key_st {
    SGD_HANDLE hSessionHandle;
    DEVICEINFO stDeviceInfo;
    SGD_HANDLE hKeyHandle;
	GMSSL_ALG_ST gm_alg_info;
}SM1_KEY;

int  SM1_set_encrypt_key(const unsigned char *userKey, SM1_KEY *key, GMSSL_ALG_ST *gm_alg_info);
int  SM1_set_decrypt_key(const unsigned char *userKey, SM1_KEY *key, GMSSL_ALG_ST *gm_alg_info);
void SM1_encrypt(const unsigned char *in, unsigned char *out, const SM1_KEY *key);
void SM1_decrypt(const unsigned char *in, unsigned char *out, const SM1_KEY *key);
void SM1_ecb_encrypt(const unsigned char *in, unsigned char *out, 
		             const SM1_KEY *key, const int enc);
void SM1_cbc_encrypt(const unsigned char *in, unsigned char *out,
	                 size_t length, const SM1_KEY *key,
	                 unsigned char *ivec, const int enc);
void SM1_cfb128_encrypt(const unsigned char *in, unsigned char *out,
	size_t len, const SM1_KEY *key, unsigned char *ivec, int *num, int enc);
void SM1_ofb128_encrypt(const unsigned char *in, unsigned char *out,
	size_t len, const SM1_KEY *key, unsigned char *iv, int *num);
int  SM1_cleanup(SM1_KEY *key);
#endif

#ifdef __cplusplus
}
#endif
#endif
