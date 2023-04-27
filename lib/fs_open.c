#include <xinu.h>
#include <fs.h>

extern fsystem_t* fsd;
extern filetable_t oft[NUM_FD];

/*
 * fs_open - Return SYSERR if the file is already open or does not exist
 *     otherwise, add a record to the oft in the first current FSTATE_CLOSED
 *     slot corresponding to the opened file and copy the inode from the block
 *     device to the oft record and return the index into the oft table for the
 *     newly opened file.
 */

int fs_open(char* filename, int flags) {
  int root_idx = file_exists(filename);
  if (file_open(filename) || (root_idx == -1)) {
    return SYSERR;
  }
  int i = 0;
  while (i < NUM_FD) {
    if (oft[i].state == FSTATE_CLOSED) {
      filetable_t ft_entry;
      ft_entry.state = FSTATE_OPEN;
      ft_entry.fileptr = 0;
      ft_entry.de = root_idx;
      bs_read(fsd->root_dir.entry[root_idx].inode_block, 0, 
              (void*)&(ft_entry.in), sizeof(inode_t));
      ft_entry.flag = flags;
      oft[i] = ft_entry;

      return i;
    }
    i++;
  }
  return SYSERR;
}

int file_open(char* filename) {
  int i = 0;
  while (i < NUM_FD) {
    if (oft[i].state == FSTATE_OPEN) {
      int root_idx = oft[i].de;
      dirent_t root_de = fsd->root_dir.entry[root_idx];
      if (strcmp(root_de.name, filename) == 0) {
        return 1;
      }
    }
    i++;
  }
  return 0;
}
