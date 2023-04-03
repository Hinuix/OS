#include <xinu.h>
#include <heap.h>

extern heap_t heaptab[];

void initheap(char* startaddr, uint32 size) {
  /* This function is called when the heap is constructed */
  /*   Use as needed to setup the free list               */    
    struct memblk *block = (struct memblk *)(startaddr);
    
    block -> mnext = (struct memblk *)NULL;
    block -> mlength = size;
    
    return;
}

void* malloc(uint32 size) {
  /* Implement memory allocation within the process heap here       */
  /*   Your implementation MUST NOT use getmem                      */
  /*   Your implementation SHOULD use explicit in-memory free lists */
    intmask mask = disable();
    pid32 pid = getpid();

    if(size <= 0){
        return (char*) SYSERR;
    }

    struct memblk *head = (struct memblk*)(heaptab[pid].freelist);
    struct memblk *curr, *prev;
    

    if(heaptab[pid].freelist==NULL){
        restore(mask);
        return (char *)SYSERR;
    }

    prev = head;
    curr = head -> mnext;

    if(head->mlength>size){
        struct memblk *end = (struct memblk *)(size + (uint32)head);
        end -> mnext = head -> mnext;
        end -> mlength = head -> mlength - size;
        heaptab[pid].freelist = (char *)end;
        restore(mask);
        return (char *)(head);
    }
    else if(head -> mlength==size){
        heaptab[pid].freelist = (char *)head -> mnext;
        restore(mask);
        return (char *)(head);
    } 
    while(curr!=NULL){
        if(curr -> mlength == size){
            prev -> mnext = curr->mnext;
            restore(mask);
            return (char *)(curr);
        }
        else if(curr->mlength>size){
            struct memblk *end = (struct memblk *)(size + (uint32)curr);
            prev -> mnext = end;
            end -> mnext = curr ->mnext;
            end -> mlength = curr -> mlength - size;
            restore(mask);
            return (char *)(curr);
        }
        else{
            prev = curr;
            curr = curr -> mnext;
        }
    }
    restore(mask);
  return (char*)SYSERR;
}

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