/**
 * BinSum Balanced h file
 * Create any typedefs/#defines/data structures you want in here
 * Make sure they are an ADT!
 * You can change anything, except the binSum prototype
 */

void binSum(char* bin1, char* bin2, char* res);
int btoi(char ch);
typdef struct node * Node;
typdef struct list * List;
typdef struct node{
	int val;
	int pos;
	Node next;
} node;
struct list{
	int size;
	Node first;
	Node last;
} list;
