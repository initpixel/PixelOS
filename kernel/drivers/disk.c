#include <drivers/disk.h>
#include <stdint.h>

#define DATA_PORT         0x1F0
#define SECTOR_COUNT_PORT 0x1F2
#define LBA_LOW_PORT      0x1F3
#define LBA_MID_PORT      0x1F4
#define LBA_HIGH_PORT     0x1F5
#define DRIVE_SELECT_PORT 0x1F6
#define COMMAND_PORT      0x1F7
#define STATUS_PORT       0x1F7

#define CMD_READ_PIO      0x20

extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);

void disk_read_sector(uint32_t lba, uint8_t *buffer) {
    while ((inb(STATUS_PORT) & 0x80));

    outb(DRIVE_SELECT_PORT, 0xE0 | ((lba >> 24) & 0x0F));

    outb(SECTOR_COUNT_PORT, 1);
    outb(LBA_LOW_PORT,  (uint8_t)lba);
    outb(LBA_MID_PORT,  (uint8_t)(lba >> 8));
    outb(LBA_HIGH_PORT, (uint8_t)(lba >> 16));

    outb(COMMAND_PORT, CMD_READ_PIO);

    while (!(inb(STATUS_PORT) & 0x08));

    uint16_t *ptr = (uint16_t *)buffer;
    for (int i = 0; i < 256; i++) {
        ptr[i] = inw(DATA_PORT);
    }
}
