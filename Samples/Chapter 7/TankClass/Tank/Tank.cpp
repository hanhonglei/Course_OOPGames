// Tank.cpp : Defines the entry point for the application.
//
/************************************************************************/
/*	程序目的：用户自定义数据类型
程序功能：简化版坦克大战。在游戏场景的顶端三个点随机生成两种不同的敌方坦克
玩家坦克在下方居中。方向键控制坦克前进方向，空格键发射炮弹
待读者改进:增加胜利画面,并允许玩家重新开始游戏
编写者：韩红雷（hanhonglei@sina.com）*/
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

// 游戏初始化
void Init()
{
	for(EnemyTank::nEnemy = 0; EnemyTank::nEnemy < MAX_ENEMY; EnemyTank::nEnemy++)
	{
		enemys[EnemyTank::nEnemy].Init();
	}
	player.ResetPlayer();
	bInit = true;
}

// 将特定的实体从数组中删除,后续元素向前移动
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

// 更新各种游戏信息,定时器消息中会调用这个函数
void Update(int ts)
{
	// 可移动物体位置进行更新
	for (int i = 0; i < EnemyTank::nEnemy; i++)	// 敌人位置更新
	{
		enemys[i].Move(ts);
		enemys[i].Fire();
	}
	for (int i = 0; i < PlayerTank::nBullet; i++)	// 玩家炮弹位置进行更行
	{
		bullets[i].Move(ts);
	}
	for (int i = 0; i < EnemyTank::nEnemyBullet; i++)	// 敌人炮弹位置进行更新
	{
		enemyBullets[i].Move(ts);
	}
	player.Move(ts);					// 玩家位置进行更新

	player.Fire();						// 玩家开火


	// 判断子弹是否和敌人碰撞
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
	// 判断敌人的子弹是否和玩家碰撞
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
	// 判断敌人是否和玩家碰撞
	for (int i = 0; i < EnemyTank::nEnemy; i++)
	{
		if (player.IsCollide(&enemys[i]))
		{
			player.ResetPlayer();
			player.nLife--;
		}
	}
	// 判断各种实体是否和游戏边界发生碰撞
	for (int i = 0; i < EnemyTank::nEnemy; i++)	// 敌人
	{
		if (!enemys[i].WallCollide())		// 有一定几率改变方向
		{
			enemys[i].RandDir();
		}
	}
	for (int i = 0; i < PlayerTank::nBullet; i++)	// 玩家炮弹
	{
		if (bullets[i].WallCollide())
		{
			Destroy(bullets, i, &PlayerTank::nBullet);
			i--;
		}
	}
	for (int i = 0; i < EnemyTank::nEnemyBullet; i++)	// 敌人炮弹
	{
		if (enemyBullets[i].WallCollide())
		{
			Destroy(enemyBullets, i, &EnemyTank::nEnemyBullet);
			i--;
		}
	}
	player.WallCollide();			// 玩家
}

// 绘制整个游戏场景,在其中调用各自的绘制函数完整绘制
void DrawScene(HDC hdc)
{
	if (!bInit)
		return;

	// 绘制游戏提示信息
	HFONT hf;
	WCHAR str[32];
	long lfHeight;
	lfHeight = -MulDiv(16, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	hf = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
	HFONT hfOld = (HFONT)SelectObject(hdc, hf);
	if (player.nLife <= 0)							// 如果玩家生命为0,则显示结束画面
	{
		SetTextColor(hdc, RGB(122, 0, 0));
		TextOut(hdc, wndWidth/2-100, wndHeight/2-40,L"Game Over",9);
		SelectObject(hdc, hfOld);
		return;
	}	
	// 显示游戏统计信息
	SetTextColor(hdc, RGB(100, 100, 100));
	wsprintf(str, L"Life:%d Score:%d", player.nLife, player.nScore);	
	TextOut(hdc, wndWidth/2-100, wndHeight/2-40,str,wcslen(str));

	SelectObject(hdc, hfOld);				// 恢复默认字体
	DeleteObject(hf);

	// 绘制各种游戏实体
	Entity* ent = NULL;
	for (int i = 0; i < EnemyTank::nEnemy; i++)		// 敌人
		enemys[i].DrawEntity(hdc);
	for (int i = 0; i < PlayerTank::nBullet; i++)		// 玩家发出的炮弹
		bullets[i].DrawEntity(hdc);
	for (int i = 0; i < EnemyTank::nEnemyBullet; i++)	// 敌人发出的炮弹
		enemyBullets[i].DrawEntity(hdc);

	player.DrawEntity(hdc);				// 玩家
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

	srand(time(NULL));		// 生成随机种子
	Init();					// 游戏信息初始化

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
	case WM_CREATE:			// 程序启动后,开始设置一个定时器
		SetTimer(hWnd,1,timeStep,NULL);
		break;
	case WM_TIMER:			// 定时器响应
		if (wParam == 1)	// 对游戏进行更新
		{
			if (player.nLife > 0)
				Update(timeStep/10);
			InvalidateRect(hWnd, NULL, TRUE);	// 让窗口变为无效,从而触发重绘消息
		}
		break;
	case WM_SIZE:			// 获取窗口的尺寸
		wndWidth = LOWORD(lParam);
		wndHeight = HIWORD(lParam);
		break;
	case WM_KEYDOWN:		// 玩家按下键盘按键
		{
			InvalidateRect(hWnd, NULL, TRUE);
			player.Interaction(wParam);
			break;
		}
	case WM_ERASEBKGND:		// 不擦除背景,避免闪烁
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			// 以下的步骤是为了避免产生屏幕闪烁,将画面绘制到内存中,一次性拷贝到屏幕上
			//创建内存HDC
			HDC memHDC = CreateCompatibleDC(hdc);

			//获取客户区大小
			RECT rectClient;
			GetClientRect(hWnd, &rectClient);

			//创建位图
			HBITMAP bmpBuff = CreateCompatibleBitmap(hdc,wndWidth,wndHeight);
			HBITMAP pOldBMP = (HBITMAP)SelectObject(memHDC, bmpBuff);
			PatBlt(memHDC,0,0,wndWidth,wndHeight,WHITENESS);	// 设置背景为白色

			// 进行真正的绘制
			DrawScene(memHDC);

			//拷贝内存HDC内容到实际HDC
			BOOL tt = BitBlt(hdc, rectClient.left, rectClient.top, wndWidth,
				wndHeight, memHDC, rectClient.left, rectClient.top, SRCCOPY);

			//内存回收
			SelectObject(memHDC, pOldBMP);
			DeleteObject(bmpBuff);
			DeleteDC(memHDC);

			EndPaint(hWnd, &ps);
			break;
		}
	case WM_DESTROY:
		KillTimer(hWnd,1);		// 程序退出时，将定时器删除
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
