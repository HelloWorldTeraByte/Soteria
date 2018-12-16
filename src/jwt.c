#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mbedtls/base64.h"
#include "mbedtls/md5.h"
#include "mbedtls/sha256.h"
#include "jwt.h"
#include "sim808.h"

char *jwt_create()
{
    char header[] = "{\"alg\":\"RS256\",\"typ\":\"JWT\"}";
    char payload[64];

    char iat[11] = "asdf";
    char exp[11] = "asdf";

    strcpy(payload, "{\"iat\": ");
    strcat(payload, iat);
    strcat(payload, ",\"exp\": ");
    strcat(payload, exp);
    strcat(payload, ",\"aud\": \"");
    strcat(payload, PROJECT_ID);
    strcat(payload, "\"}");

    size_t p_len = strlen(payload);
    size_t pb64_len;
    mbedtls_base64_encode(NULL, 0, &pb64_len, (const unsigned char*)payload, p_len);
    char *pb64 = (char*)malloc(sizeof(char)*pb64_len);
    size_t pb64_w;
    mbedtls_base64_encode((unsigned char *)pb64, pb64_len, &pb64_w, (const unsigned char*)payload, p_len);

    size_t h_len = strlen(header);
    size_t hb64_len;
    mbedtls_base64_encode(NULL, 0, &hb64_len, (const unsigned char*)header, h_len);
    char *hb64 = (char*)malloc(sizeof(char)*hb64_len);
    size_t hb64_w;
    mbedtls_base64_encode((unsigned char*)hb64, hb64_len, &hb64_w, (const unsigned char*)header, h_len);

    size_t hpb64_len = hb64_len+pb64_len;
    char hpb64[hpb64_len];
    memset(hpb64, 0, hpb64_len);

    strcpy(hpb64, hb64);
    strcat(hpb64, ".");
    strcat(hpb64, pb64);

    unsigned char hash[32];
    memset(hash, 0, 32);
    mbedtls_sha256_ret((const unsigned char*)hpb64, (hpb64_len-1), hash, 0);

    unsigned char hash_str[64];
    int i;
    for(i = 0; i < 32; i++)
        sprintf(&hash_str[i*2], "%02x ", hash[i]);

    /* Move these up */
    free(pb64);
    free(hb64);

    return "ad";
}
