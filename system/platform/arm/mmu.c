#ifndef MT

#include <xinu.h>
#include <mmu.h>

#else

#include <stdlib.h>
#include <stdio.h>
#include "../../../include/memmax.h"
#include "../../../include/platform/arm/armv7a.h"

#endif


/* These constants are based on the 1MB Section descriptor format
 * described in Ffigure 5.6 of this:
 * https://developer.arm.com/documentation/ddi0360/f/memory-management-unit/hardware-page-table-translation/armv6-page-table-translation-subpage-ap-bits-disabled
 */

#ifndef NPROC
#define NPROC 4
#endif

#define MEM_SIZE  0x20000000   /* 512MB */
#define SECTION_SIZE 0x100000 /* 1MB */

#ifdef MT
//#define NUM_PAGE_TABLE_ENTRIES 0x1000 /* 4K, 4096 */
#define NUM_PAGE_TABLE_ENTRIES 256
#endif

#define SDRAM_START       0x80000000
#define SDRAM_END         0x8fffffff

#define SECTION_BASE      0x02        /* b10 this is a section base, not PT base */
#define CACHE_ENABLE      0x10
#define CACHE_WRITEBACK   0x1e
#define AP_RW 0x0c00   /* this was 3 << 10 -- bits 10 and 11, RW access permission */

#define BREAK 64 // 320

#ifndef MT

uint32 __attribute__((aligned(16384))) page_table[NPROC][NUM_PAGE_TABLE_ENTRIES];

#else

uint32_t page_table[NPROC][NUM_PAGE_TABLE_ENTRIES];

#endif

#ifndef MT

void initialize_mmu(void) {

#else

int main(int argc, char *argv[]) {

#endif

    int i,j,k=0;
    int pg;
    int asid;

    /* Identity mappings for the first 274 pages was 258? */
    for (pg = 0; pg < NUM_PAGE_TABLE_ENTRIES; pg++){ 
      for (asid = 0; asid < NPROC; asid++){
        page_table[asid][pg] = (pg << 20 | AP_RW | CACHE_ENABLE | SECTION_BASE);
        
#ifdef MT
        printf("pt[%d][%08x]: %08x\n", asid, pg*1024*1024, page_table[asid][pg]);
#endif
      }
    }

#ifdef MT
    printf("-------------------------\n");
#endif    
    
    /* Per process mappings in here, except for the first two */
    for (j = BREAK; j <BREAK+(512-32)/NPROC; j++){ 
      for (i=BREAK,k = 2; k < NPROC;i++, k++){
        //page_table[k][j] = (i << 20 | AP_RW | CACHE_ENABLE | SECTION_BASE);
        
#ifdef MT
        //        printf("pt[%d][%08x]: %08x\n", k, j*1024*1024, page_table[k][j]);
#endif
      }
    }

#ifdef MT
    printf("-------------------------\n");
#endif 

    /* common stack pages at the top */
    for (i = 510; i < 512; i++){ // 258
      for (j = 0; j < NPROC; j++){
        //page_table[j][i] = (i << 20 | AP_RW | CACHE_ENABLE | SECTION_BASE);
        
#ifdef MT        
        //        printf("pt[%d][%08x]: %08x\n", j, i*1024*1024, page_table[j][i]);
#endif
      }

    }

#ifdef MT    
}
#else

 setPageTable();    
 FlushTLB();
 setAccessControl();
 enableMMU(); 
}

 
 void setPageTable(){
	uint32 pagetable =(uint32) page_table[currpid];
	/* Copy the page table address to cp15 */
    asm volatile("mcr p15, 0, %0, c2, c0, 0" : : "r" (pagetable) : "memory");
 }

void setAccessControl(){
/* Set the access control to all-supervisor */
    asm volatile("mcr p15, 0, %0, c3, c0, 0" : : "r" (~0));
}

void FlushTLB(){
/* Set the access control to all-supervisor */
  asm volatile("mcr p15, 0, %0, c8, c7, 0" : : "r" (0));
}

void enableMMU(){
    uint32 reg;
/* Enable the MMU */
    asm("mrc p15, 0, %0, c1, c0, 0" : "=r" (reg) : : "cc");
    reg |= ARMV7A_C1CTL_M;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r" (reg) : "cc");
}

void pageFaultHandler(){
	kprintf("Segmentation Fault\n");
}

#endif
