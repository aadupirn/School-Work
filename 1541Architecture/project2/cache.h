
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
  int hits;
  int misses;
  struct cache_blk_t **blocks;    // the array of cache blocks
};

struct cache_t *
	cache_create(int size, int blocksize, int assoc, int latency)
{
  int i;			// number of sets (entries) in the cache

  int sizeInBytes = size*1024;
  int nblocks = sizeInBytes/blocksize;
  int nsets = nblocks/assoc;

  struct cache_t *C = (struct cache_t *)calloc(1, sizeof(struct cache_t));

  C->nsets = nsets;
  C->assoc = assoc;
  C->blocksize = blocksize;
  C->hit_latency = latency;
  C->hits = 0;
  C->misses = 0;

  C->blocks= (struct cache_blk_t **)calloc(nsets, sizeof(struct cache_blk_t));

  for(i = 0; i < nsets; i++) {
		C->blocks[i] = (struct cache_blk_t *)calloc(assoc, sizeof(struct cache_blk_t));
    }

  return C;
}

int cache_access(struct cache_t *cp, unsigned long address,
			 char access_type, unsigned long long now, struct cache_t *next_cp, int mem_latency, char accessSource)
{
    if(cp == NULL)
    {
        return mem_latency;
    }

    int index = (address/(cp->blocksize))%(cp->nsets);
    unsigned long tag = (address/(cp->blocksize))/(cp->nsets);
    int i;

    if(access_type == 'r')
    {
        for(i = 0; i < cp->assoc; i++)
        {
            if((cp->blocks[index][i].valid) && (cp->blocks[index][i].tag == tag))
            {
                cp->blocks[index][i].ts = now;
                if(accessSource == '1')
                {
                    cp->hits = cp->hits + 1;
                }
                return(cp->hit_latency);
            }
        }
        if(accessSource == '1')
        {
            cp->misses = cp->misses + 1;
        }
        int LRUIndex = 0;
        for(i = 0; i < cp->assoc; i++)
        {
            if(!(cp->blocks[index][i].valid))
            {
                cp->blocks[index][i].tag = tag;
                cp->blocks[index][i].valid = '1';
                cp->blocks[index][i].dirty = '0';
                cp->blocks[index][i].ts = now;
                return cache_access(next_cp, address, access_type, now, NULL, mem_latency, accessSource) + cp->hit_latency;
            }
            if(cp->blocks[index][i].ts < cp->blocks[index][LRUIndex].ts)
            {
                LRUIndex = i;
            }
        }
        int writeBackPenalty = 0;
        if(cp->blocks[index][LRUIndex].dirty == '1')
        {
            writeBackPenalty = cache_access(next_cp, address, 'w', now, NULL, mem_latency, '0');
        }
        cp->blocks[index][LRUIndex].tag = tag;
        cp->blocks[index][LRUIndex].valid = '1';
        cp->blocks[index][LRUIndex].dirty = '0';
        cp->blocks[index][LRUIndex].ts = now;
        return cache_access(next_cp, address, access_type, now, NULL, mem_latency, accessSource) + cp->hit_latency + writeBackPenalty;
    }
    else
    {
        for(i = 0; i < cp->assoc; i++)
        {
            if((cp->blocks[index][i].valid) && (cp->blocks[index][i].tag == tag))
            {
                cp->blocks[index][i].ts = now;
                if(accessSource == '1')
                {
                    cp->hits = cp->hits + 1;
                }
                cp->blocks[index][i].dirty = '1';
                return(cp->hit_latency);
            }
        }
        int LRUIndex = 0;
        if(accessSource == '1')
        {
            cp->misses = cp->misses + 1;
        }
        for(i = 0; i < cp->assoc; i++)
        {
            if(!(cp->blocks[index][i].valid))
            {
                cp->blocks[index][i].tag = tag;
                cp->blocks[index][i].valid = '1';
                cp->blocks[index][i].dirty = '1';
                cp->blocks[index][i].ts = now;
                if(cp->blocksize > 4)
                {
                    return cache_access(next_cp, address, 'r', now, NULL, mem_latency, accessSource) + cp->hit_latency;
                }
                else
                {
                    return (cp->hit_latency);
                }
            }
            if(cp->blocks[index][i].ts < cp->blocks[index][LRUIndex].ts)
            {
                LRUIndex = i;
            }
        }
        int writeBackPenalty = 0;
        if(cp->blocks[index][LRUIndex].dirty == '1')
        {
            writeBackPenalty = cache_access(next_cp, address, 'w', now, NULL, mem_latency, 0);
        }
        cp->blocks[index][LRUIndex].tag = tag;
        cp->blocks[index][LRUIndex].valid = '1';
        cp->blocks[index][LRUIndex].dirty = '1';
        cp->blocks[index][LRUIndex].ts = now;
        if(cp->blocksize > 4)
        {
            return cache_access(next_cp, address, 'r', now, NULL, mem_latency, accessSource) + cp->hit_latency + writeBackPenalty;
        }
        else
        {
            return (cp->hit_latency + writeBackPenalty);
        }
    }

	return(cp->hit_latency);
}
