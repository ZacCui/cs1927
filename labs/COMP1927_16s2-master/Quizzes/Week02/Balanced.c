/**
 * The implementation of the methods in Balanced.h
 */

#include <stdlib.h>
#include <stdio.h>
#include "Balanced.h"

// Given a string which can only contain characters of:
// {}, []. ()
// Determine whether it is a valid combination of brackets
// i.e. for string [}, return 0
//      for string (), return 1
// Make sure to assert that all characters in the string are indeed
// valid (use the validChar method)
// You cannot assume any particular size of the string,
// i.e. for all you know, it is infinite or empty =)
int isValid(char* str) {
  // Answer the question here
    char ch = 0; 
    int i = 0;
    int j = 0;
    char * bracket = malloc(sizeof(str));
    for(i = 0; str[i] != 0; ch = str[i++]){
	  	if(validChar(ch) == VALID){
	  		switch (ch){
	  		
	  		}
	  		
		}
	}
	j = 0;
	
	
			
  
    return 0;
}

int validChar(char ch) {
  switch (ch) {
    case '[': case ']':
    case '{': case '}':
    case '(': case ')':
      return VALID;
    default:
      return INVALID;
  }
}
