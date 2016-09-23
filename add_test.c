#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <sys/time.h>

#define NUM_ARGS 6

static long elapsed(struct timeval t0, struct timeval t1)
{
	return (t1.tv_sec-t0.tv_sec)*1000000 +
		t1.tv_usec-t0.tv_usec;
}

static long sum(long *a, unsigned int n)
{
	long ret = 0;
	for (unsigned i=0; i<n; i++)
		ret += a[i];

	return ret;
}

static unsigned int gen_data(long *a, int n, long start, unsigned long gap)
{
	a[0] = start;
	for (unsigned i=1;i<n;i++)
		a[i] = a[i-1] + ( rand() % gap );
	return n;
}

static unsigned int add_test(long *a, int n, long c)
{
	int low = 0, high = n-1;

	/* sanity test */
	if ( 2*a[low] > c || 2*a[high] < c )
		return 0;

	while (1)
	{
		long val = a[low] +a [high];
		if ( val == c )
			return 1;
		else if ( val < c )
			low++;
		else if ( val > c )
			high--;

		if (low == high)
			break;
	}
	
	return 0;
}

static unsigned int add_test_brute(long *a, int n, long c)
{
	for (unsigned i=0; i<n-1; i++){
		for (unsigned j=0; j<n-1; j++){
			if ( a[i]+a[j]==c )
				return 1;
		}
	}

	return 0;
}

static int get_target(long *a, int n)
{
	/* The target is a uniformly random value between 2*a[0] and 
	 * 2*a[n-1]. */

	int range;

	range = 2*a[n-1] - 2*a[0];
	range = rand() % range-2*a[0];

	return range + 2*a[0];
}

static void report(unsigned int seed, unsigned int n, long start,
		   unsigned long gap, long c, unsigned int result,
		   struct timeval starttime, struct timeval finishtime)
{
	fprintf(stdout,"add_test\n");
	fprintf(stdout,"--------\n\n");
	fprintf(stdout,"seed  = %20d\n", seed);
	fprintf(stdout,"n     = %20d\n", n);
	fprintf(stdout,"start = %20ld\n", start);
	fprintf(stdout,"gap   = %20ld\n", gap);
	fprintf(stdout,"c     = %20ld\n\n", c);
	fprintf(stdout,"Processed %d elements in %ld us\n",
		n, elapsed(starttime, finishtime));
	if ( result )
		fprintf(stdout, "%ld was FOUND!\n", c);
	else
		fprintf(stdout, "%ld was NOT FOUND!\n", c);
	
}

static void report_loops(unsigned int seed, unsigned int n, long start,
			 unsigned long gap, unsigned int loops, long *stats)
{
	fprintf(stdout,"add_test\n");
	fprintf(stdout,"--------\n\n");
	fprintf(stdout,"seed  = %20d\n", seed);
	fprintf(stdout,"n     = %20d\n", n);
	fprintf(stdout,"start = %20ld\n", start);
	fprintf(stdout,"gap   = %20ld\n\n", gap);
	fprintf(stdout,"Processed %d loops of %d elements in %ld us\n",
		loops, n, sum(stats, loops));
	fprintf(stdout,"Average = %f us\n",
		(float)sum(stats, loops)/loops);
}

int main(int argc, char **argv)
{
	unsigned int seed=0, n=1000, loops=1, result, result_brute;
	long start=0, gap=10, c, *a;
	long *stats;
	struct timeval starttime, finishtime;

	if ( argc>1 && argc!=NUM_ARGS ) {
		fprintf(stderr,"Usage Error: %s <seed> "\
			"<n> <start> <gap> <loops>\n",
			argv[0]);
		exit(-1);
	}

	if (argc==NUM_ARGS) {
		seed  = atoi(argv[1]);
		n     = atoi(argv[2]);
		start = atoi(argv[3]);
		gap   = atoi(argv[4]);
		loops = atoi(argv[5]);
	}
	
	a = malloc(n*sizeof(*a));
	if ( a==NULL ) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(-1);
	}

	if ( seed==0 )
		seed = time(NULL);
	srand(seed);

	if ( loops > 1 ){
		stats = malloc(loops*sizeof(long));
		if ( a==NULL ) {
			fprintf(stderr, "Cannot allocate memory\n");
			exit(-1);
		}
	}
	
	for (unsigned loop=0 ; loop<loops; loop++){

		gen_data(a, n, start, gap);

		c = get_target(a, n);

		gettimeofday(&starttime, 0);
		result = add_test(a, n, c);
		gettimeofday(&finishtime, 0);

		if ( loops > 1 )
			stats[loop] = elapsed(starttime, finishtime);

#ifdef BRUTE
		result_brute = add_test_brute(a, n, c);
		if (result != result_brute){
			fprintf(stderr,"Mismatch in algorithms!\n");
			exit(-1);
		}
#endif
	}
	if ( loops==1 )
		report(seed, n, start, gap, c, result, starttime, finishtime);
	else {
		report_loops(seed, n, start, gap, loops, stats);
	}

	if ( loops > 1 )
		free(stats);
	free(a);
	return 0;
}
      
      
