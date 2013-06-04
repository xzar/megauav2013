#ifndef _H_NET_FIFO_
#define _H_NET_FIFO_

#include <stdlib.h>
#include "var.h"
#define MAX_SIZE 100

/*
 * use to stock received message from control tower.
 */
typedef struct netfifo
{
	int nf_index_start;
	int nf_index_end;
	int nf_size;
	void *nf_fifo[MAX_SIZE];
} NetFifo;


/*
 * init net fifo
 */
int initNetFifo(NetFifo * nf);

/*
 * add pointer at the end of the list
 */
int addNetFifo(NetFifo * nf, void * data);

/*
 * delete the first pointer
 */
int removeNetFifo(NetFifo * nf);

/*
 * return the first pointer of the fifo
 * and delete it.
 */
void * pullNetFifo(NetFifo * nf);

/*
 * return the first pointer of the fifo
 * NULL if fifo is empty
 */
void * firstNetFifo(NetFifo * nf);

/*
 * clear the fifo.
 */
int clearNetFifo(NetFifo * nf);

/*
 * return 1 if fifo is full,
 * 0 else.
 */
int isFullNetFifo(NetFifo * nf);

/*
 * return 1 if fifo is empty,
 * 0 else.
 */
int isEmptyNetFifo(NetFifo * nf);

/*
 * print the fifo
 */
void printNF(NetFifo * nf);
#endif
