/*
������ģ�⣬�����ǹ���������ռ�ռ�
��������Ϊ��ͷ����˫������
*/
#include <stdio.h>
#include <stdlib.h>

#define MAXID 20
#define MAXSIZE 640

struct MemoryBlock
{
    unsigned int id;
    unsigned int address;
    size_t size;
    int stat;
    struct MemoryBlock * pre;
    struct MemoryBlock * nxt;
    struct MemoryBlock * freenext;
};

int algotype = 0;
struct MemoryBlock* id_record[MAXID];	  //����ϵͳidΪ0��ָ���һ���ڴ�����

void	Init();
int	Cmd(unsigned int id, int cmd, size_t size);
int	Alloc(unsigned int id, size_t nsize);
void	Insert(struct MemoryBlock* newblock);
void	RecycleStat_1(struct MemoryBlock* mb);
void	RecycleStat_2(struct MemoryBlock* mb);
void	RecycleStat_3(struct MemoryBlock* mb);
void	RecycleStat_4(struct MemoryBlock* mb);
void	Recycle(struct MemoryBlock* mb);
int	Free(unsigned int id, size_t nsize);
void	PrintStat();

int main()
{
    unsigned int id;
    int cmd;
    size_t size;
    printf("> > > > > ѡ���㷨���� < < < < <\n");
    printf("* * * * 1 �״���Ӧ���� * * * * * \n");
    printf("* * * * 2 �����Ӧ���� * * * * * \n");
    printf("* * * * 0     �˳�     * * * * *\n");
    printf(">> ");
    scanf("%d", &algotype);
    printf("#### ָ���ʽ˵�� : [id] [cmd] [size]  (0 0 0�˳�)\n");
    Init();
    PrintStat();
    while (algotype)
    {
        printf("$$ �������ڴ����ָ��:\n");
        printf(">> ");
        scanf("%d%d%Iu", &id, &cmd, &size);
	if (id)
	{
	    Cmd(id, cmd, size);
	}
	else
	{
	    break;
	}
	PrintStat();
    }
    return 0;
}

void Init()
{
    id_record[0] = (struct MemoryBlock*)malloc(sizeof(struct MemoryBlock));
    struct MemoryBlock * firstblock = (struct MemoryBlock*)malloc(sizeof(struct MemoryBlock));
    if (id_record[0] == NULL || firstblock == NULL)
    {
	printf("malloc failed\n");
	return;
    }
    struct MemoryBlock tmp1 = { 0,0,MAXSIZE,1,NULL,firstblock,firstblock };
    struct MemoryBlock tmp2 = { 0,0,MAXSIZE,0,id_record[0],NULL,NULL };
    *(id_record[0]) = tmp1;
    *firstblock = tmp2;
}//Init()


int Cmd(unsigned int id, int cmd, size_t size)
{
    if (id >= MAXID)
    {
	printf("invalid id\n");
	return 0;
    }
    else if (!size)
    {
	printf("invalid size\n");
	return 0;
    }
    else if (!cmd)
    {
	Free(id, size);
        printf("��ҵ%u�ͷ�%uKB�ڴ�\n\n", id, size);
	return 1;
    }
    else
    {
	Alloc(id, size);
        printf("��ҵ%u����%uKB�ڴ�\n\n", id, size);
	return 1;
    }
}//Cmd()


int Alloc(unsigned int id, size_t nsize)
{
    if (id_record[id] != NULL)
    {
        printf("the id had been malloc!\n");
        return 0;
    }
    struct MemoryBlock* pcur = id_record[0];
    while (pcur)
    {
	if (pcur->freenext && pcur->freenext->size == nsize)  //�պ������С���������
	{
	    id_record[id] = pcur->freenext;
	    pcur->freenext->stat = 1;
	    pcur->freenext->id = id;
	    pcur->freenext = pcur->freenext->freenext;
	    break;
	}
	else if (pcur->freenext && pcur->freenext->size > nsize)	//���з����ռ�������
	{
	    struct MemoryBlock * newblock = (struct MemoryBlock*)malloc(sizeof(struct MemoryBlock));
	    if (newblock == NULL)
	    {
		printf("malloc failed\n");
		return 0;
	    }
	    newblock->address = pcur->freenext->address + nsize;
	    newblock->size = pcur->freenext->size - nsize;
	    newblock->stat = 0;
            newblock->id = 0;
	    newblock->pre = pcur->freenext;
	    newblock->nxt = pcur->freenext->nxt;
	    newblock->freenext = pcur->freenext->freenext;

	    id_record[id] = pcur->freenext;

	    pcur->freenext->stat = 1;
	    pcur->freenext->id = id;
	    pcur->freenext->size = nsize;
	    pcur->freenext->nxt = newblock;

	    if (algotype == 1)
	    {
		pcur->freenext = newblock;
	    }
	    else if (algotype == 2)
	    {
		pcur->freenext = pcur->freenext->freenext;
		Insert(newblock);
	    }
	    break;
	}
	else
	{
	    pcur = pcur->freenext;
	}
    }
    
    if (pcur == NULL)
    {
	printf("Alloc failed\n");	//û���ҵ��ɷ���ռ�
	return 0;
    }
    else
    {
	return 1;
    }
}//Alloc()

