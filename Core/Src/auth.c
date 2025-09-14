#include "auth.h"
#include "hmac.h"
#include "sha256.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Input format:  data|hmac_hex
// Example: HELLO|aabbccddeeff...

int authenticate_message(const char *msg) {
    // Split into <data> and <hmac_hex>
    char *sep = strchr(msg, '|');
    if (!sep) return 0;

    size_t data_len = sep - msg;
    char data[128];
    strncpy(data, msg, data_len);
    data[data_len] = '\0';

    const char *recv_hmac_hex = sep + 1;

    // Convert hex → bytes
    uint8_t recv_hmac[TC_SHA256_DIGEST_SIZE];
    for (int i = 0; i < TC_SHA256_DIGEST_SIZE; i++) {
        sscanf(&recv_hmac_hex[i * 2], "%2hhx", &recv_hmac[i]);
    }

    // Compute local HMAC
    uint8_t calc_hmac[TC_SHA256_DIGEST_SIZE];
    struct tc_hmac_state_struct h;
    tc_hmac_set_key(&h, (const uint8_t*)HMAC_KEY, HMAC_KEY_LEN);
    tc_hmac_init(&h);
    tc_hmac_update(&h, (const uint8_t*)data, strlen(data));
    tc_hmac_final(calc_hmac, TC_SHA256_DIGEST_SIZE, &h);

    // Compare
    if (memcmp(calc_hmac, recv_hmac, TC_SHA256_DIGEST_SIZE) == 0) {
        return 1; //Auth OK
    }
    return 0;     // Auth Fail
}
