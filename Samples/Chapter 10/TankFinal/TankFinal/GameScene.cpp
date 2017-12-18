/************************************************************************/
/*	���ļ���Ҫʵ����Ϸ���й����еľ�����Ϸ�ؿ����ֹ���*/
/************************************************************************/
#include "StdAfx.h"
#include "GameScene.h"
#include "resource.h"
#include "GameManage.h"
#include <fstream>
#include <mmsystem.h>

GameScene::GameScene(LEVEL l):curL(l)
{
	fileIni = L"Resources\\Init.ini";	// ���ڱ��浱ǰ��Ҫ�����Ini�ļ���	
	block = NULL;
	background = NULL;					// ���������ͼƬ
	nScore = 0;							// ������Ϸͳ����Ϣ
	bFire = false;						// �Ƿ��ڿ���״̬
	nLife = 3;							// �������
	enemyFirePer = 300;					// ���˷����ӵ����������
	enemyDirPer = 200;					// ��������ı䷽��ı���
	nPassScore = 10;					// ���ط���
	pPlayer = NULL;
	Init();		// ��ʼ����Ϸ����
	if(curL != LEVEL1 && curL != LEVEL2)	// ֻ���ڽ��������ؿ�ʱ,���б�Ҫ����Ϸ���ݽ��г�ʼ��
		return;
	InitMap();
	ResetPlayer();	// ���������Ϣ
}

GameScene::~GameScene(void)
{
	for (list<Tank*>::iterator ite = pHeadEnemys.begin(); ite != pHeadEnemys.end(); ite++)
		delete (*ite);
	for (list<Entity*>::iterator ite = pHeadBullets.begin(); ite != pHeadBullets.end(); ite++)
		delete (*ite);
	for (list<Entity*>::iterator ite = pHeadEnemyBullets.begin(); ite != pHeadEnemyBullets.end(); ite++)
		delete (*ite);
	for (list<Entity*>::iterator ite = pHeadBlocks.begin(); ite != pHeadBlocks.end(); ite++)
		delete (*ite);
	if (!pPlayer)
		delete pPlayer;
	pHeadBlocks.clear();
	pHeadBullets.clear();
	pHeadEnemyBullets.clear();
	pHeadEnemys.clear();
	// Stop all sounds
	PlaySound(NULL, GameManage::hInst, NULL);
}
// ���������е�ÿ��ʵ��
void GameScene::DrawEntities(HDC hdc)
{
	for (list<Tank*>::iterator ite = pHeadEnemys.begin(); ite != pHeadEnemys.end(); ite++)
		(*ite)->DrawEntity(hdc);
	for (list<Entity*>::iterator ite = pHeadBullets.begin(); ite != pHeadBullets.end(); ite++)
		(*ite)->DrawEntity(hdc);
	for (list<Entity*>::iterator ite = pHeadEnemyBullets.begin(); ite != pHeadEnemyBullets.end(); ite++)
		(*ite)->DrawEntity(hdc);
	for (list<Entity*>::iterator ite = pHeadBlocks.begin(); ite != pHeadBlocks.end(); ite++)
		(*ite)->DrawEntity(hdc);

	pPlayer->DrawEntity(hdc);
}

