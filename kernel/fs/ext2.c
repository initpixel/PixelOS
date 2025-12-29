// kernel/fs/ext2.c
#include <fs/ext2.h>
#include <drivers/disk.h>
#include <stdio.h>
#include <string.h>

extern void jump_to_user_mode(uint32_t entry_point);

void ext2_read_inode(uint32_t inode_no, struct ext2_inode* inode_out) {
    uint8_t sb_buf[1024];
    disk_read_sector(2, sb_buf); 
    disk_read_sector(3, sb_buf + 512);
    struct ext2_superblock* sb = (struct ext2_superblock*)sb_buf;

    uint32_t group = (inode_no - 1) / sb->s_inodes_per_group;
    uint32_t index = (inode_no - 1) % sb->s_inodes_per_group;

    uint8_t gd_buf[1024];
    disk_read_sector(4, gd_buf);
    struct ext2_group_desc* gd = (struct ext2_group_desc*)gd_buf;

    uint32_t inode_table_block = gd[group].bg_inode_table;
    uint32_t inode_size = 128; 
    uint32_t byte_offset = index * inode_size;
    uint32_t sector = (inode_table_block * 2) + (byte_offset / 512);
    uint32_t offset_in_sector = byte_offset % 512;

    uint8_t inode_buf[512];
    disk_read_sector(sector, inode_buf);

    memcpy(inode_out, (uint8_t*)inode_buf + offset_in_sector, inode_size);
}

void ext2_ls() {
    struct ext2_inode root;
    ext2_read_inode(2, &root); 

    uint8_t buffer[1024];
    disk_read_sector(root.i_block[0] * 2, buffer);
    disk_read_sector(root.i_block[0] * 2 + 1, buffer + 512);

    struct ext2_dir_entry* entry = (struct ext2_dir_entry*)buffer;
    uint32_t offset = 0;

    printf("Contents of root directory:\n");
    while (offset < 1024 && entry->rec_len > 0) {
        if (entry->inode != 0) {
            printf("- ");
            for (int i = 0; i < entry->name_len; i++) {
                printf("%c", entry->name[i]);
            }
            printf("\n");
        }
        offset += entry->rec_len;
        entry = (struct ext2_dir_entry*)((uint8_t*)buffer + offset);
    }
}

uint32_t ext2_find_file(const char* name) {
    struct ext2_inode root;
    ext2_read_inode(2, &root);

    uint8_t buffer[1024];
    disk_read_sector(root.i_block[0] * 2, buffer);
    disk_read_sector(root.i_block[0] * 2 + 1, buffer + 512);

    struct ext2_dir_entry* entry = (struct ext2_dir_entry*)buffer;
    uint32_t offset = 0;

    while (offset < 1024 && entry->rec_len > 0) {
        if (strlen(name) == entry->name_len) {
            if (strncmp(name, entry->name, entry->name_len) == 0) {
                return entry->inode;
            }
        }
        offset += entry->rec_len;
        entry = (struct ext2_dir_entry*)((uint8_t*)buffer + offset);
    }
    return 0;
}

void run_app(const char* filename) {
    uint32_t start_sector = 20480; 
    uint8_t* load_address = (uint8_t*)0x1000000;

    printf("PixelOS: Loading %s from disk (Fixed Offset)...\n", filename);
    for (int i = 0; i < 64; i++) {
        disk_read_sector(start_sector + i, load_address + (i * 512));
    }
    uint32_t* magic = (uint32_t*)load_address;
    printf("DEBUG: First bytes of app: 0x%x\n", *magic);

    if (*magic == 0) {
        printf("ERROR: App code is empty! Check disk offset.\n");
        return;
    }

    printf("PixelOS: Jumping to User Mode at 0x1000000...\n");
    jump_to_user_mode(0x1000000); 
}
