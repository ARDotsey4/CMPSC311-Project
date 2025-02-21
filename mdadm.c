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

int mdadm_mount(void) {
  // Already mounted - fail
  if(status == 1){;
    return -1;
  }
  // Mount success
  status*=-1;
  return 1;
}

int mdadm_unmount(void) {
  // Already unmounted - fail
  if(status == -1){
    return -1;
  }
  // Unmount success
  status*=-1;
  return 1;
}

int mdadm_read(uint32_t addr, uint32_t len, uint8_t *buf) {
  if(status == -1 || len >= 1024){
    return -1;
  }
  return len;
}
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
/*CWD /home/ardotsey4/cmpsc311/sp2-lab2-ARDotsey4 */
