#include <xinu.h>
#include <fs.h>

extern fsystem_t* fsd;

/*
 * fs_create - Return SYSERR if not enough space is available
 *     in the directory or the filename already exists.
 *     Otherwise:
 *        1. Find an available block on the block store
 *        2. Create an inode_t for the new file
 *        3. Update the root directory
 *        4. Write the inode and free bitmask back to the block device
 */
syscall fs_create(char* filename) {
  if (file_exists(filename) != -1 || fsd->root_dir.numentries >= DIR_SIZE) {
    return SYSERR;
  }

  int i = 2;
  while (i < DIR_SIZE) {
    if (fs_getmaskbit(i) == 0) {
      dirent_t de;
      de.inode_block = i;
      strncpy(de.name, filename, FILENAME_LEN);

      fsd->root_dir.entry[fsd->root_dir.numentries] = de;
      fsd->root_dir.numentries++;

      bs_write(0, 0, (void*)fsd, sizeof(*fsd));

      inode_t in;
      in.id = i;
      in.size = 0;
      memset(in.blocks, 0, sizeof(in.blocks));

      bs_write(i, 0, (void*)&in, sizeof(in));

      fs_setmaskbit(i);
      bs_write(1, 0, (void*)fsd->freemask, fsd->freemasksz);

      return OK;
    }
    i++;
  }

  return SYSERR;
}

int file_exists(char* filename) {
  for (int i = 0; i < fsd->root_dir.numentries; i++) {
    if (strcmp(filename, fsd->root_dir.entry[i].name) == 0) {
      return i;
    }
  }
  return -1;
}
