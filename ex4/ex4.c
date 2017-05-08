#include <stdio.h>
#include <stdlib.h>

#define N 5

struct PCB
{
    unsigned int id;
    unsigned int priority;
    unsigned int cputime;
    unsigned int alltime;
    int startblock;
    unsigned int blocktime;
    enum State{ Ready, Run, Block, Finish } state;
    struct PCB * next;
};

struct Queue_Head
{
    unsigned int num;
    struct PCB* head;
};

unsigned int num = 0;	    //记录record的数量
int timeblock = 0;
struct PCB record[N];
struct Queue_Head ready_queue = { 0,NULL };
struct Queue_Head block_queue = { 0,NULL };
struct PCB* run;

int judge_state(struct PCB* pcb)
{
    switch (pcb->state)
    {
    case Ready:
	if (pcb->alltime > 0)
	    return 1;
	else return 0;
	break;
    case Block:
	if (pcb->blocktime > 0)
	    return 1;
	else return 0;
	break;
    case Finish:
	if (pcb->alltime == 0)
	    return 1;
	else return 0;
	break;
    default:
	return 1;
	break;
    }
}

struct PCB* ready_pop()
{
    if (!ready_queue.num)
    {
	return NULL;
    }
    else
    {
	ready_queue.num--;
	struct PCB* ret = ready_queue.head;
	ready_queue.head = ret->next;
	ret->state = Run;
	ret->next = NULL;
	return ret;
    }
}

void ready_push(struct PCB* pcb)
{
    ready_queue.num++;
    pcb->state = Ready;
    struct PCB* pre = ready_queue.head;
    if (pre == NULL)
    {
	ready_queue.head = pcb;
	return;
    }
    else if(pcb->priority > pre->priority)
    {
	ready_queue.head = pcb;
	pcb->next = pre;
	return;
    }
    while (pre&&pre->next)
    {
	if (pcb->priority > pre->next->priority)
	{
	    break;
	}
	else
	{
	    pre = pre->next;
	}
    }
    struct PCB* temp = pre->next;
    pre->next = pcb;
    pcb->next = temp;
}

void ready_update()
{
    struct PCB* cur = ready_queue.head;
    while (cur)
    {
	cur->priority += 1;
	cur = cur->next;
    }
}

void ready_print()
{
    struct PCB* cur = ready_queue.head;
    printf("READY_QUEUE: ");
    if (cur == NULL)
    {
	printf("NULL\n");
    }
    else
    {
	while (cur)
	{
	    printf("->%u", cur->id);
	    cur = cur->next;
	}
	printf("\n");
    }
}

void block_push(struct PCB* pcb)
{
    if (pcb->blocktime == 0)
    {
	ready_push(pcb);
	return;
    }
    block_queue.num++;
    pcb->state = Block;
    struct PCB* pre = block_queue.head;
    if (pre == NULL)
    {
	block_queue.head = pcb;
	return;
    }
    else if (pcb->blocktime < pre->blocktime)
    {
	block_queue.head = pcb;
	pcb->next = pre;
	return;
    }
    while (pre&&pre->next)
    {
	if (pcb->blocktime > pre->next->blocktime)
	{
	    break;
	}
	else
	{
	    pre = pre->next;
	}
    }
    struct PCB* temp = pre->next;
    pre->next = pcb;
    pcb->next = temp;
}

void block_update()
{
    struct PCB* cur = block_queue.head;
    while (cur)
    {
	cur->blocktime -= 1;
	cur = cur->next;
    }
    cur = block_queue.head;
    while (cur)
    {
	if (cur->blocktime == 0)
	{
	    struct PCB* tem = cur;
	    tem->next = NULL;
	    block_queue.num -= 1;
	    ready_push(tem);	    
	}
	else
	{
	    break;
	}
	cur = cur->next;
    }
    block_queue.head = cur;
}

