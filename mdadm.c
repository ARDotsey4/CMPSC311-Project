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
static int status = -1; // 1 = mounted, 0 = unmounted
const int MAX_SIZE = JBOD_NUM_DISKS*JBOD_BLOCK_SIZE*JBOD_NUM_BLOCKS_PER_DISK;

int mdadm_mount(void) {
  int fail = jbod_operation(JBOD_MOUNT,NULL);
  if (fail){
    return fail;
  }
  status = 1;
  return 1;
}

int mdadm_unmount(void) {
  int fail = jbod_operation(JBOD_UNMOUNT,NULL);
  if (fail){
    return fail;
  }
  status = -1;
  return 1;
}

int mdadm_read(uint32_t addr, uint32_t len, uint8_t *buf) {
  if(status == -1 || addr >= (MAX_SIZE - len) || len >= 1024 || (len > 0 && buf == NULL)){
    return -1;
  }
  else if(buf == NULL){
    return len;
  }
  // Calculate read location
  uint32_t disk = locateDisk(addr);
  uint32_t block = locateDisk(addr);

  // Move to read location
  uint32_t locarg = (disk << 20) | (block << 24);
  uint32_t seekDiskOp = locarg | JBOD_SEEK_TO_DISK;
  uint32_t seekBlockOp = locarg | JBOD_SEEK_TO_BLOCK;
  jbod_operation(seekDiskOp,NULL);
  jbod_operation(seekBlockOp,NULL);

  uint8_t blockRead[1024];
  jbod_operation(JBOD_READ_BLOCK, blockRead);
  
  uint32_t startInBlock = addr % (JBOD_NUM_BLOCKS_PER_DISK * JBOD_BLOCK_SIZE);
  //uint32_t endInBlock = (startInBlock + len) % JBOD_BLOCK_SIZE;
  //uint32_t interBlocks = (len - startInBlock - endInBlock) / JBOD_BLOCK_SIZE;
  uint32_t lenInBlock1 = JBOD_BLOCK_SIZE - startInBlock;

  // Read within block
  if((startInBlock + len) < JBOD_BLOCK_SIZE){ //correct?
    memcpy(buf, blockRead + startInBlock, len);
    return len;
  }
  // Read first block
  memcpy(buf, blockRead + startInBlock, lenInBlock1);

  // Read intermediate blocks
  uint32_t tempLen = len - lenInBlock1;
  while(tempLen >= JBOD_BLOCK_SIZE){
    jbod_operation(JBOD_READ_BLOCK, blockRead);
    memcpy(buf + len - tempLen, blockRead, JBOD_BLOCK_SIZE);
    tempLen -= JBOD_BLOCK_SIZE;
  }

  // Read final block
  jbod_operation(JBOD_READ_BLOCK, blockRead);
  memcpy(buf + len - tempLen, blockRead, tempLen);
  
  return len;
}

uint32_t locateDisk(uint32_t addr){
  return (uint32_t)(addr/(JBOD_NUM_BLOCKS_PER_DISK * JBOD_BLOCK_SIZE));
}

uint32_t locateBlock(uint32_t addr){
  return (uint32_t)((addr % (JBOD_NUM_BLOCKS_PER_DISK * JBOD_BLOCK_SIZE)) / JBOD_BLOCK_SIZE);
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
