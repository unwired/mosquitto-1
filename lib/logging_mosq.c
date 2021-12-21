/*
Copyright (c) 2009-2019 Roger Light <roger@atchoo.org>

All rights reserved. This program and the accompanying materials
are made available under the terms of the Eclipse Public License v1.0
and Eclipse Distribution License v1.0 which accompany this distribution.

The Eclipse Public License is available at
   http://www.eclipse.org/legal/epl-v10.html
and the Eclipse Distribution License is available at
  http://www.eclipse.org/org/documents/edl-v10.php.

Contributors:
   Roger Light - initial implementation and documentation.
*/

#include "config.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <logging_util.h>
#include "memory_mosq.h"
#include "mosquitto.h"
#include "mosquitto_internal.h"

int log__printf(struct mosquitto *mosq, int priority, const char *fmt, ...)
{
	va_list va;
	char *s;
	int len;
    char new_fmt[2048];

	assert(mosq);
	assert(fmt);

	pthread_mutex_lock(&mosq->log_callback_mutex);
	if(mosq->on_log){
		char remote_address[1024] = { 'u', 'n', 'k', 'n', 'o', 'w', 'n', '\0' };
        log__socket_get_full_address(mosq->sock, remote_address, sizeof(remote_address));
        snprintf(new_fmt, sizeof(new_fmt), "[ Remote: %s ] %s", remote_address, fmt);
		len = strlen(new_fmt) + 500;
		s = mosquitto__malloc(len*sizeof(char));
		if(!s){
			pthread_mutex_unlock(&mosq->log_callback_mutex);
			return MOSQ_ERR_NOMEM;
		}

		va_start(va, fmt);
		vsnprintf(s, len, new_fmt, va);
		va_end(va);
		s[len-1] = '\0'; /* Ensure string is null terminated. */

		mosq->on_log(mosq, mosq->userdata, priority, s);

		mosquitto__free(s);
	}
	pthread_mutex_unlock(&mosq->log_callback_mutex);

	return MOSQ_ERR_SUCCESS;
}
