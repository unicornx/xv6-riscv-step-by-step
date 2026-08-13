// File system implementation.  Five layers:
//   + Blocks: allocator for raw disk blocks.
//   + Log: crash recovery for multi-step updates.
//   + Files: inode allocator, reading, writing, metadata.
//   + Directories: inode with special contents (list of other inodes!)
//   + Names: paths like /usr/rtm/xv6/fs.c for convenient naming.
//
// This file contains the low-level file system manipulation
// routines.  The (higher-level) system call implementations
// are in sysfile.c.

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "spinlock.h"
#include "proc.h"
#include "sleeplock.h"
#include "fs.h"
#include "buf.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
// there should be one superblock per disk device, but we run with
// only one device
struct superblock sb; 

// Read the super block.
static void
readsb(int dev, struct superblock *sb)
{
  struct buf *bp;

  bp = bread(dev, 1);
  memmove(sb, bp->data, sizeof(*sb));
  brelse(bp);
}

// Init fs
void
fsinit(int dev) {
  readsb(dev, &sb);
  if(sb.magic != FSMAGIC)
    panic("invalid file system");
}

// The scenario of multi-tasks concurrently accessing a single file
// has not yet been considered, so a single global inode is sufficient.
struct dinode icache;

// Find the inode with number inum on device dev
// and return the in-memory copy. Does not lock
// the inode and does not read it from disk.
static struct dinode*
iget(uint dev, uint inum)
{
  struct buf *bp;
  struct dinode *dip;

  bp = bread(dev, IBLOCK(inum, sb));
  dip = (struct dinode*)bp->data + inum%IPB;
  icache.type = dip->type;
  icache.nlink = dip->nlink;
  icache.size = dip->size;
  memmove(icache.addrs, dip->addrs, sizeof(icache.addrs));
  brelse(bp);

  return &icache;
}

// Inode content
//
// The content (data) associated with each inode is stored
// in blocks on the disk. The first NDIRECT block numbers
// are listed in ip->addrs[].  The next NINDIRECT blocks are
// listed in block ip->addrs[NDIRECT].

// Return the disk block address of the nth block in inode ip.
// Just panic for the case where there is no such block. Theoretically,
// bmap should allocate one for such case, but we have not implemented
// bwrite yet, so we just panic. We will add this support soon later.
static uint
bmap(struct dinode *ip, uint bn)
{
  uint addr, *a;
  struct buf *bp;

  if(bn < NDIRECT){
    if((addr = ip->addrs[bn]) == 0){
      // TBD
      panic("bmap < NDIRECT: addr does not exist");
    }
    return addr;
  }
  bn -= NDIRECT;

  if(bn < NINDIRECT){
    // Load indirect block, allocating if necessary.
    if((addr = ip->addrs[NDIRECT]) == 0){
      // TBD
      panic("bmap = NDIRECT: addr does not exist");
    }
    bp = bread(ROOTDEV, addr);
    a = (uint*)bp->data;
    if((addr = a[bn]) == 0){
      // TBD
      panic("bmap > NDIRECT: addr does not exist");
    }
    brelse(bp);
    return addr;
  }

  panic("bmap: out of range");
}

// Read data from inode.
// If user_dst==1, then dst is a user virtual address;
// otherwise, dst is a kernel address.
int
readi(struct dinode *ip, int user_dst, uint64 dst, uint off, uint n)
{
  uint tot, m;
  struct buf *bp;

  if(off > ip->size || off + n < off)
    return 0;
  if(off + n > ip->size)
    n = ip->size - off;

  for(tot=0; tot<n; tot+=m, off+=m, dst+=m){
    uint addr = bmap(ip, off/BSIZE);
    if(addr == 0)
      break;
    bp = bread(ROOTDEV, addr);
    m = min(n - tot, BSIZE - off%BSIZE);
    if(either_copyout(user_dst, dst, bp->data + (off % BSIZE), m) == -1) {
      brelse(bp);
      tot = -1;
      break;
    }
    brelse(bp);
  }
  return tot;
}

// Directories

int
namecmp(const char *s, const char *t)
{
  return strncmp(s, t, DIRSIZ);
}

// Look for a directory entry in a directory.
// If found, set *poff to byte offset of entry.
struct dinode*
dirlookup(struct dinode *dp, char *name, uint *poff)
{
  uint off, inum;
  struct dirent de;

  if(dp->type != T_DIR)
    panic("dirlookup not DIR");

  for(off = 0; off < dp->size; off += sizeof(de)){
    if(readi(dp, 0, (uint64)&de, off, sizeof(de)) != sizeof(de))
      panic("dirlookup read");
    if(de.inum == 0)
      continue;
    if(namecmp(name, de.name) == 0){
      // entry matches path element
      if(poff)
        *poff = off;
      inum = de.inum;
      return iget(ROOTDEV, inum);
    }
  }

  return 0;
}

// Look up and return the inode for a path name.
// Currently, file path resolution is not supported, so the
// "path" parameter can only contain the name of the program
// file, and the program is placed in the root directory by
// default. Particularly, the "path" can be a string such as
// "echo", but absolute paths ("/echo"), and relative paths
// ("./echo") are not supported.
struct dinode*
namei(char *path)
{
  struct dinode *ip, *next;

  ip = iget(ROOTDEV, ROOTINO);
  if (ip == 0)
    return 0;

  if((next = dirlookup(ip, path, 0)) == 0){
    return 0;
  }
  return next;
}
