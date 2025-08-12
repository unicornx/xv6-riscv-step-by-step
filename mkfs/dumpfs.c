#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <endian.h>

#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/param.h"

uchar block[BSIZE];  // used for reading block data

// print superblock info.
void print_superblock(struct superblock *sb)
{
    printf("Superblock Information:\n");
    printf("  Magic number: 0x%x\n", sb->magic);
    printf("  Filesystem size: %d blocks\n", sb->size);
    printf("  Data blocks: %d\n", sb->nblocks);
    printf("  Inodes: %d\n", sb->ninodes);
    printf("  Inode start block: %d\n", sb->inodestart);
    printf("  Bitmap start block: %d\n", sb->bmapstart);
}

// read data block for file-system
int read_block(int fd, uint32_t blockno, void *buf)
{
    if (lseek(fd, blockno * BSIZE, SEEK_SET) != blockno * BSIZE) {
        perror("lseek");
        return -1;
    }
    if (read(fd, buf, BSIZE) != BSIZE) {
        perror("read");
        return -1;
    }
    return 0;
}

// print content of directory entries
void print_dirents(int fd, struct dinode *dip, int inum)
{
    printf("  Directory entries:\n");

    char buf[BSIZE];
    uint32_t indirect[NINDIRECT];
    int entries_printed = 0;

    // direct block
    for (int i = 0; i < NDIRECT; i++) {
        if (dip->addrs[i] == 0) continue;

        if (read_block(fd, dip->addrs[i], buf) < 0) {
            fprintf(stderr, "Failed to read block %d\n", dip->addrs[i]);
            continue;
        }

        struct dirent *de = (struct dirent *)buf;
        for (int j = 0; j < BSIZE / sizeof(struct dirent); j++, de++) {
            if (de->inum == 0) continue;

            printf("    inum: %d, name: %s\n", de->inum, de->name);
            entries_printed++;
        }
    }

    // non-direct block
    if (dip->addrs[NDIRECT] != 0) {
        if (read_block(fd, dip->addrs[NDIRECT], indirect) < 0) {
            fprintf(stderr, "Failed to read indirect block %d\n", dip->addrs[NDIRECT]);
            return;
        }

        for (int i = 0; i < NINDIRECT; i++) {
            if (indirect[i] == 0) continue;

            if (read_block(fd, indirect[i], buf) < 0) {
                fprintf(stderr, "Failed to read block %d\n", indirect[i]);
                continue;
            }

            struct dirent *de = (struct dirent *)buf;
            for (int j = 0; j < BSIZE / sizeof(struct dirent); j++, de++) {
                if (de->inum == 0) continue;

                printf("    inum: %d, name: %s\n", de->inum, de->name);
                entries_printed++;
            }
        }
    }

    if (entries_printed == 0) {
        printf("    (empty directory)\n");
    }
}

// print content of inode
void print_inode(int fd, struct dinode *ip, int inum)
{
    char *type_str;
    switch(ip->type) {
        case T_DIR:  type_str = "directory"; break;
        case T_FILE: type_str = "file"; break;
        default: type_str = "unknown"; break;
    }

    printf("Inode %d:\n", inum);
    printf("  Type: %s\n", type_str);
    printf("  Links: %d\n", ip->nlink);
    printf("  Size: %d bytes\n", ip->size);
    printf("  Data blocks: ");
    for (int i = 0; i < NDIRECT; i++) {
        if (ip->addrs[i] != 0) {
            printf("%d ", ip->addrs[i]);
        }
    }
    if (ip->addrs[NDIRECT] != 0) {
        printf("+ ind(%d) ", ip->addrs[NDIRECT]);
    }
    printf("\n");

    if (ip->type == T_DIR) {
        print_dirents(fd, ip, inum);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fs.img>\n", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    // superblock is block 1, block 0 is for bootloader
    struct superblock sb;
    if (lseek(fd, BSIZE, SEEK_SET) != BSIZE) {
        perror("lseek");
        close(fd);
        exit(1);
    }

    if (read(fd, &sb, sizeof(sb)) != sizeof(sb)) {
        perror("read");
        close(fd);
        exit(1);
    }

    if (sb.magic != FSMAGIC) {
        fprintf(stderr, "Not an xv6 filesystem (magic = 0x%x)\n", sb.magic);
        close(fd);
        exit(1);
    }

    print_superblock(&sb);

    printf("\nInode Information:\n");
    // The offset will be changed in print_inode, so we use inode_offset
    // to track the position of the currently processed inode blocks.
    off_t inode_offset = sb.inodestart * BSIZE;
    for (int i = 0; i < sb.ninodes; i++) {
        // Each inode block contains BSIZE/sizeof(struct dinode) inodes.
        // inode_number_in_block is the index of the current inode in the current block
        int inode_number_in_block = i % (BSIZE / sizeof(struct dinode));

        // read new inode block
        if (inode_number_in_block == 0) {
            inode_offset = lseek(fd, inode_offset, SEEK_SET);
            if (read(fd, block, BSIZE) != BSIZE) {
                perror("read inodes");
                close(fd);
                exit(1);
            }
            inode_offset += BSIZE;
        }

        // Only print allocated inode
        struct dinode *ip = (struct dinode *)block + inode_number_in_block;
        if (ip->type != 0) {
            print_inode(fd, ip, i);
            printf("\n");
        }
    }

    close(fd);
    return 0;
}