// �������е�ʵ������ƶ�
void GameScene::MoveEntities(int ts)
{
	for (list<Tank*>::iterator ite = pHeadEnemys.begin(); ite != pHeadEnemys.end(); ite++)
		(*ite)->Move(ts);
	for (list<Entity*>::iterator ite = pHeadBullets.begin(); ite != pHeadBullets.end(); ite++)
		(*ite)->Move(ts);
	for (list<Entity*>::iterator ite = pHeadEnemyBullets.begin(); ite != pHeadEnemyBullets.end(); ite++)
		(*ite)->Move(ts);
	pPlayer->Move(ts);
}
// ���������ؿ��Ĳ�ͬ,�������һ������
void GameScene::EnemyBirth()
{
	if (curL != LEVEL1 && curL != LEVEL2)	// ֻ���ڽ��������ؿ�ʱ,���б�Ҫ����Ϸ���ݽ��г�ʼ��
		return;

	int type = 0;
	switch (curL)
	{
	case LEVEL1:
		type =	(rand()%3)== 0 ? 1: 0;	// ����֮���ļ��ʵõ����ٵ�̹��
		break;
	case LEVEL2:
		type =	(rand()%3)== 0 ? 0: 1;	// ����֮���ļ��ʵõ����ٵ�̹��
		break;
	}

	Tank *ent;
	int size = GetPrivateProfileInt(L"Enemy", L"size",50,fileIni);

	switch (type)
	{
	case 0:
		ent = new Tank((rand()%3)*(GameManage::wndWidth-size)/2 + size/2, size, Dir(UP+rand()%4), GetPrivateProfileInt(L"Enemy", L"vels",2, fileIni)
			,size, 1, GetPrivateProfileInt(L"Enemy", L"frames",1, fileIni), false, 0, tank2);
		break;
	case 1:
		ent = new Tank((rand()%3)*(GameManage::wndWidth-size)/2 + size/2, size, Dir(UP+rand()%4), GetPrivateProfileInt(L"Enemy", L"velf",4, fileIni)
			,size, 2, GetPrivateProfileInt(L"Enemy", L"frames",1, fileIni), false, 0, tank3);
		break;
	}
	pHeadEnemys.push_back(ent);

}
// ��������Ϣ
void GameScene::EnterKey(int key)
{
	switch(curL)
	{
	case OPEN:					// ����������,ֻ�Իس������д���
		if (key == VK_RETURN)
			GameManage::ChangeLevel(LEVEL1);
		break;
	case LEVEL2_OPEN:			
		if (key == VK_RETURN)
			GameManage::ChangeLevel(LEVEL2);
		break;
	case FAIL:
	case SUCCEED:
		if (key == VK_RETURN)
			GameManage::ChangeLevel(OPEN);
		break;
	case LEVEL1:
	case LEVEL2:
		switch (key)	// ��������������Ϣ������ҿ��Ƶ�̹��״̬
		{
		case VK_LEFT:
			pPlayer->ChangeDir(LEFT);
			break;
		case VK_RIGHT:
			pPlayer->ChangeDir(RIGHT);
			break;
		case VK_UP:
			pPlayer->ChangeDir(UP);
			break;
		case VK_DOWN:
			pPlayer->ChangeDir(DOWN);
			break;
		case VK_SPACE:
			bFire = true;
			break;
		}		
		break;
	}
}
// ���������Ϣ
void GameScene::ResetPlayer()
{
	if (pPlayer == NULL)
		pPlayer = new Tank(GameManage::wndWidth/2, GameManage::wndHeight-GetPrivateProfileInt(L"Player", L"size",50, fileIni), UP, GetPrivateProfileInt(L"Player", L"vel",2, fileIni), GetPrivateProfileInt(L"Player", L"size",50, fileIni)
		,nLife, GetPrivateProfileInt(L"Player", L"frames",1, fileIni), true, 0, tank1);

	pPlayer->SetPos(GameManage::wndWidth/2, GameManage::wndHeight-GetPrivateProfileInt(L"Player", L"size",50, fileIni));
}
// �����Ϸÿ֡�Ļ���
void GameScene::Draw(HDC hdc, HWND hWnd)
{
	// ���µĲ�����Ϊ�˱��������Ļ��˸,��������Ƶ��ڴ���,һ���Կ�������Ļ��
	//�����ڴ�HDC
	HDC memHDC = CreateCompatibleDC(hdc);

	//��ȡ�ͻ�����С
	RECT rectClient;
	GetClientRect(hWnd, &rectClient);

	//����λͼ
	HBITMAP bmpBuff = CreateCompatibleBitmap(hdc,GameManage::wndWidth,GameManage::wndHeight);
	HBITMAP pOldBMP = (HBITMAP)SelectObject(memHDC, bmpBuff);
	PatBlt(memHDC,0,0,GameManage::wndWidth,GameManage::wndHeight,WHITENESS);	// ���ñ���Ϊ��ɫ

	// ���������Ļ���
	DrawScene(memHDC);

	//�����ڴ�HDC���ݵ�ʵ��HDC
	BOOL tt = BitBlt(hdc, rectClient.left, rectClient.top, GameManage::wndWidth,
		GameManage::wndHeight, memHDC, rectClient.left, rectClient.top, SRCCOPY);

	//�ڴ����
	SelectObject(memHDC, pOldBMP);
	DeleteObject(bmpBuff);
	DeleteDC(memHDC);
}
// ���ݵ�ǰ�����Ĺؿ�,����������Ϸ����,�����е��ø��ԵĻ��ƺ�����������
void GameScene::DrawScene(HDC hdc)
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
	HBITMAP cb = GameManage::logo;
	switch (curL)
	{
	case OPEN:		// �����׶�,�����ʾ��Ϣ
		cb = GameManage::logo;
		wsprintf(str, L"%s", L"���»س�����ʼ��Ϸ");	
		break;
	case LEVEL1:	// ��Ϸ���н׶�,��ɶ���Ϸ�������ݵĻ���
	case LEVEL2:
		SetTextColor(hdc, RGB(100, 0, 0));
		lfHeight = -MulDiv(16, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		DeleteObject(hf);
		hf = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
		SelectObject(hdc, hf);
		wsprintf(str, L"Life:%d Score:%d", pPlayer->GetLife(), nScore);	
		DrawGameScene(hdc);
		break;
	case LEVEL2_OPEN:// �����׶�,�����ʾ��Ϣ
		cb = GameManage::succ_pic;
		wsprintf(str, L"%s", L"���»س�����ʼ��һ��");	
		break;
	case SUCCEED:	// �����ʾ��Ϣ
		cb = GameManage::succ_pic;
		wsprintf(str, L"%s", L"ʤ�������»س������¿�ʼ");	
		break;
	case FAIL:		// �����ʾ��Ϣ
		cb = GameManage::fail_pic;
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
		BitBlt(hdc, GameManage::wndWidth/2-bm.bmWidth/2, GameManage::wndHeight/2-bm.bmHeight/2, GameManage::wndWidth, GameManage::wndHeight, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, hbmOld);
		DeleteDC(hdcMem);

	}
	TextOut(hdc, GameManage::wndWidth/2-100, GameManage::wndHeight-100,str,wcslen(str));

	SelectObject(hdc, hfOld);		// �ָ�Ĭ������
	DeleteObject(hf);
}
// �����Ϸ���ݵĻ���
void GameScene::DrawGameScene(HDC hdc)
{
	// ���Ʊ���
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, GetBackPic());
	BITMAP bm;
	GetObject(GetBackPic(), sizeof(bm), &bm);
	SetStretchBltMode(hdc,STRETCH_HALFTONE);
	BitBlt(hdc, 0, 0, GameManage::wndWidth, GameManage::wndHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	// ������Ϸʵ��
	DrawEntities(hdc);
}

