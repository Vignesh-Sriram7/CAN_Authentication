#ifndef AUTH_H
#define AUTH_H

#include <stdint.h>

#define HMAC_KEY       "super_secret_key"
#define HMAC_KEY_LEN   16   // match your chosen secret length

int authenticate_message(const char *msg);

#endif
