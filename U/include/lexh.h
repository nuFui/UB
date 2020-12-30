#ifndef __LEXH_H__
#define __LEXH_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

extern uint8_t lexh_read_file(const char *path, char **buffer, uint32_t *size);

#endif