// unsort.c ... make a sequence of numbers not sorted
// Usage: unsort [FileName]
// If no file name supplied, unsorts stdin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int binarysreach(int *a);

int main (int argc, char *argv[]){
   FILE *in;

   if (argc == 1) 
      in = stdin;
   else {
      if ((in = fopen(argv[1],"r")) == NULL) {
         fprintf(stderr, "Can't open file '%s'\n",argv[1]);
         exit(1);
      }
   }
  
   // Put code here read numbers and "unsort" them
   // Hint: for reading the input
   //   while (fscanf(in, "%d", &val) != EOF)
	int *val;
	int i = 0;
	for(;fscanf(in,"%d",val[i]) != EOF; i++);
	int *a = malloc(sizeof(int)*(i+1));
	int length  = i+1;
	int j = 0;
	int k = 0;
	for(i = 0; val[i] != '\0'; i++){
		while(a[j] != '\0' && k < length){
			j = rand()%length;
			k++;
		}
		k = 0;
		a[j] = val[i];
	}
	for(i=0; i< length ; i++) {fprintf(stdout,"%d",a[i]);}
   return 0;
} 
/*
int binarysreach(int *a){
	
	return NULL;
}  
*/   
