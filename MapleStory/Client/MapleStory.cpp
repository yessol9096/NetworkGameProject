// MapleStory.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "MapleStory.h"
#include "Maingame.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
/*INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);*/

HWND g_hWnd;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// �޸� ���� Ȯ��
	// _CrtSetBreakAlloc: ���� ���� ���� �޸� �Ҵ� ��ȣ�� ���ڷ� �־��ָ� �� ��ġ�� �ߴ����� �ɾ��ִ� �Լ�.
	//_CrtSetBreakAlloc(170);


	// _CrtSetDbgFlag: ���� �޸𸮸� ��� ���������� �����ϱ� ���� �Լ�.
	// _CRTDBG_ALLOC_MEM_DF �÷���: ����� �������� ��� �޸� �Ҵ��� �Ͼ� �� ������ ������ �����ϵ���
	// ����� �߰����ִ� �ɼ�.

	// _CRTDBG_LEAK_CHECK_DF �÷���: ���α׷��� ����Ǳ� ������ ���� �������� ���� �޸𸮰� �ִ��� �˻��ϴ� �ɼ�.	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


#ifdef _DEBUG
	// api���� �ܼ�â ����.
// 	if(AllocConsole())
// 		freopen("CONOUT$", "w", stdout);
#endif


	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MAPLESTORY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAPLESTORY));


	msg.message = WM_NULL;

	CMaingame mainGame;
	mainGame.Initialize();

	// GetTickCount : �ü���� ���۵��� ������ �귯�� �ð��� 1/1000�� ������
	// ���������� ��ȯ
	DWORD dwOldTime = GetTickCount();
	DWORD dwCurTime = 0;

	TCHAR szFPS[32] = __T("");
	DWORD dwFPSOldTime = GetTickCount();
	DWORD dwFPSCurTime = 0;
	int   iFps = 0;

	while(WM_QUIT != msg.message)
	{
		dwFPSCurTime = GetTickCount();

		if(dwFPSOldTime + 1000 < dwFPSCurTime)
		{
			dwFPSOldTime = dwFPSCurTime;
			swprintf_s(szFPS, __T("FPS: %d"), iFps);
			SetWindowText(g_hWnd, szFPS);
			iFps = 0;
		}

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurTime = GetTickCount();
			if(dwOldTime + 20 < dwCurTime)
			{
				dwOldTime = dwCurTime;
				mainGame.Update();
//				if(SCENE_LOGO != CSceneMgr::GetInstance()->GetSceneType())
					mainGame.Render();

				iFps++;
			}
		}
	}
#ifdef _DEBUG
//	FreeConsole(); // �ܼ�â ����
#endif
	return (int) msg.wParam;
}

//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAPLESTORY));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;/*MAKEINTRESOURCE(IDC_MAPLESTORY);*/
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   RECT rc = { 0, 0, WINCX, WINCY };

   //    RECT rc = { WINCX * 0.5, WINCY * 0.5, WINCX + WINCX * 0.5, WINCY + WINCY * 0.5 };

   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   ShowCursor(FALSE);



   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
// 	int wmId, wmEvent;
// 	PAINTSTRUCT ps;
// 	HDC hdc;

	switch (message)
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
// 	case WM_COMMAND:
// 		wmId    = LOWORD(wParam);
// 		wmEvent = HIWORD(wParam);
// 		// �޴��� ���� ������ ���� �м��մϴ�.
// 		switch (wmId)
// 		{
// 		case IDM_ABOUT:
// 			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
// 			break;
// 		case IDM_EXIT:
// 			DestroyWindow(hWnd);
// 			break;
// 		default:
// 			return DefWindowProc(hWnd, message, wParam, lParam);
// 		}
//		break;
// 	case WM_PAINT:
// 		hdc = BeginPaint(hWnd, &ps);
// 		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
// 		EndPaint(hWnd, &ps);
// 		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
// INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
// {
// 	UNREFERENCED_PARAMETER(lParam);
// 	switch (message)
// 	{
// 	case WM_INITDIALOG:
// 		return (INT_PTR)TRUE;
// 
// 	case WM_COMMAND:
// 		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
// 		{
// 			EndDialog(hDlg, LOWORD(wParam));
// 			return (INT_PTR)TRUE;
// 		}
// 		break;
// 	}
// 	return (INT_PTR)FALSE;
// }
