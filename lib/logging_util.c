#include <logging_util.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <string.h>

int log__socket_get_full_address(mosq_sock_t sock, char *buf, int len) {
  struct sockaddr_storage addr;
  socklen_t addrlen;

  memset(&addr, 0, sizeof(struct sockaddr_storage));
  addrlen = sizeof(addr);
  if (!getpeername(sock, (struct sockaddr *)&addr, &addrlen)) {
    if (addr.ss_family == AF_INET) {
      if (inet_ntop(AF_INET, &((struct sockaddr_in *)&addr)->sin_addr.s_addr,
                    buf, len) == NULL) {
        return 1;
      }
      char port[16];
      snprintf(port, sizeof(port), "%u", ntohs(((struct sockaddr_in *)&addr)->sin_port));
      strcat(buf, ":");
      strcat(buf, port);
      return 0;
    } else if (addr.ss_family == AF_INET6) {
      if (inet_ntop(AF_INET6,
                    &((struct sockaddr_in6 *)&addr)->sin6_addr.s6_addr, buf,
                    len) == NULL) {
        return 1;
      }
      char port[16];
      snprintf(port, sizeof(port), "%u", ntohs(((struct sockaddr_in6 *)&addr)->sin6_port));
      strcat(buf, ":");
      strcat(buf, port);
      return 0;
    }
  }
  return 1;
}
