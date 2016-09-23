#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <sys/time.h>

#define NUM_ARGS 5

static long elapsed(struct timeval t0, struct timeval t1)
{
	return (t1.tv_sec-t0.tv_sec)*1000000 +
		t1.tv_usec-t0.tv_usec;
}

static unsigned int gen_data(int *a, int n, int start, unsigned gap)
{
	a[0] = start;
	for (unsigned i=1;i<n;i++)
		a[i] = a[i-1] + ( rand() % gap );
	return n;
}

static unsigned int add_test(int *a, int n, int c)
{
	int low = 0, high = n-1;

	/* sanity test */
	if ( 2*a[low] > c || 2*a[high] < c )
		return 0;

	while (1)
	{
		int val = a[low] +a [high];
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

static unsigned int add_test_brute(int *a, int n, int c)
{
	for (unsigned i=0; i<n-1; i++){
		for (unsigned j=0; j<n-1; j++){
			if ( a[i]+a[j]==c )
				return 1;
		}
	}

	return 0;
}

static int get_target(int *a, int n)
{
	/* The target is a uniformly random value between 2*a[0] and 
	 * 2*a[n-1]. */

	int range;

	range = 2*a[n-1] - 2*a[0];
	range = rand() % range-2*a[0];

	return range + 2*a[0];
}

static void report(unsigned int seed, unsigned int n, int start,
		   unsigned int gap, int c, unsigned int result,
		   struct timeval starttime, struct timeval finishtime)
{
	fprintf(stdout,"add_test\n");
	fprintf(stdout,"--------\n\n");
	fprintf(stdout,"seed  = %20d\n", seed);
	fprintf(stdout,"n     = %20d\n", n);
	fprintf(stdout,"start = %20d\n", start);
	fprintf(stdout,"gap   = %20d\n", gap);
	fprintf(stdout,"c     = %20d\n\n", c);
	fprintf(stdout,"Processed %d elements in %ld us\n",
		n, elapsed(starttime, finishtime));
	if ( result )
		fprintf(stdout, "%d was FOUND!\n", c);
	else
		fprintf(stdout, "%d was NOT FOUND!\n", c);
	
}

int main(int argc, char **argv)
{
	unsigned int seed=0, n=1000, result, result_brute;
	int start=0, gap=10, c, *a;
	struct timeval starttime, finishtime;

	if ( argc>1 && argc!=NUM_ARGS ) {
		fprintf(stderr,"Usage Error: %s <seed> "\
			"<n> <start> <gap>\n",
			argv[0]);
		exit(-1);
	}

	if (argc==NUM_ARGS) {
		seed  = atoi(argv[1]);
		n     = atoi(argv[2]);
		start = atoi(argv[3]);
		gap   = atoi(argv[4]);
	}
	
	a = malloc(n*sizeof(*a));
	if ( a==NULL ) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(-1);
	}

	if ( seed==0 )
		seed = time(NULL);
	srand(seed);

	gen_data(a, n, start, gap);

	c = get_target(a, n);

	gettimeofday(&starttime, 0);
	result = add_test(a, n, c);
	gettimeofday(&finishtime, 0);

#ifdef BRUTE	
	result_brute = add_test_brute(a, n, c);

	if (result != result_brute){
		fprintf(stderr,"Mismatch in algorithms!\n");
		exit(-1);
	}
#endif
	report(seed, n, start, gap, c, result, starttime, finishtime);
	
	
	free(a);
	return 0;
}
      
      
