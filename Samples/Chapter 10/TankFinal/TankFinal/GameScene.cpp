/************************************************************************/
/*	该文件主要实现游戏运行过程中的具体游戏关卡各种功能*/
/************************************************************************/
#include "StdAfx.h"
#include "GameScene.h"
#include "resource.h"
#include "GameManage.h"
#include <fstream>
#include <mmsystem.h>

GameScene::GameScene(LEVEL l):curL(l)
{
	fileIni = L"Resources\\Init.ini";	// 用于保存当前所要载入的Ini文件名	
	block = NULL;
	background = NULL;					// 各种物体的图片
	nScore = 0;							// 重置游戏统计信息
	bFire = false;						// 是否处于开火状态
	nLife = 3;							// 玩家生命
	enemyFirePer = 300;					// 敌人发射子弹的随机比例
	enemyDirPer = 200;					// 敌人随机改变方向的比例
	nPassScore = 10;					// 过关分数
	pPlayer = NULL;
	Init();		// 初始化游戏参数
	if(curL != LEVEL1 && curL != LEVEL2)	// 只有在进入真正关卡时,才有必要对游戏内容进行初始化
		return;
	InitMap();
	ResetPlayer();	// 重置玩家信息
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
// 绘制链表中的每个实体
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

// 对链表中的实体进行移动
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
// 依据所处关卡的不同,随机生成一个敌人
void GameScene::EnemyBirth()
{
	if (curL != LEVEL1 && curL != LEVEL2)	// 只有在进入真正关卡时,才有必要对游戏内容进行初始化
		return;

	int type = 0;
	switch (curL)
	{
	case LEVEL1:
		type =	(rand()%3)== 0 ? 1: 0;	// 三分之二的几率得到慢速的坦克
		break;
	case LEVEL2:
		type =	(rand()%3)== 0 ? 0: 1;	// 三分之二的几率得到快速的坦克
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
// 处理按键消息
void GameScene::EnterKey(int key)
{
	switch(curL)
	{
	case OPEN:					// 开场场景中,只对回车键进行处理
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
		switch (key)	// 依据玩家输入的信息调整玩家控制的坦克状态
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
// 重置玩家信息
void GameScene::ResetPlayer()
{
	if (pPlayer == NULL)
		pPlayer = new Tank(GameManage::wndWidth/2, GameManage::wndHeight-GetPrivateProfileInt(L"Player", L"size",50, fileIni), UP, GetPrivateProfileInt(L"Player", L"vel",2, fileIni), GetPrivateProfileInt(L"Player", L"size",50, fileIni)
		,nLife, GetPrivateProfileInt(L"Player", L"frames",1, fileIni), true, 0, tank1);

	pPlayer->SetPos(GameManage::wndWidth/2, GameManage::wndHeight-GetPrivateProfileInt(L"Player", L"size",50, fileIni));
}
// 完成游戏每帧的绘制
void GameScene::Draw(HDC hdc, HWND hWnd)
{
	// 以下的步骤是为了避免产生屏幕闪烁,将画面绘制到内存中,一次性拷贝到屏幕上
	//创建内存HDC
	HDC memHDC = CreateCompatibleDC(hdc);

	//获取客户区大小
	RECT rectClient;
	GetClientRect(hWnd, &rectClient);

	//创建位图
	HBITMAP bmpBuff = CreateCompatibleBitmap(hdc,GameManage::wndWidth,GameManage::wndHeight);
	HBITMAP pOldBMP = (HBITMAP)SelectObject(memHDC, bmpBuff);
	PatBlt(memHDC,0,0,GameManage::wndWidth,GameManage::wndHeight,WHITENESS);	// 设置背景为白色

	// 进行真正的绘制
	DrawScene(memHDC);

	//拷贝内存HDC内容到实际HDC
	BOOL tt = BitBlt(hdc, rectClient.left, rectClient.top, GameManage::wndWidth,
		GameManage::wndHeight, memHDC, rectClient.left, rectClient.top, SRCCOPY);

	//内存回收
	SelectObject(memHDC, pOldBMP);
	DeleteObject(bmpBuff);
	DeleteDC(memHDC);
}
// 依据当前所处的关卡,绘制整个游戏场景,在其中调用各自的绘制函数完整绘制
void GameScene::DrawScene(HDC hdc)
{
	HFONT hf;
	WCHAR str[32];
	SetBkMode(hdc, TRANSPARENT);	// 背景是透明色
	long lfHeight;
	lfHeight = -MulDiv(18, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	HFONT hfOld;
	hf = CreateFont(lfHeight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
	hfOld = (HFONT)SelectObject(hdc, hf);
	SetTextColor(hdc, RGB(122, 0, 0));
	HBITMAP cb = GameManage::logo;
	switch (curL)
	{
	case OPEN:		// 开场阶段,输出提示信息
		cb = GameManage::logo;
		wsprintf(str, L"%s", L"按下回车键开始游戏");	
		break;
	case LEVEL1:	// 游戏进行阶段,完成对游戏场景内容的绘制
	case LEVEL2:
		SetTextColor(hdc, RGB(100, 0, 0));
		lfHeight = -MulDiv(16, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		DeleteObject(hf);
		hf = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
		SelectObject(hdc, hf);
		wsprintf(str, L"Life:%d Score:%d", pPlayer->GetLife(), nScore);	
		DrawGameScene(hdc);
		break;
	case LEVEL2_OPEN:// 开场阶段,输出提示信息
		cb = GameManage::succ_pic;
		wsprintf(str, L"%s", L"按下回车键开始下一关");	
		break;
	case SUCCEED:	// 输出提示信息
		cb = GameManage::succ_pic;
		wsprintf(str, L"%s", L"胜利！按下回车键重新开始");	
		break;
	case FAIL:		// 输出提示信息
		cb = GameManage::fail_pic;
		wsprintf(str, L"%s", L"失败！按下回车键重新开始");	
		break;
	}
	if (curL==OPEN||curL==LEVEL2_OPEN||curL==FAIL||curL==SUCCEED)
	{
		// 绘制背景
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

	SelectObject(hdc, hfOld);		// 恢复默认字体
	DeleteObject(hf);
}
// 完成游戏内容的绘制
void GameScene::DrawGameScene(HDC hdc)
{
	// 绘制背景
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, GetBackPic());
	BITMAP bm;
	GetObject(GetBackPic(), sizeof(bm), &bm);
	SetStretchBltMode(hdc,STRETCH_HALFTONE);
	BitBlt(hdc, 0, 0, GameManage::wndWidth, GameManage::wndHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	// 绘制游戏实体
	DrawEntities(hdc);
}

// 更新游戏场景中的动画元素
void GameScene::NextFrame(int ts)
{
	// 调用Entity中的函数,完成各种有动画物体的当前帧更新
	for (list<Tank*>::iterator ite = pHeadEnemys.begin(); ite != pHeadEnemys.end(); ite++)
		(*ite)->NextFrame(ts);
	for (list<Entity*>::iterator ite = pHeadBullets.begin(); ite != pHeadBullets.end(); ite++)
		(*ite)->NextFrame(ts);
	for (list<Entity*>::iterator ite = pHeadEnemyBullets.begin(); ite != pHeadEnemyBullets.end(); ite++)
		(*ite)->NextFrame(ts);
	if (pPlayer!=NULL)
		pPlayer->NextFrame(ts);
}
// 更新各种游戏信息,定时器会触发这个函数
void GameScene::Update(int ts)
{
	if (curL != LEVEL1 && curL != LEVEL2)
		return;

	// 调用Entity中的函数,完成各种可移动物体进行位置更新
	MoveEntities(ts);

	// 玩家处于开火状态的话,调用Entity中的函数发射子弹
	if (bFire) 
	{
		pHeadBullets.push_back(pPlayer->Fire(bulletPic,GetPrivateProfileInt(L"Bullet", L"vel",6, fileIni),GetPrivateProfileInt(L"Bullet", L"frames",1, fileIni),GetPrivateProfileInt(L"Bullet", L"size",20, fileIni)));
		bFire = false;
	}

	// 判断子弹是否和敌人碰撞,does collider with walls
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
			if ((*iteE)->IsCollide(*ite))		// 如果子弹碰到了当前的敌人
			{
				delete *ite;
				ite = pHeadBullets.erase(ite);
				if(!(*iteE)->BeHit())
				{
					delete *iteE;
					iteE = pHeadEnemys.erase(iteE);
					nScore++;					// 得分
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

	// 判断敌人的子弹是否和玩家碰撞
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
	// 判断敌人是否和玩家碰撞
	for (list<Tank*>::iterator iteE = pHeadEnemys.begin(); iteE != pHeadEnemys.end();iteE++)
	{
		if (RandomFire())
			pHeadEnemyBullets.push_back((*iteE)->Fire(bulletPic,GetPrivateProfileInt(L"Bullet", L"vel",6, fileIni),GetPrivateProfileInt(L"Bullet", L"frames",1, fileIni),GetPrivateProfileInt(L"Bullet", L"size",20, fileIni)));

		if ((*iteE)->IsCollide(pPlayer))
		{
			ResetPlayer();
			pPlayer->BeHit();
		}

		if (!(*iteE)->WallCollide(GameManage::wndWidth, GameManage::wndHeight))		// 有一定几率改变方向
		{
			bool cg = false;
			for (list<Entity*>::iterator iteB = pHeadBlocks.begin(); iteB != pHeadBlocks.end(); iteB++)	// 判断敌人是否和场景中障碍物发生碰撞
			{
				if ((*iteE)->IsCollide(*iteB))
				{
					cg = true;
					(*iteE)->Move(-ts);					// 后退一步
					break;
				}
			}
			if (RandomDir() || cg)
				(*iteE)->ChangeDir(Dir(((*iteE)->GetDir()+1+rand()%3)%4));
		}
		else
			(*iteE)->ChangeDir(Dir(((*iteE)->GetDir()+1+rand()%3)%4));
	}

	// 判断玩家角色是否和障碍物发生碰撞,如果是,则停止
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

	// 玩家和边界的碰撞
	if (pPlayer->WallCollide(GameManage::wndWidth, GameManage::wndHeight))
		pPlayer->Pause(true);

	if (pPlayer->GetLife() <= 0)
	{
		GameManage::ChangeLevel(FAIL);
	}
	else if (nScore >= GetPassScore())	// 依据得分决定是否过关
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

// 返回当前的背景图片句柄
HBITMAP GameScene::GetBackPic()
{
	return background;
}
// 是否随机开火
int GameScene::RandomFire()
{
	return ((rand()%enemyFirePer) == 0);
}
// 是否随机改变方向
int GameScene::RandomDir()
{
	return (rand()%enemyDirPer == 0);
}
// 获取通关分数
int GameScene::GetPassScore()
{
	return nPassScore;
}
// 依据所处的关卡,通过ini文件读取部分初始化信息
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
// 依据所处的关卡，读取不同的地图配置文件,标记为1的地方有障碍物
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
// 依据不同的关卡，对游戏参数进行初始化
void GameScene::Init()
{
	// 首先读取配置文件中的参数填充全局变量
	if (curL != LEVEL1 && curL != LEVEL2)
		return;

	ReadIni();

	// 从硬盘载入图片
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
		block=LoadBitmap (GameManage::hInst,MAKEINTRESOURCE(IDB_GRASS));		// 从资源文件中下载图片
		background = (HBITMAP)LoadImage( NULL, L"Resources\\back.bmp", IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );	// 从硬盘中读取背景文件
	}
	else if (LEVEL2 == curL)
	{
		block=LoadBitmap (GameManage::hInst,MAKEINTRESOURCE(IDB_ROCK)); 		//从资源文件中下载图片
		background = (HBITMAP)LoadImage( NULL, L"Resources\\back2.bmp", IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );// 从硬盘中读取背景文件
	}
}