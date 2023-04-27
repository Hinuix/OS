#include <xinu.h>
#include <fs.h>

extern fsystem_t* fsd;
extern filetable_t oft[NUM_FD];


/*
 * fs_close - Return SYSERR if the file is already closed
 *     otherwise, write the file's inode back to the block
 *     device and set the entry in the oft to FSTATE_CLOSED
 */
syscall fs_close(int fd) {
 if (fd < 0 || fd >= NUM_FD || oft[fd].state == FSTATE_CLOSED) {
    return SYSERR;
  }

  int root_idx = oft[fd].de;
  dirent_t root_de = fsd->root_dir.entry[root_idx];
  int inode_block = root_de.inode_block;
  inode_t* inode = &(oft[fd].in);

  bs_write(inode_block, 0, (void*)inode, sizeof(inode_t));

  oft[fd].state = FSTATE_CLOSED;

  return OK;
}