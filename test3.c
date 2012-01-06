// This program exploits Nehalem's L1 data cache size of 64 bytes by using
// two threads to fill an array. If the array is contained in a single 64 byte
// line it will be pulled into two L1 caches even though each thread operates
// on half of the array. A 128 byte array can be split into two 64 byte arrays,
// so each core works on a 64 byte slice of memory in its own cache.
//
// Fewer cache misses are experienced when run with LEN 128 as each thread/core
// has its own slice of the array contained it its L1 cache. With LEN 64, each
// thread/core must access remote memory, increasing cache misses. Doubling the
// array has an effect on running time, the percentage of cache misses is more
// important in this test.

#include <pthread.h>

#define N   1000 // Times to write a value to array
#define LEN 64   // Length of the array

char arr[N];

void *fill (void *val);

int main (void)
{
    // Use two threads to fill the array in parallel.
    long int a = 0, b = LEN / 2;
    pthread_t t1, t2;
    pthread_create (&t1, NULL, fill, (void *) a);
    pthread_create (&t2, NULL, fill, (void *) b);
    pthread_join (t1, NULL);
    pthread_join (t2, NULL);

    return 0;
}

// Fill contents of ARR at index START onward.
void *fill (void *start)
{
    long int x = (long int ) start;
    for (; x < (LEN / 2) + (long int) start; x++) {
        int i;
        for (i = 0; i < N; i++)
            arr[x] = 'A';
    }
    return 0;
}
