#ifndef LOGGING_UTIL_H
#define LOGGING_UTIL_H

#include <mosquitto_internal.h>

int log__socket_get_full_address(mosq_sock_t sock, char *buf, int len);

#endif /* LOGGING_UTIL_H */
