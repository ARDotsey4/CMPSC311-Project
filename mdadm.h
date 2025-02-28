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

// Return the desired disk/block to run operation on (op ID parameters)
uint32_t locateDisk(uint32_t addr);
uint32_t locateBlock(uint32_t addr);

// Increment to next disk and reset block if final block is reached
void incrementDiskCheck(uint32_t *disk, uint32_t *block);

#endif
