#include<stdlib.h>
#include "queue.h"



int length_LinkList(LinkList L)
{
    LNode * p =L;
    int j=0;
    while(p->next)
    {
        p = p->next;
        j++;
    }
    return j;
}
 
void print_Link(LinkList H, int front, int rear)
{
    LNode *p= H;
    int i;
	while(front<rear)
	{
		front++;
		p=p->next;		
		for(i=0; i<9; i++)
		{
			printf("%d ",(int)p->data[i]);
		}
		printf("\n");
	}
}

 
void insert_LinkList(LinkList H,int i,int array[9])
{
    LNode *p,*s;
    p=H;
    int j=0;
    while(p->next!=NULL&&j<i-1){
        p=p->next;
        j++;
    }
    if(j!=i-1) p=NULL;
    if(p==NULL) printf("参数i错误：\n");
    else
    {
        s=(LNode *)malloc(sizeof(LNode));
        for(int t=0; t< 9; t++)	s->data[t] = array[t];
        s->next = p->next;
        p->next =s;
    }
}
 


void enQueue(int *rear,int data[9]){
    (*rear)++;
    /*****链表操作*****/
    insert_LinkList(H_link,*rear,data);
    /********************/
}

void deQueue(int *front,int rear){
    LNode *p;
    //如果 front==rear，表示队列为空
    if ((*front)!=rear) {
        /*****链表操作*****/
        p=H_link;
        H_link=H_link->next;
        free(p);
        /********************/        
        (*front)++;
    }
}

void deQueueall(int *front,int rear){
    //如果 front==rear，表示队列为空
    while ((*front)!=rear) {
        // printf("出队元素：%d\n", data);
        /*****链表操作*****/
        H_link=H_link->next;
        free(H_link);
        /********************/        
        (*front)++;    }
}