#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "mbedtls/base64.h"
#include "mbedtls/sha256.h"
#include "jwt.h"
#include "sim808.h"

char *jwt_create()
{
    char header[] = "{\"alg\":\"RS256\",\"typ\":\"JWT\"}";
    char payload[64];

    char iat[11];
    char exp[11];

    GSM_iat(iat);
    GSM_exp(exp);
    strcpy(payload, "{\"iat\": ");
    strcat(payload, iat);
    strcat(payload, ",\"exp\": ");
    strcat(payload, exp);
    strcat(payload, ",\"aud\": \"" PROJECT_ID "\"}");

    size_t p_len = strlen(payload);
    size_t pb64_len;
    mbedtls_base64_encode(NULL, 0, &pb64_len, payload, p_len);
    char *pb64 = (char*)malloc(sizeof(char)*pb64_len);
    size_t pb64_w;
    mbedtls_base64_encode(pb64, pb64_len, &pb64_w, payload, p_len);

    size_t h_len = strlen(header);
    size_t hb64_len;
    mbedtls_base64_encode(NULL, 0, &hb64_len, header, h_len);
    char *hb64 = (char*)malloc(sizeof(char)*hb64_len);
    size_t hb64_w;
    mbedtls_base64_encode(hb64, hb64_len, &hb64_w, header, h_len);

    size_t hpb64_len = hpb64_len+pb64_len+1;
    char hpb64[hpb64_len];

    strcpy(hpb64, hb64);
    strcat(hpb64, ".");
    strcat(hpb64, pb64);

    char *s_buff = (char*)malloc(sizeof(char)*32);
    mbedtls_sha256_context c_sha256;

    mbedtls_sha256_init(&c_sha256);
    mbedtls_sha256_starts_ret(&c_sha256, 0);
    mbedtls_sha256_update_ret(&c_sha256, hpb64, hpb64_len);
    mbedtls_sha256_finish_ret(&c_sha256, s_buff);

    mbedtls_sha256_free(&c_sha256);
    free(pb64);
    free(hb64);
    free(s_buff);

    return "ad";
}
