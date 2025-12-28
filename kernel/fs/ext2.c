#include <fs/ext2.h>
#include <drivers/disk.h>
#include <stdio.h>    

void ext2_detect() {
    uint8_t buffer[1024];
    
    disk_read_sector(2, buffer);       
    disk_read_sector(3, buffer + 512); 

    struct ext2_superblock* sb = (struct ext2_superblock*)buffer;
    
    if (sb->s_magic == EXT2_MAGIC) {
        printf("PixelOS: Found EXT2 filesystem!\n");
        printf("Blocks: %d, Inodes: %d\n", sb->s_blocks_count, sb->s_inodes_count);
    } else {
        printf("PixelOS: EXT2 not found. Magic: 0x%x\n", sb->s_magic);
    }
}

struct __attribute__((packed)) ext2_dir_entry {
    uint32_t inode;
    uint16_t rec_len;
    uint8_t  name_len;
    uint8_t  file_type;
    char     name[];
};

void ext2_ls() {
    uint8_t buffer[1024];
    
    uint32_t root_dir_block = 5; 
    disk_read_sector(root_dir_block * 2, buffer);
    disk_read_sector(root_dir_block * 2 + 1, buffer + 512);

    struct ext2_dir_entry* entry = (struct ext2_dir_entry*)buffer;
    uint32_t offset = 0;

    printf("Contents of root directory:\n");

    while (offset < 1024 && entry->rec_len > 0) {
        printf("- ");
        for (int i = 0; i < entry->name_len; i++) {
            printf("%c", entry->name[i]);
        }
        printf("\n");

        offset += entry->rec_len;
        entry = (struct ext2_dir_entry*)((uint8_t*)buffer + offset);
    }
}

extern void jump_to_user_mode(uint32_t entry_point);

void run_app(const char* filename) {
    uint32_t file_block = 10; 
    uint8_t* load_address = (uint8_t*)0x400000;

    disk_read_sector(file_block * 2, load_address);
    disk_read_sector(file_block * 2 + 1, load_address + 512);

    printf("PixelOS: Switching to User Space to run %s...\n", filename);
    
    jump_to_user_mode(0x400000); 
}
