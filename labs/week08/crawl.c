// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders <vince@kyllikki.org>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <curl/curl.h>
#include "queue.h"
#include "stack.h"
#include "set.h"
#include "graph.h"
#include "html.h"
#include "url_file.h"

#define BUFSIZE 1024

void setFirstURL(char *, char *);
void normalise(char *, char *, char *, char *, int);

int main(int argc, char **argv)
{
	URL_FILE *handle;
	char buffer[BUFSIZE];
	char baseURL[BUFSIZE];
	char firstURL[BUFSIZE];
	char next[BUFSIZE];
	int  maxURLs;

	if (argc > 2) {
		strcpy(baseURL,argv[1]);
		setFirstURL(baseURL,firstURL);
		maxURLs = atoi(argv[2]);
		if (maxURLs < 40) maxURLs = 40;
	}
	else {
		fprintf(stderr, "Usage: %s BaseURL MaxURLs\n",argv[0]);
		exit(1);
	}
	
	
	
	Queue TODO = newQueue();
	enterQueue(TODO,firstURL);     								// add firstURL to the ToDo list
	Graph G = newGraph(maxURLs);   								// initialise Graph to hold up to maxURLs
	Set seen = newSet();           								// initialise set of Seen URLs
	//Link curr = TODO->front;
	char *c;
	while(!emptyQueue(TODO) && nVertices(G) < maxURLs){  		// while (ToDo list not empty and Graph not filled)
		
		memset(baseURL, 0, BUFSIZE);
		strcpy( baseURL , leaveQueue(TODO) );                 // grab Next URL from ToDo list
		
		if (!(handle = url_fopen(baseURL, "r"))) {            // if (not allowed) continue
			fprintf(stderr,"Couldn't open %s\n", next);
			exit(1);
		}
		
		if ((c = strstr( baseURL, "unsw")) == NULL) continue;   // make sure the web is inside UNSW
		if ((c = strstr( baseURL, "http")) == NULL) continue;   //filter the non-http web
		
		while(!url_feof(handle) /*&& nVertices(G) < maxURLs*/) {    //    foreach line in the opened URL
			url_fgets(buffer,sizeof(buffer),handle);
			//fputs(buffer,stdout);
			int pos = 0;
			char result[BUFSIZE];
			memset(result,0,BUFSIZE);
			while ((pos = GetNextURL(buffer, baseURL, result, pos)) > 0){  //foreach URL on that line
				printf("Found: '%s'\n",result);
				if(nVertices(G) < maxURLs || isConnected(G, baseURL, result)){                  //   if (Graph not filled or both URLs in Graph)
					addEdge(G,baseURL,result);              //             add an edge from Next to this URL
				}
				if(!isElem(seen,result)){                  //          if (this URL not Seen already) 
					insertInto(seen,result);               //             add it to the Seen set
					enterQueue(TODO, result);              //             add it to the ToDo list
				}
				//if(nVertices(G) == maxURLs) break;
				memset(result,0,BUFSIZE);
			}
		}
		url_fclose(handle);
		sleep(1);
		
	}
	showGraph(G,0);
	showGraph(G,1);
	showGraph(G,3);			
	return 0;
}

// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
void setFirstURL(char *base, char *first)
{
	char *c;
	if ((c = strstr(base, "/index.html")) != NULL) {
		strcpy(first,base);
		*c = '\0';
	}
	else if (base[strlen(base)-1] == '/') {
		strcpy(first,base);
		strcat(first,"index.html");
		base[strlen(base)-1] = '\0';
	}
	else {
		strcpy(first,base);
		strcat(first,"/index.html");
	}
}
	
	// You need to modify the code below to implement:
	//
	// add firstURL to the ToDo list
	// initialise Graph to hold up to maxURLs
	// initialise set of Seen URLs
	// while (ToDo list not empty and Graph not filled) {
	//    grab Next URL from ToDo list
	//    if (not allowed) continue
	//    foreach line in the opened URL {
	//       foreach URL on that line {
	//          if (Graph not filled or both URLs in Graph)
	//             add an edge from Next to this URL
	//          if (this URL not Seen already) {
	//             add it to the Seen set
	//             add it to the ToDo list
	//          }
	//       }
    //    }
	//    close the opened URL
	//    sleep(1)
	// }