// ������Ϸ�����еĶ���Ԫ��
void GameScene::NextFrame(int ts)
{
	// ����Entity�еĺ���,��ɸ����ж�������ĵ�ǰ֡����
	for (list<Tank*>::iterator ite = pHeadEnemys.begin(); ite != pHeadEnemys.end(); ite++)
		(*ite)->NextFrame(ts);
	for (list<Entity*>::iterator ite = pHeadBullets.begin(); ite != pHeadBullets.end(); ite++)
		(*ite)->NextFrame(ts);
	for (list<Entity*>::iterator ite = pHeadEnemyBullets.begin(); ite != pHeadEnemyBullets.end(); ite++)
		(*ite)->NextFrame(ts);
	if (pPlayer!=NULL)
		pPlayer->NextFrame(ts);
}
// ���¸�����Ϸ��Ϣ,��ʱ���ᴥ���������
void GameScene::Update(int ts)
{
	if (curL != LEVEL1 && curL != LEVEL2)
		return;

	// ����Entity�еĺ���,��ɸ��ֿ��ƶ��������λ�ø���
	MoveEntities(ts);

	// ��Ҵ��ڿ���״̬�Ļ�,����Entity�еĺ��������ӵ�
	if (bFire) 
	{
		pHeadBullets.push_back(pPlayer->Fire(bulletPic,GetPrivateProfileInt(L"Bullet", L"vel",6, fileIni),GetPrivateProfileInt(L"Bullet", L"frames",1, fileIni),GetPrivateProfileInt(L"Bullet", L"size",20, fileIni)));
		bFire = false;
	}

	// �ж��ӵ��Ƿ�͵�����ײ,does collider with walls
	for (list<Entity*>::iterator ite = pHeadBullets.begin(); ite != pHeadBullets.end();)
	{
		// collider with walls?
		if ((*ite)->WallCollide(GameManage::wndWidth, GameManage::wndHeight))
		{
			delete *ite;
			ite = pHeadBullets.erase(ite);
			continue;
		}
		bool hit = false;
		for (list<Tank*>::iterator iteE = pHeadEnemys.begin(); iteE != pHeadEnemys.end();)
		{
			if ((*iteE)->IsCollide(*ite))		// ����ӵ������˵�ǰ�ĵ���
			{
				delete *ite;
				ite = pHeadBullets.erase(ite);
				if(!(*iteE)->BeHit())
				{
					delete *iteE;
					iteE = pHeadEnemys.erase(iteE);
					nScore++;					// �÷�
				}
				hit = true;
				break;
			}
			else
				++iteE;
		}
		if (!hit)
			++ite;
	}

	// �жϵ��˵��ӵ��Ƿ�������ײ
	for (list<Entity*>::iterator ite = pHeadEnemyBullets.begin(); ite != pHeadEnemyBullets.end();)
	{
		// collider with walls?
		if ((*ite)->WallCollide(GameManage::wndWidth, GameManage::wndHeight))
		{
			delete *ite;
			ite = pHeadEnemyBullets.erase(ite);
			continue;
		}
		if (pPlayer->IsCollide(*ite))
		{
			delete *ite;
			ite = pHeadEnemyBullets.erase(ite);
			pPlayer->BeHit();
			ResetPlayer();
			continue;
		}
		else
			++ite;
	}
	// �жϵ����Ƿ�������ײ
	for (list<Tank*>::iterator iteE = pHeadEnemys.begin(); iteE != pHeadEnemys.end();iteE++)
	{
		if (RandomFire())
			pHeadEnemyBullets.push_back((*iteE)->Fire(bulletPic,GetPrivateProfileInt(L"Bullet", L"vel",6, fileIni),GetPrivateProfileInt(L"Bullet", L"frames",1, fileIni),GetPrivateProfileInt(L"Bullet", L"size",20, fileIni)));

		if ((*iteE)->IsCollide(pPlayer))
		{
			ResetPlayer();
			pPlayer->BeHit();
		}

		if (!(*iteE)->WallCollide(GameManage::wndWidth, GameManage::wndHeight))		// ��һ�����ʸı䷽��
		{
			bool cg = false;
			for (list<Entity*>::iterator iteB = pHeadBlocks.begin(); iteB != pHeadBlocks.end(); iteB++)	// �жϵ����Ƿ�ͳ������ϰ��﷢����ײ
			{
				if ((*iteE)->IsCollide(*iteB))
				{
					cg = true;
					(*iteE)->Move(-ts);					// ����һ��
					break;
				}
			}
			if (RandomDir() || cg)
				(*iteE)->ChangeDir(Dir(((*iteE)->GetDir()+1+rand()%3)%4));
		}
		else
			(*iteE)->ChangeDir(Dir(((*iteE)->GetDir()+1+rand()%3)%4));
	}

	// �ж���ҽ�ɫ�Ƿ���ϰ��﷢����ײ,�����,��ֹͣ
	for (list<Entity*>::iterator iteB = pHeadBlocks.begin(); iteB != pHeadBlocks.end(); iteB++)	
	{
		if (pPlayer->IsCollide(*iteB))
		{
			switch(pPlayer->GetDir())
			{
			case UP:
				pPlayer->SetY((*iteB)->GetY()+((*iteB)->GetSize()+pPlayer->GetSize())/2);
				break;
			case DOWN:
				pPlayer->SetY((*iteB)->GetY()-((*iteB)->GetSize()+pPlayer->GetSize())/2);
				break;
			case LEFT:
				pPlayer->SetX((*iteB)->GetX()+((*iteB)->GetSize()+pPlayer->GetSize())/2);
				break;
			case RIGHT:
				pPlayer->SetX((*iteB)->GetX()-((*iteB)->GetSize()+pPlayer->GetSize())/2);
				break;
			}
			pPlayer->Pause(true);
			break;
		}
	}

	// ��Һͱ߽����ײ
	if (pPlayer->WallCollide(GameManage::wndWidth, GameManage::wndHeight))
		pPlayer->Pause(true);

	if (pPlayer->GetLife() <= 0)
	{
		GameManage::ChangeLevel(FAIL);
	}
	else if (nScore >= GetPassScore())	// ���ݵ÷־����Ƿ����
	{
		switch (curL)
		{
		case LEVEL1:
			GameManage::ChangeLevel(LEVEL2_OPEN);
			break;
		case LEVEL2:
			GameManage::ChangeLevel(SUCCEED);
			break;
		}
	}
}

