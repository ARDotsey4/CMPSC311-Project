/* Author: Armand Dotsey
   Date: 11 April 2025
    */
    
    
    
/***
 *      ______ .___  ___. .______     _______.  ______              ____    __   __  
 *     /      ||   \/   | |   _  \   /       | /      |            |___ \  /_ | /_ | 
 *    |  ,----'|  \  /  | |  |_)  | |   (----`|  ,----'              __) |  | |  | | 
 *    |  |     |  |\/|  | |   ___/   \   \    |  |                  |__ <   | |  | | 
 *    |  `----.|  |  |  | |  |   .----)   |   |  `----.             ___) |  | |  | | 
 *     \______||__|  |__| | _|   |_______/     \______|            |____/   |_|  |_| 
 *                                                                                   
 */


#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "cache.h"
#include "mdadm.h"
#include "util.h"
#include "jbod.h"

static int status = -1; // 1 = mounted, -1 = unmounted
const int MAX_SIZE = JBOD_NUM_DISKS*JBOD_DISK_SIZE;

int mdadm_mount(void) {
  int fail = jbod_operation(JBOD_MOUNT,NULL);
  if (fail){
    return fail;
  }
  // Mount success
  status = 1;
  return 1;
}

int mdadm_unmount(void) {
  int fail = jbod_operation(JBOD_UNMOUNT,NULL);
  if (fail){
    return fail;
  }
  // Unmount success
  status = -1;
  return 1;
}

int mdadm_read(uint32_t addr, uint32_t len, uint8_t *buf) {
  // Parameter check
  if(invalidJbodParams(addr, len, buf)){
    return -1;
  }
  else if(buf == NULL){
    return len;
  }
  
  // Move to read location
  uint32_t disk = locateDisk(addr);
  uint32_t block = locateBlock(addr);

  // Initial read and increment
  uint8_t blockRead[JBOD_BLOCK_SIZE];
  readHelp(&disk, &block, blockRead);
  block += 1;
  
  uint32_t startInBlock = addr % JBOD_BLOCK_SIZE;
  uint32_t lenInBlock1 = JBOD_BLOCK_SIZE - startInBlock;

  // Full read within block bounds
  if((startInBlock + len) <= JBOD_BLOCK_SIZE){
    memcpy(buf, blockRead + startInBlock, len);
    return len;
  }

  // Read in first block
  memcpy(buf, blockRead + startInBlock, lenInBlock1);
  incrementDiskCheck(&disk, &block);
  
  // Read intermediate blocks
  uint32_t tempLen = len - lenInBlock1;
  while(tempLen > JBOD_BLOCK_SIZE){
    readHelp(&disk, &block, blockRead);
    block += 1;
    memcpy(buf + len - tempLen, blockRead, JBOD_BLOCK_SIZE);
    // Increments
    incrementDiskCheck(&disk, &block);
    tempLen -= JBOD_BLOCK_SIZE;
    }

  // Read final block
  readHelp(&disk, &block, blockRead);
  memcpy(buf + len - tempLen, blockRead, tempLen);
  
  return len;
}

int mdadm_write(uint32_t addr, uint32_t len, const uint8_t *buf) {
  // Parameter check
  if(invalidJbodParams(addr, len, buf)){
    return -1;
  }
  else if(buf == NULL){
    return len;
  }

  // Move to write location
  uint32_t disk = locateDisk(addr);
  uint32_t block = locateBlock(addr);
  seekLoc(disk, block);

  // Initialize writing buffer and initial write
  uint8_t blockWrite[JBOD_BLOCK_SIZE];
  jbod_operation(JBOD_READ_BLOCK, blockWrite);
  seekLoc(disk, block); // *** Seems unnecessary
  
  uint32_t startInBlock = addr % JBOD_BLOCK_SIZE;
  uint32_t lenInBlock1 = JBOD_BLOCK_SIZE - startInBlock;

  // Full write within block bounds
  if((startInBlock + len) <= JBOD_BLOCK_SIZE){
    memcpy(blockWrite + startInBlock, buf, len);
    jbod_operation(JBOD_WRITE_BLOCK, blockWrite);
    return len;
  }

  // Write first block
  memcpy(blockWrite + startInBlock, buf, lenInBlock1);
  jbod_operation(JBOD_WRITE_BLOCK, blockWrite);
  block += 1;
  incrementDiskCheck(&disk, &block);
  
  // Write intermediate blocks
  uint32_t tempLen = len - lenInBlock1;
  while(tempLen > JBOD_BLOCK_SIZE){
    memcpy(blockWrite, buf + len - tempLen, JBOD_BLOCK_SIZE);
    jbod_operation(JBOD_WRITE_BLOCK, blockWrite);
    block += 1;
    // Increments
    incrementDiskCheck(&disk, &block);
    tempLen -= JBOD_BLOCK_SIZE;
    }

  // Write final block
  jbod_operation(JBOD_READ_BLOCK, blockWrite);
  seekLoc(disk, block);
  memcpy(blockWrite, buf + len - tempLen, tempLen);
  jbod_operation(JBOD_WRITE_BLOCK, blockWrite);

  return len;
}

int invalidJbodParams(uint32_t addr, uint32_t len, const uint8_t *buf){
  // Invalid Parameters
  return (status == -1 || addr > (MAX_SIZE - len) || len > 1024 || (len > 0 && buf == NULL));
}

uint32_t locateDisk(uint32_t addr){
  return (uint32_t)(addr/JBOD_DISK_SIZE);
}

uint32_t locateBlock(uint32_t addr){
  return (uint32_t)((addr % JBOD_DISK_SIZE) / JBOD_BLOCK_SIZE);
}

void seekLoc(uint32_t disk, uint32_t block){
  uint32_t locarg = (disk << 20) | (block << 24);
  uint32_t seekDiskOp = locarg | JBOD_SEEK_TO_DISK;
  uint32_t seekBlockOp = locarg | JBOD_SEEK_TO_BLOCK;
  jbod_operation(seekDiskOp,NULL);
  jbod_operation(seekBlockOp,NULL);
}

void incrementDiskCheck(uint32_t *disk, uint32_t *block){
  if(*block >= JBOD_NUM_BLOCKS_PER_DISK){
    *disk += 1;
    *block = 0;
    seekLoc(*disk, *block);
  }
}

void readHelp(uint32_t *disk, uint32_t *block, uint8_t *blockRead){
  if (cache_lookup(*disk, *block, blockRead) == 1){
    return;
  }
  incrementDiskCheck(disk, block);
  seekLoc(*disk, *block);
  jbod_operation(JBOD_READ_BLOCK, blockRead);
  if (!cache_enabled()){
    return;
  }
  cache_insert(*disk, *block, blockRead);
}/*CWD /home/ardotsey4/cmpsc311/sp25-lab4-ARDotsey4 */
