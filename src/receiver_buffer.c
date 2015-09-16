#include "receiver_buffer.h"
#include <stdio.h>
#include <stdlib.h>

//push di tail, pop dari head

char receiver_buffer_tab[BUFFER_MAXEL];
int head = -1;
int tail = -1;

bool buffer_isEmpty(){
	return head==-1;
}

bool buffer_isFull(){
	return head==tail+1 || (head==0&&tail==BUFFER_MAXEL-1);
}

//prekondisi: !isEmpty();
char buffer_pop(){
	if (buffer_isEmpty()){ fprintf(stderr,"buffer error\n");exit(1);};
	char retval = receiver_buffer_tab[head];
	if (head==tail){
		head=-1;
		tail=-1;
	}else{
		head++;
		if (head>=BUFFER_MAXEL) head=0;
	}
	return retval;
}

//prekondisi: !isFull();
void buffer_push(char X){
	if (buffer_isFull()){ fprintf(stderr,"buffer error\n");exit(1);};
	if (buffer_isEmpty()){
		head=0;
		tail=0;
		receiver_buffer_tab[head]=X;
	}else{
		tail++;
		if (tail>=BUFFER_MAXEL) tail=0;
		receiver_buffer_tab[tail]=X;
	}
}

int buffer_numElements(){
	if (buffer_isEmpty()) return 0;
	return tail>=head?tail-head+1:tail-head+1+BUFFER_MAXEL;
}

void buffer_print(){
	int i;
	if (buffer_isEmpty()) printf("empty\n");
	else{
		if (head<=tail){
			for (i=0;i<head;i++)printf("* ");
			for (i=head;i<=tail;i++) printf("%c ",receiver_buffer_tab[i]);
			for (;i<BUFFER_MAXEL;i++) printf("* ");
		}else{
			for (i=0;i<=tail;i++) printf("%c ",receiver_buffer_tab[i]);
			for (;i<head;i++) printf("* ");
			for (;i<BUFFER_MAXEL;i++) printf("%c ",receiver_buffer_tab[i]);

		}
		printf("\n");
	}
}
