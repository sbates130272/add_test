# add_test : Search through sorted arrays

## Introduction

There is an interesting problem around determining if, in a set A,
there exists members a[i] and a[j] such that a[i]+a[j]=c. It is quite
obvious to see that this existance test can be solved O(n^2) using
brute force (where n is the size of set A).

Things get more interested when A is sorted such that a[i+1]>=a[i] for
all i in 0 to n-1. In this case lower complexity existance algorithms
can be found.

Even more interesting is when the elements of A are drawn from some
form of random distribution. For example if a[i+1] = a[1] + g where g
is uniformly distributed over the range 0 to G then certain fun bounds
can be placed on the probable span of A (a[n-1]-a[0]). One can also
show that there are limits that show that the existance of a[i]+a[j]=c
tend to 1 when certain restrictions are placed on n, g and c. This
code allows one to explore this fun space.

## Running the code

The C code can be built by typing make. It is pretty portable and
should compile on most machines. You can run the code with 0 arguments
(see defaults in the C code) or with five arguments:

./add_test <seed> <n> <start> <gap> <loops>

When seed is 0 a random seed based on the current time is
generated. When loops is one a more verbose output is
generarted. There is a pre-processor define in the code calledf BRUTE
which, when set, runs the O(n^2) as a comparison.

There is a helper script called run.

./run

This will generate a results file called add_test.results with average
run time for 1000 loops for a range of sizes of n and put the results
in a file called add_test.results.

## License

Code is delivered as is. Use and abuse as you see fit.
