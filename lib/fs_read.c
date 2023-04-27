#include <xinu.h>
#include <fs.h>

extern fsystem_t* fsd;
extern filetable_t oft[NUM_FD];

/*
 * Copy the data from the block device into the 'buff' argument
 *    Use the fileptr field of the oft to track the location of the next read/write
 *    operation.
 *    - Determine the current block and offset from the fileptr
 *    - Copy bytes into the 'buff' until you have exhausted the block or copied 'len' bytes
 *    - Repeat until 'len' bytes are copied
 *    - Update the fileptr field
 *    - Return the number of bytes read
 */
int fs_read(int fd, char* buff, int len) {
    if (fd < 0 || fd >= NUM_FD || oft[fd].state == FSTATE_CLOSED) {
        return SYSERR;
    }
    
    int bytes_read = 0;
    int remaining_bytes = len;
    int block_size = fsd->device.blocksz;
    
    for (int i = 0; remaining_bytes > 0; i++) {
        int block_num = oft[fd].fileptr / block_size;
        int block_offset = oft[fd].fileptr % block_size;
        int bytes_to_read = block_size - block_offset;
        
        if (bytes_to_read > remaining_bytes) {
            bytes_to_read = remaining_bytes;
        }
        
        char temp_buf[block_size];
        bs_read(oft[fd].in.blocks[block_num], block_offset, temp_buf, bytes_to_read);
        memcpy(buff + bytes_read, temp_buf, bytes_to_read);
        oft[fd].fileptr += bytes_to_read;
        remaining_bytes -= bytes_to_read;
        bytes_read += bytes_to_read;
    }
    return bytes_read;
}
