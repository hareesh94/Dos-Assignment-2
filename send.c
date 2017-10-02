/* send.c - send */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}




syscall sendMsg(pid32 pid, umsg32 msg)
{
	intmask mask;
	struct procent *prptr;
	mask = disable();

	if (isbadpid(pid))
	{
		restore(mask);
		return SYSERR;
	}
	prptr = &proctab[pid];

	
      if ((prptr->front == prptr->rear + 1) && (prptr->front == 0 && prptr->rear == SIZE - 1))
        {     
              restore(mask);    
              return SYSERR;
         }
		 
             wait(prptr->mutex);
	else
	{
		if (prptr->rear == -1)
		{
		prptr->front = 0;
		prptr->rear = 0;
		}
		else if (prptr->rear == SIZE - 1)
		prptr->rear = 0;
		else
                {

		prptr->rear++;
		prptr->queue[prptr->rear] = msg;
		
		}
	}
	prptr->prmsg = msg;
	prptr->prhasmsg = TRUE;
        signal(prptr->mutex);

	if (prptr->prstate == PR_RECV)
	{
		ready(pid);
	}
	else if (prptr->prstate == PR_RECTIM)
	{
		unsleep(pid);
		ready(pid);
	}
	restore(mask);
	return OK;
}




uint32 sendMsgs(pid32  pid, umsg32 *msgs, uint32 msg_count){
intmask mask;
struct procent *prptr;
mask=disable();
int k=0;
if(msg_count<=SIZE)
{
for(k=0;k<=msg_count;k++)

sendMsg(pid, msgs[k]);
}
restore(mask);
return msg_count;

}



uint32 sendnMsg(uint32 pid_count, pid32* pids, umsg32 msg){
intmask mask;
struct	procent *prptr;		
int k=0;
mask=disable();
if(pid_count<=max_receivers)
for(k=0;k<=pid_count;k++){
sendMsg( pids[k],  msg);
}
restore(mask);
return k;

}


