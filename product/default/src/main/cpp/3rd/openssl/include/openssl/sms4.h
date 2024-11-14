
#ifndef HEADER_GM_SMS4_H
#define HEADER_GM_SMS4_H

#include <openssl/e_os2.h>
#include <openssl/gmssl.h>
#include <openssl/ossl_typ.h>
#include <stddef.h>
#include <openssl/swsds.h>

#define SMS4_ENCRYPT     1
#define SMS4_DECRYPT     0

#define SMS4_KEY_LENGTH     16
#define SMS4_BLOCK_SIZE     16
#define SMS4_IV_LENGTH     16
#define CST_INVALID_HANLDE ((SGD_HANDLE)NULL)

#ifdef __cplusplus
extern "c" {
#endif

typedef struct sansec_sms4_key_st {
	SGD_HANDLE hSessionHandle;
	DEVICEINFO stDeviceInfo;
	SGD_HANDLE hKeyHandle;
#if defined(__OPENSSL_NO_SWSDS__)
	unsigned char userKey[SMS4_KEY_LENGTH]; 
#endif
	GMSSL_ALG_ST gm_alg_info;
}SANSEC_SMS4_KEY;

typedef struct sms4_key_st {
    unsigned int Mkey[4];
    unsigned int ENRK[32];
    unsigned int DERK[32];
    int RKok;
	SANSEC_SMS4_KEY gm_alg_info;
}SMS4_KEY;

static unsigned char SMS4SBOX(unsigned char in);
static unsigned int SMS4_T(unsigned int in, int flag);
static void SMS4_RK_get(unsigned int* MK,SMS4_KEY *key);
static unsigned int SMS4_F(unsigned int x0,unsigned int x1,unsigned int x2,unsigned int x3,unsigned int rk);

int SMS4_set_encrypt_key(const unsigned char *userKey, SMS4_KEY *key, GMSSL_ALG_ST *gm_alg_info);
int SMS4_set_decrypt_key(const unsigned char *userKey, SMS4_KEY *key, GMSSL_ALG_ST *gm_alg_info);
int SMS4_ss_set_encrypt_key(const unsigned char *userKey, SMS4_KEY *key);
int SMS4_cy_set_encrypt_key(const unsigned char *userKey, SMS4_KEY *key);
void SMS4_encrypt(const unsigned char *in, unsigned char *out, const SMS4_KEY *key);
void SMS4_cy_encrypt(const unsigned char *in, unsigned char *out, const SMS4_KEY *key);
void SMS4_ss_encrypt(const unsigned char *in, unsigned char *out, const SMS4_KEY *key);
void SMS4_decrypt(const unsigned char *in, unsigned char *out, const SMS4_KEY *key);
void SMS4_cy_decrypt(const unsigned char *in, unsigned char *out, const SMS4_KEY *key);
void SMS4_ss_decrypt(const unsigned char *in, unsigned char *out, const SMS4_KEY *key);
void SMS4_ecb_encrypt(const unsigned char *in, unsigned char *out, 
		             const SMS4_KEY *key, const int enc);
void SMS4_cbc_encrypt(const unsigned char *in, unsigned char *out,
	                 size_t length, const SMS4_KEY *key,
	                 unsigned char *ivec, const int enc);
void SMS4_ofb128_encrypt(const unsigned char *in, unsigned char *out,
	size_t len, const SMS4_KEY *key, unsigned char *ivec, int *num);
void SMS4_cfb128_encrypt(const unsigned char *in, unsigned char *out,
	size_t len, const SMS4_KEY *key, unsigned char *ivec, int *num, int enc);
int  SMS4_cleanup(SMS4_KEY *key);

#ifdef __cplusplus
}
#endif
#endif
