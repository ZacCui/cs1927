// HashTable.c ... implementation of HashTable ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "HashTable.h"
#include "List.h"

// Types and functions local to HashTable ADT

typedef struct HashTabRep {
	List *lists;  // either use this
	int   nslots; // # elements in array
	int   nitems; // # items stored in HashTable
} HashTabRep;

// convert key into index (from Sedgewick)
unsigned int hash(Key k, int tableSize)
{
	unsigned int h = 0;
	int a = 31415, b = 27183;
	Key pos = k;
	for (; *k != '\0'; k++) {
		a = a*b % (tableSize-1);
		//h = (a*h + (*k)) % tableSize;
		h = (a*h+(*k)+(k - pos)) % tableSize;
	}
	return (h % tableSize);
}


// Interface functions for HashTable ADT

// create an empty HashTable
HashTable newHashTable(int N)
{
	HashTabRep *new = malloc(sizeof(HashTabRep));
	assert(new != NULL);
	new->lists = malloc(N*sizeof(List));
	assert(new->lists != NULL);
	int i;
	for (i = 0; i < N; i++)
		new->lists[i] = newList();
	new->nslots = N; new->nitems = 0;
	return new;
}

// free memory associated with HashTable
void dropHashTable(HashTable ht)
{
	assert(ht != NULL);
	int i;
    for (i = 0; i < ht->nslots; i++)
		dropList(ht->lists[i]);
	free(ht);
}

// display HashTable stats
void HashTableStats(HashTable ht)
{
	assert(ht != NULL);
	printf("Hash Table Stats:\n");
	printf("Number of slots = %d\n",ht->nslots);
	printf("Number of items = %d\n",ht->nitems);
	printf("Chain length distribution\n");
	printf("%8s %8s\n","Length","#Chains");

	int i, length;
	int *num_chain = calloc(2, sizeof(int));
	int arrayLength = 1;
	
	// Go through each hash table slot
	for(i = 0; i < ht->nslots; ++i){
		length = 0;
		
		// Count the chain length for each slot
		length = ListLength(ht->lists[i]);
		
		// If necessary increase array size that stores chain lengths
		if (length+1 > arrayLength) {
			num_chain = realloc(num_chain, (length+1) * sizeof(int));
		}
		
		// Set newly allocated memory to 0
		for (; arrayLength < length+1; arrayLength++) {
			num_chain[arrayLength] = 0;		
		}
		
		
		// Increase data counter
		num_chain[length]++;
	}
	// Print chain results
	for(length = 0; length < arrayLength; length++ ){
		if(num_chain[length]){
			printf("%8d %8d\n",length,num_chain[length]);
		}
	}
	
	//caculate the average chain length of the HashTable
	double aver = 0.0;
	for(i = 0 ; i < arrayLength; ++i){
		aver += num_chain[i]+i;
	}
	aver = aver/ht->nslots;
	printf("The average length of chain is %lf \n", aver);
	
	//caculate the proportion of slots be occupied
	double proportion = 0.0;
	for(i = 1; i < arrayLength; ++i){
		proportion += num_chain[i];
	}
	proportion = proportion/ht->nslots;
	printf("proportion is %lf \n", proportion);
	free(num_chain);
}

// insert a new value into a HashTable
void HashTableInsert(HashTable ht, Item it)
{
	assert(ht != NULL);
	int i = hash(key(it), ht->nslots);
	if (ListSearch(ht->lists[i], key(it)) == NULL) {
		ListInsert(ht->lists[i], it);
		ht->nitems++;
	}
}

// delete a value from a HashTable
void HashTableDelete(HashTable ht, Key k)
{
	assert(ht != NULL);
	int h = hash(k, ht->nslots);
	ListDelete(ht->lists[h], k);
}

// get Item from HashTable using Key
Item *HashTableSearch(HashTable ht, Key k)
{
	assert(ht != NULL);
	int i = hash(k, ht->nslots);
	return ListSearch(ht->lists[i], k);
}

