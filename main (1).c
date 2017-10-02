/*  main.c  - main */

#include <xinu.h>
pid32 pid1;
pid32 pid2;
pid32 pid3;
pid32 pid4;
pid32 pid5;
pid32 pid6;

umsg32 msg_recv;
umsg32 msg_sent=10;

uint32 pid_count = 1;
pid32  a[3];
uint32 total_sent;

umsg32	grp[3];
umsg32	*msgs =grp;
uint32	msg_count;
uint32	sent;



process p1(void)
{
  printf("P1 sending message to P2 ");
  sendMsg(pid2,4);
	return OK;
	
}


process p2(void)
{
	printf("P2 receiving from P1 ");
	msg_recv = receiveMsg();
	printf("message received is %d\n",msg_recv);
        return OK;

}


process p3(void)
{
	printf("message group sent from P3 to P4");
        int32 i;
	for(i=0; i<msg_count; i++){
           grp[i] = i;	
	}
	sent = sendMsgs(pid4,msgs,msg_count);
	printf("Number of messages sent are %d\n", sent);
	return OK;
}


process p4(void)
{
	printf("receiving group of messages");
            int32 i=0;	
	receiveMsgs(msgs, msg_count);
	printf("received messages in a group are \n");
	for(i=0; i<msg_count; i++){

	    printf("%d\n",grp[i]);
	
	}

	return OK;

}


process p5(void)
{
	
	printf("Sending message to many processes");
        pid32* pids=a;
        a[0] = pid2;
	a[1] = pid4;
	a[2] = pid6;
        sent = sendnMsg(pid_count, pids, msg_sent);
        printf("number of sent messages are %d\n",sent);

	return OK;
}


process p6(void)
{
  	printf("receiving process");
        return OK;

}


process    main(void)
{
    printf("Hello\n");
    recvclr();



    pid1 = create(p1, 4096, 50, "p1", 0);
    pid2 = create(p2, 4096, 50, "p2", 0);
    pid3 = create(p3, 4096, 50, "p3", 0);
    pid4 = create(p4, 4096, 50, "p4", 0);
    pid5 = create(p5, 4096, 50, "p5", 0);
    pid6 = create(p6, 4096, 50, "p6", 0);

   

    resched_cntl(DEFER_START);
    resume(pid1);
    resume(pid2);
    resume(pid3);
    resume(pid4);
    resume(pid5);
    resume(pid6);
    resched_cntl(DEFER_STOP);

    return OK;
}
