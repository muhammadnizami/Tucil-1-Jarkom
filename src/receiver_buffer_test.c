/*Tugas kecil 1 Jaringan Komputer
pembuat:
13512501 Muhammad Nizami
13513039 Ivan Andrianto
*/

#include "receiver_buffer.h"

#include <stdio.h>

int main(){
	printf("empty: %x\n",buffer_isEmpty());
	buffer_push('c');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('d');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('e');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	buffer_print();
	buffer_push('f');
	printf("empty: %x\n",buffer_isEmpty());
	printf("full: %x\n",buffer_isFull());
	buffer_print();
	printf("pop: %c\n",buffer_pop());
	printf("empty: %x\n",buffer_isEmpty());
	printf("full: %x\n",buffer_isFull());
	buffer_print();
	buffer_print();
	printf("pop: %c\n",buffer_pop());
	printf("empty: %x\n",buffer_isEmpty());
	printf("full: %x\n",buffer_isFull());
	buffer_print();
	buffer_print();
	printf("pop: %c\n",buffer_pop());
	printf("empty: %x\n",buffer_isEmpty());
	printf("full: %x\n",buffer_isFull());
	buffer_print();
	buffer_print();
	printf("pop: %c\n",buffer_pop());
	printf("empty: %x\n",buffer_isEmpty());
	printf("full: %x\n",buffer_isFull());
	buffer_print();
	buffer_push('g');
	printf("empty: %x\n",buffer_isEmpty());
	printf("full: %x\n",buffer_isFull());
	buffer_print();
	buffer_push('g');
	printf("empty: %x\n",buffer_isEmpty());
	printf("full: %x\n",buffer_isFull());
	buffer_print();
	buffer_push('g');
	printf("empty: %x\n",buffer_isEmpty());
	printf("full: %x\n",buffer_isFull());
	buffer_print();
	buffer_push('g');
	printf("empty: %x\n",buffer_isEmpty());
	printf("full: %x\n",buffer_isFull());
	buffer_print();
	int i;
	for (i=0;i<20;i++){	
		printf("pop: %c\n",buffer_pop());
		printf("empty: %x\n",buffer_isEmpty());
		printf("full: %x\n",buffer_isFull());
		buffer_print();
	}
	buffer_push('g');
	printf("empty: %x\n",buffer_isEmpty());
	printf("full: %x\n",buffer_isFull());
	buffer_print();
	buffer_push('g');
	printf("empty: %x\n",buffer_isEmpty());
	printf("full: %x\n",buffer_isFull());
	buffer_print();
	
}
