#include <stdio.h>


// #define bool int
#define false 0
#define true 1

typedef uint32_t datatype;
typedef struct lnode
{
    datatype data [9];
    struct lnode *next;
}LNode,*LinkList;

extern LinkList H_link;

extern int length_LinkList(LinkList L);
extern void print_Link(LinkList H, int front, int rear);
extern void insert_LinkList(LinkList H,int i,int array[9]);
extern void enQueue(int *rear,int data[9]);
extern void deQueue(int *front,int rear);
extern void deQueueall(int *front,int rear);
