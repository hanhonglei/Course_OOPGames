/************************************************************************/
/*	���ļ���Ҫʵ����Ϸ��������� */
/************************************************************************/
#include "StdAfx.h"
#include "GameManage.h"
#include <mmsystem.h>
#include <process.h> 

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

void GameManage::Init(HINSTANCE hI)
{
	hInst = hI;
	reset_time = true;
	timeStep = 20;			// ��ʱ��ʱ����
	animStep = 100;			// ����֡���¶�ʱ��ʱ����
	birthTime = 5000;		// ���˵ĳ���ʱ����

	WCHAR *fileIni = L"Resources\\Init.ini";
	animStep=GetPrivateProfileInt(L"Global", L"animStep",GameManage::animStep, fileIni);
	timeStep=GetPrivateProfileInt(L"Global", L"timeStep",GameManage::timeStep, fileIni);
	birthTime = GetPrivateProfileInt(L"Enemy", L"birthTime",GameManage::birthTime, fileIni);

}

void GameManage::PlayWaveResource(void *name)
{ 
	PlaySound((LPCWSTR)name, GameManage::hInst,  SND_FILENAME | SND_ASYNC);
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
		_beginthread(PlayWaveResource, 0, L"Resources\\Start.wav");  
		break;
	case LEVEL2_OPEN:	
	case SUCCEED:
		succ_pic = (succ_pic==NULL)?(HBITMAP)LoadImage( NULL, L"Resources\\success.bmp", IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE ):succ_pic;
		_beginthread(PlayWaveResource, 0, L"Resources\\Succ.wav");  
		break;
	case FAIL:
		fail_pic = (fail_pic==NULL)?(HBITMAP)LoadImage( NULL, L"Resources\\fail.bmp", IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE ):fail_pic;
		_beginthread(PlayWaveResource, 0, L"Resources\\Fail.wav");  
		break;
	}
}