#include <stdio.h>  
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/ossl_typ.h>
#include "../../crypto/ec/ec_lcl.h"

#define NUMLEN 32


typedef struct point_s
{
	BIGNUM *pointx;
	BIGNUM *pointy;
	int ulimit;
}point;

point point_new();

typedef struct group_s
{
	BIGNUM *p;
	BIGNUM *a;
	BIGNUM *b;
	BIGNUM *h;
	point G;
	BIGNUM *n;
}group;

group group_new(BIGNUM *p,BIGNUM *a,BIGNUM *b,BIGNUM *h,point G,BIGNUM *n);

group group_new();

point point_add(point p1,point p2,group gp);

point point_double(point p1,group gp);

point point_from_ecpoint(EC_POINT *ecpoint,group gp);

point point_mul(BIGNUM *k, point g,group gp);

void int2con(int k, unsigned char *con);

void KDF(unsigned char* Z,int zlen, int kLen,unsigned char* r, GMSSL_ALG_ST *gm_alg_info);//input zlen data, output klen cipher

typedef struct Ciphertext_s
{
	unsigned char C1[NUMLEN*2+1];
	unsigned char C2[4096];
	unsigned char C3[32];
	unsigned int len;//length of message
}Ciphertext;

typedef struct SIGdata_s
{
        unsigned char r[NUMLEN];
        unsigned char s[NUMLEN];        
}SIGdata;

Ciphertext sm2_encrypt(unsigned char* M,int klen,point pb,group gp, GMSSL_ALG_ST *gm_alg_info);//input message M, length of M, B's public key,output message after encrypt C

int sm2_decrypt(Ciphertext C,BIGNUM *db,unsigned char* M,group gp, GMSSL_ALG_ST *gm_alg_info);//input message after encrypt C ,B's private key,output message after decrypt M

SIGdata SM2_my_sign(const unsigned char *dgst, int dgstlen,BIGNUM *privkeypoint , group gp);

int SM2_my_verify(SIGdata pucEncData,const unsigned char *dgst, int dgstlen, point pubkeypoint,group gp);

int sm2_my_compute_key(unsigned char *p,BIGNUM *da,point pa,point pb,BIGNUM *ra,unsigned char* RA,unsigned char *RB,group gp,unsigned char *ZA,unsigned char *ZB, GMSSL_ALG_ST *gm_alg_info);

