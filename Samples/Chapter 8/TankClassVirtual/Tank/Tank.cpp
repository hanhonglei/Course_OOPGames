// Tank.cpp : Defines the entry point for the application.
//
/************************************************************************/
/*	����Ŀ�ģ��û��Զ�����������
�����ܣ��򻯰�̹�˴�ս������Ϸ�����Ķ�������������������ֲ�ͬ�ĵз�̹��
���̹�����·����С����������̹��ǰ�����򣬿ո�������ڵ�
�����߸Ľ�:����ʤ������,������������¿�ʼ��Ϸ
��д�ߣ������ף�hanhonglei@sina.com��*/
/************************************************************************/

#include "stdafx.h"
#include "Tank.h"
#include <time.h>
#include "Classes.h"

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

// ��Ϸ��ʼ��
void Init()
{
	for(EnemyTank::nEnemy = 0; EnemyTank::nEnemy < MAX_ENEMY; EnemyTank::nEnemy++)
	{
		enemys[EnemyTank::nEnemy].Init();
	}
	player.ResetPlayer();
	bInit = true;
}

// ���ض���ʵ���������ɾ��,����Ԫ����ǰ�ƶ�
void Destroy(Entity ents[], int n, int *num)
{
	memcpy(ents+n, ents+n+1, sizeof(Entity)*((*num)-1-n));
	(*num)--;
}
void Destroy(EnemyTank ents[], int n, int *num)
{
	memcpy(ents+n, ents+n+1, sizeof(EnemyTank)*((*num)-1-n));
	(*num)--;
}

// ���¸�����Ϸ��Ϣ,��ʱ����Ϣ�л�����������
void Update(int ts)
{
	// ���ƶ�����λ�ý��и���
	for (int i = 0; i < EnemyTank::nEnemy; i++)	// ����λ�ø���
	{
		enemys[i].Move(ts);
		enemys[i].Fire();
	}
	for (int i = 0; i < PlayerTank::nBullet; i++)	// ����ڵ�λ�ý��и���
	{
		bullets[i].Move(ts);
	}
	for (int i = 0; i < EnemyTank::nEnemyBullet; i++)	// �����ڵ�λ�ý��и���
	{
		enemyBullets[i].Move(ts);
	}
	player.Move(ts);					// ���λ�ý��и���

	player.Fire();						// ��ҿ���


	// �ж��ӵ��Ƿ�͵�����ײ
	for(int i = 0; i < PlayerTank::nBullet; i++)
	{
		for (int j = 0; j < EnemyTank::nEnemy; j++)
		{
			if (bullets[i].IsCollide(&enemys[j]))
			{
				Destroy(bullets, i, &PlayerTank::nBullet);
				Destroy(enemys, j, &EnemyTank::nEnemy);
				player.nScore++;
				i--;
				j--;
				break;
			}
		}
	}
	// �жϵ��˵��ӵ��Ƿ�������ײ
	for(int i = 0; i < EnemyTank::nEnemyBullet; i++)
	{
		if (enemyBullets[i].IsCollide(&player))
		{
			Destroy(enemyBullets, i, &EnemyTank::nEnemyBullet);
			player.ResetPlayer();
			player.nLife--;
			i--;
			break;
		}
	}
	// �жϵ����Ƿ�������ײ
	for (int i = 0; i < EnemyTank::nEnemy; i++)
	{
		if (player.IsCollide(&enemys[i]))
		{
			player.ResetPlayer();
			player.nLife--;
		}
	}
	// �жϸ���ʵ���Ƿ����Ϸ�߽緢����ײ
	for (int i = 0; i < EnemyTank::nEnemy; i++)	// ����
	{
		if (!enemys[i].WallCollide())		// ��һ�����ʸı䷽��
		{
			enemys[i].RandDir();
		}
	}
	for (int i = 0; i < PlayerTank::nBullet; i++)	// ����ڵ�
	{
		if (bullets[i].WallCollide())
		{
			Destroy(bullets, i, &PlayerTank::nBullet);
			i--;
		}
	}
	for (int i = 0; i < EnemyTank::nEnemyBullet; i++)	// �����ڵ�
	{
		if (enemyBullets[i].WallCollide())
		{
			Destroy(enemyBullets, i, &EnemyTank::nEnemyBullet);
			i--;
		}
	}
	player.WallCollide();			// ���
}

// ����������Ϸ����,�����е��ø��ԵĻ��ƺ�����������
void DrawScene(HDC hdc)
{
	if (!bInit)
		return;

	// ������Ϸ��ʾ��Ϣ
	HFONT hf;
	WCHAR str[32];
	long lfHeight;
	lfHeight = -MulDiv(16, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	hf = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
	HFONT hfOld = (HFONT)SelectObject(hdc, hf);
	if (player.nLife <= 0)							// ����������Ϊ0,����ʾ��������
	{
		SetTextColor(hdc, RGB(122, 0, 0));
		TextOut(hdc, wndWidth/2-100, wndHeight/2-40,L"Game Over",9);
		SelectObject(hdc, hfOld);
		return;
	}	
	// ��ʾ��Ϸͳ����Ϣ
	SetTextColor(hdc, RGB(100, 100, 100));
	wsprintf(str, L"Life:%d Score:%d", player.nLife, player.nScore);	
	TextOut(hdc, wndWidth/2-100, wndHeight/2-40,str,wcslen(str));

	SelectObject(hdc, hfOld);				// �ָ�Ĭ������
	DeleteObject(hf);

	// ���Ƹ�����Ϸʵ��
	Entity* ent = NULL;
	for (int i = 0; i < EnemyTank::nEnemy; i++)		// ����
		enemys[i].DrawEntity(hdc);
	for (int i = 0; i < PlayerTank::nBullet; i++)		// ��ҷ������ڵ�
		bullets[i].DrawEntity(hdc);
	for (int i = 0; i < EnemyTank::nEnemyBullet; i++)	// ���˷������ڵ�
		enemyBullets[i].DrawEntity(hdc);

	player.DrawEntity(hdc);				// ���
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
	LoadString(hInstance, IDC_TANK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TANK));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TANK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TANK);
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
		break;
	case WM_TIMER:			// ��ʱ����Ӧ
		if (wParam == 1)	// ����Ϸ���и���
		{
			if (player.nLife > 0)
				Update(timeStep/10);
			InvalidateRect(hWnd, NULL, TRUE);	// �ô��ڱ�Ϊ��Ч,�Ӷ������ػ���Ϣ
		}
		break;
	case WM_SIZE:			// ��ȡ���ڵĳߴ�
		wndWidth = LOWORD(lParam);
		wndHeight = HIWORD(lParam);
		break;
	case WM_KEYDOWN:		// ��Ұ��¼��̰���
		{
			InvalidateRect(hWnd, NULL, TRUE);
			player.Interaction(wParam);
			break;
		}
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