void	Insert(struct MemoryBlock* newblock)//�������򷨣�����
{
    struct MemoryBlock* pcur = id_record[0];
    struct MemoryBlock* pre;
    while (pcur)
    {
	if (pcur->freenext && (pcur->freenext->size > newblock->size))
	{
	    pre = pcur;
	    break;
	}
	else
	{
	    pre = pcur;
	    pcur = pcur->freenext;
	}
    }
    struct MemoryBlock* tmp = pre->freenext;
    pre->freenext = newblock;
    newblock->freenext = tmp;
}
void RecycleStat_1(struct MemoryBlock* mb)
{
    mb->pre->size += mb->size;
    mb->pre->nxt = mb->nxt;
    mb->nxt->pre = mb->pre;
    if (algotype == 1)
    {
        free(mb);
    }
    else if (algotype == 2)
    {
        struct MemoryBlock* pcur = id_record[0];
        while (pcur)
        {
            if (pcur->freenext == mb->pre)
            {
                pcur->freenext = pcur->freenext->freenext;      //���޳��ٲ���
                Insert(mb->pre);
                break;
            }
            else
            {
                pcur = pcur->freenext;
            }
        }
        free(mb);
    }
}
void RecycleStat_2(struct MemoryBlock* mb)
{
    struct MemoryBlock* tmp = mb->nxt;
    mb->size += mb->nxt->size;
    mb->nxt = mb->nxt->nxt;
    tmp->pre = mb;

    if (algotype == 1)
    {
        struct MemoryBlock* pcur = mb->pre;
        while (pcur)
        {
            if (pcur->stat == 0 || pcur == id_record[0])
            {
                mb->freenext = pcur->freenext;
                pcur->freenext = mb;
                break;
            }
            else
            {
                pcur = pcur->pre;
            }
        }
        free(tmp);
    }
    else if (algotype == 2)
    {
        struct MemoryBlock* pcur = id_record[0];
        while (pcur)
        {
            if (pcur->freenext == tmp)
            {
                pcur->freenext = tmp->freenext;      //���޳��ٲ���
                Insert(mb);
                break;
            }
            else
            {
                pcur = pcur->freenext;
            }
        }
        free(tmp);
    }
}
void RecycleStat_3(struct MemoryBlock* mb)
{
    mb->pre->size += mb->size;
    mb->pre->size += mb->nxt->size;
    mb->pre->nxt = mb->nxt->nxt;
    mb->nxt->nxt->pre = mb->pre;
    if (algotype == 1)
    {
        free(mb->nxt);
        free(mb);
    }
    else if (algotype == 2)
    {
        struct MemoryBlock* pcur = id_record[0];
        int count = 0;
        while (pcur)
        {
            if (pcur->freenext == mb->pre)
            {
                pcur->freenext = mb->pre->freenext;      //���޳��ٲ���
                count++;
            }
            else if (pcur->freenext == mb->nxt)
            {
                pcur->freenext = mb->nxt->freenext;
                count++;
            }
            if (count == 2)
            {
                Insert(mb->pre);
                break;
            }
            else
            {
                pcur = pcur->freenext;
            }
        }
        free(mb->nxt);
        free(mb);
    }
}
void RecycleStat_4(struct MemoryBlock* mb)
{
    if (algotype == 1)
    {
        struct MemoryBlock* pcur = mb->pre;
        while (pcur)
        {
            if (pcur->stat == 0 || pcur == id_record[0])
            {
                mb->freenext = pcur->freenext;
                pcur->freenext = mb;
                break;
            }
            else
            {
                pcur = pcur->pre;
            }
        }
    }
    else if (algotype == 2)
    {
        Insert(mb);
    }
}
void Recycle(struct MemoryBlock* mb)
{
    int index = (mb->pre->stat) * 1 + (mb->nxt->stat) * 2;
    switch (index)
    {
    case 0:
	RecycleStat_3(mb);
	break;
    case 1:
	RecycleStat_2(mb);
	break;
    case 2:
	RecycleStat_1(mb);
	break;
    case 3:
	RecycleStat_4(mb);
	break;
    default:
	break;
    }
}
int Free(unsigned int id, size_t nsize)
{
    if (id_record[id] != NULL && id_record[id]->stat == 1)
    {
	if (id_record[id]->size == nsize)	//������������ͷŶ���
	{
	    id_record[id]->stat = 0;
            id_record[id]->id = 0;
	    Recycle(id_record[id]);
	    id_record[id] = NULL;
	    return 1;
	}
	else
	{
	    printf("free memory size invalid!\n");
	    return 0;
	}
    }
    else
    {
	printf("free memory command invalid\n");
    }
}
void PrintStat()
{
    struct MemoryBlock* pcur = id_record[0]->nxt;
    printf("* * * * * * * * * * ��ǰ�ڴ������� * * * * * * * * * *\n\n");
    printf("��ʼ��ַ\t�ڴ��С\t״̬\t\t����id\n");
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    while (pcur)
    {
        printf(" %-4uk\t|\t %-3uk\t|\t %d\t|\t  %u    |\n", pcur->address, pcur->size, pcur->stat, pcur->id);
        printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
        pcur = pcur->nxt;
    }
}