#ifndef _CRC_H
#define _CRC_H

#include <stdint.h>
uint32_t rc_crc32(uint32_t crc, const char *buf, size_t len);
uint32_t getCrcGivenPath(char *path);
#endif // _CRC_H
