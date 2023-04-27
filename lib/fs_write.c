#include <xinu.h>
#include <fs.h>

extern fsystem_t* fsd;
extern filetable_t oft[NUM_FD];

/*
 * Copy 'len' bytes from 'buff' into the blocks associated with the file 'fd'
 *   - Until 'len' bytes are copied...
 *   - Check if a new block is needed (find free block and add to the file's blocks if necessary)
 *   - Write bytes from the 'buff' into the block starting at the fileptr
 *   - Write 'len' bytes or fill the block, whichever is less
 *   - Repeat until write is complete
 *   - Update inode state to reflect changes
 *   - Return the number of bytes written
 */

int fs_write(int fd, char* buff, int len) {
    if (oft[fd].state != FSTATE_OPEN || oft[fd].flag == O_RDONLY) {
        return SYSERR;
    }

    int remaining = len;
    char* buff_ptr = buff;
    inode_t* inode = &oft[fd].in;
    int total = 0;

    for (int i = 0; remaining > 0; i++) {
        int curr_idx = oft[fd].fileptr / fsd->device.blocksz;
        int curr_offset = oft[fd].fileptr % fsd->device.blocksz;

        if (oft[fd].fileptr >= inode->size) {
            int free_block = -1;
            for (int j = 0; j < fsd->device.nblocks; j++) {
                if (fs_getmaskbit(j) == 0) {
                    free_block = j;
                    break;
                }
            }
            if (free_block == -1) {
                return total;
            }
            fs_setmaskbit(free_block);
            inode->blocks[curr_idx] = free_block;
        }

        int block_remaining = fsd->device.blocksz - curr_offset;
        int write_size = (remaining < block_remaining) ? remaining : block_remaining;

        char block_buff[fsd->device.blocksz];
        memset(block_buff, 0, fsd->device.blocksz);

        bs_read(inode->blocks[curr_idx], 0, block_buff, fsd->device.blocksz);
        memcpy(block_buff + curr_offset, buff_ptr, write_size);
        bs_write(inode->blocks[curr_idx], 0, block_buff, fsd->device.blocksz);

        buff_ptr += write_size;
        oft[fd].fileptr += write_size;
        total += write_size;
        remaining -= write_size;
    }

    inode->size = oft[fd].fileptr;
    return total;
}