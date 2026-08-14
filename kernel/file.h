struct file {
  enum { FD_NONE, FD_INODE } type;
  int ref; // reference count
  struct inode *ip;  // FD_INODE
};

// in-memory copy of an inode
struct inode {
  uint dev;           // Device number
  uint inum;          // Inode number
  int ref;            // Reference count
  struct sleeplock lock; // protects everything below here
  int valid;          // inode has been read from disk?

  short type;         // copy of disk inode
  short pack[2];
  short nlink;
  uint size;
  uint addrs[NDIRECT+1];
};
