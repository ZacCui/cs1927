#include <stdio.h>
#include <stdlib.h>
#include "Q1.h"
#include "Stack.h"

/**
 * Complete this section please
 * You are given a map, where:
 * - '#' represents a flooded section
 * - '.' represents an empty section
 * - '*' represents a wall
 * This method will spread the # in the surrounding four directions from its position
 * only if there is an empty section there. E.g. in the map
 * . . .
 * . # .
 * . * .
 *
 * After steps = 1, the map will have:
 * . # .
 * # # #
 * . * .
 *
 * After steps = 2,
 * # # #
 * # # #
 * # * #
 */
 
void flood(char** map, int size, int steps) {
	int ctr = 0;
	int i=0;
	int j=0;
	Stack seen = createStack();
	Stack new = createStack();
	for(i = 0;i<size;i++){
		for(j=0; j<size;j++){
			if(map[i][j] == '#'){
				push(seen,100*i+j);
			}
		}
	}
	int num;
	for(ctr = 0; ctr < steps; ++ctr){
		while(getSize(seen)){
			num = pop(seen);
			i = num/100;
			j = num%100;
			if(i < size && map[i+1][j] == '.') map[i+1][j] = '#'; push(new,100*(i+1)+j);
			if(i > 0 && map[i-1][j] == '.') map[i-1][j] = '#'; push(new,100*(i-1)+j);
			if(j < size && map[i][j+1] == '.') map[i][j+1] = '#'; push(new,100*i+j+1);
			if(j > 0 && map[i][j-1] == '.') map[i][j-1] = '#'; push(new,100*i+j-1);
		}
		seen = new;
		while(getSize(new)) pop(new);
	}
  
}