void block_print()
{
    struct PCB* cur = block_queue.head;
    printf("BLOCK_QUEUE: ");
    if (cur == NULL)
    {
	printf("NULL\n");
    }
    else
    {
	while (cur)
	{
	    printf("->%u", cur->id);
	    cur = cur->next;
	}
	printf("\n");
    }
}

void running_print()
{
    printf("RUNNING PROG: ");
    if (run == NULL)
    {
	printf("NULL\n");
    }
    else 
    {
	printf("%u\n", run->id);
    }
}

void init()
{
    struct PCB init_data[N] = {
	{ 0, 9, 0, 3, 2, 3, Ready, NULL },
	{ 1, 38, 0, 3, -1, 0, Ready, NULL },
	{ 2, 30, 0, 6, -1, 0, Ready, NULL },
	{ 3, 29, 0, 3, -1, 0, Ready, NULL },
	{ 4, 0, 0, 4, -1, 0, Ready, NULL },
    };
    for (int i = 0; i < N; i++)
    {
	if (judge_state(&init_data[i]))
	{
	    record[num] = init_data[i];
	    num++;
	}
	else
	{
	    printf("init_data %d is invalid\n", i);
	}
    }

    for (unsigned int i = 0; i < num; i++)
    {
	switch (record[i].state)
	{
	case Ready:
	    ready_push(&record[i]);
	    break;
	case Block:
	    block_push(&record[i]);
	    break;
	default:
	    ;
	}
    }
}


void runned_update()
{
    ready_update();
    block_update();

    if (run)
    {
	run->alltime -= 1;
	run->cputime += 1;
	run->startblock = run->startblock >= 0 ? run->startblock - 1 : -1;
	run->priority = run->priority <= 2 ? 0 : run->priority - 3;
	if (run->alltime == 0)
	{
	    run->state = Finish;
	}
	else if (run->startblock == 0)
	{
	    run->state = Block;
	    block_push(run);
	}
	else
	{
	    run->state = Run;
	    ready_push(run);
	}
    }
    else
    {
	;
    }
}

void state_print()
{
    printf("ID\t");
    for (unsigned int i = 0; i < num; i++)
    {
	printf("\t%u", record[i].id);
    }
    printf("\n");

    printf("PRIORITY");
    for (unsigned int i = 0; i < num; i++)
    {
	printf("\t%u", record[i].priority);
    }
    printf("\n");

    printf("CPUTIME\t");
    for (unsigned int i = 0; i < num; i++)
    {
	printf("\t%u", record[i].cputime);
    }
    printf("\n");

    printf("ALLTIME\t");
    for (unsigned int i = 0; i < num; i++)
    {
	printf("\t%u", record[i].alltime);
    }
    printf("\n");

    printf("STARTBLOCK");
    for (unsigned int i = 0; i < num; i++)
    {
	printf("\t%d", record[i].startblock);
    }
    printf("\n");

    printf("BLOCKTIME");
    for (unsigned int i = 0; i < num; i++)
    {
	printf("\t%u", record[i].blocktime);
    }
    printf("\n");

    printf("STATE\t");
    for (unsigned int i = 0; i < num; i++)
    {
	switch(record[i].state)
	{
	case Ready:
	    printf("\tREADY");
	    break;
	case Run:
	    printf("\tRUN");
	    break;
	case Block:
	    printf("\tBLOCK");
	    break;
	case Finish:
	    printf("\tFINISH");
	    break;
	default:
	    printf("\tUNDEFINE");
	}
    }
    printf("\n");
}

void print()
{
    timeblock++;
    printf("==========================================================\n\n");
    printf("************************第%d个时间片**********************\n\n", timeblock);
    running_print();
    ready_print();
    block_print();
    printf("----------------------------------------------------------\n");
    state_print();
    printf("\n");
}

int main()
{
    init();
    while (ready_queue.num || block_queue.num)
    {
	run = ready_pop();
	print();
	runned_update();
    }
    run = NULL;
    print();
    return 0;
}