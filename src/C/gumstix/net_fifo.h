#include <stdlib.h>
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
 * GLOBAL NetFifo
 */
//NetFifo globalNetFifo;


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
