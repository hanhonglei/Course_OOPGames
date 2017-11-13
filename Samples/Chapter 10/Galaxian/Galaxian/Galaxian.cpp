// Galaxian.cpp : Defines the entry point for the application.
//
/************************************************************************/
/*	����Ŀ�ģ���ϰ����汾���ƺʹ������
	�����ܣ�ʵ�֡�С�۷䡱��Ϸ����̹����Ϸ�����Ͻ��и�д����������ʹ����̹����Ϸ�Ĵ��룬
	����Ϸ�����߼��ͽ����������˸Ķ�����Ҫ�ر�ע���ڴ����޸�ʱ��ʹ�ô�����Ʒ�ʽ���������޶���
	���ڹ��̹������⣬������bugʱ��Ҫʹ���Լ�������λ������Դ��Ȼ����
	�����߸Ľ�:ʵ�������ĺ�׻��汾�ġ�С�۷䡱��Ϸ������ͼƬ����ֵ�趨�������Ϸ������
	��д�ߣ������ף�hanhonglei@sina.com��*/
/************************************************************************/
#include "stdafx.h"
#include "Galaxian.h"
#include <time.h>
#include <stdlib.h>
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

enum Dir{UP, DOWN, LEFT, RIGHT};	// ö�����ͣ��˶�������ܵ��˶�����
typedef struct				// �ṹ�����ͣ���Ϸ�е�̹�˽ṹ��
{
	int x, y;				// λ��
	Dir dir;				// ����
	int v;					// ����
	int s;					// �߳���������
	int b;					// �Ƿ����ӵ�
	int p;					// �Ƿ�ֹͣ,ֻ����Ҳſ���ֹͣ
	int e;					// �Ƿ��ǵ���
	COLORREF c;				// ��ɫ
}Entity;

#define MAX_ENEMY 18		// ��������Ŀ
#define MAX_BULLETS 32		// ����ӵ���Ŀ
int nLife = 3;				// �������
int nScore = 0;				// ��ҵ÷�
int nBullet = 0;			// ��Ҵ�����ӵ�����
int nEnemyBullet = 0;		// ���˷������ӵ�����
int nEnemy = 0;				// ��ǰ�ĵ�������
int timeStep = 20;			// ��ʱ��ʱ����
int sz = 50;				// ̹�˳ߴ�
int velf = 4;				// ����̹������
int vels = 2;				// ����̹������
int szb = 20;				// �ӵ��ߴ�
int velb = 6;				// �ӵ�����
int enemyFirePer = 300;		// ���˷����ӵ����������
int enemyDir = 200;			// ���˸ı䷽����������
int bFire = 0;				// ����Ƿ������״̬
Entity enemys[MAX_ENEMY];	// ��������
Entity player;				// ���
Entity bullets[MAX_BULLETS];// ����ӵ�����
Entity enemyBullets[MAX_BULLETS];// �����ӵ�����
int wndWidth = 0;	int wndHeight = 0;  // ���ڳߴ�

