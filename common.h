/**
 * Common functions and macros for the prime calculator
 * Used by both the monothread and the multithread versions
 */

#ifndef __COMMON_H
#define __COMMON_H

#include <stddef.h>
typedef long long prime_t;

//BEGIN vector class

/**
 * A vector is an array that scales automagically when needed
 * The `vector_t` type should be treated as opaque
 * Operations on a vector:
 * - set
 * - get
 * - length
 * - resize
 * - prealloc
 * - push
 * - pop
 */
typedef struct vector_st{
	size_t size;
	size_t allocSize;
	prime_t* items;
}* vector_t;

/** vector_t vector_create(size_t initialSize)
 * @Param initialSize the initial size of the vector
 * @Returns a vector object, or NULL on error
 * Creates a vector with a given size
 * This function is guaranteed to fail if the size is negative or null
 */
vector_t vector_create(size_t initialSize);

/** int vector_destroy(vector_t vector)
 * @Param vector the vector to testroy
 * @Returns 0 on success, -1 on failure
 * Destroys a vector
 * The destroyed vector must no longer be used
 */
#define vector_destroy(vector) ((vector)?(free((vector)->items), free(vector), 0):-1)

/** int vector_set(vector_t vector, size_t index, prime_t value)
 * @Param vector the vector on which to operate
 * @Param index the index at which set the value
 * @Param value the value to store into the vector
 * @Returns 0 on success, -1 on failure
 * Sets a value in a vector
 * This function is guaranteed to fail if the index is negative
 * If the index is within the vector, then the value at that index is overwritten
 * If the index is outside of the vector, then the vector will be resized first
 */
int vector_set(vector_t vector, size_t index, prime_t value);

/** int vector_get(vector_t vector, size_t index, prime_t* value)
 * @Param vector the vector on which to operate
 * @Param index the index at which to read
 * @Param value the pointer to update
 * @Returns 0 on success, -1 on failure
 * Reads a value from a vector
 * This function is guaranteed to fail if the index is outside of the vector
 * The value is read into the given pointer, not returned
 */
#define vector_get(vector, index, value) (((vector)&&((vector)->size)>(index))?(*(value)=(vector)->items[(index)], 0): -1)

/** size_t vector_size(vector)
 * @Param vector the vector of which to read the size
 * @Returns the size of the vector on success, 0 on error
 * Reads the size of a vector
 */
#define vector_length(vector) ((vector)?(vector)->size:0)

/** int vector_resize(vector_t vector, size_t newsize)
 * @Param vector the vector to resize
 * @Param newsize the new size of the vector
 * @Returns 0 on success, -1 on failure
 * Resizes a vector
 * This function is guaranteed to fail if the size is negative or null
 * If this function enlarges the vector, then the new values will be filled with zeroes
 * It this function shrinks the vector, then the excess values are discarded
 */
int vector_resize(vector_t vector, size_t newsize);

/** int vector_prealloc(vector_t vector, size_t min, size_t add)
 * @Param vector the vector to pre-allocate
 * @Param min the minimum allocation size
 * @Param add the minimum number of slots to add
 * @Returns 0 on success, -1 on failure
 * Pre-allocates a vector
 * This function will only allocate more room, never de-allocate
 * The vector's allocation size will be set to the greatest of:
 * - its current allocation size
 * - the value of `min`
 * - its current length, plus `add` slots
 */
int vector_prealloc(vector_t vector, size_t min, size_t add);

/** int vector_push(vector_t vector, prime_t value)
 * @Param vector the vector on which to operate
 * @Param value the value to append
 * @Returns 0 on success, -1 on failure
 * Pushes (appends) a value at the end of the vector
 */
int vector_push(vector_t vector, prime_t value);

/** int vector_pop(vector_t vector, prime_t* value)
 * @Param vector the vector on which to operate
 * @Param value the address to update
 * @Returns 0 on success, -1 on failure
 * Pops (removes and retrieves) a value at the end of the vector
 * The value is read into the given pointer, not returned
 */
int vector_pop(vector_t vector, prime_t* value);

#define VECTOR_BUMP 0x100

//END vector class

//BEGIN prime calculator common data


// disk persistence
#define PRIMES_FILENAME "primes.bin"
extern int primesFd;
int readDisk(void);
int writeDisk(void);
int updateDisk(void);

// prime calculation
#define PRIMES_MAX 10000
extern vector_t primes;

//END prime calculator common data

#endif //__COMMON_H
