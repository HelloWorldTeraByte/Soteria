#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "mbedtls/base64.h"
#include "mbedtls/sha256.h"
#include "mbedtls/bignum.h"
#include "mbedtls/rsa.h"
#include "jwt.h"
#include "sim808.h"

#define RSA_N "00B63F9172CCCDF2881D21EA1BAEE9"\
    "9C0F046E5DF46F332AF19989C19FC4"\
    "9AF772530F6C71959F278F58736334"\
    "48A91D5FA626538A9DD091D6C69F25"\
    "0EC439F7A95AC4F6A9F0FEBCA2D7D4"\
    "BE3490878681BDB9D32068C7C02F87"\
    "31E201BE55E77C0F8A7A3A1F5D6F32"\
    "40F30976CD7B1A78C88AD5D0E839B9"\
    "D309BD985C9E8590FD04E557DE5331"\
    "D1518CADA0C024B8EB88E42BC22DF2"\
    "3A716D75FA5CD7BC8143366CFC3CFF"\
    "757DA79A02F18625459720C847DF4D"\
    "29BEFB1050AF92199472E692FBBDEF"\
    "41A00F647F63CF3F4BF271C6485C1B"\
    "E06BAA03BDD3B9C1D6524D87EFDB80"\
    "BC93CBC9B796778A00C09381AD7B79"\
    "61F43C3780F460958BBFB127CC5386"\
    "4AED"

#define RSA_E "010001"

#define RSA_D "22027EDC138823486F5752DED3A758"\
    "AE1CBF93DCF79B6EDA2332E7E52EC5"\
    "54840B2807AEB2A33EB8AB62ED1266"\
    "B4D87C6EFF1BFBF43365E3D4B10233"\
    "5D3F0C0E9897C50835A6F400D79225"\
    "E49D4C0D37CCF60E862FC786FFD794"\
    "E17E3DCCBE2412AA5EE0D5D593D8A6"\
    "F233C3EDD8993A0D028BA02E96072D"\
    "70D5E6C6F1F62EF617DD32778754D3"\
    "993EB0D2D2962B13E11299A29316DB"\
    "77B0D7728F40B35B15967EC3F26DA3"\
    "56E9EF47E8C6D5883DD1064FA3C680"\
    "36CC1E266D8C3C7889F96D8451B016"\
    "EA0C9026F0BC40785C1ECB730533F2"\
    "01B3AA64C7392DE66619DB3A382542"\
    "F0E4FD67F879200F4A6B9237F981F7"\
    "7F315E2EC536B6CD7B6006A2C1ABA9"\
    "81"

#define RSA_P "00e59d6c2d2c831da1216a470cabb5"\
    "63dd99a9620fb17a228e30a61fa432"\
    "3e42fb1be8e4b10b28f135a5f22f59"\
    "0413dfb9ad8ceb26a4049886bdce7e"\
    "7e9d4bb0bc698cc47f04cb764a8217"\
    "856727e71c5e8e619e7511ac62536e"\
    "4c21426821d14ed20d60462e857f74"\
    "4a47fe7734e21f41b11213758bac13"\
    "dfbda52ecc29c307f1"

#define RSA_Q "00cb30c3471cf6655c0e72d5ac6b34"\
    "a2c08c4e1ab12aa29d672e2617095e"\
    "ba35e9167f8a020496d4d2c33a8fba"\
    "10761fc5b4065002be44b141cee093"\
    "52fd2025fd33631aa9520e4ec8c57d"\
    "6ac3e820b17b6061d6c23c48c0f155"\
    "8429c0f46e8f806e44c73b6ccf786d"\
    "f435f54602f756ac109310d372f8ff"\
    "026980f0f241fe4ebd"

