#include "common.h"

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//BEGIN vector implementation

vector_t vector_create(size_t initialSize) {
	// sanity checks
	if(initialSize<=0u) return (vector_t) 0;
	
	// object allocation
	vector_t vector=malloc(sizeof(struct vector_st));
	if(!vector) return (vector_t) 0;
	
	// array allocation
	prime_t* items=calloc(initialSize, sizeof(prime_t));
	if(!items) {
		free(vector);
		return (vector_t) 0;
	}
	
	// final touch
	vector->items=items;
	vector->size=vector->allocSize=initialSize;
	return vector;
}

int vector_set(vector_t vector, size_t index, prime_t value) {
	// sanity checks
	if(!vector) return -1;
	
	// auto-resize
	if(index>=vector->size) {
		if(vector_resize(vector, index+1)) return -1;
	}
	
	// set value
	vector->items[index]=value;
	return 0;
}

int vector_resize(vector_t vector, size_t newSize) {
	// sanity checks
	if(newSize<=0u || !vector) return -1;
	
	// reallocate if we don't have enough room
	if(newSize>vector->allocSize) {
		size_t newAlloc=newSize+VECTOR_BUMP;
		prime_t* ptr=reallocarray(vector->items, newAlloc, sizeof(prime_t));
		if(ptr) vector->items=ptr, vector->allocSize=newAlloc;
		else return -1;
	}
	
	// discard excess values
	if(newSize<vector->size) {
		memset(&vector->items[vector->size], 0, sizeof(prime_t)*(newSize-vector->size));
	}
	
	// resize the vector
	vector->size=newSize;
	return 0;
}

int vector_prealloc(vector_t vector, size_t min, size_t add) {
	// sanity checks
	if(!vector) return -1;
	
	// get the required size
	size_t newSize;
	if(min>vector->size+add) newSize=min;
	else newSize=vector->size+add;
	
	// allocate accordingly
	if(newSize>vector->allocSize) {
		prime_t* ptr=reallocarray(vector->items, newSize, sizeof(prime_t));
		if(ptr) vector->items=ptr, vector->allocSize=newSize;
		else return -1;
	}
	
	return 0;
}

int vector_push(vector_t vector, prime_t value) {
	//TODO improve this
	return vector_set(vector, vector->size, value);
}

int vector_pop(vector_t vector, prime_t* value) {
	// sanity checks
	if(!vector) return -1;
	
	// read the value
	if(vector_get(vector, vector->size, value)) return -1;
	
	// shrink the vector by one
	vector->size--;
	return 0;
}

//END vector implementation

vector_t primes;
int primesFd;

int readDisk(void) {
	// open the file and get its size
	primesFd=open(PRIMES_FILENAME, O_RDWR);
	if(primesFd==-1) primesFd=open(PRIMES_FILENAME, O_CREAT | O_RDWR, 0644);
	if(primesFd==-1) {
		perror("open");
		return -1;
	}
	int size=lseek(primesFd, 0, SEEK_END);
	lseek(primesFd, 0, SEEK_SET);
	if(size==-1) {
		perror("lseek");
		return -2;
	}
	
	// ensure the file isn't empty
	if(size==0) {
		prime_t i=2;
		write(primesFd, &i, sizeof(prime_t));
		lseek(primesFd, 0, SEEK_SET);
		size=sizeof(prime_t);
	}
	
	// create a vector with the data
	primes=vector_create(size/sizeof(prime_t));
	if(!primes) return -3;
	for(size_t i=0; i<(size_t)(size/sizeof(prime_t)); i++) {
		prime_t p;
		read(primesFd, &p, sizeof(prime_t));
		if(vector_set(primes, i, p)) return -3;
	}
	return 0;
}

int updateDisk(void) {
	// seek to the end of file
	int size=lseek(primesFd, 0, SEEK_END);
	if(size==-1) return -1;
	
	// write new primes
	for(size_t i=size/sizeof(prime_t); i<vector_length(primes); i++) {
		prime_t prime=0;
		vector_get(primes, i, &prime);
		write(primesFd, &prime, sizeof(prime_t));
	}
	return 0;
}

int writeDisk(void) {
	// empty the file
	if(ftruncate(primesFd, 0)) return -2;
	
	// update the file
	if(updateDisk()) return -1;
	
	// close the file
	return close(primesFd);
}
