#ifndef cdcacm_h
#define cdcacm_h

#include "common.h"

#define NOTIF_PACKET_SIZE 16
#define CDCACM_PACKET_SIZE 64

void cdcacm_init(void);
int cdcacm_get_configuration(void);
const char *cdcacm_get_serialno(void);
void cdcacm_write_now(const char *buf, int len);

#endif
