
#include <stdlib.h>
#include <stdio.h>

struct cache_blk_t {
  unsigned long tag;
  char valid;
  char dirty;
  unsigned long long ts;	//a timestamp that may be used to implement LRU replacement
  // To guarantee that L2 is inclusive of L1, you may need an additional flag
  // in L2 to indicate that the block is cached in L1
};

struct cache_t {
	// The cache is represented by a 2-D array of blocks.
	// The first dimension of the 2D array is "nsets" which is the number of sets (entries)
	// The second dimension is "assoc", which is the number of blocks in each set.
  int nsets;				// # sets
  int blocksize;			// block size
  int assoc;				// associativity
  int hit_latency;			// latency in case of a hit
  struct cache_blk_t **blocks;    // the array of cache blocks
};

struct cache_t *
	cache_create(int size, int blocksize, int assoc, int latency)
{
  int i;			// number of sets (entries) in the cache

  int sizeInBytes = size * 1024;
  int nblocks = sizeInBytes/blocksize;
  int nsets = nblocks/assoc;


  // YOUR JOB: calculate the number of sets and blocks in the cache
  //
  // nblocks = X;
  // nsets = Y;

  struct cache_t *C = (struct cache_t *)calloc(1, sizeof(struct cache_t));

  C->nsets = nsets;
  C->assoc = assoc;
  C->hit_latency = latency;

  C->blocks= (struct cache_blk_t **)calloc(nsets, sizeof(struct cache_blk_t));

  for(i = 0; i < nsets; i++) {
		C->blocks[i] = (struct cache_blk_t *)calloc(assoc, sizeof(struct cache_blk_t));
    }

  return C;
}

int cache_access(struct cache_t *cp, unsigned long address,
			 char access_type, unsigned long long now, struct cache_t *next_cp, int mem_latency)
{
    if(cp == NULL)
    {
        return mem_latency;
    }
    int index = (address/(cp->blocksize))%(cp->nsets);
    unsigned long tag = (address/(cp->blocksize))/(cp->nsets);
    if(acccess_type == 'r')
    {
        int i;
        for(i = 0; i < cp->assoc; i++)
        {
            if((cp->blocks[index][i]->valid) && (cp->blocks[index][i]->tag == tag))
            {
                cp->blocks[index][i]->ts = now;
                return(cp->hit_latency);
            }
        }
        int LRUIndex = 0;
        for(i = 0; i < cp->assoc; i++)
        {
            if(!(cp->blocks[index][i]->valid))
            {
                cp->blocks[index][i]->tag = tag;
                cp->blocks[index][i]->valid = '1';
                cp->blocks[index][i]->ts = now;
                return cache_access(next_cp, address, access_type, now, NULL, mem_latency) + cp->hit_latency;
            }
            if(cp->blocks[index][i]->ts < cp->blocks[index][LRUIndex])
            {
                LRUIndex = i;
            }
        }
        cp->blocks[index][LRUIndex]->tag = tag;
        cp->blocks[index][LRUIndex]->valid = '1';
        cp->blocks[index][LRUIndex]->ts = now;
        return cache_access(next_cp, address, access_type, now, NULL, mem_latency) + cp->hit_latency;


    }

	return(cp->hit_latency);
}
