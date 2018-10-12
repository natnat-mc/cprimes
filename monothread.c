#include "monothread.h"
#include "common.h"

#include <stdio.h>

int runMono(void) {
	// read the amount of primes and the last prime
	size_t beginningSize=vector_length(primes);
	prime_t currentNo=0;
	vector_get(primes, beginningSize-1, &currentNo);
	
	// get current number and size
	size_t currentSize=beginningSize;
	if(currentNo==2) currentNo++;
	else currentNo+=2;
	
	while(1) {
		int ok=1;
		prime_t p=1;
		for(size_t i=0; i<currentSize; i++) {
			vector_get(primes, i, &p);
			if(p*p>currentNo) break;
			if(currentNo%p==0) {
				ok=0;
				break;
			}
		}
		if(ok) {
			vector_push(primes, currentNo);
			currentSize++;
			if((currentSize-beginningSize)%MONO_PERSIST_THRESHOLD==0) {
				printf("New primes, latest is %lld\n", currentNo);
				updateDisk();
			}
		}
		currentNo+=2;
	}
	
	return 0;
}
