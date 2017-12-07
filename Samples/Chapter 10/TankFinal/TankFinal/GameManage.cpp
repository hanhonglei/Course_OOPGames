/************************************************************************/
/*	���ļ���Ҫʵ����Ϸ��������� */
/************************************************************************/
#include "StdAfx.h"
#include "GameManage.h"

// static ��Ա�����Ķ���
GameScene *(GameManage::curScene) = NULL;
int GameManage::wndWidth; // ���ڳߴ�
int GameManage::wndHeight; 
HBITMAP GameManage::logo, GameManage::succ_pic, GameManage::fail_pic;	// logoͼƬ�ͳɹ�ʧ��ͼƬ
int GameManage::timeStep;			// ��ʱ��ʱ����
int GameManage::animStep;			// ����֡���¶�ʱ��ʱ����
int GameManage::birthTime;			// ���˵ĳ���ʱ����
HINSTANCE GameManage::hInst;		// current instance,�����ļ��е�ȫ�ֱ���
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
// ���ڴ�С�ı�֪ͨ����,�ļ���ȫ�ֱ����б��洰�ڵĳߴ�
void GameManage::ChangeWndSize(int w, int h)
{
	wndWidth = w, wndHeight = h;
}
// ת���ؿ�
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