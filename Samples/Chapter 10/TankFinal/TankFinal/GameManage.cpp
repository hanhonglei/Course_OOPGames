/************************************************************************/
/*	该文件主要实现游戏总体管理功能 */
/************************************************************************/
#include "StdAfx.h"
#include "GameManage.h"

// static 成员变量的定义
GameScene *(GameManage::curScene) = NULL;
int GameManage::wndWidth; // 窗口尺寸
int GameManage::wndHeight; 
HBITMAP GameManage::logo, GameManage::succ_pic, GameManage::fail_pic;	// logo图片和成功失败图片
int GameManage::timeStep;			// 定时器时间间隔
int GameManage::animStep;			// 动画帧更新定时器时间间隔
int GameManage::birthTime;			// 敌人的出生时间间隔
HINSTANCE GameManage::hInst;		// current instance,在主文件中的全局变量
bool GameManage::reset_time;

GameManage::GameManage(void)
{
}
GameManage::~GameManage(void)
{
}
void GameManage::NextFrame(int step)
{
	if (curScene == NULL)
		return;
	curScene->NextFrame(step);
}
void GameManage::EnterKey(int key)
{
	if (curScene == NULL)
		return;
	curScene->EnterKey(key);
}
void GameManage::Destroy()
{
	if (curScene == NULL)
		return;
	delete curScene;
}
void GameManage::Update(int ts)
{
	if (curScene == NULL)
		return;
	curScene->Update(ts);
}
void GameManage::EnemyBirth()
{
	if (curScene == NULL)
		return;
	curScene->EnemyBirth();
}

void GameManage::Draw(HDC hdc, HWND hWnd)
{
	if (curScene == NULL)
		return;
	curScene->Draw(hdc, hWnd);
}
// 窗口大小改变通知函数,文件的全局变量中保存窗口的尺寸
void GameManage::ChangeWndSize(int w, int h)
{
	wndWidth = w, wndHeight = h;
}
// 转换关卡
void GameManage::ChangeLevel(LEVEL newL)
{
	if (curScene != NULL)
		delete curScene;
	curScene = new GameScene(newL);
	//////////////////////////////////////////////////////////////////////////
	switch (newL)
	{
	case OPEN:					
		logo = (logo==NULL)?(HBITMAP)LoadImage( NULL, L"Resources\\logo.bmp", IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE ):logo;
		break;
	case LEVEL2_OPEN:	
	case SUCCEED:
		succ_pic = (succ_pic==NULL)?(HBITMAP)LoadImage( NULL, L"Resources\\success.bmp", IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE ):succ_pic;
		break;
	case FAIL:
		fail_pic = (fail_pic==NULL)?(HBITMAP)LoadImage( NULL, L"Resources\\fail.bmp", IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE ):fail_pic;
		break;
	}
}