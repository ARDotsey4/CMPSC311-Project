/* Author: Armand Dotsey
   Date: 2-23-25
    */

/***
 *      ______ .___  ___. .______     _______.  ______        ____    __   __
 *     /      ||   \/   | |   _  \   /       | /      |      |___ \  /_ | /_ |
 *    |  ,----'|  \  /  | |  |_)  | |   (----`|  ,----'        __) |  | |  | |
 *    |  |     |  |\/|  | |   ___/   \   \    |  |            |__ <   | |  | |
 *    |  `----.|  |  |  | |  |   .----)   |   |  `----.       ___) |  | |  | |
 *     \______||__|  |__| | _|   |_______/     \______|      |____/   |_|  |_|
 *
 */

#include "mdadm.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "jbod.h"
#include "util.h"
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
  // Invalid Parameters
  if(status == -1 || addr >= (MAX_SIZE - len) || len >= 1024 || (len > 0 && buf == NULL)){
    return -1;
  }
  else if(buf == NULL){
    return len;
  }
  
  // Calculate start read location
  uint32_t disk = locateDisk(addr);
  uint32_t block = locateBlock(addr);
  
  // Move to read location
  uint32_t locarg = (disk << 20) | (block << 24);
  uint32_t seekDiskOp = locarg | JBOD_SEEK_TO_DISK;
  uint32_t seekBlockOp = locarg | JBOD_SEEK_TO_BLOCK;
  jbod_operation(seekDiskOp,NULL);
  jbod_operation(seekBlockOp,NULL);

  // Initial read and increment
  uint8_t blockRead[JBOD_BLOCK_SIZE];
  jbod_operation(JBOD_READ_BLOCK, blockRead);
  block += 1;
  
  uint32_t startInBlock = addr % JBOD_BLOCK_SIZE;
  uint32_t lenInBlock1 = JBOD_BLOCK_SIZE - startInBlock;

  // Full read within block
  if((startInBlock + len) < JBOD_BLOCK_SIZE){
    memcpy(buf, blockRead + startInBlock, len);
    return len;
  }
  // Copy in first block
  memcpy(buf, blockRead + startInBlock, lenInBlock1);
  incrementDiskCheck(&disk, &block);
  
  // Read/copy intermediate blocks
  uint32_t tempLen = len - lenInBlock1;
  while(tempLen >= JBOD_BLOCK_SIZE){
    jbod_operation(JBOD_READ_BLOCK, blockRead);
    memcpy(buf + len - tempLen, blockRead, JBOD_BLOCK_SIZE);
    // Increments
    incrementDiskCheck(&disk, &block);
    tempLen -= JBOD_BLOCK_SIZE;
    block += 1;
    }

  // Read/copy final block
  jbod_operation(JBOD_READ_BLOCK, blockRead);
  memcpy(buf + len - tempLen, blockRead, tempLen);
  
  return len;
}

uint32_t locateDisk(uint32_t addr){
  return (uint32_t)(addr/JBOD_DISK_SIZE);
}

uint32_t locateBlock(uint32_t addr){
  return (uint32_t)((addr % JBOD_DISK_SIZE) / JBOD_BLOCK_SIZE);
}

void incrementDiskCheck(uint32_t *disk, uint32_t *block){
  if(*block >= JBOD_NUM_BLOCKS_PER_DISK){
    *disk += 1;
    *block = 0;
    uint32_t seekDiskOp = (*disk << 20) | (*block << 24) | JBOD_SEEK_TO_DISK;
    jbod_operation(seekDiskOp,NULL);
    jbod_operation(JBOD_SEEK_TO_BLOCK,NULL);
  }
}
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
