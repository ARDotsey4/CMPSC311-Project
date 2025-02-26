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
  status = 0;
  return 1;
}

int mdadm_read(uint32_t addr, uint32_t len, uint8_t *buf) {
  if(status == -1 || addr > (MAX_SIZE - len) || len >= 1024 || (len != 0 && buf == NULL)){
    return -1;
  }
  return len;
}

int locateDisk(uint32_t addr){
  return 1 + (addr + 1)/(JBOD_NUM_BLOCKS_PER_DISK * JBOD_BLOCK_SIZE);
}

int locateBlock(uint32_t addr){
  return (addr % (JBOD_NUM_BLOCKS_PER_DISK * JBOD_BLOCK_SIZE));
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
