// TankFinal.cpp : Defines the entry point for the application.
//
/************************************************************************/
/*	����Ŀ�ģ��ࡢ��׼ģ�塢��ļ̳С���̬��Ա����������ͼ�������˫���������Ƹ�����˸����Ϸ�߼�ʵ�֡���������Ϸ����
	�����ܣ����հ�̹�˴�ս������Ϸ�����Ķ�������������������ֲ�ͬ�ĵз�̹��
		���̹�����·����С����������̹��ǰ�����򣬿ո�������ڵ���
		��Ϸ�߱��˶�ȡini�ļ���txt�ļ������������ڴ�����������Ϸ����
		����ͨ����ȡ�ⲿ�ļ��ķ������������Ӳ���룬������Ϸά��
		�߻���Ա��������Ϸexe������ϵ�����£�ͨ�������ⲿ�ļ��ķ�ʽ�����ϵ�����Ϸ��ֵ
		ͬʱ����Ϸ�е��������ͼƬ�ķ�ʽ������������Ϸ�������ϰ��
		��Ϸ�еĲ������廹ʵ���˶������ܡ�
		��Ϸ�еĵ��ˡ��ӵ����ô���ͷ���ĵ�������ʽ���й�������ʵ����ɾ����
		����Ϸ�еĲ�ͬ���ܺ������з��࣬�������ڲ�ͬ�ļ��У�����ά��
		���ļ���ʵ�־��幦�ܣ�ֻ���й��ܵ���
	�����߸Ľ�:ʵ�������ĺ�׻���̹�˴�ս��
		���ӵ�ͼ�༭���ܣ���������Զ����ͼ
		����һ�����ڱ༭���������������ܣ�������������Զ������̹�����Ե�
		���������������Ϸ�������Ϸ�༭������		
	��д�ߣ������ף�hanhonglei@sina.com��*/
/************************************************************************/

#include "stdafx.h"
#include "TankFinal.h"
#include <time.h>
#include "Entity.h"
#include "GameManage.h"

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
	LoadString(hInstance, IDC_TANKFINAL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TANKFINAL));

	srand(unsigned int (time(NULL)));						// �������
	GameManage::ChangeLevel(OPEN);			// ���Ƚ�����Ϸ��ӭ����

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TANKFINAL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;/*MAKEINTRESOURCE(IDC_TANKFINAL);*/	// ȡ���˵�
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
   SetWindowLong(hWnd, GWL_STYLE,WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU);	// ������ı䴰�ڴ�С
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
	case WM_CREATE:			// ����������,��ʼ����3����ʱ��
		GameManage::timeStep = 20;			// ��ʱ��ʱ����
		GameManage::animStep = 100;			// ����֡���¶�ʱ��ʱ����
		GameManage::birthTime = 5000;		// ���˵ĳ���ʱ����
		GameManage::hInst = hInst;
		GameManage::reset_time = true;
		SetTimer(hWnd,1,GameManage::timeStep,NULL);
		SetTimer(hWnd,2,GameManage::birthTime,NULL);
		SetTimer(hWnd,3,GameManage::animStep,NULL);
		break;
	case WM_TIMER:			// ��ʱ����Ӧ
		InvalidateRect(hWnd, NULL, TRUE);	// �ô��ڱ�Ϊ��Ч,�Ӷ������ػ���Ϣ
		switch (wParam) 
		{ 
		case 1:				// 1�Ŷ�ʱ�����ڸ�����Ϸ����
			GameManage::Update(GameManage::timeStep/10);
			if (GameManage::reset_time)	// ��������ؿ�֮�󣬰����µĲ������ö�ʱ��
			{
				GameManage::reset_time = false;
				KillTimer(hWnd,1);		// �����˳�ʱ������ʱ��ɾ��
				KillTimer(hWnd,2);
				KillTimer(hWnd,3);
				SetTimer(hWnd,1,GameManage::timeStep,NULL);
				SetTimer(hWnd,2,GameManage::birthTime,NULL);
				SetTimer(hWnd,3,GameManage::animStep,NULL);
			}
			break ; 
		case 2:				// 2�Ŷ�ʱ���������ɵ���̹��
			GameManage::EnemyBirth();
			break ; 
		case 3:				// 3�Ŷ�ʱ�����ڸ��¶���֡
			GameManage::NextFrame(GameManage::timeStep);
			break ; 
		} 
		break;
	case WM_SIZE:			// ��ȡ���ڵĳߴ�
		GameManage::ChangeWndSize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:		// ��Ұ��°�����Ϣ����GameProc�ļ��е��ض���������
		InvalidateRect(hWnd, NULL, TRUE);
		GameManage::EnterKey(wParam);
		break;
	case WM_ERASEBKGND:		// ����������,������˸
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			GameManage::Draw(hdc, hWnd);		// ������Ϸ����
			EndPaint(hWnd, &ps);
			break;
		}
	case WM_DESTROY:
		KillTimer(hWnd,1);		// �����˳�ʱ������ʱ��ɾ��
		KillTimer(hWnd,2);
		KillTimer(hWnd,3);
		GameManage::Destroy();
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
