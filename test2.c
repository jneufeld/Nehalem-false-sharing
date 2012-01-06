// This program exploits Nehalem's L1 data cache line of 64 bytes by using two
// threads to increment a test variable, allocating memory in both a single 64
// byte line and in two separate lines. To ensure 8 byte variables, long int is
// used for test variables. Changing the filler length from 56 to 48 bytes
// places both test variables in the same cache line and testing confirms the 64
// byte line size.
//
// When run without the filler, the test variables are in the same 64 byte line
// and performance suffers in both cache misses and running time. With the
// filler, each thread can run on a distinct core and pull in the memory
// containing it's respective test variable, decreasing cache misses and running
// time.

#include <pthread.h>

#define N 1000000 // Number of iterations

void *iterate (void *val);

int main (void)
{
    // Allocate 'a' at the beginning of a 64 byte slice and add filler, 'f', so
    // 'b' is at the beginning of a different slice. Comment out 'f' to test
    // without filler.
    long int a = 0;
    char f[56];
    long int b = 0;

    // Ensure compiler allocates filler.
    int i;
    for (i = 0; i < 56; i++)
        f[i] = 0;

    // Repeatedly increment 'a' and 'b' in separate threads.
    pthread_t t1, t2;
    pthread_create (&t1, NULL, iterate, (void *) &a);
    pthread_create (&t2, NULL, iterate, (void *) &b);
    pthread_join (t1, NULL);
    pthread_join (t2, NULL);

    return 0;
}

// Increment value given, i.e. 'a' or 'b', then yield, forcing CPU to pull a
// different 64 byte slice of memory into L1 cache.
void *iterate (void *val)
{
    long int *x = val;
    for (; *x < N; (*x)++);
    return 0;
}
