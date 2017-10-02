/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	restore(mask);
	return msg;
}






umsg32 receiveMsg(void)
{
	intmask mask;
	struct procent *prptr;
	umsg32 msg;
	mask = disable();
	prptr = &proctab[currpid];
        if (prptr->rear == -1)
         {
           kprintf("blocked untill message arrives");
           prptr->prstate=PR_RECV;
           resched();
          }
	 wait(prptr->mutex);
        else
	{
		msg = prptr->queue[prptr->front];
		if (prptr->front == prptr-> rear)
		{
			prptr->rear = -1;
			prptr->front = -1;

		}
		else if (prptr->front == SIZE - 1)
			prptr->front = 0;
		else
			prptr->front++;

	}	
	
msg=prptr->prmsg;

prptr->prhasmsg=FALSE;
signal(prptr->mutex);
restore(mask);
return msg ;
}




syscall receiveMsgs(umsg32* msgs, uint32 msg_count)
{

intmask mask;
struct procent *prptr;
umsg32 msg;
mask=disable();
prptr=&proctab[currpid];
uint32 i=prptr->rear;
if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		
	}


	wait(prptr->mutex);
	for(i=0;i<msg_count;i++){
       msgs[i] = prptr->queue[prptr->front];		/* get message		*/	
	if (prptr->front == prptr-> rear)
		{
			prptr->rear = -1;
			prptr->front = -1;

		}
		else if (prptr->front == SIZE - 1)
			prptr->front = 0;
		else
			prptr->front++;

	}	
}
	signal(prptr->mutex);
	restore(mask);
	return OK;
}

         
	
