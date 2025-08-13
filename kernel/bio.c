// Buffer cache.
//

#include "types.h"
#include "param.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "buf.h"

// NO Cache and NO LRU buffer list, just a single buffer to store
// data from disk.
struct {
  struct buf buf[NBUF];
} bcache;

void
binit(void)
{
  // Todo: nothing to do for now.
}

// Return a buf with the contents of the indicated block.
struct buf*
bread(uint dev, uint blockno)
{
  struct buf *b;
  b = bcache.buf;
  b->dev = dev;
  b->blockno = blockno;
  virtio_disk_rw(b, 0);
  return b;
}
