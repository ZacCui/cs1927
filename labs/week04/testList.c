// Completed by ZIYI CUI (z5097491) and HUIJIA WANG (z5097494)
// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "DLList.h"

#define BUFF 1000
int main(int argc, char *argv[])
{
	DLList myList;
	myList = getDLList(stdin);
	putDLList(stdout,myList);
	assert(validDLList(myList));
	// TODO: more tests needed here
	
	char * val[BUFF];
	int i = 0;
	printf("=== TESTING DLListMove and STORE VALUES ===\n");
	while ( i < DLListLength(myList) ){
		val[i++] = DLListCurrent(myList);
		DLListMove(myList ,1);
		assert(validDLList(myList));
	}
	printf("PASSED\n");
	i = 0;
	printf("=== TESTING DLListMoveTo ===\n");
	while( i < DLListLength(myList) ){
		DLListMoveTo(myList,i+1);
		assert(DLListCurrent(myList) == val[i]);
		assert(validDLList(myList));
		i++;
	}
	printf("PASSED\n");
	printf("=== TESTING DLListDelete 1 ===\n");
	while( DLListLength(myList) != 1 ){
		DLListDelete(myList);
		assert(validDLList(myList));
		assert(DLListCurrent(myList) == val[i-2]);
		assert( DLListIsEmpty(myList) != 1 );      //ask!!!!!!!!! done
		i--;
	}
	printf("PASSED\n");
	printf("=== TESTING DLListAfter ===\n");
	i = 1;
	while( val[i] != NULL){
		DLListAfter(myList, val[i]);
		assert(validDLList(myList));
		assert(strcmp(DLListCurrent(myList),val[i]) == 0);     //ask!!!!!!!! done
		i++;
	}
	printf("PASSED\n");
	printf("=== TESTING DLListBefore ===\n");
	int a = i;
	
	while( val[i-a] != NULL ){
		DLListBefore(myList, val[i-a]);
		assert(validDLList(myList));
		assert(strcmp(DLListCurrent(myList),val[i-a]) == 0); 
		i++;
	}
	printf("PASSED\n");
	printf("=== TESTING DLListDelete 2 ===\n");
	DLListMoveTo(myList,1);
	assert(DLListCurrent(myList) == val[0]);
	while( DLListLength(myList) != 0 ){
		DLListDelete(myList);
		if(DLListIsEmpty(myList)) {
			assert(validDLList(myList));
			continue;
		}
		assert( DLListIsEmpty(myList) != 1 );
	}
	assert(DLListIsEmpty(myList) == 1);
	
	printf("PASSED\n");
	return 0;
}
