#include "stdafx.h"
#include "MakingPlayer.h"
#include "Mouse.h"
#include <comdef.h>

CMakingPlayer::CMakingPlayer()
{
}

CMakingPlayer::~CMakingPlayer()
{
	Release();
}

void CMakingPlayer::Initialize()
{
	m_hDC = (CBitmapMgr::GetInstance()->GetMapBit()[L"BackBuffer"])->GetMemDC();
	m_pImgName = L"MakingPlayer1";

	CBitmapMgr::GetInstance()->LoadImageByScene(SCENE_MAKINGPLAYER);

	// ���콺
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMouse>::CreateObj(), OBJ_MOUSE);
}

int CMakingPlayer::Update()
{
	CObjMgr::GetInstance()->UpdateObj();

	// ���� �ٲٴ� ȭ��ǥ - ���콺 �浹 �˻�.
	RECT rcRight, rcLeft;
	POINT pt;
	POINT pt2;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt2.x = static_cast<LONG>(pt.x - g_fScrollX) - 10 + static_cast<LONG>(g_fScrollX);
	pt2.y = static_cast<LONG>(pt.y - g_fScrollY) - 21 + static_cast<LONG>(g_fScrollY);

	rcRight.left = WINCX - 160; rcRight.right = WINCX - 120;
	rcRight.bottom = WINCY - 350; rcRight.top = WINCY - 380;
	rcLeft.left = WINCX - 260; rcLeft.right = WINCX - 230;
	rcLeft.bottom = WINCY - 350; rcLeft.top = WINCY - 380;

	// Debugging
	//Rectangle(m_hDC, rcRight.left, rcRight.top, rcRight.right, rcRight.bottom);
	//Rectangle(m_hDC, rcLeft.left, rcLeft.top, rcLeft.right, rcLeft.bottom);

	if (PtInRect(&rcRight, pt2))
	{
		if (KEYMGR->OnceKeyDown(VK_LBUTTON)) {
			if (sceneNum) // 1
				sceneNum = 0;
			else
				sceneNum = 1;
		}
	}
	if (PtInRect(&rcLeft, pt2))
	{
		if (KEYMGR->OnceKeyDown(VK_LBUTTON)) {
			if (sceneNum) // 1
				sceneNum = 0;
			else
				sceneNum = 1;
		}
	}

	switch (sceneNum) {
	case 0:
		m_pImgName = L"MakingPlayer1";
		g_myinfo.job = JOB_STRIKER;
		break;
	case 1:
		m_pImgName = L"MakingPlayer2";
		g_myinfo.job = JOB_CAPTIN;
		break;
	}
	
	// ĳ���� ���� ��ư - ���콺 �浹 �˻�.
	RECT rcMake{ WINCX - 300, WINCY - 180, WINCX - 200, WINCY - 130 };

	// (Debugging)
	// Rectangle(m_hDC, rcMake.left, rcMake.top, rcMake.right, rcMake.bottom);
	
	if (PtInRect(&rcMake, pt2)) {
		if (KEYMGR->OnceKeyDown(VK_LBUTTON)) {
			char buf[BUFSIZE];
			// �Է� ���� id�� InitInfo.id�� �����Ѵ�
			char* pStr;
			int strSize = WideCharToMultiByte(CP_ACP, 0, g_idbuf, -1, NULL, 0, NULL, NULL);
			pStr = new char[strSize];
			WideCharToMultiByte(CP_ACP, 0, g_idbuf, -1, pStr, strSize, 0, 0);
			memcpy(&(g_myinfo.nickname), (pStr), strSize);

			// ���� ���� �����͸� ������.
			PACKETINFO packetinfo;
			packetinfo.type = CS_PACKET_PLAYERINFO_INITIALLY;
			packetinfo.size = sizeof(g_myinfo);
			// ZeroMemory(buf, sizeof(buf));
			memcpy(buf, &packetinfo, sizeof(packetinfo));
			g_retval = send(g_sock, (char*)&packetinfo/*buf*/, /*sizeof(packetinfo)*/BUFSIZE, 0);
			if (g_retval == SOCKET_ERROR)
				MessageBoxW(g_hWnd, L"send()", L"send()", MB_OK);

			// �� PlayerInfo ������ ������ send �Ѵ�.
			ZeroMemory(buf, sizeof(buf));
			memcpy(buf, &g_myinfo, sizeof(g_myinfo));
			g_retval = send(g_sock, (char*)&g_myinfo/*buf*/, /*sizeof(g_myinfo)*/BUFSIZE, 0);
			if (g_retval == SOCKET_ERROR)
				MessageBoxW(g_hWnd, L"send()", L"send()", MB_OK);

			{
				// Debugging
				{
					if (g_myinfo.job == JOB_CAPTIN)
						cout << "PlayerInfo - �г��� : " << g_myinfo.nickname << ", ���� : ĸƾ " << " / ���� ����" << endl;
					else
						cout << "PlayerInfo - �г��� : " << g_myinfo.nickname << ", ���� : ��Ʈ����Ŀ " << " / ���� ����" << endl;
				}
				
				// id�� �ο��޾ƾ� ���� ������ �Ѿ���� ����..! maingame���� �ٸ� �����Ͱ� �޴� ��ó��
				// �Ȱ��� ������ ������ ����.
				// ���� ����
				ZeroMemory(buf, sizeof(buf));
				g_retval = recv(g_sock, buf, BUFSIZE, 0);
				if (g_retval == SOCKET_ERROR)
					MessageBoxW(g_hWnd, L"recvn() - SC_PACKET_PLAYERINFO_ID", MB_OK, MB_OK);
				else
					memcpy(&g_myid, buf, sizeof(g_myid));

				// send�� �����ϸ�, �ʵ�� �Ѿ��.
				CSceneMgr::GetInstance()->SetScene(SCENE_FIELD);
				CSoundMgr::GetInstance()->StopSoundAll();
				CSoundMgr::GetInstance()->PlaySound(L"Start.MP3", CSoundMgr::CHANNEL_EFFECT);
				CSoundMgr::GetInstance()->PlayBGM(L"BGM_Field.mp3");
				// tempinfo�� ������� ������ ������ �־��ش�.
				//break;
			//}
			}
		}
	}

	return 0;
}

void CMakingPlayer::Render(HDC hDc)
{
	CMyBmp* pBmp = CBitmapMgr::GetInstance()->FindImage(m_pImgName);
	if (NULL == pBmp)
		return;

	TransparentBlt(hDc, 0, 0, WINCX, WINCY,
		pBmp->GetMemDC(),
		0, 0,
		WINCX, WINCY, RGB(255, 255, 255));

	// �Է��ϴ� id ���.
	RECT rc = { WINCX - 240, WINCY - 313, WINCX - 100, WINCY - 293 };

	DrawText(m_hDC, g_idbuf, wcslen(g_idbuf), &rc, DT_SINGLELINE);
}

void CMakingPlayer::Release()
{

}
 