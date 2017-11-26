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
	for (int i =0; i < MAX_ENEMY; i++)
	{
		EnemyTank* e = new EnemyTank;
		e->Init();
		enemys.push_back(e);
	}
	player.ResetPlayer();
	allEntities.push_back(&player);

	bInit = true;
}

// ���ض���ʵ���������ɾ��,����Ԫ����ǰ�ƶ�
//void Destroy(Entity ents[], int n, int *num)
//{
//	memcpy(ents+n, ents+n+1, sizeof(Entity)*((*num)-1-n));
//	ents[*num-1].SetValid(false);
//	(*num)--;
//}
//void Destroy(EnemyTank ents[], int n, int *num)
//{
//	memcpy(ents+n, ents+n+1, sizeof(EnemyTank)*((*num)-1-n));
//	ents[*num-1].SetValid(false);
//	(*num)--;
//}

// ���¸�����Ϸ��Ϣ,��ʱ����Ϣ�л�����������
void Update(int ts)
{
	// ���ƶ�����λ�ý��и���
	player.Move(ts);					// ���λ�ý��и���
	player.Fire();						// ��ҿ���

	for (list<EnemyTank*>::iterator ite = enemys.begin(); ite != enemys.end(); ite++)	// ����λ�ø���
	{
		(*ite)->Move(ts);
		(*ite)->Fire();
		if (player.IsCollide(*ite))
		{
			player.ResetPlayer();
			player.nLife--;
		}

		if ((*ite)->WallCollide())		// ��һ�����ʸı䷽��
		{
			(*ite)->RandDir();
		}

	}

	bool hit = false;
	for (list<Entity*>::iterator ite = bullets.begin(); ite != bullets.end();)	// ����ڵ�λ�ý��и���
	{
		(*ite)->Move(ts);
		if ((*ite)->WallCollide())
		{
			Entity *p = *ite;
			ite=bullets.erase(ite);
			delete p;
			continue;
		}		
		hit = false;
		// �ж��ӵ��Ƿ�͵�����ײ
		for (list<EnemyTank*>::iterator iteE = enemys.begin(); iteE != enemys.end();)
		{
			if ((*ite)->IsCollide(*iteE))
			{
				Entity *p1 = *ite;
				EnemyTank *p2 = *iteE;
				ite=bullets.erase(ite);
				iteE=enemys.erase(iteE);
				delete p1, p2;
				player.nScore++;
				hit = true;
				break;
			}
			else
				++iteE;
		}

		if (!hit)
			++ite;
	}
	hit = false;
	for (list<Entity*>::iterator ite = enemyBullets.begin(); ite != enemyBullets.end();)	// �����ڵ�λ�ý��и���
	{
		(*ite)->Move(ts);
		// �жϵ��˵��ӵ��Ƿ�������ײ
		if (!hit)
		{
			if((*ite)->IsCollide(&player))
			{
				Entity* p = *ite;
				ite = enemyBullets.erase(ite);
				delete p;
				hit = true;
				player.ResetPlayer();
				player.nLife--;
				hit = true;
				continue;
			}			
		}
		if ((*ite)->WallCollide())
		{
			Entity* p = *ite;
			ite = enemyBullets.erase(ite);
			delete p;
		}
		else
			++ite;
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
	for (list<EnemyTank*>::iterator ite = enemys.begin(); ite != enemys.end(); ite++)	// ����λ�ø���
		(*ite)->DrawEntity(hdc);

	for (list<Entity*>::iterator ite = bullets.begin(); ite != bullets.end();ite++)	// ����ڵ�λ�ý��и���
		(*ite)->DrawEntity(hdc);

	for (list<Entity*>::iterator ite = enemyBullets.begin(); ite != enemyBullets.end();ite++)	// �����ڵ�λ�ý��и���
		(*ite)->DrawEntity(hdc);

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
		for (list<EnemyTank*>::iterator ite = enemys.begin(); ite != enemys.end(); ite++)	
			delete *ite;

		for (list<Entity*>::iterator ite = bullets.begin(); ite != bullets.end();ite++)	
			delete *ite;

		for (list<Entity*>::iterator ite = enemyBullets.begin(); ite != enemyBullets.end();ite++)
			delete *ite;


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