// ���ص�ǰ�ı���ͼƬ���
HBITMAP GameScene::GetBackPic()
{
	return background;
}
// �Ƿ��������
int GameScene::RandomFire()
{
	return ((rand()%enemyFirePer) == 0);
}
// �Ƿ�����ı䷽��
int GameScene::RandomDir()
{
	return (rand()%enemyDirPer == 0);
}
// ��ȡͨ�ط���
int GameScene::GetPassScore()
{
	return nPassScore;
}
// ���������Ĺؿ�,ͨ��ini�ļ���ȡ���ֳ�ʼ����Ϣ
void GameScene::ReadIni()
{
	switch (curL)
	{
	case OPEN:
		fileIni = L"Resources\\Init.ini";
		break;
	case LEVEL1:
		fileIni = L"Resources\\Init.ini";
		break;
	case LEVEL2_OPEN:
		break;
	case LEVEL2:	
		fileIni = L"Resources\\Init2.ini";
		break;
	default:
		fileIni = L"Resources\\Init.ini";
		return;
	}
	nPassScore=GetPrivateProfileInt(L"Global", L"nPassScore",nPassScore, fileIni);
	enemyFirePer = GetPrivateProfileInt(L"Enemy", L"firePer",enemyFirePer, fileIni);
	enemyDirPer = GetPrivateProfileInt(L"Enemy", L"dirPer",enemyDirPer, fileIni);
	nLife = GetPrivateProfileInt(L"Player", L"nLife",nLife, fileIni);
}
// ���������Ĺؿ�����ȡ��ͬ�ĵ�ͼ�����ļ�,���Ϊ1�ĵط����ϰ���
void GameScene::InitMap()
{
	char *fileName = "Resources\\Map.txt";
	switch (curL)
	{
	case LEVEL1:
		fileName = "Resources\\Map.txt";
		break;
	case LEVEL2:
		fileName = "Resources\\Map2.txt";
		break;
	default:
		return;
	}
	ifstream f(fileName);
	if (f == NULL)
		return;
	char line[64];
	int nLine = 0;
	// block
	int size  = GetPrivateProfileInt(L"Block", L"size",50,fileIni);

	HBITMAP anim[4];
	anim[0] = anim[1] = anim[2] = anim[3] = block;

	while(f.getline(line, 64))
	{
		for (int i = 0; line[i] != '\0'; i++)
			if (line[i] == '1')
				pHeadBlocks.push_back(new Entity(size/2+size*i, size/2+size*nLine,NONE,0,size,1,1,true,0,anim));
		nLine++;
	}
	f.close();
}
// ���ݲ�ͬ�Ĺؿ�������Ϸ�������г�ʼ��
void GameScene::Init()
{
	// ���ȶ�ȡ�����ļ��еĲ������ȫ�ֱ���
	if (curL != LEVEL1 && curL != LEVEL2)
		return;

	ReadIni();

	// ��Ӳ������ͼƬ
	tank1[0] = (HBITMAP)LoadImage( NULL, L"Resources\\TankBlue.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	tank2[0] = (HBITMAP)LoadImage( NULL, L"Resources\\TankYellow.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	tank3[0] = (HBITMAP)LoadImage( NULL, L"Resources\\TankLarge.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	bulletPic[0] = (HBITMAP)LoadImage( NULL, L"Resources\\bullet.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	tank1[1] = (HBITMAP)LoadImage( NULL, L"Resources\\TankBlue1.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	tank2[1] = (HBITMAP)LoadImage( NULL, L"Resources\\TankYellow1.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	tank3[1] = (HBITMAP)LoadImage( NULL, L"Resources\\TankLarge1.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	bulletPic[1] = (HBITMAP)LoadImage( NULL, L"Resources\\bullet1.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	tank1[2] = (HBITMAP)LoadImage( NULL, L"Resources\\TankBlue2.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	tank2[2] = (HBITMAP)LoadImage( NULL, L"Resources\\TankYellow2.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	tank3[2] = (HBITMAP)LoadImage( NULL, L"Resources\\TankLarge2.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	bulletPic[2] = (HBITMAP)LoadImage( NULL, L"Resources\\bullet2.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	tank1[3] = (HBITMAP)LoadImage( NULL, L"Resources\\TankBlue3.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	tank2[3] = (HBITMAP)LoadImage( NULL, L"Resources\\TankYellow3.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	tank3[3] = (HBITMAP)LoadImage( NULL, L"Resources\\TankLarge3.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	bulletPic[3] = (HBITMAP)LoadImage( NULL, L"Resources\\bullet3.bmp", IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	if (LEVEL1 == curL)
	{
		block=LoadBitmap (GameManage::hInst,MAKEINTRESOURCE(IDB_GRASS));		// ����Դ�ļ�������ͼƬ
		background = (HBITMAP)LoadImage( NULL, L"Resources\\back.bmp", IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );	// ��Ӳ���ж�ȡ�����ļ�
	}
	else if (LEVEL2 == curL)
	{
		block=LoadBitmap (GameManage::hInst,MAKEINTRESOURCE(IDB_ROCK)); 		//����Դ�ļ�������ͼƬ
		background = (HBITMAP)LoadImage( NULL, L"Resources\\back2.bmp", IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );// ��Ӳ���ж�ȡ�����ļ�
	}
}