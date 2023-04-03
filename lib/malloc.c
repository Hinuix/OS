void free(char* block, uint32 size) {
    intmask mask = disable();
    pid32 pid = getpid();

    struct memblk *otheblk, *prev, *curr;
    otheblk = (struct memblk*)(block - sizeof(struct memblk));
    size = otheblk->mlength;
    prev = NULL;
    curr = (struct memblk *)heaptab[pid].freelist;
  
    while (curr != NULL && curr < otheblk) {
        prev = curr;
        curr = curr->mnext;
    }
    if (block == NULL || size == 0){
        restore(mask);
        return;
    }
    if (prev != NULL && (char *)prev + prev -> mlength == (char *)otheblk) {
        prev -> mlength += size;
        otheblk = prev;
    } 
    else {
        otheblk -> mnext = curr;
        if (prev) {
            prev -> mnext = otheblk;
        } else {
            heaptab[pid].freelist = (char *)otheblk;
        }
    }
    if (curr != NULL && (char *)otheblk + otheblk -> mlength == (char *)curr) {
        otheblk -> mnext = curr -> mnext;
        otheblk -> mlength += curr -> mlength;
    }

    restore(mask);
    return;
}
