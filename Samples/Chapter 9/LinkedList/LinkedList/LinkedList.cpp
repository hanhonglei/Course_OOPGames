// test.cpp : Defines the entry point for the console application.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <crtdbg.h>		// �ж��ڴ�й©
// �������ڵ�ṹ������
typedef struct P{
	float hp;
	float mp;
	char name[16];
	struct P *next;
}Player;
// �������ͷ�ڵ�ṹ������
typedef struct{
	int num;
	Player* next;
}PlayerLinklist;
// �����������һ���ڵ㣬�˴�ֱ�Ӳ嵽����ͷ��
void AddNewPlayer(PlayerLinklist *head, Player *newPlayer)
{
	newPlayer->next = head->next;
	head->next = newPlayer;
	head->num++;
}
// ��������ɾ��һ���ڵ㣬��Ҫע�������ܶϿ�
void RemovePlayer(PlayerLinklist *head, Player *player)
{
	Player *p = head->next;
	if (p == player)	// ��һ���ڵ��Ǵ�ɾ���ڵ�
	{
		head->next = p->next;
		free(p);
		head->num--;
		return;
	}
	while (p->next != player && p->next != NULL)
		p = p->next;
	if (p->next == player)	// ɾ���ڵ㣬��ֹ����
	{
		Player *tmp = p->next->next;
		free(p->next);
		p->next = tmp;
		head->num--;
	}
}
// �������в���ָ�����ԵĽڵ�
Player *FindeByName(PlayerLinklist *head, char* name)
{
	Player *p = head->next;
	while (p != NULL && strcmp(p->name, name) != 0)
		p = p->next;
	return p;
}
// ���õݹ麯��ʵ�֣�������ҵ�hp�������еĽڵ���������
// ����ֵΪ����֮���ͷ�ڵ�ָ��
Player* SortByHP(Player *first)
{
	// ������û�нڵ����ֻ��һ������ֱ�ӷ���
	if (first == NULL || first->next == NULL)
		return first;
	Player *minPre = first;	// ������������СԪ�ص�ǰ��һ��Ԫ��ָ��
	Player *p = first->next;

	// ��minPre����Ϊ��С��Ԫ��ǰ��һ��Ԫ�ص�ָ��
	while(p->next != NULL)
	{
		if (p->next->hp < minPre->next->hp)
			minPre = p;
		p = p->next;
	}
	if (minPre->next->hp < first->hp)	// �Ƚ���С�ڵ���׽ڵ��С
		// ����С�ڵ���뵽�׽ڵ�ǰ��
	{
		p = minPre->next;
		minPre->next = p->next;
		p->next = first;
		first = p;
	}
	first->next = SortByHP(first->next);// �ݹ飬�������Ľڵ�����С�Ľڵ���Ϊ�׽ڵ㣬�����¹ҽӵ�������
	return first;
}
// �������нڵ㣬��ӡ�����е������Ϣ
void Print(PlayerLinklist *head)
{
	Player* p = head->next;
	while (p != NULL)
	{
		printf("%s,%.2f,%.2f\t",p->name, p->hp, p->mp);
		p = p->next;
	}
	printf("\n");
}
// �ͷ�������ʱռ���ڴ�
void Destroy(PlayerLinklist *head)
{
	Player* p = head->next;
	Player* q = p->next;
	while (q != NULL)
	{
		q = p->next;
		free(p);
		p = q;
	}
	free(head);
}

int main()
{
	srand(time(NULL));		// �������
	// ��̬�����������ͷ
	PlayerLinklist *head = (PlayerLinklist*)malloc(sizeof(PlayerLinklist));
	head->next = NULL;
	head->num = 0;
	int nCand = rand()%20;	// ��������Һ�ɾ����������
	char *find = NULL;		// �������������
	for (int i = 0; i < 20; i++)	// �������20����ң�����ӵ�������
	{
		Player *p = (Player*)malloc(sizeof(Player));
		p->hp = (rand()%101)/100.0f;	// ���Ѫ��
		p->mp = (rand()%101)/100.0f;	// ���ħ��ֵ
		p->name[0] = 'A' + rand()%26;	// ��������֣�Ϊ�˼��ֻʹ��3����ĸ
		p->name[1] = 'A' + rand()%26;
		p->name[2] = 'A' + rand()%26;
		p->name[3] = '\0';
		AddNewPlayer(head, p);			// �������ɵ���Ҽ����б�
		if (nCand == i)
			find = p->name;
	}
	Print(head);						// ��ӡ���ɵ��������

	Player *p = FindeByName(head, find);// ����������
	printf("The player found:name:%s,hp:%.2f,mp:%.2f\n", p->name, p->hp, p->mp);

	RemovePlayer(head, p);				// ɾ��������
	printf("Remove it from the list\n");
	Print(head);

	printf("Sort the list by hp\n");	// ����
	head->next = SortByHP(head->next);
	Print(head);
	// ���������ڳ���������к��output����������ڴ�й©��Ϣ
	// ���߳��Խ������Destroy�����������ɾ�����ͻᵼ���ڴ�й©
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	Destroy(head);						// �ƺ󣬽�������ʱ�����ڴ�ɾ��

	getchar();
	return 0;
}