// ���������Ϣ,�ָ�����ʼ״̬
void ResetPlayer()
{
	player.s = sz;
	player.b = 0;
	player.c = RGB(122,30,0);
	player.dir = UP;
	player.v = vels;
	player.x = wndWidth/2;
	player.y = wndHeight-sz;
	player.p = 1;
	player.e = 0;
}
// ��Ϸ��ʼ��
void Init()
{
	int line = 3;
	nEnemy = 0;
	for (int i = 0; i < line; i++)
	{
		int count = 0;
		for(; count < MAX_ENEMY/line; nEnemy++, count++)
		{
			enemys[nEnemy].s = sz;
			enemys[nEnemy].b = 0;
			enemys[nEnemy].e = 1;
			enemys[nEnemy].dir = RIGHT;	
			enemys[nEnemy].v = vels;
			enemys[nEnemy].c = i%2==0?RGB(0,122,122):RGB(0,60,30);
			enemys[nEnemy].x = (wndWidth-sz)/2 - sz*MAX_ENEMY/line + 2*count*sz;
			enemys[nEnemy].y = sz/2 + 2*i*sz;
			enemys[nEnemy].p = 0;
		}
	}
	ResetPlayer();
}
// ���ض���Ϸʵ�����ݳ�������ʽ����ƶ�
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
// �����е��˳�����
void ChangeEnemyDir(Entity *ents)
{
	for (int i = 0; i < nEnemy; i++)	// ���˳������
		ents[i].dir = ents[i].dir==LEFT?RIGHT:LEFT;
}
// �ض�ʵ��������
void Fire(const Entity* ent)
{
	Entity *pBulletes = (ent->e)?enemyBullets:bullets;	// �ж��ǵ��˻�����ҷ����ڵ�
	int nB = (ent->e)?nEnemyBullet:nBullet;			
	if (nB >= MAX_BULLETS)
		return;
	(pBulletes+nB)->s = szb;
	(pBulletes+nB)->b = 1;
	(pBulletes+nB)->e = 0;
	(pBulletes+nB)->c = (ent->e)?RGB(0,0,255):RGB(255,0,0);	// ���˷����ڵ�����ҷ����Ĳ�һ��
	(pBulletes+nB)->dir = ent->e?DOWN:UP;			// ����ǵ��ˣ����ӵ����£������ӵ�����
	(pBulletes+nB)->v = velb;
	(pBulletes+nB)->x = ent->x;
	(pBulletes+nB)->p = 0;
	(pBulletes+nB)->y = ent->y;
	switch((pBulletes+nB)->dir)	// �ӵ��ĳ���λ�õ���
	{
	case UP:
		(pBulletes+nB)->y -= ent->s;
		break;
	case DOWN:
		(pBulletes+nB)->y += ent->s;
		break;
	case LEFT:
		(pBulletes+nB)->x -= ent->s;
		break;
	case RIGHT:
		(pBulletes+nB)->x += ent->s;
		break;
	}
	if (ent->e)
		nEnemyBullet++;
	else
		nBullet++;
}
// ���ض���ʵ���������ɾ��,����Ԫ����ǰ�ƶ�
void Destroy(Entity ents[], int n, int *num)
{
	memcpy(ents+n, ents+n+1, sizeof(Entity)*((*num)-1-n));
	(*num)--;
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
int WallCollide(Entity *ent)
{
	int bC = 0;
	switch(ent->dir)
	{
	case UP:
		if ((ent->y - ent->s/2) < 0)			// �ϱ߽�
		{
			bC = 1;
			ent->y = ent->s/2;
		}
		break;
	case DOWN:
		if ((ent->y + ent->s/2) > wndHeight)	// �±߽�
		{
			bC = 1;
			ent->y = wndHeight - ent->s/2;
		}
		break;
	case LEFT:
		if((ent->x - ent->s/2) < 0)				// ��߽�
		{
			bC = 1;
			ent->x = ent->s/2;
		}
		break;
	case RIGHT:
		if((ent->x + ent->s/2) > wndWidth)		// �ұ߽�
		{
			bC = 1;
			ent->x = wndWidth - ent->s/2;
		}
		break;
	}
	if (bC)
	{
		if (!ent->e)					// �������Һͱ߽緢����ײ,�����ֹͣ
			ent->p = 1;
	}
	return bC;
}
// ���¸�����Ϸ��Ϣ,��ʱ����Ϣ�л�����������
void Update(int ts)
{
	// ���ƶ�����λ�ý��и���
	Entity* ent = NULL;

	for (int i = 0; i < nEnemy; i++)	// ����λ�ø���
	{
		ent = enemys+i;
		Move(ent, ts);
		if ((rand()%enemyFirePer) == 0)
			Fire(ent);
	}
	for (int i = 0; i < nBullet; i++)	// ����ڵ�λ�ý��и���
	{
		ent = bullets+i;
		Move(ent, ts);
	}
	for (int i = 0; i < nEnemyBullet; i++)	// �����ڵ�λ�ý��и���
	{
		ent = enemyBullets+i;
		Move(ent, ts);
	}
	Move(&player, ts);					// ���λ�ý��и���

	if (bFire)							// �����Ҵ������״̬,�����ڵ�
	{
		Fire(&player);
		bFire = 0;
	}

	// �ж��ӵ��Ƿ�͵�����ײ
	for(int i = 0; i < nBullet; i++)
	{
		for (int j = 0; j < nEnemy; j++)
		{
			if (IsCollide(&bullets[i], &enemys[j]))
			{
				Destroy(bullets, i, &nBullet);
				Destroy(enemys, j, &nEnemy);
				nScore++;
				i--;
				j--;
				break;
			}
		}
	}
	// �жϵ��˵��ӵ��Ƿ�������ײ
	for(int i = 0; i < nEnemyBullet; i++)
	{
		if (IsCollide(&enemyBullets[i], &player))
		{
			Destroy(enemyBullets, i, &nEnemyBullet);
			ResetPlayer();
			nLife--;
			i--;
			break;
		}
	}
	// �жϸ���ʵ���Ƿ����Ϸ�߽緢����ײ
	for (int i = 0; i < nBullet; i++)	// ����ڵ�
	{
		ent = bullets+i;
		if (WallCollide(ent))
		{
			Destroy(bullets, i, &nBullet);
			i--;
		}
	}
	for (int i = 0; i < nEnemyBullet; i++)	// �����ڵ�
	{
		ent = enemyBullets+i;
		if (WallCollide(ent))
		{
			Destroy(enemyBullets, i, &nEnemyBullet);
			i--;
		}
	}
	WallCollide(&player);			// ���
}
// ���Ʋ���ָ������Ϸʵ��
void DrawEntity(HDC hdc, const Entity *ent)
{
	HBRUSH brush;
	brush = CreateSolidBrush(ent->c);		// ����ʵ��ָ������ɫ������ˢ
	RECT rc;								// ʵ�峤����
	rc.top = ent->y-ent->s/2;
	rc.left = ent->x-ent->s/2;
	rc.bottom = ent->y+ent->s/2;
	rc.right = ent->x+ent->s/2;
	FillRect(hdc, &rc, brush);				// ����ʵ������
	if (!ent->b)							// ������ʵ�岻���ӵ�,�����ݽ�ɫ���ͻ�����Ͳ
	{
		if (ent->e)
		{
			rc.top = rc.bottom;
			rc.bottom = rc.bottom + ent->s/2;
			rc.left = rc.left + ent->s/4;
			rc.right = rc.right - ent->s/4;
		}
		else
		{
			rc.bottom = rc.top;
			rc.top = rc.bottom - ent->s/2;
			rc.left = rc.left + ent->s/4;
			rc.right = rc.right - ent->s/4;
		}
		FillRect(hdc, &rc, brush);
	}
	DeleteObject(brush);			// ��ʹ����ı�ˢɾ��
}
// ����������Ϸ����,�����е��ø��ԵĻ��ƺ�����������
void DrawScene(HDC hdc)
{
	// ������Ϸ��ʾ��Ϣ
	HFONT hf;
	WCHAR str[32];
	long lfHeight;
	lfHeight = -MulDiv(16, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	hf = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
	HFONT hfOld = (HFONT)SelectObject(hdc, hf);
	if (nLife <= 0)							// ����������Ϊ0,����ʾ��������
	{
		SetTextColor(hdc, RGB(122, 0, 0));
		TextOut(hdc, wndWidth/2-100, wndHeight/2-40,L"Game Over",9);
		SelectObject(hdc, hfOld);
		return;			
	}
	// ��ʾ��Ϸͳ����Ϣ
	SetTextColor(hdc, RGB(100, 100, 100));
	wsprintf(str, L"Life:%d Score:%d", nLife, nScore);	
	TextOut(hdc, wndWidth/2-100, wndHeight/2-40,str,wcslen(str));

	SelectObject(hdc, hfOld);				// �ָ�Ĭ������
	DeleteObject(hf);

	// ���Ƹ�����Ϸʵ��
	Entity* ent = NULL;
	for (int i = 0; i < nEnemy; i++)		// ����
	{
		ent = enemys+i;
		DrawEntity(hdc, ent);
	}
	for (int i = 0; i < nBullet; i++)		// ��ҷ������ڵ�
	{
		ent = bullets+i;
		DrawEntity(hdc, ent);
	}
	for (int i = 0; i < nEnemyBullet; i++)	// ���˷������ڵ�
	{
		ent = enemyBullets+i;
		DrawEntity(hdc, ent);
	}

	DrawEntity(hdc, &player);				// ���
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GALAXIAN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GALAXIAN));

	srand(time(NULL));		// �����������
	Init();					// ��Ϸ��Ϣ��ʼ��

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GALAXIAN));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GALAXIAN);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:			// ����������,��ʼ����һ����ʱ��
		SetTimer(hWnd,1,timeStep,NULL);
		SetTimer(hWnd,2,1000,NULL);	// ��ʱ�������ڶ�ʱ�ı�����˶�����
		break;
	case WM_TIMER:			// ��ʱ����Ӧ
		InvalidateRect(hWnd, NULL, TRUE);	// �ô��ڱ�Ϊ��Ч,�Ӷ������ػ���Ϣ
		if (wParam == 1)	// ����Ϸ���и���
		{
			if (nLife > 0)
				Update(timeStep/10);
		}
		else if (wParam == 2)	// �����˶�����ı�
		{
			ChangeEnemyDir(enemys);
		}
		break;
	case WM_SIZE:			// ��ȡ���ڵĳߴ�
		wndWidth = LOWORD(lParam);
		wndHeight = HIWORD(lParam);
		break;
	case WM_KEYDOWN:		// ��Ұ��¼��̰���
		{
			InvalidateRect(hWnd, NULL, TRUE);
			switch (wParam)	// ��������������Ϣ������ҿ��Ƶ�̹��״̬
			{
			case VK_LEFT:
				player.dir = LEFT;
				player.p = 0;
				break;
			case VK_RIGHT:
				player.dir = RIGHT;
				player.p = 0;
				break;
			case VK_SPACE:	// ���
				bFire = 1;;
				break;
			}
			break;
		}
	case WM_KEYUP:
		InvalidateRect(hWnd, NULL, TRUE);
		switch (wParam)	
		{
			case VK_LEFT:
			case VK_RIGHT:
				player.p = 1;
				break;
		}
		break;
	case WM_ERASEBKGND:		// ����������,������˸
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
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
			DrawScene(memHDC);

			//�����ڴ�HDC���ݵ�ʵ��HDC
			BOOL tt = BitBlt(hdc, rectClient.left, rectClient.top, wndWidth,
				wndHeight, memHDC, rectClient.left, rectClient.top, SRCCOPY);

			//�ڴ����
			SelectObject(memHDC, pOldBMP);
			DeleteObject(bmpBuff);
			DeleteDC(memHDC);

			EndPaint(hWnd, &ps);
			break;
		}
	case WM_DESTROY:
		KillTimer(hWnd,1);		// �����˳�ʱ������ʱ��ɾ��
		KillTimer(hWnd,2);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
