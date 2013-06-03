#include "net_fifo.h"

/*
 * PRIVATE function, don't declare it in .h
 * return the next index in fifo.
 */
int getNextEndIndex(NetFifo * nf)
{
	
	if ( nf->nf_index_end == MAX_SIZE - 1 ) return 0;
	
	return ++nf->nf_index_end;
}

/*
 * PRIVATE function, don't declare it in .h
 * return the next index in fifo.
 */
int getNextStartIndex(NetFifo * nf)
{
	
	if ( nf->nf_index_start == MAX_SIZE - 1 ) return 0;
	
	return ++nf->nf_index_start;
}

/*
 * init net fifo
 */
int initNetFifo(NetFifo * nf)
{
	nf->nf_index_start=0;
	nf->nf_index_end=0;
	nf->nf_size=0;
	
	int i;
	
	for (i = 0; i < MAX_SIZE; i++)
	{
		nf->nf_fifo[i]=NULL;
	}
	
	return 1;
}

/*
 * add pointer at the end of the list
 * return -1 if error
 */
int addNetFifo(NetFifo * nf, void * data)
{
	if ( isFullNetFifo(nf) ) return -1;
	
	nf->nf_fifo[nf->nf_index_end] = malloc(BUFFER_SIZE);
	
	memset(nf->nf_fifo[nf->nf_index_end], 0, BUFFER_SIZE);
	memcpy(nf->nf_fifo[nf->nf_index_end], data, BUFFER_SIZE);
	
	nf->nf_index_end = getNextEndIndex(nf);
	
	return 1;
}

/*
 * delete the first pointer
 */
int removeNetFifo(NetFifo * nf)
{
	free(nf->nf_fifo[nf->nf_index_start]);
	nf->nf_fifo[nf->nf_index_start] = NULL;
	nf->nf_index_start = getNextStartIndex(nf);
	return 1;
}

/*
 * return the first pointer of the fifo
 */
void * firstNetFifo(NetFifo * nf)
{
	return nf->nf_fifo[nf->nf_index_start];
}

/*
 * clear the fifo.
 */
int clearNetFifo(NetFifo * nf)
{
	while (!isEmptyNetFifo(nf))
	{
		removeNetFifo(nf);
	}
	return initNetFifo(nf);
}

/*
 * return 1 if fifo is full,
 * 0 else.
 */
int isFullNetFifo(NetFifo * nf)
{
	if ( getNextEndIndex(nf) == nf->nf_index_start ) return 0;
	return 1;
}

/*
 * return 1 if fifo is empty,
 * 0 else.
 */
int isEmptyNetFifo(NetFifo * nf)
{
	
}