int jwt_create(char **jwt)
{
    char header[] = "{\"typ\":\"JWT\",\"alg\":\"RS256\"}";
    char payload[64];

    char *iat = GSM_iat();
    char *exp = GSM_exp();

    strcpy(payload, "{\"iat\": ");
    strcat(payload, iat);
    strcat(payload, ",\"exp\": ");
    strcat(payload, exp);
    strcat(payload, ",\"aud\": \"");
    strcat(payload, PROJECT_ID);
    strcat(payload, "\"}");

    free(iat);
    free(exp);

    size_t p_len = strlen(payload);
    size_t pb64_len;
    mbedtls_base64url_encode(NULL, 0, &pb64_len, (const unsigned char*)payload, p_len);
    char *pb64 = (char*)malloc(sizeof(char)*pb64_len);
    size_t pb64_w;
    mbedtls_base64url_encode((unsigned char *)pb64, pb64_len, &pb64_w, (const unsigned char*)payload, p_len);

    size_t h_len = strlen(header);
    size_t hb64_len;
    mbedtls_base64url_encode(NULL, 0, &hb64_len, (const unsigned char*)header, h_len);
    char *hb64 = (char*)malloc(sizeof(char)*hb64_len);
    size_t hb64_w;
    mbedtls_base64url_encode((unsigned char*)hb64, hb64_len, &hb64_w, (const unsigned char*)header, h_len);

    size_t hpb64_len = hb64_len+pb64_len;
    char hpb64[hpb64_len];
    memset(hpb64, 0, hpb64_len);

    strcpy(hpb64, hb64);
    strcat(hpb64, ".");
    strcat(hpb64, pb64);

    free(pb64);
    free(hb64);

    unsigned char hash[33];
    memset(hash, 0, 33);
    mbedtls_sha256_ret((const unsigned char*)hpb64, (hpb64_len-1), hash, 0);

    /* /1* TODO: Possibily remove sprintf *1/ */
    /* unsigned char hash_str[65]; */
    /* int i; */
    /* for(i = 0; i < 32; i++) */
    /*     sprintf((char*)&hash_str[i*2], "%02x ", hash[i]); */


    unsigned char rsa_buf[MBEDTLS_MPI_MAX_SIZE];
    mbedtls_rsa_context rsa_cntx;
    mbedtls_mpi N, P, Q, D, E, DP, DQ, QP;

    mbedtls_rsa_init(&rsa_cntx, MBEDTLS_RSA_PKCS_V15, 0);

    mbedtls_mpi_init(&N); mbedtls_mpi_init(&P); mbedtls_mpi_init(&Q);
    mbedtls_mpi_init(&D); mbedtls_mpi_init(&E); mbedtls_mpi_init(&DP);
    mbedtls_mpi_init(&DQ); mbedtls_mpi_init(&QP);

    int ret = 1;
    if((ret = mbedtls_mpi_read_string(&N, 16, RSA_N) ) != 0 ||
            (ret = mbedtls_mpi_read_string(&E, 16, RSA_E) ) != 0 ||
            (ret = mbedtls_mpi_read_string(&D, 16, RSA_D) ) != 0 ||
            (ret = mbedtls_mpi_read_string(&P, 16, RSA_P) ) != 0 ||
            (ret = mbedtls_mpi_read_string(&Q, 16, RSA_Q) ) != 0)
        goto CLEANUP;

    if((ret = mbedtls_rsa_import(&rsa_cntx, &N, &P, &Q, &D, &E )) != 0)
        goto CLEANUP;
    if((ret = mbedtls_rsa_complete(&rsa_cntx)) != 0)
        goto CLEANUP;
    if((ret = mbedtls_rsa_check_privkey(&rsa_cntx)) != 0)
        goto CLEANUP;

    if((ret = mbedtls_rsa_pkcs1_sign(&rsa_cntx, NULL, NULL, MBEDTLS_RSA_PRIVATE, MBEDTLS_MD_SHA256, 32, hash, rsa_buf)) != 0)
        goto CLEANUP;

    /* size_t s_len = strlen((const char*)rsa_buf); */
    size_t s_len = RSA_LEN;
    size_t sb64_len;
    mbedtls_base64url_encode(NULL, 0, &sb64_len, (const unsigned char*)rsa_buf, s_len);
    char *sb64 = (char*)malloc(sizeof(char)*sb64_len);
    memset(sb64, 0, RSA_LEN);
    size_t sb64_w;
    mbedtls_base64url_encode((unsigned char *)sb64, sb64_len, &sb64_w, (const unsigned char*)rsa_buf, s_len);
    
    *jwt = (char*)realloc(*jwt, sizeof(char)*(hpb64_len+sb64_len));
    strcpy(*jwt, hpb64);
    strcat(*jwt, ".");
    strcat(*jwt, sb64);

CLEANUP:
    mbedtls_rsa_free(&rsa_cntx);
    mbedtls_mpi_free(&N); mbedtls_mpi_free(&P); mbedtls_mpi_free(&Q);
    mbedtls_mpi_free(&D); mbedtls_mpi_free(&E); mbedtls_mpi_free(&DP);
    mbedtls_mpi_free(&DQ); mbedtls_mpi_free(&QP);

    return ret;
}
