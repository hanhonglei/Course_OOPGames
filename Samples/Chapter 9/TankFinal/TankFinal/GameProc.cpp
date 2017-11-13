/************************************************************************/
/*	���ļ���Ҫʵ����Ϸ���й����еĸ��ֹ���
	����Ҫ���������Ϸ���в���*/
/************************************************************************/
#include "stdafx.h"
#include "GameProc.h"
#include "Init.h"

int nScore = 0;				// ��ҵ÷�
int bFire = 0;				// �Ƿ��ڿ���״̬
int wndWidth = 0;	int wndHeight = 0;  // ���ڳߴ�
LEVEL curLevel = OPEN;		// ��ǰ�����Ĺؿ�

Entity *pHeadBlocks = NULL;	// �ϰ�������
Entity *pHeadEnemys = NULL;	// ��������
Entity *pPlayer = NULL;		// ���
Entity *pHeadBullets = NULL;// ����ӵ�����
Entity *pHeadEnemyBullets = NULL;// �����ӵ�����

extern int reset_time;		// �Ƿ�����������ʱ��

// ���������ؿ��Ĳ�ͬ,�������һ������
void EnemyBirth()
{
	if (curLevel != LEVEL1 && curLevel != LEVEL2)	// ֻ���ڽ��������ؿ�ʱ,���б�Ҫ����Ϸ���ݽ��г�ʼ��
		return;

	int type = 0;
	switch (curLevel)
	{
	case LEVEL1:
		type =	(rand()%3)== 0 ? 1: 0;	// ����֮���ļ��ʵõ����ٵ�̹��
		break;
	case LEVEL2:
		type =	(rand()%3)== 0 ? 0: 1;	// ����֮���ļ��ʵõ����ٵ�̹��
		break;
	}
	// ����Init�еĵ������ɺ���,��ɶԸõ��˵ĳ�ʼ��
	EnemyBirth(pHeadEnemys, type, wndWidth);
}
// ��������Ϣ
void EnterKey(int key)
{
	switch(curLevel)
	{
	case OPEN:					// ����������,ֻ�Իس������д���
		if (key == VK_RETURN)
			ChangeLevel(LEVEL1);
		break;
	case LEVEL2_OPEN:			
		if (key == VK_RETURN)
			ChangeLevel(LEVEL2);
		break;
	case FAIL:
	case SUCCEED:
		if (key == VK_RETURN)
			ChangeLevel(OPEN);
		break;
	case LEVEL1:
	case LEVEL2:
		switch (key)	// ��������������Ϣ������ҿ��Ƶ�̹��״̬
		{
		case VK_LEFT:
			pPlayer->dir = LEFT;
			pPlayer->p = 0;
			break;
		case VK_RIGHT:
			pPlayer->dir = RIGHT;
			pPlayer->p = 0;
			break;
		case VK_UP:
			pPlayer->dir = UP;
			pPlayer->p = 0;
			break;
		case VK_DOWN:
			pPlayer->dir = DOWN;
			pPlayer->p = 0;
			break;
		case VK_SPACE:
			bFire = 1;
			break;
		}		
		break;
	}
}
// �Ը�������ͷ�����ж�̬����
void NewEntity()
{
	if(pHeadBlocks == NULL)
	{
		pHeadBlocks = (Entity*)malloc(sizeof(Entity));
		pHeadBlocks->next = NULL;
	}
	if(pHeadEnemys == NULL)
	{
		pHeadEnemys = (Entity*)malloc(sizeof(Entity));
		pHeadEnemys->next = NULL;
	}
	if(pPlayer == NULL)
	{
		pPlayer = (Entity*)malloc(sizeof(Entity));
		pPlayer->next = NULL;
	}
	if(pHeadBullets == NULL)
	{
		pHeadBullets = (Entity*)malloc(sizeof(Entity));
		pHeadBullets->next = NULL;
	}
	if(pHeadEnemyBullets == NULL)
	{
		pHeadEnemyBullets = (Entity*)malloc(sizeof(Entity));
		pHeadEnemyBullets->next =NULL;
	}
}
// ���ڴ�С�ı�֪ͨ����,�ļ���ȫ�ֱ����б��洰�ڵĳߴ�
void ChangeWndSize(int w, int h)
{
	wndWidth = w, wndHeight = h;
}
// ת���ؿ�
void ChangeLevel(LEVEL newL)
{
	curLevel = newL;
	Init(newL);		// ��ʼ����Ϸ����
	if (newL != LEVEL1 && newL != LEVEL2)	// ֻ���ڽ��������ؿ�ʱ,���б�Ҫ����Ϸ���ݽ��г�ʼ��
		return;
	NewEntity();	// �½���������ͷ
	InitEntity(pHeadEnemys, pHeadBlocks, pHeadBullets, pHeadEnemyBullets, pPlayer, newL);	// ����Щ����ͷ���г�ʼ��
	ResetPlayer(pPlayer);	// ���������Ϣ
	nScore = 0;		// ������Ϸͳ����Ϣ
	reset_time = 1;
}
// ���������Ϣ
void ResetPlayer(Entity *player)
{
	player->p = 1;
	player->dir = UP;
	player->x = wndWidth/2;
	player->y = wndHeight-player->s;
}
// �����Ϸÿ֡�Ļ���
void Draw(HDC hdc, HWND hWnd)
{
	// ���µĲ�����Ϊ�˱��������Ļ��˸,��������Ƶ��ڴ���,һ���Կ�������Ļ��
	//�����ڴ�HDC
	HDC memHDC = CreateCompatibleDC(hdc);

	//��ȡ�ͻ�����С
	RECT rectClient;
	GetClientRect(hWnd, &rectClient);

	//����λͼ
	HBITMAP bmpBuff = CreateCompatibleBitmap(hdc,wndWidth,wndHeight);
	HBITMAP pOldBMP = (HBITMAP)SelectObject(memHDC, bmpBuff);
	PatBlt(memHDC,0,0,wndWidth,wndHeight,WHITENESS);	// ���ñ���Ϊ��ɫ

	// ���������Ļ���
	DrawScene(memHDC, curLevel);

	//�����ڴ�HDC���ݵ�ʵ��HDC
	BOOL tt = BitBlt(hdc, rectClient.left, rectClient.top, wndWidth,
		wndHeight, memHDC, rectClient.left, rectClient.top, SRCCOPY);

	//�ڴ����
	SelectObject(memHDC, pOldBMP);
	DeleteObject(bmpBuff);
	DeleteDC(memHDC);
}
// ���ݵ�ǰ�����Ĺؿ�,����������Ϸ����,�����е��ø��ԵĻ��ƺ�����������
void DrawScene(HDC hdc, LEVEL curL)
{
	HFONT hf;
	WCHAR str[32];
	SetBkMode(hdc, TRANSPARENT);	// ������͸��ɫ
	long lfHeight;
	lfHeight = -MulDiv(18, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	HFONT hfOld;
	hf = CreateFont(lfHeight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
	hfOld = (HFONT)SelectObject(hdc, hf);
	SetTextColor(hdc, RGB(122, 0, 0));
	HBITMAP cb = GetLogoPic();
	switch (curL)
	{
	case OPEN:		// �����׶�,�����ʾ��Ϣ
		cb = GetLogoPic();
		wsprintf(str, L"%s", L"���»س�����ʼ��Ϸ");	
		break;
	case LEVEL1:	// ��Ϸ���н׶�,��ɶ���Ϸ�������ݵĻ���
	case LEVEL2:
		SetTextColor(hdc, RGB(100, 0, 0));
		lfHeight = -MulDiv(16, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		DeleteObject(hf);
		hf = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
		SelectObject(hdc, hf);
		wsprintf(str, L"Life:%d Score:%d", pPlayer->life, nScore);	
		DrawGameScene(hdc);
		break;
	case LEVEL2_OPEN:// �����׶�,�����ʾ��Ϣ
		cb = GetSuccPic();
		wsprintf(str, L"%s", L"���»س�����ʼ��һ��");	
		break;
	case SUCCEED:	// �����ʾ��Ϣ
		cb = GetSuccPic();
		wsprintf(str, L"%s", L"ʤ�������»س������¿�ʼ");	
		break;
	case FAIL:		// �����ʾ��Ϣ
		cb = GetFailPic();
		wsprintf(str, L"%s", L"ʧ�ܣ����»س������¿�ʼ");	
		break;
	}
	if (curL==OPEN||curL==LEVEL2_OPEN||curL==FAIL||curL==SUCCEED)
	{
		// ���Ʊ���
		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, cb);
		BITMAP bm;
		GetObject(cb, sizeof(bm), &bm);
		SetStretchBltMode(hdc,STRETCH_HALFTONE);
		BitBlt(hdc, wndWidth/2-bm.bmWidth/2, wndHeight/2-bm.bmHeight/2, wndWidth, wndHeight, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, hbmOld);
		DeleteDC(hdcMem);

	}
	TextOut(hdc, wndWidth/2-100, wndHeight-100,str,wcslen(str));

	SelectObject(hdc, hfOld);		// �ָ�Ĭ������
	DeleteObject(hf);
}
// �����Ϸ���ݵĻ���
void DrawGameScene(HDC hdc)
{
	// ���Ʊ���
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, GetBackPic());
	BITMAP bm;
	GetObject(GetBackPic(), sizeof(bm), &bm);
	SetStretchBltMode(hdc,STRETCH_HALFTONE);
	BitBlt(hdc, 0, 0, wndWidth, wndHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	// ������Ϸʵ��
	DrawEntities(hdc, pHeadEnemys);
	DrawEntities(hdc, pHeadBullets);
	DrawEntities(hdc, pHeadEnemyBullets);
	DrawEntities(hdc, pHeadBlocks);
	DrawEntity(hdc, pPlayer);
}
// �Զ�̬��������ݽ����ͷ�,�����ڴ�й¶
void Destroy()
{
	Destroy(pHeadBlocks);
	Destroy(pHeadEnemys);
	Destroy(pHeadBullets);
	Destroy(pHeadEnemyBullets);

	free(pPlayer);
	free(pHeadBlocks);
	free(pHeadEnemys);
	free(pHeadBullets);
	free(pHeadEnemyBullets);

	pHeadBlocks = NULL;		
	pHeadEnemys = NULL;		
	pPlayer = NULL;			
	pHeadBullets = NULL;	
	pHeadEnemyBullets = NULL;
}
// ������Ϸ�����еĶ���Ԫ��
void NextFrame(int ts)
{
	// ����Entity�еĺ���,��ɸ����ж�������ĵ�ǰ֡����
	NextFrameEntites(pHeadEnemys, ts);
	NextFrameEntites(pHeadBullets, ts);
	NextFrameEntites(pHeadEnemyBullets, ts);
	NextFrame(pPlayer, ts);
}
// ���¸�����Ϸ��Ϣ,��ʱ���ᴥ���������
void Update(int ts)
{
	if (curLevel != LEVEL1 && curLevel != LEVEL2)
		return;

	// ����Entity�еĺ���,��ɸ��ֿ��ƶ��������λ�ø���
	MoveEntities(pHeadEnemys, ts);
	MoveEntities(pHeadBullets, ts);
	MoveEntities(pHeadEnemyBullets, ts);
	Move(pPlayer, ts);

	// ��Ҵ��ڿ���״̬�Ļ�,����Entity�еĺ��������ӵ�
	if (bFire)
	{
		Fire(pPlayer, pHeadBullets);
		bFire = 0;
	}

	Entity *freeP = NULL;
	// �ж��ӵ��Ƿ�͵�����ײ
	for(Entity *ip = pHeadBullets->next; ip != NULL; ip = ip->next)	// �����ӵ�����
	{
		if (freeP != NULL)				// �����һ���ӵ������˵���,�������Ǹ��ӵ�
		{
			Destroy(pHeadBullets, freeP);
			freeP = NULL;
		}
		for (Entity *jp = pHeadEnemys->next; jp != NULL; jp = jp->next)
		{
			if (IsCollide(ip, jp))		// ����ӵ������˵�ǰ�ĵ���
			{
				freeP = ip;				// ��¼������ײ���ӵ�ָ��,���´��ӵ�ѭ���н���ɾ��
				jp->life--;
				if (jp->life <= 0)
				{
					Destroy(pHeadEnemys, jp);	// ���ٵ���
					nScore++;					// �÷�
				}
				break;
			}
		}
	}
	if (freeP != NULL)					// ��������һ���ӵ������˵���,������
	{
		Destroy(pHeadBullets, freeP);
		freeP = NULL;
	}

	// �жϵ��˵��ӵ��Ƿ�������ײ
	for (Entity *p = pHeadEnemyBullets->next; p != NULL; p = p->next)
	{
		if (IsCollide(p, pPlayer))
		{
			Destroy(pHeadEnemyBullets, p);
			ResetPlayer(pPlayer);
			pPlayer->life--;
			break;
		}
	}
	// �жϵ����Ƿ�������ײ
	for (Entity *p = pHeadEnemys->next; p != NULL; p = p->next)
	{
		if (RandomFire())
			Fire(p, pHeadEnemyBullets);


		if (IsCollide(pPlayer,p))
		{
			ResetPlayer(pPlayer);
			pPlayer->life--;
		}
	}
	// �жϸ���ʵ���Ƿ����Ϸ�߽緢����ײ
	for (Entity *p = pHeadEnemys->next; p != NULL; p = p->next)
	{
		if (!WallCollide(p, wndWidth, wndHeight))		// ��һ�����ʸı䷽��
		{
			int cg = 0;
			for (Entity *q = pHeadBlocks->next; q != NULL; q = q->next)	// �жϵ����Ƿ�ͳ������ϰ��﷢����ײ
			{
				if (IsCollide(p, q))
				{
					cg = 1;

					Move(p, -ts);					// ����һ��
					break;
				}
			}
			if (RandomDir() || cg)
				p->dir = Dir((p->dir+1+rand()%3)%4);
		}
		else
			p->dir = Dir((p->dir+1+rand()%3)%4);

	}
	// �ж���ҽ�ɫ�Ƿ���ϰ��﷢����ײ,�����,��ֹͣ
	for (Entity *p = pHeadBlocks->next; p != NULL; p = p->next)	
	{
		if (IsCollide(p, pPlayer))
		{
			switch(pPlayer->dir)
			{
			case UP:
				pPlayer->y = p->y+(p->s+pPlayer->s)/2;
				break;
			case DOWN:
				pPlayer->y = p->y-(p->s+pPlayer->s)/2;
				break;
			case LEFT:
				pPlayer->x = p->x+(p->s+pPlayer->s)/2;
				break;
			case RIGHT:
				pPlayer->x = p->x-(p->s+pPlayer->s)/2;
				break;
			}
			pPlayer->p = 1;
			break;
		}
	}
	// �ж��ӵ��ͱ߽����ײ
	for (Entity *p = pHeadBullets->next; p != NULL; p = p->next)	
	{
		if (freeP != NULL)
		{
			Destroy(pHeadBullets, freeP);
			freeP = NULL;
		}
		if (WallCollide(p, wndWidth, wndHeight))
			freeP = p;
	}
	if (freeP != NULL)
	{
		Destroy(pHeadBullets, freeP);
		freeP = NULL;
	}

	for (Entity *p = pHeadEnemyBullets->next; p != NULL; p = p->next)	
	{
		if (freeP != NULL)
		{
			Destroy(pHeadEnemyBullets, freeP);
			freeP = NULL;
		}
		if (WallCollide(p, wndWidth, wndHeight))
			freeP = p;
	}
	if (freeP != NULL)
	{
		Destroy(pHeadEnemyBullets, freeP);
		freeP = NULL;
	}

	// ��Һͱ߽����ײ
	if (WallCollide(pPlayer, wndWidth, wndHeight))
		pPlayer->p = 1;

	if (pPlayer->life <= 0)
	{
		ChangeLevel(FAIL);
	}
	else if (nScore >= GetPassScore())	// ���ݵ÷־����Ƿ����
	{
		switch (curLevel)
		{
		case LEVEL1:
			ChangeLevel(LEVEL2_OPEN);
			break;
		case LEVEL2:
			ChangeLevel(SUCCEED);
			break;
		}
	}
}