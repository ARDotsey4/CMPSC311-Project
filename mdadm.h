#ifndef MDADM_H_
#define MDADM_H_

#include <stdint.h>
#include "jbod.h"

/* Return 1 on success and -1 on failure */
int mdadm_mount(void);

/* Return 1 on success and -1 on failure */
int mdadm_unmount(void);

/* Return the number of bytes read on success, -1 on failure. */
int mdadm_read(uint32_t addr, uint32_t len, uint8_t *buf);

/* Return the number of bytes written on success, -1 on failure. */
int mdadm_write(uint32_t addr, uint32_t len, const uint8_t *buf);

// Test for invalid parameters in read/write
int invalidJbodParams(uint32_t addr, uint32_t len, const uint8_t *buf);

// Return the desired disk/block to run operation on (op ID parameters)
uint32_t locateDisk(uint32_t addr);
uint32_t locateBlock(uint32_t addr);

// Seek to location in disks/blocks
void seekLoc(uint32_t disk, uint32_t block);

// Increment to next disk and reset block if final block is reached
int incrementDiskCheck(uint32_t *disk, uint32_t *block);

// Helper for mdadm_read to potentially read from cache or JBOD
void readHelp(uint32_t disk, uint32_t block, uint8_t *blockRead);

#endif
