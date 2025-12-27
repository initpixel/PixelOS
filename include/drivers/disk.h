#ifndef DISK_H
#define DISK_H

#include <stdint.h>

void disk_read_sector(uint32_t lba, uint8_t *buffer);
void init_disk(void); // init IDE

#endif
