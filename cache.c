#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cache.h"

static cache_entry_t *cache = NULL;
static int cache_size = 0;
static int clock = 0;
static int num_queries = 0;
static int num_hits = 0;

int cache_create(int num_entries) {
  // Cache state and parameter verification
  if((cache_enabled()) || (num_entries<2) || (num_entries>4096)){
    return -1;
  }
  // Allocate to heap with initialization to invalid for all entries
  cache = calloc(sizeof(cache_entry_t),num_entries);
  cache_size = num_entries;
  return 1;
}

int cache_destroy(void) {
  if(!cache_enabled()){
    return -1;
  }
  // Free from heap
  free(cache);
  cache = NULL;
  cache_size = 0;
  return 1;
}

int cache_lookup(int disk_num, int block_num, uint8_t *buf) {
  // Parameter check
  if (invalidCacheParams(disk_num, block_num, buf)){
    return -1;
  }

  clock+=1;
  num_queries+=1;

  int loc = existingEntry(disk_num, block_num);
  // Return failure if entry not in cache
  if (loc == -1){
    return -1;
  }

  // Otherwise copy from cache to buffer at loc
  memcpy(buf, cache[loc].block, JBOD_BLOCK_SIZE);
  num_hits+=1;
  cache[loc].access_time = clock;

  return 1;
}

void cache_update(int disk_num, int block_num, const uint8_t *buf) {
  // Parameter check
  if (invalidCacheParams(disk_num, block_num, buf)){
    return;
  }

  int updateLoc = existingEntry(disk_num, block_num);
  // Return failure if entry not in cache
  if (updateLoc == -1){
    return;
  }

  // Update cache from buffer at loc
  memcpy(cache[updateLoc].block, buf, JBOD_BLOCK_SIZE);
  cache[updateLoc].access_time = clock;
}

int cache_insert(int disk_num, int block_num, const uint8_t *buf) {
  // Parameter check
  if (invalidCacheParams(disk_num, block_num, buf)){
    return -1;
  }

  // Entry already exists
  if(existingEntry(disk_num, block_num) != -1){
    cache_update(disk_num, block_num, buf);
    return 1;
  }

  // Entry does not exist
  int replaceLoc = -1;
  int i = 0;

  // Find location of least recently used entry in valid cache
  while (i < cache_size && cache[i].valid == true){
    if (replaceLoc == -1 || cache[i].access_time < cache[replaceLoc].access_time){
      replaceLoc = i;
    }
    i++;
  }

  // If cache not full, set replacement for first invalid entry
  if (i < cache_size){
    replaceLoc = i;
  }

  // Replace entry at replaceLoc with new insert
  cache[replaceLoc].valid = true;
  cache[replaceLoc].disk_num = disk_num;
  cache[replaceLoc].block_num = block_num;
  memcpy(cache[replaceLoc].block, buf, JBOD_BLOCK_SIZE);
  cache[replaceLoc].access_time = clock;

  return 1;
}

bool cache_enabled(void) {
  return cache_size > 1;
}

void cache_print_hit_rate(void) {
  fprintf(stderr, "Hit rate: %5.1f%%\n", 100 * (float) num_hits / num_queries);
}

int invalidCacheParams(int disk_num, int block_num, const uint8_t *buf){
  if (!cache_enabled() || disk_num > 15 || disk_num < 0 || block_num > 255 || block_num < 0 || buf == NULL){
    return 1;
  }
  return 0;
}

int existingEntry(int disk_num, int block_num){
  int i = 0;
  // Searches for entry with specified disk and block number
  while (i<cache_size && cache[i].valid==true){
    if (cache[i].disk_num == disk_num && cache[i].block_num == block_num){
      return i;
    }
    i++;
  }
  // Fails if no entry with specified parameters is found
  return -1;
}