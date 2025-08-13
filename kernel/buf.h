struct buf {
  int disk;    // does disk "own" buf?
  uint dev;
  uint blockno;
  uchar data[BSIZE];
};

