#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "monothread.h"
#include "multithread.h"

void printUsage(char* command) {
	printf("Usage: %s [threads]\n", command);
	printf("\tthreads: number of threads to start, at least one\n");
}

int main(int argc, char** argv) {
	int threads=1;
	int status;
	
	// parse command line arguments
	if(argc!=1 && argc!=2) {
		printUsage(argv[0]);
		return EXIT_FAILURE;
	}
	if(argc==2) {
		if(!strcmp(argv[1], "-h")) {
			printUsage(argv[0]);
			return EXIT_SUCCESS;
		}
		else threads=atoi(argv[1]);
	}
	if(threads<1) {
		printUsage(argv[0]);
		return EXIT_FAILURE;
	}
	
	// load primes from disk
	printf("Loading from disk...");
	if((status=readDisk())) {
		printf("Fail!\n");
		printf("Error code: %d\n", status);
		return EXIT_FAILURE;
	}
	printf("\tDone.\n");
	
	// dump primes
	printf("Dumping primes:\n");
	for(size_t i=0; i<vector_length(primes); i++) {
		int prime=0;
		vector_get(primes, i, &prime);
		printf("%d\t", prime);
		if(i%4==0) printf("\n");
	}
	printf("\n");
	
	// run the thing
	printf("Starting with thread count: %d\n", threads);
	if(threads==1) status=runMono();
	else status=runMulti();
	printf("Ended with status %d\n", status);
	
	// write primes to disk
	printf("Writing to disk...");
	if((status=writeDisk())) {
		printf("Fail!\n");
		printf("Error code: %d\n", status);
		return EXIT_FAILURE;
	}
	printf("\tDone.\n");
	
	return EXIT_SUCCESS;
}
