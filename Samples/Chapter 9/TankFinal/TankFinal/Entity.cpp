// TankPlus.cpp : Defines the entry point for the application.
//
/************************************************************************/
/*	������Ϸʵ���������Ĺ��� */
/************************************************************************/
#include "stdafx.h"
#include "Entity.h"
// ���������е�ÿ��ʵ��
void DrawEntities(HDC hdc, const Entity *head)
{
	Entity* ent = head->next;
	while(ent != NULL)
	{
		DrawEntity(hdc, ent);
		ent = ent->next;
	}
}
// ���Ʋ���ָ������Ϸʵ��
void DrawEntity(HDC hdc, const Entity *ent)
{
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP bmp = ent->a[int(ent->dir)%4];
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, bmp);
	BITMAP bm;
	GetObject(bmp, sizeof(bm), &bm);
	SetStretchBltMode(hdc,STRETCH_HALFTONE);
	// ��Ӷ���Ч�������Ƶ�ǰ֡
	TransparentBlt(hdc,ent->x-ent->s/2, ent->y-ent->s/2, ent->s, ent->s, hdcMem, ent->f*bm.bmWidth/(ent->frames), 0, bm.bmWidth/ent->frames, bm.bmHeight,RGB(255,255,255)); // ��ɫ��Ϊ͸��ɫ��Library Msimg32.lib
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
}
// �������е�ʵ�嵱ǰ֡����Ϊ��һ֡
void NextFrameEntites(Entity *head, int ts)
{
	if (head == NULL)
		return;
	Entity* ent = head->next;
	while(ent != NULL)
	{
		NextFrame(ent, ts);
		ent = ent->next;
	}
}
// ���ض�ʵ������Ϊ��һ֡
void NextFrame(Entity *ent, int ts)
{
	if (ent == NULL)
		return;
	if(ent->f == ent->frames-1)
		ent->animD = 1;
	else if (ent->f == 0)
		ent->animD = 1;

	ent->f = (ent->f+ent->animD)%ent->frames;
}
// �������е�ʵ������ƶ�
void MoveEntities(Entity *head, int ts)
{
	if (head == NULL)
		return;
	Entity* ent = head->next;
	while(ent != NULL)
	{
		Move(ent, ts);
		ent = ent->next;
	}
}
// ���ض���Ϸʵ������ƶ�
void Move(Entity *ent, int ts)
{
	if (ent->p)
		return;
	switch(ent->dir)
	{
	case UP:
		ent->y -= ent->v * ts;
		break;
	case DOWN:
		ent->y += ent->v * ts;
		break;
	case LEFT:
		ent->x -= ent->v * ts;
		break;
	case RIGHT:
		ent->x += ent->v * ts;
		break;
	}
}
// �ض�ʵ������������
void Fire(const Entity* ent, Entity *pHeadB)
{
	Entity *newBullet = (Entity*)malloc(sizeof(Entity));
	memcpy(newBullet, pHeadB, sizeof(Entity));
	newBullet->dir = ent->dir;
	newBullet->x = ent->x;
	newBullet->y = ent->y;
	switch(ent->dir)
	{
	case UP:
		newBullet->y -= ent->s;
		break;
	case DOWN:
		newBullet->y += ent->s;
		break;
	case LEFT:
		newBullet->x -= ent->s;
		break;
	case RIGHT:
		newBullet->x += ent->s;
		break;
	}
	newBullet->next = pHeadB->next;
	pHeadB->next = newBullet;
}
// ���ض���ʵ���������ɾ��
void Destroy(Entity *pHead, Entity* ent)
{
	Entity *p = pHead;
	Entity *pn = pHead->next;
	while(pn != NULL)
	{
		if (pn == ent)
		{
			p->next = pn->next;
			free(pn);
			return;
		}
		p = pn;
		pn = pn->next;
	}
}
// ɾ��ͷ��������������
void Destroy(Entity *pHead)
{
	if (pHead == NULL)	return;
	Entity *p = pHead;
	Entity *pn = pHead->next;
	while(pn != NULL)
	{
		p->next = pn->next;
		free(pn);
		pn = p->next;
	}
	pHead->next = NULL;
}
// �ж�����ʵ���Ƿ�����ײ,��������֮�䷢����ײ���ж�
int IsCollide(const Entity *ent1, const Entity *ent2)
{

	if (ent1->x+ent1->s/2 <= ent2->x-ent2->s/2 || ent1->x-ent1->s/2 >= ent2->x + ent2->s/2)
		return 0;
	if (ent1->y+ent1->s/2 <= ent2->y-ent2->s/2 || ent1->y-ent1->s/2 >= ent2->y + ent2->s/2)
		return 0;
	return 1;
}
// �ж��ض�ʵ���Ƿ�ͱ߽緢����ײ
int WallCollide(Entity *ent, int w, int h)
{
	int bC = 0;
	switch(ent->dir)
	{
	case UP:
		if ((ent->y - ent->s/2) < 0)	// �ϱ߽�
		{
			bC = 1;
			ent->y = ent->s/2;
		}
		break;
	case DOWN:
		if ((ent->y + ent->s/2) > h)	// �±߽�
		{
			bC = 1;
			ent->y = h - ent->s/2;
		}
		break;
	case LEFT:
		if((ent->x - ent->s/2) < 0)		// ��߽�
		{
			bC = 1;
			ent->x = ent->s/2;
		}
		break;
	case RIGHT:
		if((ent->x + ent->s/2) > w)		// �ұ߽�
		{
			bC = 1;
			ent->x = w - ent->s/2;
		}
		break;
	}
	return bC;
}
