#include "mouse.h"


CInputMouse::CInputMouse()
{
	memset(m_aMouseState, 0, sizeof(m_aMouseState));
	memset(m_aMouseStateTrigger, 0, sizeof(m_aMouseStateTrigger));
	memset(m_aMouseStateRelease, 0, sizeof(m_aMouseStateRelease));
}

CInputMouse::~CInputMouse()
{
}

HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイス（キーボード）の作成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

void CInputMouse::Update(void)
{
	int nCntKey;
	DIMOUSESTATE  ms;
	HRESULT hr;

	if (m_pDevice != NULL)
	{
		m_pDevice->Poll();

		hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &ms);

		//デバイスからデータを取得
		if (SUCCEEDED(hr))
		{
			for (nCntKey = 0; nCntKey < MAX_MOUSE_BOTTON; nCntKey++)
			{
				//キートリガー
				m_aMouseStateTrigger[nCntKey] = (m_aMouseState[nCntKey] ^ ms.rgbButtons[nCntKey]) &  ms.rgbButtons[nCntKey];

				//キーリリース
				m_aMouseStateRelease[nCntKey] = (m_aMouseState[nCntKey] ^ ms.rgbButtons[nCntKey]) & ~ms.rgbButtons[nCntKey];

				//キープレス情報を保存
				m_aMouseState[nCntKey] = ms.rgbButtons[nCntKey];
			}
		}
		else
		{
			m_pDevice->Acquire();
		}
	}
}
bool CInputMouse::GetMousePress(int nKey)
{
	return m_aMouseState[nKey] & 0x80 ? true : false;
}

bool CInputMouse::GetMouseTrigger(int nKey)
{
	return m_aMouseStateTrigger[nKey] & 0x80 ? true : false;
}

bool CInputMouse::GetMouseRelease(int nKey)
{
	return m_aMouseStateRelease[nKey] & 0x80 ? true : false;
}

D3DXVECTOR3 CInputMouse::GetMousePos(void)
{
	// マウス座標を取得する
	POINT p;
	GetCursorPos(&p);

	// スクリーン座標をクライアント座標に変換する
	ScreenToClient(FindWindowA(WINDOW_CLASS_NAME, nullptr), &p);

	return D3DXVECTOR3(p.x, p.y, 0);
}

void CInputMouse::SetMousePos(D3DXVECTOR3 pos)
{
	POINT p;
	p.x = pos.x;
	p.y = pos.y;
	// クライアント座標をスクリーン座標に変換する
	ClientToScreen(FindWindowA(WINDOW_CLASS_NAME, nullptr), &p);

	SetCursorPos(p.x, p.y);
}
