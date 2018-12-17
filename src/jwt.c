#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "mbedtls/base64.h"
#include "mbedtls/sha256.h"
#include "mbedtls/rsa.h"
#include "jwt.h"
#include "sim808.h"

#define RSA_N "00b63f9172cccdf2881d21ea1baee9\
    9c0f046e5df46f332af19989c19fc4\
    9af772530f6c71959f278f58736334\
    48a91d5fa626538a9dd091d6c69f25\
    0ec439f7a95ac4f6a9f0febca2d7d4\
    be3490878681bdb9d32068c7c02f87\
    31e201be55e77c0f8a7a3a1f5d6f32\
    40f30976cd7b1a78c88ad5d0e839b9\
    d309bd985c9e8590fd04e557de5331\
    d1518cada0c024b8eb88e42bc22df2\
    3a716d75fa5cd7bc8143366cfc3cff\
    757da79a02f18625459720c847df4d\
    29befb1050af92199472e692fbbdef\
    41a00f647f63cf3f4bf271c6485c1b\
    e06baa03bdd3b9c1d6524d87efdb80\
    bc93cbc9b796778a00c09381ad7b79\
    61f43c3780f460958bbfb127cc5386\
    4aed"

#define RSA_E "010001"

#define RSA_D "22027edc138823486f5752ded3a758\
    ae1cbf93dcf79b6eda2332e7e52ec5\
    54840b2807aeb2a33eb8ab62ed1266\
    b4d87c6eff1bfbf43365e3d4b10233\
    5d3f0c0e9897c50835a6f400d79225\
    e49d4c0d37ccf60e862fc786ffd794\
    e17e3dccbe2412aa5ee0d5d593d8a6\
    f233c3edd8993a0d028ba02e96072d\
    70d5e6c6f1f62ef617dd32778754d3\
    993eb0d2d2962b13e11299a29316db\
    77b0d7728f40b35b15967ec3f26da3\
    56e9ef47e8c6d5883dd1064fa3c680\
    36cc1e266d8c3c7889f96d8451b016\
    ea0c9026f0bc40785c1ecb730533f2\
    01b3aa64c7392de66619db3a382542\
    f0e4fd67f879200f4a6b9237f981f7\
    7f315e2ec536b6cd7b6006a2c1aba9\
    81"

#define RSA_P "00e59d6c2d2c831da1216a470cabb5\
    63dd99a9620fb17a228e30a61fa432\
    3e42fb1be8e4b10b28f135a5f22f59\
    0413dfb9ad8ceb26a4049886bdce7e\
    7e9d4bb0bc698cc47f04cb764a8217\
    856727e71c5e8e619e7511ac62536e\
    4c21426821d14ed20d60462e857f74\
    4a47fe7734e21f41b11213758bac13\
    dfbda52ecc29c307f1"

#define RSA_Q "00cb30c3471cf6655c0e72d5ac6b34\
    a2c08c4e1ab12aa29d672e2617095e\
    ba35e9167f8a020496d4d2c33a8fba\
    10761fc5b4065002be44b141cee093\
    52fd2025fd33631aa9520e4ec8c57d\
    6ac3e820b17b6061d6c23c48c0f155\
    8429c0f46e8f806e44c73b6ccf786d\
    f435f54602f756ac109310d372f8ff\
    026980f0f241fe4ebd"

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

    free(pb64);
    free(hb64);

    unsigned char hash[32];
    memset(hash, 0, 32);
    mbedtls_sha256_ret((const unsigned char*)hpb64, (hpb64_len-1), hash, 0);

    /* TODO: Possibily remove sprintf */
    unsigned char hash_str[64];
    int i;
    for(i = 0; i < 32; i++)
        sprintf((char*)&hash_str[i*2], "%02x ", hash[i]);

    mbedtls_rsa_context rsa_cntx;


    return "Yeeticus";
}
