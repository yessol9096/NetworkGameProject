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

	// 마우스
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMouse>::CreateObj(), OBJ_MOUSE);
}

int CMakingPlayer::Update()
{
	CObjMgr::GetInstance()->UpdateObj();

	// 직업 바꾸는 화살표 - 마우스 충돌 검사.
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
	
	// 캐릭터 생성 버튼 - 마우스 충돌 검사.
	RECT rcMake{ WINCX - 300, WINCY - 180, WINCX - 200, WINCY - 130 };

	// (Debugging)
	// Rectangle(m_hDC, rcMake.left, rcMake.top, rcMake.right, rcMake.bottom);
	
	if (PtInRect(&rcMake, pt2)) {
		if (KEYMGR->OnceKeyDown(VK_LBUTTON)) {
			char buf[BUFSIZE];
			// 입력 받은 id를 InitInfo.id에 갱신한다
			char* pStr;
			int strSize = WideCharToMultiByte(CP_ACP, 0, g_idbuf, -1, NULL, 0, NULL, NULL);
			pStr = new char[strSize];
			WideCharToMultiByte(CP_ACP, 0, g_idbuf, -1, pStr, strSize, 0, 0);
			memcpy(&(g_myinfo.nickname), (pStr), strSize);

			// 고정 길이 데이터를 보낸다.
			PACKETINFO packetinfo;
			packetinfo.type = CS_PACKET_PLAYERINFO_INITIALLY;
			packetinfo.size = sizeof(g_myinfo);
			// ZeroMemory(buf, sizeof(buf));
			memcpy(buf, &packetinfo, sizeof(packetinfo));
			g_retval = send(g_sock, (char*)&packetinfo/*buf*/, /*sizeof(packetinfo)*/BUFSIZE, 0);
			if (g_retval == SOCKET_ERROR)
				MessageBoxW(g_hWnd, L"send()", L"send()", MB_OK);

			// 내 PlayerInfo 정보를 서버에 send 한다.
			ZeroMemory(buf, sizeof(buf));
			memcpy(buf, &g_myinfo, sizeof(g_myinfo));
			g_retval = send(g_sock, (char*)&g_myinfo/*buf*/, /*sizeof(g_myinfo)*/BUFSIZE, 0);
			if (g_retval == SOCKET_ERROR)
				MessageBoxW(g_hWnd, L"send()", L"send()", MB_OK);

			{
				// Debugging
				{
					if (g_myinfo.job == JOB_CAPTIN)
						cout << "PlayerInfo - 닉네임 : " << g_myinfo.nickname << ", 직업 : 캡틴 " << " / 정보 전송" << endl;
					else
						cout << "PlayerInfo - 닉네임 : " << g_myinfo.nickname << ", 직업 : 스트라이커 " << " / 정보 전송" << endl;
				}
				
				// id를 부여받아야 다음 씬으로 넘어가도록 하자..! maingame에서 다른 데이터가 받는 것처럼
				// 똑같이 받으면 구조가 꼬임.
				// 가변 길이
				ZeroMemory(buf, sizeof(buf));
				g_retval = recv(g_sock, buf, BUFSIZE, 0);
				if (g_retval == SOCKET_ERROR)
					MessageBoxW(g_hWnd, L"recvn() - SC_PACKET_PLAYERINFO_ID", MB_OK, MB_OK);
				else
					memcpy(&g_myid, buf, sizeof(g_myid));

				// send에 성공하면, 필드로 넘어간다.
				CSceneMgr::GetInstance()->SetScene(SCENE_FIELD);
				CSoundMgr::GetInstance()->StopSoundAll();
				CSoundMgr::GetInstance()->PlaySound(L"Start.MP3", CSoundMgr::CHANNEL_EFFECT);
				CSoundMgr::GetInstance()->PlayBGM(L"BGM_Field.mp3");
				// tempinfo에 현재까지 설정된 정보를 넣어준다.
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

	// 입력하는 id 출력.
	RECT rc = { WINCX - 240, WINCY - 313, WINCX - 100, WINCY - 293 };

	DrawText(m_hDC, g_idbuf, wcslen(g_idbuf), &rc, DT_SINGLELINE);
}

void CMakingPlayer::Release()
{

}
 