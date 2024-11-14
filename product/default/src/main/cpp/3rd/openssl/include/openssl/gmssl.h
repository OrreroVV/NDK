#ifndef HEADER_GMSSL_H
#define HEADER_GMSSL_H


#ifdef __cplusplus
extern "C" {
#endif

# define GMSSL1_VERSION                    0x0100

# ifndef __USE_VER_3_1__
# define GMSSL1_1_VERSION                  0x0101
# else
# define GMSSL1_1_VERSION                  0x0302
# endif

# define GMSSL1_2_VERSION                  0x0102
# define GMSSL_MAX_VERSION                 GMSSL1_2_VERSION

# define GMSSL1_VERSION_MAJOR              0x01
# define GMSSL1_VERSION_MINOR              0x00

# ifndef __USE_VER_3_1__
# define GMSSL1_1_VERSION_MAJOR            0x01
# define GMSSL1_1_VERSION_MINOR            0x01
# else
# define GMSSL1_1_VERSION_MAJOR            0x03
# define GMSSL1_1_VERSION_MINOR            0x02
# endif

# define GMSSL1_2_VERSION_MAJOR            0x01
# define GMSSL1_2_VERSION_MINOR            0x02

# define SSL_OP_NO_GMSSLv1_1               0x20000000L

# ifndef __USE_VER_3_1__
# define GMSSL_VERSION_MAJOR               0x01
# else
# define GMSSL_VERSION_MAJOR               0x03
# endif


# define GMSSL1_get_version(s) \
	((s->version >> 8) == GMSSL1_1_VERSION_MAJOR ? s->version : 0)

# define GMSSL1_get_client_version(s) \
	((s->client_version >> 8) == GMSSL1_1_VERSION_MAJOR ? s->client_version : 0)

/* from GM/T 0024-2014 Table 2 */

# define GM1_CK_ECDHE_SM1_SM3                      0x0300E001
# define GM1_CK_ECDHE_SM1_SHA1                     0x0300E002
# define GM1_CK_ECC_SM1_SM3                        0x0300E003
# define GM1_CK_ECC_SM1_SHA1                       0x0300E004
# define GM1_CK_RSA_SM1_SM3                        0x0300E009
# define GM1_CK_RSA_SM1_SHA1                       0x0300E00A

# define GM1_CK_ECDHE_SM4_SM3                      0x0300E011
# define GM1_CK_ECC_SM4_SM3                        0x0300E013
# define GM1_CK_RSA_SM4_SM3                        0x0300E019
# define GM1_CK_RSA_SM4_SHA1                       0x0300E01A

/* GmSSL specific */
# define GM1_CK_ECDHE_SM2_SM4_SM3 	               0x0300E031
# define GM1_CK_SM2_SM4_SM3						   0x0300E033

# define GM1_TXT_ECDHE_SM1_SM3					   "ECDHE-SM1-SM3"
# define GM1_TXT_ECDHE_SM1_SHA1                    "ECDHE-SM1-SHA1"
# define GM1_TXT_ECC_SM1_SM3					   "ECC-SM1-SM3"
# define GM1_TXT_ECC_SM1_SHA1                      "ECC-SM1-SHA1"
# define GM1_TXT_IBSDH_SM1_SM3					   "IBSDH-SM1-SM3"
# define GM1_TXT_IBC_SM1_SM3					   "IBC-SM1-SM3"
# define GM1_TXT_RSA_SM1_SM3					   "RSA-SM1-SM3"
# define GM1_TXT_RSA_SM1_SHA1					   "RSA-SM1-SHA1"
# define GM1_TXT_ECDHE_SM4_SM3					   "ECDHE-SM4-SM3"
# define GM1_TXT_ECC_SM4_SM3		               "ECC-SM4-SM3"
# define GM1_TXT_IBSDH_SM4_SM3		               "IBSDH-SM4-SM3"
# define GM1_TXT_IBC_SM4_SM3		               "IBC-SM4-SM3"
# define GM1_TXT_RSA_SM4_SM3		               "RSA-SM4-SM3"
# define GM1_TXT_RSA_SM4_SHA1		               "RSA-SM4-SHA1"
/* GmSSL specific */
# define GM1_TXT_ECDHE_SM2_SM4_SM3	               "ECDHE-SM2-SM4-SM3"
# define GM1_TXT_SM2_SM4_SM3		               "SM2-SM4-SM3"


/* from GM/T 0024-2014 Table 1 */
# define GM1_AD_UNSUPPORTED_SITE2SITE	           200 /* fatal */
# define GM1_AD_NO_AREA			                   201
# define GM1_AD_UNSUPPORTED_AREATYPE	           202
# define GM1_AD_BAD_IBCPARAM		               203 /* fatal */
# define GM1_AD_UNSUPPORTED_IBCPARAM	           204 /* fatal */
# define GM1_AD_IDENTITY_NEED		               205 /* fatal */

# define SSL_TXT_SM1                               "SM1"
# define SSL_TXT_SM3                               "SM3"
# define SSL_TXT_SM4                               "SM4"

# define SSL_TXT_kSM2                              "kSM2"
# define SSL_TXT_kSM2_ECC                          "kSM2ECC"
# define SSL_TXT_kSM2_ECDHE                        "kSM2ECDHE"

# define SSL_TXT_GMSSLV1_1         				   "GMSSLv1.1"

# define SSL_R_NO_SM2_CERTIFICATE_SENT_BY_PEER    380

#if 0
const SSL_METHOD *GMSSLv1_1_method(void);              /* GMSSLv1.1 */
const SSL_METHOD *GMSSLv1_1_server_method(void);       /* GMSSLv1.1 */
const SSL_METHOD *GMSSLv1_1_client_method(void);       /* GMSSLv1.1 */
#endif

# define HW_SDF_ALG_SM1       0x00000001L
# define HW_SDF_ALG_SM2       0x00000002L
# define HW_SDF_ALG_SM3       0x00000004L
# define HW_SDF_ALG_SM4       0x00000008L
# define HW_SDF_ALG_ALL       (HW_SDF_ALG_SM1 | HW_SDF_ALG_SM2 | HW_SDF_ALG_SM3 | HW_SDF_ALG_SM4)
#ifdef __cplusplus
}
#endif
#endif

